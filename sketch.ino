#include <Arduino.h>
#include <SPI.h>
#include <MFRC522.h>
#include <EEPROM.h>

#define RST_PIN 9
#define SS_PIN 10
#define BUTTON_PIN 6
#define MAX_NAME_LEN 50

class Person {
private:
    char name[MAX_NAME_LEN + 1];
    int balance;

public:
    Person() : balance(0) {
        memset(name, 0, sizeof(name));
    }

    Person(const char* surname, int balance) : balance(balance) {
        strncpy(name, surname, MAX_NAME_LEN);
        name[MAX_NAME_LEN] = '\0';
    }

    const char* getSurname() const { return name; }
    int getBalance() const { return balance; }

    void updateBalance(int amount) {
        balance += amount;
    }

    void saveToEEPROM(int address) const {
        EEPROM.put(address, balance);
        address += sizeof(balance);
        
        uint16_t len = strnlen(name, MAX_NAME_LEN);
        EEPROM.put(address, len);
        address += sizeof(len);
        
        for(uint16_t i = 0; i < len; i++) {
            EEPROM.update(address + i, name[i]);
        }
    }

    void loadFromEEPROM(int address) {
        EEPROM.get(address, balance);
        address += sizeof(balance);
        
        uint16_t len;
        EEPROM.get(address, len);
        address += sizeof(len);
        
        len = min(len, (uint16_t)MAX_NAME_LEN);
        for(uint16_t i = 0; i < len; i++) {
            name[i] = EEPROM.read(address + i);
        }
        name[len] = '\0';
    }
};

MFRC522 mfrc522(SS_PIN, RST_PIN);
Person user;
const byte authorizedUID[4] = {0xE3, 0x4E, 0x90, 0xD};

void setup() {
    Serial.begin(9600);
    SPI.begin();
    mfrc522.PCD_Init();
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    user.loadFromEEPROM(0);

    if(strlen(user.getSurname()) != "Себежко") {
        user = Person("Себежко", 0);
        user.saveToEEPROM(0);
    }
}

void loop() {
    if(mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
        bool authorized = true;
        Serial.println("Прочитанный UID:");
        for(byte i = 0; i < 4; i++) {
            Serial.print(mfrc522.uid.uidByte[i], HEX);
            Serial.print(" ");
            if(mfrc522.uid.uidByte[i] != authorizedUID[i]) {
                authorized = false;
            }
        }
        Serial.println();

        if(authorized) {
            user.updateBalance(100);
            user.saveToEEPROM(0);
            Serial.println("Баланс пополнен!");
        } else {
            Serial.println("Ошибка авторизации: неверный UID");
            Serial.println("Авторизация не пройдена.");
        }

        mfrc522.PICC_HaltA();
    }

    if(digitalRead(BUTTON_PIN) == LOW) {
        delay(50);
        if(digitalRead(BUTTON_PIN) == LOW) {
            Serial.print("Пользователь: ");
            Serial.print(user.getSurname());
            Serial.print(", Баланс: ");
            Serial.println(user.getBalance());
            delay(200);
        }
    }
}