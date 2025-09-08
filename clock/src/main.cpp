// Learn about the ESP32 WiFi simulation in
// https://docs.wokwi.com/guides/esp32-wifi

#include "Wire.h"
#include "hal/buttonModule.h"
#include "hal/Clockmodule.h"
#include "hal/DHTmodule.h"
#include "hal/displayModule.h"
#include "hal/TftDisplay.h"


#define DHT_PIN 17
#define DHTTYPE DHT22
#define BUTTON_PIN 15
#define DATA_PIN 23 //25
#define CLOCK_PIN 18 //33 
#define LATCH_PIN  5 //32
#define BUTTON_MODE_PIN 26
#define BUTTON_UP_PIN   27
#define BUZZER_PIN      14
const byte digitmap[13] = {
  B00111111, // 0
  B00000110, // 1
  B01011011, // 2
  B01001111, // 3
  B01100110, // 4
  B01101101, // 5
  B01111101, // 6
  B00000111, // 7
  B01111111, // 8
  B01101111, // 9
  B01100011, // 'do'
  B00111001, // 'C'
  B01011100  // 'do duoi'
};

Clockmodule clockmodule;
DHTmodule dhtmodule(DHT_PIN, DHTTYPE); 
displayModule display(DATA_PIN, CLOCK_PIN, LATCH_PIN);
ButtonModule button(BUTTON_PIN);

#define TFT_CS  5
#define TFT_DC  2
#define TFT_RST 4

ButtonModule buttonMode(BUTTON_MODE_PIN);
ButtonModule buttonUp(BUTTON_UP_PIN);

uint8_t alarmHour = 6;
uint8_t alarmMinute = 30;
bool selectingHour = true;  // true: chỉnh giờ, false: chỉnh phút

void ringBuzzer();  // Prototype
int mode = 0;
unsigned long lastUpdate = 0;
const unsigned long interval = 1000;
void ringBuzzer() {
  for (int i = 0; i < 3; i++) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(200);
    digitalWrite(BUZZER_PIN, LOW);
    delay(200);
  }
}

void setup() {
  Serial.begin(115200);
  buttonMode.begin();
  buttonUp.begin();

  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW); // Tắt còi lúc đầu

  TftDisplay::getInstance().setup(TFT_CS, TFT_DC, TFT_RST);
  TftDisplay::getInstance().showTimeSetting(alarmHour, alarmMinute, selectingHour);
  clockmodule.begin();
  dhtmodule.begin();
  display.begin();
  button.begin();
  Wire.begin(21, 22);
  DateTime now = clockmodule.getTime();
  Serial.printf("⏰ Thời gian hiện tại: %02d:%02d:%02d %02d/%02d/%04d\n",
              now.hour(), now.minute(), now.second(),
              now.day(), now.month(), now.year());

}

void loop() {
  static int lastTemp = -100;
  static int lastHumi = -100;
  static int lastHour = -1;
  static int lastMinute = -1;
  static int lastDay = -1;
  static int lastMonth = -1;
  static int lastYear = -1;

  if(button.wasPress()) {
    mode++;
    if(mode >= 5) {
      mode = 0;
    }
    Serial.print("Mode changed to: ");
    Serial.println(mode);

    // Hiển thị lên Serial Monitor theo mode
    switch(mode) {
      case 0:
        Serial.printf("🕒 Thời gian: %02d:%02d\n", lastHour, lastMinute);
        break;
      case 1:
        Serial.printf("📅 Ngày: %02d/%02d\n", lastDay, lastMonth);
        break;
      case 2:
        Serial.printf("📆 Năm: %04d\n", lastYear);
        break;
      case 3:
        Serial.printf("🌡️ Nhiệt độ: %d°C\n", lastTemp);
        break;
      case 4:
        Serial.printf("💧 Độ ẩm: %d%%\n", lastHumi);
        break;
    }
  }

  // Cập nhật dữ liệu mỗi giây
  if (millis() - lastUpdate >= interval) {
    lastUpdate = millis();
    DateTime now = clockmodule.getTime();
    lastTemp = dhtmodule.getTemperature();
    lastHumi = dhtmodule.getHumidity();
    lastHour = now.hour();
    lastMinute = now.minute();
    lastDay = now.day();
    lastMonth = now.month();
    lastYear = now.year();
  }

  // Hiển thị lên LED 7 đoạn
//   switch(mode) {
//     case 0:
//       display.showDigits(
//         digitmap[lastHour / 10],
//         digitmap[lastHour % 10],
//         digitmap[lastMinute / 10],
//         digitmap[lastMinute % 10]
//       );
//       break;

//     case 1:
//       display.showDigits(
//         digitmap[lastDay / 10],
//         digitmap[lastDay % 10],
//         digitmap[lastMonth / 10],
//         digitmap[lastMonth % 10]
//       );
//       break;

//     case 2:
//       display.showDigits(
//         digitmap[(lastYear / 1000) % 10],
//         digitmap[(lastYear / 100) % 10],
//         digitmap[(lastYear / 10) % 10],
//         digitmap[lastYear % 10]
//       );
//       break;

//     case 3:
//       display.showDigits(
//         digitmap[lastTemp / 10],
//         digitmap[lastTemp % 10],
//         digitmap[10],
//         digitmap[11]
//       );
//       break;

//     case 4:
//       display.showDigits(
//         digitmap[lastHumi / 10],
//         digitmap[lastHumi % 10],
//         digitmap[10],
//         digitmap[12]
//       );
//       break;
//   }

// display.update();

static int prevMode = -1;

if (mode != prevMode) {
  prevMode = mode;  // cập nhật mode hiện tại

  switch(mode) {
    case 0:
      display.showDigits(
        digitmap[lastHour / 10],
        digitmap[lastHour % 10],
        digitmap[lastMinute / 10],
        digitmap[lastMinute % 10]
      );
      break;

    case 1:
      display.showDigits(
        digitmap[lastDay / 10],
        digitmap[lastDay % 10],
        digitmap[lastMonth / 10],
        digitmap[lastMonth % 10]
      );
      break;

    case 2:
      display.showDigits(
        digitmap[(lastYear / 1000) % 10],
        digitmap[(lastYear / 100) % 10],
        digitmap[(lastYear / 10) % 10],
        digitmap[lastYear % 10]
      );
      break;

    case 3:
      display.showDigits(
        digitmap[lastTemp / 10],
        digitmap[lastTemp % 10],
        digitmap[10],
        digitmap[11]
      );
      break;

    case 4:
      display.showDigits(
        digitmap[lastHumi / 10],
        digitmap[lastHumi % 10],
        digitmap[10],
        digitmap[12]
      );
      break;
  }
}

// Kiểm tra để tắt sau 1 giây
display.update();

if (buttonMode.wasPress()) {
    selectingHour = !selectingHour;
    TftDisplay::getInstance().showTimeSetting(alarmHour, alarmMinute, selectingHour);
    delay(200);
  }

  if (buttonUp.wasPress()) {
    if (selectingHour) {
      alarmHour = (alarmHour + 1) % 24;
    } else {
      alarmMinute = (alarmMinute + 1) % 60;
    }
    TftDisplay::getInstance().showTimeSetting(alarmHour, alarmMinute, selectingHour);
    delay(200);
  }

  static unsigned long lastCheck = 0;
  if (millis() - lastCheck > 1000) {
    lastCheck = millis();

    /// So sánh thời gian thực từ RTC
if (lastHour == alarmHour && lastMinute == alarmMinute) {
  ringBuzzer();
}
  }
}
