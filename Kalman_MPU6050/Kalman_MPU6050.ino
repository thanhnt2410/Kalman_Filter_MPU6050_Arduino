// Basic demo for accelerometer readings from Adafruit MPU6050

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

// Cấu trúc cho bộ lọc Kalman
struct Kalman {
    float angle; // Góc ước lượng
    float bias;  // Độ lệch của con quay hồi chuyển
    float P[2][2]; // Ma trận hiệp phương sai
};

// Tham số cho bộ lọc Kalman
const float Q_angle = 0.001; // Độ biến thiên của góc
const float Q_bias = 0.003;  // Độ biến thiên của độ lệch
const float R_measure = 0.03; // Độ biến thiên của phép đo

// Biến cho bộ lọc Kalman
Kalman kalmanPitch;
Kalman kalmanRoll;
// Biến thời gian
unsigned long lastTime = 0;

// Hàm khởi tạo bộ lọc Kalman
void KalmanInit(Kalman &k) {
    k.angle = 0.0;
    k.bias = 0.0;
    k.P[0][0] = 1.0;
    k.P[0][1] = 0.0;
    k.P[1][0] = 0.0;
    k.P[1][1] = 1.0;
}

// Hàm cập nhật bộ lọc Kalman
float KalmanUpdate(Kalman &k, float newAngle, float newRate, float dt) {
    // Dự đoán
    float rate = newRate - k.bias;
    k.angle += dt * rate;

    // Cập nhật ma trận hiệp phương sai
    k.P[0][0] += dt * (dt * k.P[1][1] - k.P[0][1] - k.P[1][0] + Q_angle);
    k.P[0][1] -= dt * k.P[1][1];
    k.P[1][0] -= dt * k.P[1][1];
    k.P[1][1] += Q_bias * dt;

    // Tính toán Kalman Gain
    float S = k.P[0][0] + R_measure;
    float K[2];
    K[0] = k.P[0][0] / S;
    K[1] = k.P[1][0] / S;

    // Cập nhật góc và độ lệch
    float y = newAngle - k.angle;
    k.angle += K[0] * y;
    k.bias += K[1] * y;

    // Cập nhật ma trận hiệp phương sai
    float P00_temp = k.P[0][0];
    float P01_temp = k.P[0][1];

    k.P[0][0] -= K[0] * P00_temp;
    k.P[0][1] -= K[0] * P01_temp;
    k.P[1][0] -= K[1] * P00_temp;
    k.P[1][1] -= K[1] * P01_temp;

    return k.angle;
}


void setup(void) {
  Serial.begin(115200);
  while (!Serial) {
    delay(10); // will pause Zero, Leonardo, etc until serial console opens
  }

  // Try to initialize!
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.println("");
  // Khởi tạo bộ lọc Kalman cho pitch và roll
    KalmanInit(kalmanPitch);
    KalmanInit(kalmanRoll);
  delay(100);
  // Lưu thời gian hiện tại
    lastTime = millis();
}

void loop() {

  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
// Tính toán khoảng thời gian giữa hai lần đo
  unsigned long currentTime = millis();
  float dt = (currentTime - lastTime) / 1000.0;
  lastTime = currentTime;


  // Lấy giá trị gia tốc trên các trục
  float ax = a.acceleration.x;
  float ay = a.acceleration.y;
  float az = a.acceleration.z;
  //Lấy giá trị vận tốc
  double gyroX = g.gyro.x;
  double gyroY = g.gyro.y;

// Tốc độ góc từ con quay hồi chuyển
  float gyroPitchRate = gyroX * 180.0 / PI;
  float gyroRollRate = gyroY * 180.0 / PI;

  // Tính toán góc pitch và roll
  float accPitch = atan2(ay, sqrt(ax * ax + az * az)) * 180.0 / PI;
  float accRoll = atan2(-ax, sqrt(ay * ay + az * az)) * 180.0 / PI;

// Áp dụng bộ lọc Kalman
  float pitch = KalmanUpdate(kalmanPitch, accPitch, gyroPitchRate, dt);
  float roll = KalmanUpdate(kalmanRoll, accRoll, gyroRollRate, dt);

  // Hiển thị kết quả
  Serial.print("PitchY:");
  Serial.print(pitch);
  //Serial.print(" độ, ");
  Serial.print(" RollX:");
  Serial.print(roll);
  //Serial.println(" độ");
  Serial.println("");

  delay(10);
}