#include <Keypad.h>

#include <LiquidCrystal_I2C.h>

#include <EEPROM.h>


 

String code_gayri;

String code_jourdes;

String code_monserrat;

const String adminCode = "1109";

 

const byte ROWS = 4;

const byte COLS = 4;

 

char keys[ROWS][COLS] = {

    {'1', '2', '3', 'A'},

    {'4', '5', '6', 'B'},

    {'7', '8', '9', 'C'},

    {'*', '0', '#', 'D'}

};

 

byte rowPins[ROWS] = {8, 7, 6, 5};

byte colPins[COLS] = {4, 3, 2, 1};

 

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

 

const int ledPin = 10;

const int buttonPin = 9;

const int lockPin = 11;

const int internalButtonPin = 12;

bool ledState = false;

LiquidCrystal_I2C lcd(0x27, 16, 2);

 

void setup() {

    lcd.init();

    lcd.backlight();

    lcd.setCursor(0, 0);

    lcd.clear();

    lcd.setCursor(0, 0);

    lcd.print("Entrez code:");

    pinMode(ledPin, OUTPUT);

    pinMode(buttonPin, INPUT_PULLUP);

    pinMode(lockPin, OUTPUT);

    pinMode(internalButtonPin, INPUT_PULLUP);

    digitalWrite(ledPin, LOW);

    digitalWrite(lockPin, LOW);

 

    code_gayri = readCodeFromEEPROM(0);

    code_jourdes = readCodeFromEEPROM(20);

    code_monserrat = readCodeFromEEPROM(40);

    if (code_gayri == "" || code_jourdes == "" || code_monserrat == "") {

        code_gayri = "1234";

        code_jourdes = "5678";

        code_monserrat = "9012";

        saveCodeToEEPROM(0, code_gayri);

        saveCodeToEEPROM(20, code_jourdes);

        saveCodeToEEPROM(40, code_monserrat);

    }

}

 

void loop() {

    static String inputCode = "";

    char key = keypad.getKey();

 

    if (key) {

        if (key == '#') {

            processCode(inputCode);

            inputCode = "";

        } else if (key == '*') {

            inputCode = "";

            lcd.clear();

            lcd.setCursor(0, 0);

            lcd.print("Entrez code:");

        } else {

            inputCode += key;

            lcd.setCursor(inputCode.length() - 1, 1);

            lcd.print('X');

        }

    }

 

    if (ledState && digitalRead(buttonPin) == LOW) {

        digitalWrite(ledPin, LOW);

        ledState = false;

    }

 

    if (digitalRead(internalButtonPin) == LOW) {

        activateLock();

        delay(500);

    }

}

 

void processCode(String inputCode) {

    lcd.clear();

    if (inputCode == code_gayri) {

        lcd.print("Bonjour Gayri");

        activateLed();

        activateLock();

    } else if (inputCode == code_jourdes) {

        lcd.print("Bonjour Jourdes");

        activateLed();

        activateLock();

    } else if (inputCode == code_monserrat) {

        lcd.print("Bonjour");

        lcd.setCursor(0, 1);

        lcd.print("Monserrat");

        activateLed();

        activateLock();

    } else if (inputCode == adminCode) {

        enterAdminMode();

    } else {

        lcd.print("Code mauvais");

        delay(2000);

    }

    lcd.clear();

    lcd.setCursor(0, 0);

    lcd.print("Entrez code:");

}

 

void enterAdminMode() {

    lcd.clear();

    lcd.print("Admin Mode");

    delay(1000);

    lcd.clear();

    lcd.print("1: Gayri");

    lcd.setCursor(0, 1);

    lcd.print("2: Jourdes 3: Monserra");

    String choice = getInput();

    lcd.clear();

    lcd.print("Nouveau code:");

 

    if (choice == "1") {

        code_gayri = getInput();

        saveCodeToEEPROM(0, code_gayri);

        lcd.clear();

        lcd.print("Code Gayri OK");

    } else if (choice == "2") {

        code_jourdes = getInput();

        saveCodeToEEPROM(20, code_jourdes);

        lcd.clear();

        lcd.print("Code Jourdes OK");

    } else if (choice == "3") {

        code_monserrat = getInput();

        saveCodeToEEPROM(40, code_monserrat);

        lcd.clear();

        lcd.print("Code Monserrat OK");

    } else {

        lcd.clear();

        lcd.print("Mauvais choix");

    }

    delay(2000);

    lcd.clear();

    lcd.setCursor(0, 0);

    lcd.print("Entrez code:");

}

 

String getInput() {

    String input = "";

    char key;

    while (true) {

        key = keypad.getKey();

        if (key) {

            if (key == '#') {

                return input;

            } else if (key == '*') {

                input = "";

                lcd.setCursor(0, 1);

                lcd.print("                ");

            } else {

                input += key;

                lcd.setCursor(input.length() - 1, 1);

                lcd.print('X');

            }

        }

    }

}

 

void activateLed() {

    digitalWrite(ledPin, HIGH);

    ledState = true;

}

 

void activateLock() {

    digitalWrite(lockPin, HIGH);

    delay(5000);

    digitalWrite(lockPin, LOW);

}

 

void saveCodeToEEPROM(int address, String code) {

    for (int i = 0; i < code.length(); i++) {

        EEPROM.write(address + i, code[i]);

    }

    EEPROM.write(address + code.length(), '\0');

}

 

String readCodeFromEEPROM(int address) {

    String code = "";

    char ch;

    for (int i = 0; i < 20; i++) {

        ch = EEPROM.read(address + i);

        if (ch == '\0') break;

        code += ch;

    }

    return code;

}