# Практическая работа 5 - Работа с протоколом I2C

## Цель

Соединить две Arduino по протоколу I2C . К master'у подключить потенциометр и пьезоэлемент, к slave'у подключить светодиод. 

1. Считать и отправить slave'у данные потенциометра, и в зависимости от его значения установить соответствующую яркость светодиода. 
2. Если яркость светодиода > 0, начинается отсчет времени. 
3. Если с момента зажигания светодиода прошло 5 секунд, пьезоэлемент, подключенный к master'у, должен начать воспроизводить любую мелодию

## Схема подключения - [Wokwi](https://wokwi.com/projects/413907449414970369) | [Tinkercad](https://www.tinkercad.com/things/4LP0C20YbR5-i2c/editel?returnTo=https%3A%2F%2Fwww.tinkercad.com%2Fdashboard%2Fdesigns%2Fcircuits&sharecode=xKJyMV7dau7SYMp8V3X8EXWvYB8p3LMI1CG0h9wbfZ8&authuser=0)

<p align="center">
      <img src="https://github.com/user-attachments/assets/18cdf1c2-6db3-4116-ac42-1aa95fb6e951" alt="Схема подключения" width="500">
</p>

<p align="center">
      <img src="https://github.com/user-attachments/assets/f726bfa2-232b-464e-abde-443b8abb5b87" alt="Схема подключения" width="500">
</p>

> ⚠️ **Внимание!** Скетч в Wokwi не работает, так как Wokwi не поддерживает I2C. Скетч там только для хранения.

## Вывод
Были соединены две Arduino по протоколу I2C. К master'у были подключены потенциометр и пьезоэлемент, к slave'у подключен светодиод. При яркости светодиода > 10, начинается отсчет времени, от 5 секунд, пьезоэлемент, подключенный к master'у, начинает воспроизводить мелодию.
