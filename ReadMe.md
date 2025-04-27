# 🚀 **Hướng Dẫn Cài Đặt và Chạy Ứng Dụng WebSocket với ESP32-S3 và Màn Hình ST7789**

## 📝 **Giới Thiệu**
Ứng dụng này sử dụng **ESP32-S3** kết hợp với màn hình **ST7789 240x320** để nhận và hiển thị hình ảnh JPEG qua WebSocket. Các ảnh được mã hóa dưới dạng **Base64** và gửi từ máy chủ WebSocket. ESP32 giải mã Base64 và sử dụng thư viện **TJpgDecoder** để giải mã và hiển thị ảnh trên màn hình.

---

## 🧑‍💻 **Các Bước Chuẩn Bị**

### 1. **Cài Đặt Arduino IDE**
- **Tải và cài đặt Arduino IDE**: [Tải tại đây](https://www.arduino.cc/en/software).
- **Cài đặt ESP32 Board**:
  1. Mở **Arduino IDE**.
  2. Vào **File > Preferences**, và thêm URL sau vào **Additional Boards Manager URLs**:
     ```
     https://dl.espressif.com/dl/package_esp32_index.json
     ```
  3. Vào **Tools > Board > Boards Manager** và tìm **ESP32** để cài đặt.

### 2. **Cài Đặt Thư Viện Cần Thiết**
- **Adafruit GFX và ST7789**: Cài đặt thư viện **Adafruit_GFX** và **Adafruit_ST7789** từ **Library Manager** của Arduino IDE.
- **ArduinoWebsockets**: Cài đặt thư viện **ArduinoWebsockets** từ **Library Manager**.
- **TJpg Decoder**: Thư viện này được sử dụng để giải mã ảnh JPEG.

### 3. **Chọn Board và Cổng COM**
- Vào **Tools > Board**, chọn **ESP32-S3 Dev Module** hoặc module tương ứng với phần cứng của bạn (chẳng hạn **AI Thinker ESP32-CAM**).
- Vào **Tools > Port**, chọn đúng cổng COM mà ESP32 của bạn đang kết nối.

### 4. **Cấu Hình Thông Tin WiFi**
Trong mã nguồn dưới đây, bạn cần thay đổi các thông tin sau để kết nối ESP32 với mạng Wi-Fi của bạn:

```cpp
const char* ssid = "DongOi";  // Tên mạng Wi-Fi
const char* password = "123456789";  // Mật khẩu Wi-Fi
IPAddress local_IP(192, 168, 0, 111);  // Địa chỉ IP tĩnh của ESP32
IPAddress gateway(192, 168, 0, 1);  // Gateway (Router)
IPAddress subnet(255, 255, 255, 0);  // Subnet mask
```
# 📸 **Hướng Dẫn Chạy Ứng Dụng Camera với ESP32 trên Arduino IDE**

## 🔍 **Mô Tả**
Ứng dụng này sử dụng **ESP32 Camera Module** (chẳng hạn như AI Thinker) để phát trực tuyến hình ảnh từ camera qua **WiFi**. Mã nguồn sử dụng thư viện `esp_camera` để khởi tạo và cấu hình camera, sau đó sử dụng **WiFi** để kết nối mạng và khởi động một server để stream video từ camera.

---

## 📝 **Các Bước Chuẩn Bị**

### 1. **Cài Đặt Arduino IDE**

Trước khi bắt đầu, bạn cần cài đặt **Arduino IDE** và cấu hình **ESP32**:

- **Tải và cài đặt Arduino IDE**: [Tải tại đây](https://www.arduino.cc/en/software).
- **Cấu hình ESP32 trong Arduino IDE**:
  1. Mở **Arduino IDE**.
  2. Vào **File > Preferences**, trong mục **Additional Boards Manager URLs**, thêm URL sau:
     ```
     https://dl.espressif.com/dl/package_esp32_index.json
     ```
  3. Vào **Tools > Board > Boards Manager**, tìm và cài đặt **ESP32**.

---

### 2. **Cài Đặt Thư Viện**

Đảm bảo rằng thư viện **ESP32 Camera** đã được tích hợp sẵn trong bộ cài đặt ESP32. Bạn không cần phải cài thêm thư viện riêng biệt cho camera.

---

### 3. **Chọn Board và Cổng COM**

- Vào **Tools > Board**, chọn **ESP32 Wrover Module** hoặc module tương ứng với phần cứng của bạn (chẳng hạn **AI Thinker ESP32-CAM**).
- Vào **Tools > Port**, chọn đúng cổng COM mà ESP32 của bạn đang kết nối.

---

### 4. **Cấu Hình Thông Tin WiFi**

Trong mã nguồn dưới đây, bạn cần thay đổi các thông tin sau:
- **SSID**: Tên mạng Wi-Fi của bạn.
- **Password**: Mật khẩu Wi-Fi của bạn.
- **Local IP**: Địa chỉ IP tĩnh cho ESP32.

```cpp
const char* ssid = "DongOi";  // Tên WiFi
const char* password = "123456789";  // Mật khẩu WiFi
IPAddress local_IP(192, 168, 0, 20);  // Địa chỉ IP tĩnh
```