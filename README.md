# Практические работы по Arduino
Практические работы по предмету 3-го курса "Cистемное программирование" на C++ с использованием Arduino и других технологий.

Навигация:

1 семестр:
* [Практическая работа 1](https://github.com/Archive-of-practical-work-for-the-MPT/Homework-Arduino/tree/Lightshow) - Реализация светофора на Arduino
* [Практическая работа 2](https://github.com/Archive-of-practical-work-for-the-MPT/Homework-Arduino/tree/UART) - Работа со входными данными. Интерфейс UART
* [Практическая работа 3](https://github.com/Archive-of-practical-work-for-the-MPT/Homework-Arduino/tree/EEPROM) - Работа с EEPROM
* [Практическая работа 4](https://github.com/Archive-of-practical-work-for-the-MPT/Homework-Arduino/tree/music) - Работа с Arduino с помощью C#
* [Практическая работа 5](https://github.com/Archive-of-practical-work-for-the-MPT/Homework-Arduino/tree/I2C) - Работа с протоколом I2C
* [Практическая работа 6](https://github.com/Archive-of-practical-work-for-the-MPT/Homework-Arduino/tree/SPI) - Работа со сдвиговым регистром и семисегментным индикатором
* [Практическая работа 7](https://github.com/Archive-of-practical-work-for-the-MPT/Homework-Arduino/tree/display) - Мониторинг системы. Вывод данных на LCD-дисплей

2 семестр:
* [Практическая работа 8](https://github.com/Archive-of-practical-work-for-the-MPT/Homework-Arduino/tree/Servo) - Управление сервоприводом и шаговым мотором
* [Практическая работа 9](https://github.com/Archive-of-practical-work-for-the-MPT/Homework-Arduino) - Работа с WiFi-модулем WeMos D1 R2
* [Практическая работа 10](https://github.com/Archive-of-practical-work-for-the-MPT/Homework-Arduino/tree/WeMosBot) - Работа с WiFi-модулем WeMos D1 R2. Разработка Telegram-бота
* [Практическая работа 11](https://github.com/Archive-of-practical-work-for-the-MPT/Homework-Arduino/tree/Bluetooth) - Создание гирлянды с настройкой через Bluetooth-модуль HC-06
* [Практическая работа 12](https://github.com/Archive-of-practical-work-for-the-MPT/Homework-Arduino/tree/RFID) - Работа с RFID-модулем
* [Практическая работа 13](https://github.com/Archive-of-practical-work-for-the-MPT/Homework-Arduino/tree/unity) - Взаимодействие Arduino и Unity

# Практическая работа 9 - Интерфейс UART

## Описание
Проект сделан на основе [статьи](https://randomnerdtutorials.com/esp8266-nodemcu-web-server-websocket-sliders).

## Цель

Работа с WiFi-модулем WeMos D1 R2
1. Повторить код из лекции 10.
2. Ознакомиться и повторить проект со слайдерами из статьи
3. Показать работу на железе на паре.

## Демонстрация

<p align="center">
      <img src="https://github.com/user-attachments/assets/111642dd-f766-4c69-8623-e4dbdabec649" alt="Сайт" width="700">
</p>

<p align="center">
      <img src="https://github.com/user-attachments/assets/897f8168-def9-4c03-b5a0-73a0f01f4e16" alt="Результат" width="500">
</p>

## Вывод
Была собрана схему с WiFi-модулем WeMos D1 R2, после чего подключились к лампочкам через веб приложением для изменения их яркости, и была продемонстрирована работу на железе.
