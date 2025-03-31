#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "<..json..>"; // Название сети WiFi
const char *password = "12345679"; // Пароль для подключения

const char* ledOnAddress = "/led/on";

const char* ledOffAddress = "/led/off";
ESP8266WebServer server(80); // Создаем веб сервер на 80 порту

// Метод формирует стартовую страницу 192.168.4.1
// Выводит в браузер текущее состояние диода и две кнопки
void handleRoot() {
  String s = "<h1>LED</h1>";
  s += "<h2>LED On ";
  s += "LED Off</h2>";
  server.send(200, "text/html", s);
}

// Метод включения диода
void ledOn() {
  digitalWrite(BUILTIN_LED, HIGH);
  // Перенаправление обратно на стартовую страницу
  server.sendHeader("Location", String("/"), true);
  server.send ( 302, "text/plain", "");
}

// Метод выключения диода
void ledOff() {
  digitalWrite(BUILTIN_LED, LOW);
  // Перенаправление обратно на стартовую страницу
  server.sendHeader("Location", String("/"), true);
  server.send ( 302, "text/plain", "");
}

// Функция настройки. Выполняется один раз при запуске
void setup() {
  delay(1000); // Ждём секунду
  WiFi.softAP(ssid, password); // Создаём точку WiFi
  // Указываем по каким роутам какие методы запускать
  server.on("/", handleRoot);
  server.on("/led/on", ledOff);
  server.on("/led/off", ledOn);6s
  server.begin();
  // Диод по умолчанию выключен
  pinMode(BUILTIN_LED, OUTPUT);
  digitalWrite(BUILTIN_LED, LOW);
}

// Основной цикл программы
void loop() {
  // Ждём подключения
  server.handleClient();
}