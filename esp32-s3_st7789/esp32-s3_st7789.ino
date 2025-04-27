#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <WiFi.h>
#include <ArduinoWebsockets.h>
#include <TJpg_Decoder.h>
#include "mbedtls/base64.h"

using namespace websockets;

// ==== SETTING PINOUT ST7789 ====
#define TFT_CS    4
#define TFT_DC    5
#define TFT_RST   15
#define TFT_SCLK  12
#define TFT_MOSI  11

SPIClass spi = SPIClass(SPI);
Adafruit_ST7789 tft = Adafruit_ST7789(&spi, TFT_CS, TFT_DC, TFT_RST);

// ==== SETTING STATIC IP ====
const char* ssid = "DongOi";
const char* password = "123456789";
const char* websockets_server = "ws://192.168.0.108:8000/ws/0";

IPAddress local_IP(192, 168, 0, 111);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);

WebsocketsClient client;

// ==== RESET TIME IF NOT CONNECT ====
bool isConnected = false;
unsigned long lastReconnectAttempt = 0;
const unsigned long reconnectInterval = 10UL * 60UL * 1000UL; // 10 phút

// ==== Hàm hiển thị ảnh ====
bool tftOutput(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t *bitmap) {
  tft.drawRGBBitmap(x, y, bitmap, w, h);
  return true;
}

// ==== WEBSOCKET DECODE ====
void onWebSocketMessage(WebsocketsMessage message) {
  Serial.println("Received message");

  String frameBase64 = message.data();
  Serial.print("Base64 length: ");
  Serial.println(frameBase64.length());

  int base64Len = frameBase64.length();
  int estimatedSize = (base64Len * 3) / 4;

  uint8_t* jpegBuffer = (uint8_t*) malloc(estimatedSize);
  if (!jpegBuffer) {
    Serial.println("Failed to allocate JPEG buffer");
    return;
  }

  size_t outLen = 0;
  if (mbedtls_base64_decode(jpegBuffer, estimatedSize, &outLen,
                            (const unsigned char*)frameBase64.c_str(), base64Len) != 0) {
    Serial.println("Base64 decode failed");
    free(jpegBuffer);
    return;
  }

  yield();  // Cho phép tác vụ nền

  TJpgDec.setCallback(tftOutput);
  TJpgDec.setJpgScale(1);
  int decodeResult = TJpgDec.drawJpg(0, 0, jpegBuffer, outLen);

  if (decodeResult != 1) {
    Serial.println("JPEG decode/display failed");
  }

  free(jpegBuffer);
  delay(1);
}

// ==== SETUP ESP32-S3 WITH ST7789 240X320 DISPLAY ====
void setup() {
  setCpuFrequencyMhz(240);
  Serial.begin(115200);
  delay(1000);
  Serial.println("Initializing...");

  // DISPLAY ST7789
  spi.begin(TFT_SCLK, -1, TFT_MOSI, TFT_CS);
  tft.init(240, 320);
  tft.setRotation(0);  //roteate the video 180 dec
  tft.invertDisplay(false);
  tft.fillScreen(ST77XX_BLACK);

  // WiFi
  Serial.print("Connecting to WiFi...");
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("Failed to configure static IP");
  }

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  // WebSocket
  client.onMessage(onWebSocketMessage);
  Serial.print("Connecting to WebSocket: ");
  Serial.println(websockets_server);

  if (client.connect(websockets_server)) {
    Serial.println("WebSocket connected!");
    isConnected = true;
  } else {
    Serial.println("Initial WebSocket connection failed. Will retry in 10 minutes.");
    isConnected = false;
    lastReconnectAttempt = millis();
  }
}

// ==== LOOP FUNCTION ====
void loop() {
  if (isConnected) {
    client.poll();
  } else {
    unsigned long now = millis();
    if (now - lastReconnectAttempt >= reconnectInterval) {
      Serial.println("Attempting to reconnect to WebSocket...");
      if (client.connect(websockets_server)) {
        Serial.println("WebSocket reconnected!");
        isConnected = true;
      } else {
        Serial.println("Reconnect failed. Will retry in 10 minutes.");
        lastReconnectAttempt = now;
      }
    }
  }

  delay(1);
  yield();
}
