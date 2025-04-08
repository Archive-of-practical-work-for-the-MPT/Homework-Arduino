/*
  26.11.2024 Переработано: Merrcurys  https://github.com/Merrcurys
  Теперь скетч выводит только температуру CPU
   
  Общее описание:
  - Вывод температуры CPU на внешний LCD дисплей
  - Для работы необходима программа ОНМ, лежит в этой же папке
 
  Программа HardwareMonitorPlus  https://github.com/AlexGyver/PCdisplay
  - Запустить OpenHardwareMonitor.exe
  - Options/Serial/Config - проверить настройки параметров работы
    - PORT address - адрес порта, куда подключена Ардуина 
    - TEMP source - источник показаний температуры (процессор)
  - Options/Serial/Run - запуск соединения с Ардуиной 
*/


// <-------------------- БИБЛИОТЕКИ ----------------------
#include <string.h>              // библиотека строк 
#include <LiquidCrystal_I2C.h>  // библиотека дисплея 
// --------------------- БИБЛИОТЕКИ --------------------->


// <-------------------- ОПРЕДЕЛЕНИЕ ДРАЙВЕРА ДИСПЛЕЯ ----------------------
#define DRIVER_VERSION 1  // маркировка драйвера 4АТ - 0, 4T - 1
LiquidCrystal_I2C lcd(DRIVER_VERSION ? 0x27 : 0x3f, 16, 2);
// --------------------- ОПРЕДЕЛЕНИЕ ДРАЙВЕРА ДИСПЛЕЯ ---------------------> 


// <---------------- СИМВОЛЫ ДЛЯ LCD-ДИСПЛЕЯ ------------------
#define DEGREE 223

#define PROGRESS_BAR_LEFT_CAP 0
#define PROGRESS_BAR_EMPTY_SEGMENT 1
#define PROGRESS_BAR_FULL_SEGMENT 2
#define PROGRESS_BAR_RIGHT_CAP 3

const byte PROGRESS_BAR_LEFT_CAP_DATA[8] = {0b11111,  0b10000,  0b10000,  0b10000,  0b10000,  0b10000,  0b10000,  0b11111}; 
const byte PROGRESS_BAR_EMPTY_SEGMENT_DATA[8] = {0b11111, 0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111};
const byte PROGRESS_BAR_FULL_SEGMENT_DATA[8] = {0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111}; 
const byte PROGRESS_BAR_RIGHT_CAP_DATA[8] = {0b11111,  0b00001,  0b00001,  0b00001,  0b00001,  0b00001,  0b00001,  0b11111}; 
// ----------------- СИМВОЛЫ ДЛЯ LCD-ДИСПЛЕЯ ----------------->


// <---------------- ПЕРЕМЕННЫЕ ------------------
char inData[108];       // массив входных значений (СИМВОЛЫ) 
int PCdata[26];        // массив показаний с компьютера / PCdata[0] - CPU
byte index = 0; 
unsigned long timeout; 
bool clearDisplayFlag = true, connectLostFlag = true;
bool updateDisplayFlag;
// ----------------- ПЕРЕМЕННЫЕ ----------------->


// <------------------------------- SETUP / LOOP -------------------------------- 
void setup() { 
  Serial.begin(9600); 
  lcd.init(); 
  lcd.backlight(); 
  lcd.clear();
  createSymbols();
  drawLoadDisplay();     // отображаем загрузочный экран 
} 

void loop() { 
  // getCpuTemp();     // получаем температуру CPU
  getCpuTempPass();   // заглушка температуры CPU
  updateDisplay();   // обновляем температуру
  checkConnect();   // проверяем соединение
} 
// -------------------------------- SETUP / LOOP -------------------------------> 


// <------------------------- ВСПОМОГАТЕЛЬНЫЕ МЕТОДЫ --------------------------
void createSymbols() {
  /* Создание символов шкалы */
  lcd.createChar(PROGRESS_BAR_LEFT_CAP, PROGRESS_BAR_LEFT_CAP_DATA);
  lcd.createChar(PROGRESS_BAR_EMPTY_SEGMENT, PROGRESS_BAR_EMPTY_SEGMENT_DATA);
  lcd.createChar(PROGRESS_BAR_FULL_SEGMENT, PROGRESS_BAR_FULL_SEGMENT_DATA);
  lcd.createChar(PROGRESS_BAR_RIGHT_CAP, PROGRESS_BAR_RIGHT_CAP_DATA);
}

void drawLoadDisplay() {
  /* Отображение загрузочного экрана */
  lcd.setCursor(4, 0); lcd.print("CPU TEMP");
  lcd.setCursor(2, 1); lcd.print("by MERRCURYS");
  delay(3000); lcd.clear();
}

void getCpuTemp() {
  /* Парсинг данных с программы ОНМ */
  while (Serial.available() > 0) {
    char aChar = Serial.read();
    if (aChar == 'E') {
      char *p = inData;
      char *str;
      index = 0;
      while ((str = strtok_r(p, ";", &p)) != NULL) {
        PCdata[index++] = atoi(str);
      }
      index = 0;
      updateDisplayFlag = true;
    } else {
      inData[index++] = aChar;
      inData[index] = '\0';
    }
    timeout = millis();
    connectLostFlag = true;
  }
}

void getCpuTempPass() {
  /* Заглушка для тестирования без программы */
  updateDisplayFlag = true;
  PCdata[0] = 69; // заглушка температуры
  timeout = millis();
}

void updateDisplay() {
  /* Обновление дисплея */
  if (updateDisplayFlag) { 
    if (clearDisplayFlag) { 
      lcd.clear(); 
      clearDisplayFlag = false; 
    } 
    drawCpuTemperature(); 
    updateDisplayFlag = false; 
  } 
} 

void drawCpuTemperature() {
  /* Отображение температуры CPU и шкалы */
  lcd.setCursor(1, 0); lcd.print("PYTHON FOREVER"); // сюда можно любое слово
  lcd.setCursor(1, 1); lcd.print("CPU:"); lcd.print(PCdata[0]); lcd.write(DEGREE);

  // при +96С будет заполнена вся шкала, настраивайте через делитель
  byte line = min((byte)ceil(PCdata[0] / 16), (byte)6);
  lcd.setCursor(9, 1);

  lcd.write(line == 0 ? PROGRESS_BAR_LEFT_CAP : PROGRESS_BAR_FULL_SEGMENT);
  for (int n = 1; n < 5; n++) { 
    lcd.write(n < line ? PROGRESS_BAR_FULL_SEGMENT : PROGRESS_BAR_EMPTY_SEGMENT);
  } 
  lcd.write(line == 6 ? PROGRESS_BAR_FULL_SEGMENT : PROGRESS_BAR_RIGHT_CAP);
}

void checkConnect() {
  /* Отображение потери соединия */
  if (millis() - timeout > 5000 && connectLostFlag) {
    lcd.clear();
    lcd.setCursor(4, 0); lcd.print("COM PORT");
    lcd.setCursor(2, 1); lcd.print("DISCONNECTED");
    clearDisplayFlag = true;
  } else { connectLostFlag = false; }
}
// -------------------------- ВСПОМОГАТЕЛЬНЫЕ МЕТОДЫ --------------------------> 
