/*******************************************************************
    Телеграм Бот для модуля WeMos D1 R2 c МК ESP8266.
    Предназначен для вкл./выкл. светодиода на плате.
 *******************************************************************/

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <Servo.h>

// Имя и пароль вашей сети Wifi 
#define WIFI_SSID "<..python..>"
#define WIFI_PASSWORD "12345679"
// Телеграм Бот Токен, можно получить у бота @BotFather в Телеграмм 
#define BOT_TOKEN "8109941833:AAENuC8qYv7QLLYD-PCukzLWkZbW3O8Rtb8"  // НЕ ДЕЛИТЕСЬ ИМ


const unsigned long BOT_MTBS = 1000; // Через сколько времени проверять сообщения

X509List cert(TELEGRAM_CERTIFICATE_ROOT);
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);
unsigned long bot_lasttime; // Последнее время сканирования сообщения

const int ledPin = D7;
int ledStatus = 0;
int servoStatus = 0;

const int servoPin = D4;
Servo servo;

void handleNewMessages(int numNewMessages)
{
  Serial.print("Обработка нового сообщения ");
  Serial.println(numNewMessages);

  for (int i = 0; i < numNewMessages; i++)
  {
    String chat_id = bot.messages[i].chat_id;
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;

    if (text == "/ledon")
    {
      analogWrite(ledPin, 255); // Включить светодиод на плате
      ledStatus = 1;
      bot.sendMessage(chat_id, "Светодиод включен - ON", "");
    }

    if (text == "/ledoff")
    {
      ledStatus = 0;
      analogWrite(ledPin, 0); // Выключить светодиод на плате
      bot.sendMessage(chat_id, "Светодиод выключен - OFF", "");
    }

    if (text == "/ledChangePower")
    {
      if (analogRead(ledPin) / 4 >= 240) // Если светодиод включен и яркость 255
      {
        analogWrite(ledPin, 128); // Установить яркость на 128
        bot.sendMessage(chat_id, "Яркость светодиода изменена на 50%", "");
      }
      else if (analogRead(ledPin) / 4 <= 240) // Если светодиод включен и яркость 128
      {
        analogWrite(ledPin, 255); // Установить яркость на 255
        bot.sendMessage(chat_id, "Яркость светодиода изменена на 100%", "");
      }
      else if (analogRead(ledPin) == 0)
      {
        bot.sendMessage(chat_id, "Светодиод выключен. Включите его перед изменением яркости.", "");
      }
    }

    if (text == "/Open")
    {
      servoStatus = 180;
      servo.write(180);
      bot.sendMessage(chat_id, "Сервопривод открыт - ON", "");
    }

    if (text == "/Close")
    {
      servoStatus = 0;
      servo.write(0);
      bot.sendMessage(chat_id, "Сервопривод закрыт - OFF", "");
    }

    if (text == "/status")
    {
      bot.sendMessage(chat_id, ledStatus ? "Светодиод включен - ON" : "Светодиод выключен - OFF", "");
      bot.sendMessage(chat_id, servoStatus ? "Сервопривод открыт - ON" : "Сервопривод закрыт - OFF", "");
    }

    if (text == "/start" || text == "")
    {
      String welcome = "Привет, я WeMos Lighting Bot. Я умею преключать светодиод на модуле WeMos D2 R1, " + from_name + ".\n";
      welcome += "Команды:\n\n";
      welcome += "/ledon - Переключает светодиод в состояние ON\n";
      welcome += "/ledoff - Переключает светодиод в состояние OFF\n";
      welcome += "/ledChangePower - Изменяет яркость светодиода на 50% OFF\n";
      welcome += "/Open - Открывает сервопривод\n";
      welcome += "/Close - Закрывает сервопривод\n";
      welcome += "/status - Возвращает текущее состояние светодиода\n";
      bot.sendMessage(chat_id, welcome, "Markdown");
    }
  }
}


void setup()
{
  Serial.begin(115200);
  Serial.println();

  pinMode(ledPin, OUTPUT); // Настраиваем пин ledPin на выход
  delay(10);
  analogWrite(ledPin, 255); // По умолчанию светодиод выключен

  servo.attach(servoPin);  // Подключаем сервопривод к указанному пину

  configTime(0, 0, "pool.ntp.org");      // получаем UTC time
  secured_client.setTrustAnchors(&cert); // Добавление root сертификата для api.telegram.org
  Serial.print("Подключение к сети Wifi SSID: ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }
  Serial.print("\nWiFi подключен. IP адрес: ");
  Serial.println(WiFi.localIP());

  Serial.print("Время подключения: ");
  time_t now = time(nullptr);
  while (now < 24 * 3600)
  {
    Serial.print(".");
    delay(100);
    now = time(nullptr);
  }
  Serial.println(now);
}

void loop()
{
  if (millis() - bot_lasttime > BOT_MTBS)
  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages)
    {
      Serial.println("Получен ответ");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }

    bot_lasttime = millis();
  }
}
