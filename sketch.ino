#include <Wire.h>
#include <MPU6050.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
MPU6050 mpu;

const int buzzerPin = 7;
const int ledPin = 8;

// ---- Sensitivity Thresholds ----
float accelThreshold = 2.5;   // g-force for crash
float gyroThreshold = 250;    // degrees/sec for rollover

void setup() {
  Serial.begin(9600);
  Wire.begin();

  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("OLED failed to initialize");
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println("Initializing MPU6050...");
  display.display();

  mpu.initialize();
  if (mpu.testConnection()) {
    Serial.println("MPU6050 connected successfully!");
    display.clearDisplay();
    display.setCursor(0, 10);
    display.println("MPU6050 Ready!");
    display.display();
  } else {
    Serial.println("MPU6050 connection failed!");
    while (1);
  }

  delay(1000);
  display.clearDisplay();
}

void loop() {
  int16_t accX, accY, accZ;
  int16_t gyroX, gyroY, gyroZ;

  mpu.getMotion6(&accX, &accY, &accZ, &gyroX, &gyroY, &gyroZ);

  float ax = accX / 16384.0;
  float ay = accY / 16384.0;
  float az = accZ / 16384.0;

  float gx = gyroX / 131.0;
  float gy = gyroY / 131.0;
  float gz = gyroZ / 131.0;

  float accelMagnitude = sqrt(ax * ax + ay * ay + az * az);
  float gyroMagnitude = sqrt(gx * gx + gy * gy + gz * gz);

  // Print sensor values to Serial Monitor
  Serial.print("Acceleration (g): ");
  Serial.print(accelMagnitude, 2);
  Serial.print("\tGyro (°/s): ");
  Serial.print(gyroMagnitude, 1);
  Serial.print("\tTemp (approx): ");
  Serial.println(mpu.getTemperature() / 340.00 + 36.53, 2);

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Accel(g): ");
  display.println(accelMagnitude, 2);
  display.print("Gyro(d/s): ");
  display.println(gyroMagnitude, 1);

  bool crashDetected = false;
  bool rolloverDetected = false;

  // Check for crash (impact)
  if (accelMagnitude > accelThreshold) {
    crashDetected = true;
  }

  // Check for rollover (rotation)
  if (gyroMagnitude > gyroThreshold) {
    rolloverDetected = true;
  }

  // Handle alerts
  if (crashDetected || rolloverDetected) {
    digitalWrite(ledPin, HIGH);
    tone(buzzerPin, 1000);

    display.setTextSize(2);
    display.setCursor(5, 40);

    if (crashDetected && rolloverDetected) {
      Serial.println("CRASH + RAPID ROTATION DETECTED");
      display.println("CRASH+ROLL!");
    } else if (crashDetected) {
      Serial.println("CRASH DETECTED");
      display.println("CRASH!");
    } else if (rolloverDetected) {
      Serial.println("RAPID ROTATION DETECTED");
      display.println("ROLLOVER!");
    }
  } else {
    digitalWrite(ledPin, LOW);
    noTone(buzzerPin);
    display.setTextSize(1);
    display.setCursor(10, 40);
    display.println("Status: Normal");
  }

  display.display();
  delay(500);
}
