/*
 * ============================================================
 *  ESP32-CAM — Camera Streaming + Pan/Tilt + mDNS
 *  Board: AI-Thinker ESP32-CAM
 *
 *  পরিবর্তন:
 *  - /stream route যোগ — শুধু camera canvas (iframe এর জন্য)
 *  - fb_count: 2, FRAMESIZE_QVGA — reconnect fix
 *  - Queue full হলে frame skip, block না করা
 *  - PAN  Servo → GPIO 13
 *  - TILT Servo → GPIO 12
 *  - FLASH LED  → GPIO 4
 * ============================================================
 */

#include "esp_camera.h"
#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESP32Servo.h>
#include <sstream>

// ================= WIFI =================
const char* ssid     = "Net_Error";
const char* password = "123456780";

#define MDNS_NAME "firecam"

// ================= PAN/TILT =================
#define PAN_PIN  13
#define TILT_PIN 12

Servo panServo;
Servo tiltServo;

// ================= FLASH LED =================
#define LIGHT_PIN        4
#define PWMFreq          1000
#define PWMResolution    8
#define PWMLightChannel  5

// ================= CAMERA PINS (AI-Thinker) =================
#define PWDN_GPIO_NUM   32
#define RESET_GPIO_NUM  -1
#define XCLK_GPIO_NUM    0
#define SIOD_GPIO_NUM   26
#define SIOC_GPIO_NUM   27
#define Y9_GPIO_NUM     35
#define Y8_GPIO_NUM     34
#define Y7_GPIO_NUM     39
#define Y6_GPIO_NUM     36
#define Y5_GPIO_NUM     21
#define Y4_GPIO_NUM     19
#define Y3_GPIO_NUM     18
#define Y2_GPIO_NUM      5
#define VSYNC_GPIO_NUM  25
#define HREF_GPIO_NUM   23
#define PCLK_GPIO_NUM   22

// ================= SERVER =================
AsyncWebServer server(80);
AsyncWebSocket wsCamera("/Camera");
AsyncWebSocket wsInput("/CarInput");

uint32_t cameraClientId = 0;

// ================= STREAM PAGE (শুধু canvas) =================
const char* streamPage PROGMEM = R"STREAMPAGE(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    * { margin: 0; padding: 0; box-sizing: border-box; }
    body { background: #000; display: flex; align-items: center; justify-content: center; width: 100%; height: 100vh; overflow: hidden; }
    canvas { width: 100%; height: 100%; object-fit: contain; display: block; }
  </style>
</head>
<body>
  <canvas id="cam" width="320" height="240"></canvas>
<script>
  var ws = new WebSocket('ws://' + window.location.hostname + '/Camera');
  ws.binaryType = 'arraybuffer';
  var canvas = document.getElementById('cam');
  var ctx = canvas.getContext('2d');

  ws.onmessage = function(e) {
    var blob = new Blob([e.data], {type: 'image/jpeg'});
    var url  = URL.createObjectURL(blob);
    var img  = new Image();
    img.onload = function() {
      canvas.width  = img.naturalWidth  || 320;
      canvas.height = img.naturalHeight || 240;
      ctx.drawImage(img, 0, 0);
      URL.revokeObjectURL(url);
    };
    img.onerror = function() { URL.revokeObjectURL(url); };
    img.src = url;
  };

  ws.onclose = function() { setTimeout(function(){ location.reload(); }, 2000); };
</script>
</body>
</html>
)STREAMPAGE";

// ================= FULL PAGE (original) =================
const char* htmlPage PROGMEM = R"HTMLPAGE(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no">
  <style>
    * { box-sizing: border-box; margin: 0; padding: 0; }
    body {
      background: #0d0d0d;
      color: #eee;
      font-family: Arial, sans-serif;
      padding: 10px;
      -webkit-user-select: none;
      user-select: none;
    }
    .label {
      font-size: 11px;
      color: #aaa;
      margin-bottom: 4px;
      text-transform: uppercase;
      letter-spacing: 0.5px;
      display: flex;
      justify-content: space-between;
    }
    .slider-row { margin-bottom: 12px; }
    input[type=range] {
      -webkit-appearance: none;
      width: 100%;
      height: 12px;
      border-radius: 6px;
      background: #2a2a2a;
      outline: none;
    }
    input[type=range]::-webkit-slider-thumb {
      -webkit-appearance: none;
      width: 24px;
      height: 24px;
      border-radius: 50%;
      background: #ff6b35;
      cursor: pointer;
    }
    .val { color: #ff6b35; font-weight: bold; }
    .reset-btn {
      width: 100%;
      padding: 10px;
      margin-top: 4px;
      background: #1e3a5f;
      color: #60a5fa;
      border: 1px solid #2563eb;
      border-radius: 8px;
      font-size: 13px;
      cursor: pointer;
    }
    .reset-btn:active { background: #2563eb; color: #fff; }
    .status {
      font-size: 11px;
      color: #555;
      margin-bottom: 10px;
      display: flex;
      align-items: center;
      gap: 6px;
    }
    .dot { width: 8px; height: 8px; border-radius: 50%; background: #f87171; flex-shrink: 0; }
    .dot.on { background: #4ade80; }
  </style>
</head>
<body>
  <div class="status">
    <span class="dot" id="dot"></span>
    <span id="statusText">WebSocket connecting...</span>
  </div>

  <canvas id="cameraCanvas" width="320" height="240"
    style="width:100%;height:240px;border-radius:8px;background:#111;margin-bottom:12px;display:block;">
  </canvas>

  <div class="slider-row">
    <div class="label">
      <span>&#8592; Pan &#8594;</span>
      <span class="val" id="panVal">90°</span>
    </div>
    <input type="range" min="0" max="180" value="90" id="Pan"
      oninput="send('Pan',this.value); document.getElementById('panVal').textContent=this.value+'°'">
  </div>

  <div class="slider-row">
    <div class="label">
      <span>&#8593; Tilt &#8595;</span>
      <span class="val" id="tiltVal">90°</span>
    </div>
    <input type="range" min="0" max="180" value="90" id="Tilt"
      oninput="send('Tilt',this.value); document.getElementById('tiltVal').textContent=this.value+'°'">
  </div>

  <div class="slider-row">
    <div class="label">
      <span>&#128294; Flash Light</span>
      <span class="val" id="lightVal">OFF</span>
    </div>
    <input type="range" min="0" max="255" value="0" id="Light"
      oninput="send('Light',this.value); document.getElementById('lightVal').textContent=(+this.value===0?'OFF':this.value)">
  </div>

  <button class="reset-btn" onclick="resetCenter()">&#8635; Pan/Tilt Center</button>

<script>
  var host   = window.location.hostname;
  var wsCam  = null;
  var wsCtrl = null;
  var ctrlOk = false;
  var canvas = document.getElementById('cameraCanvas');
  var ctx    = canvas.getContext('2d');

  function initCam() {
    wsCam = new WebSocket('ws://' + host + '/Camera');
    wsCam.binaryType = 'arraybuffer';
    wsCam.onopen  = function() { setStatus(true,  'Camera connected'); };
    wsCam.onclose = function() {
      setStatus(false, 'Camera disconnected — reconnecting...');
      setTimeout(initCam, 2000);
    };
    wsCam.onerror = function() { setStatus(false, 'Camera error'); };
    wsCam.onmessage = function(e) {
      var blob = new Blob([e.data], {type: 'image/jpeg'});
      var url  = URL.createObjectURL(blob);
      var img  = new Image();
      img.onload = function() {
        canvas.width  = img.naturalWidth  || 320;
        canvas.height = img.naturalHeight || 240;
        ctx.drawImage(img, 0, 0);
        URL.revokeObjectURL(url);
      };
      img.onerror = function() { URL.revokeObjectURL(url); };
      img.src = url;
    };
  }

  function initCtrl() {
    wsCtrl = new WebSocket('ws://' + host + '/CarInput');
    wsCtrl.onopen = function() {
      ctrlOk = true;
      send('Pan',   document.getElementById('Pan').value);
      send('Tilt',  document.getElementById('Tilt').value);
      send('Light', document.getElementById('Light').value);
    };
    wsCtrl.onclose = function() { ctrlOk = false; setTimeout(initCtrl, 2000); };
    wsCtrl.onerror = function() { ctrlOk = false; };
    wsCtrl.onmessage = function() {};
  }

  function send(key, val) {
    if (ctrlOk && wsCtrl && wsCtrl.readyState === WebSocket.OPEN)
      wsCtrl.send(key + ',' + val);
  }

  function setStatus(ok, msg) {
    document.getElementById('dot').className = 'dot' + (ok ? ' on' : '');
    document.getElementById('statusText').textContent = msg;
  }

  function resetCenter() {
    document.getElementById('Pan').value  = 90;
    document.getElementById('Tilt').value = 90;
    document.getElementById('panVal').textContent  = '90°';
    document.getElementById('tiltVal').textContent = '90°';
    send('Pan', 90);
    send('Tilt', 90);
  }

  initCam();
  initCtrl();
</script>
</body>
</html>
)HTMLPAGE";

// ================= WEBSOCKET HANDLERS =================
void onInputEvent(AsyncWebSocket* s, AsyncWebSocketClient* client,
                  AwsEventType type, void* arg, uint8_t* data, size_t len)
{
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("Ctrl WS connected #%u\n", client->id());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("Ctrl WS disconnected #%u\n", client->id());
      panServo.write(90);
      tiltServo.write(90);
      ledcWrite(PWMLightChannel, 0);
      break;
    case WS_EVT_DATA: {
      AwsFrameInfo* info = (AwsFrameInfo*)arg;
      if (info->final && info->index == 0 &&
          info->len == len && info->opcode == WS_TEXT)
      {
        std::string msg((char*)data, len);
        std::istringstream ss(msg);
        std::string key, val;
        std::getline(ss, key, ',');
        std::getline(ss, val,  ',');
        int v = atoi(val.c_str());
        Serial.printf("  → %s = %d\n", key.c_str(), v);
        if      (key == "Pan")   panServo.write(v);
        else if (key == "Tilt")  tiltServo.write(v);
        else if (key == "Light") ledcWrite(PWMLightChannel, v);
      }
      break;
    }
    default: break;
  }
}

void onCameraEvent(AsyncWebSocket* s, AsyncWebSocketClient* client,
                   AwsEventType type, void* arg, uint8_t* data, size_t len)
{
  switch (type) {
    case WS_EVT_CONNECT:
      cameraClientId = client->id();
      Serial.printf("Camera WS connected #%u\n", client->id());
      break;
    case WS_EVT_DISCONNECT:
      cameraClientId = 0;
      Serial.printf("Camera WS disconnected #%u\n", client->id());
      break;
    default: break;
  }
}

// ================= CAMERA SETUP =================
void setupCamera() {
  camera_config_t cfg;
  cfg.ledc_channel  = LEDC_CHANNEL_4;
  cfg.ledc_timer    = LEDC_TIMER_2;
  cfg.pin_d0        = Y2_GPIO_NUM;
  cfg.pin_d1        = Y3_GPIO_NUM;
  cfg.pin_d2        = Y4_GPIO_NUM;
  cfg.pin_d3        = Y5_GPIO_NUM;
  cfg.pin_d4        = Y6_GPIO_NUM;
  cfg.pin_d5        = Y7_GPIO_NUM;
  cfg.pin_d6        = Y8_GPIO_NUM;
  cfg.pin_d7        = Y9_GPIO_NUM;
  cfg.pin_xclk      = XCLK_GPIO_NUM;
  cfg.pin_pclk      = PCLK_GPIO_NUM;
  cfg.pin_vsync     = VSYNC_GPIO_NUM;
  cfg.pin_href      = HREF_GPIO_NUM;
  cfg.pin_sscb_sda  = SIOD_GPIO_NUM;
  cfg.pin_sscb_scl  = SIOC_GPIO_NUM;
  cfg.pin_pwdn      = PWDN_GPIO_NUM;
  cfg.pin_reset     = RESET_GPIO_NUM;
  cfg.xclk_freq_hz  = 20000000;
  cfg.pixel_format  = PIXFORMAT_JPEG;
  cfg.frame_size    = FRAMESIZE_QVGA;  // ✅ VGA → QVGA (reconnect fix)
  cfg.jpeg_quality  = 12;
  cfg.fb_count      = 2;               // ✅ 1 → 2 (reconnect fix)

  esp_err_t err = esp_camera_init(&cfg);
  if (err != ESP_OK) {
    Serial.printf("Camera init FAILED: 0x%x\n", err);
    return;
  }
  if (psramFound()) {
    heap_caps_malloc_extmem_enable(20000);
    Serial.println("PSRAM enabled");
  }
  Serial.println("Camera OK");
}

// ================= SEND FRAME =================
void sendFrame() {
  if (cameraClientId == 0) return;

  // Queue full হলে frame skip করো, block না করে
  AsyncWebSocketClient* c = wsCamera.client(cameraClientId);
  if (!c || c->queueIsFull()) return;  // ✅ reconnect fix

  camera_fb_t* fb = esp_camera_fb_get();
  if (!fb) return;

  wsCamera.binary(cameraClientId, fb->buf, fb->len);
  esp_camera_fb_return(fb);
}

// ================= SETUP =================
void setup() {
  Serial.begin(115200);
  Serial.println("\nESP32-CAM starting...");

  ledcSetup(PWMLightChannel, PWMFreq, PWMResolution);
  ledcAttachPin(LIGHT_PIN, PWMLightChannel);
  ledcWrite(PWMLightChannel, 0);
  Serial.println("Light setup done");

  WiFi.begin(ssid, password);
  Serial.print("Connecting WiFi");
  int tries = 0;
  while (WiFi.status() != WL_CONNECTED && tries < 30) {
    delay(500); Serial.print("."); tries++;
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n==========================================");
    Serial.print("  ESP32-CAM IP: http://");
    Serial.println(WiFi.localIP());
    Serial.println("==========================================");
  } else {
    Serial.println("\nWiFi FAILED");
    return;
  }

  if (!MDNS.begin(MDNS_NAME)) {
    Serial.println("mDNS FAILED!");
  } else {
    Serial.println("mDNS OK! → http://firecam.local");
    MDNS.addService("http", "tcp", 80);
  }

  // Routes
  server.on("/", HTTP_GET, [](AsyncWebServerRequest* req){
    req->send_P(200, "text/html", htmlPage);
  });
  // ✅ /stream — শুধু camera canvas (main dashboard এর iframe এর জন্য)
  server.on("/stream", HTTP_GET, [](AsyncWebServerRequest* req){
    req->send_P(200, "text/html", streamPage);
  });
  server.onNotFound([](AsyncWebServerRequest* req){
    req->send(404, "text/plain", "Not found");
  });

  wsCamera.onEvent(onCameraEvent);
  wsInput.onEvent(onInputEvent);
  server.addHandler(&wsCamera);
  server.addHandler(&wsInput);

  server.begin();
  Serial.println("HTTP server started");

  setupCamera();

  // ✅ Camera init এর পরে servo attach
  panServo.attach(PAN_PIN);
  tiltServo.attach(TILT_PIN);
  panServo.write(90);
  tiltServo.write(90);
  Serial.println("Servos attached");
}

// ================= LOOP =================
void loop() {
  wsCamera.cleanupClients();
  wsInput.cleanupClients();
  sendFrame();
}
