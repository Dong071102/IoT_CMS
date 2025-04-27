#include "esp_camera.h"
#include <WiFi.h>

// ==== CHỌN MODULE CAMERA ==== //
#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"

// ==== CẤU HÌNH WIFI ==== //
const char* ssid = "DongOi";
const char* password = "123456789";

IPAddress local_IP(192, 168, 0, 20);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(8, 8, 8, 8);
IPAddress secondaryDNS(8, 8, 4, 4);

void startCameraServer();

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  // ==== CẤU HÌNH CAMERA ==== //
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;

  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;   // ✅ BẮT BUỘC để stream hoạt động

  config.frame_size = FRAMESIZE_QVGA;     // 320x240 – nhanh, đủ dùng
  config.jpeg_quality = 12;               // chất lượng nén: 10–12 là tối ưu
  config.fb_count = psramFound() ? 2 : 1; // buffer kép nếu có PSRAM
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.grab_mode = CAMERA_GRAB_LATEST;

  // ==== KHỞI TẠO CAMERA ==== //
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("❌ Camera init failed: 0x%x\n", err);
    return;
  }

  sensor_t *s = esp_camera_sensor_get();
  s->set_vflip(s, 0);     // ảnh ngược thì để 1
  s->set_hmirror(s, 0);   // ảnh ngược trái phải thì để 1
  s->set_brightness(s, 1);
  s->set_saturation(s, -2);

  // ==== KẾT NỐI WIFI ==== //
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("⚠️ Failed to configure static IP");
  }

  WiFi.begin(ssid, password);
  WiFi.setSleep(false);  // ✅ Giữ kết nối WiFi ổn định

  Serial.print("🔌 Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi connected");
  Serial.print("📡 IP: ");
  Serial.println(WiFi.localIP());

  // ==== KHỞI ĐỘNG SERVER STREAM ==== //
  startCameraServer();
  Serial.println("🚀 Camera stream ready!");
  Serial.println("🖥 Mở trình duyệt: http://192.168.10.20 để xem live stream");
}

void loop() {
  delay(10000);  // Không làm gì – server xử lý riêng
}
