#include <EEPROM.h>
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

// Définition de l'écran LCD
LiquidCrystal_I2C lcd(0x26, 16, 2);

// Définition du clavier matriciel
constexpr byte ROWS = 4, COLS = 4;
const char kp4x4Keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};
byte rowKp4x4Pin[ROWS] = {9, 8, 7, 6};
byte colKp4x4Pin[COLS] = {5, 4, 3, 2};
Keypad keypad = Keypad(makeKeymap(kp4x4Keys), rowKp4x4Pin, colKp4x4Pin, ROWS, COLS);

// Codes d'accès
constexpr int MAX_CODE_LENGTH = 4;
const char adminCode[MAX_CODE_LENGTH + 1] = "2409"; // Code admin
String userCode, code_Gayri, code_Jourdes, code_Monserrat;

// Variables
char code_saisi[MAX_CODE_LENGTH + 1] = {0};
int compteur = 0;

void setup() {
    Serial.begin(9600);
    lcd.init();
    lcd.backlight();

    // Lire les codes utilisateurs stockés dans l'EEPROM
    userCode = readCodeFromEEPROM(60);
    code_Gayri = readCodeFromEEPROM(0);
    code_Jourdes = readCodeFromEEPROM(20);
    code_Monserrat = readCodeFromEEPROM(40);

    if (code_Gayri == "" || code_Jourdes == "" || code_Monserrat == "") {
        code_Gayri = "1010";
        code_Jourdes = "2020";
        code_Monserrat = "3030";
        saveCodeToEEPROM(0, code_Gayri);
        saveCodeToEEPROM(20, code_Jourdes);
        saveCodeToEEPROM(40, code_Monserrat);
    }

    if (userCode == "") {
        userCode = "0000";
        saveCodeToEEPROM(60, userCode);
    }
}

void loop() {
    lcd.setCursor(0, 0);
    lcd.print("Entrez le code:");

    char customKey = keypad.getKey();
    if (customKey) {
        if (customKey == 'A') { // Réinitialisation
            resetInput();
            return;
        }

        if (compteur < MAX_CODE_LENGTH) {
            lcd.setCursor(compteur, 1);
            lcd.print('X');  // Affichage masqué du code
            code_saisi[compteur++] = customKey;
        }

        if (compteur == MAX_CODE_LENGTH) { // Vérification du code
            code_saisi[MAX_CODE_LENGTH] = '\0';
            String code_saisi_str = String(code_saisi);

            lcd.clear();
            if (code_saisi_str.equals(adminCode)) {
                lcd.print("Mode admin actif");
                delay(2000);
                enterAdminMode();
            } else if (code_saisi_str.equals(userCode)) {
                lcd.print("Acces autorise");
                delay(3000);
            } else if (code_saisi_str.equals(code_Gayri)) {
                lcd.print("Bonjour M.Gayri");
                delay(3000);
            } else if (code_saisi_str.equals(code_Jourdes)) {
                lcd.print("Bonjour M.Jourdes");
                delay(3000);
            } else if (code_saisi_str.equals(code_Monserrat)) {
                lcd.print("Bonjour M.Monserrat");
                delay(3000);
            } else {
                lcd.print("Code refuse");
                delay(2000);
            }
            resetInput();
        }
    }
}

void enterAdminMode() {
    lcd.clear();
    lcd.print("1. Modifier code");
    lcd.setCursor(0, 1);
    lcd.print("2. Creer un code");

    while (true) {
        char customKey = keypad.getKey();
        if (customKey == '1') {
            lcd.clear();
            lcd.print("1.Gayri 2.Jourdes");
            lcd.setCursor(0, 1);
            lcd.print("3. Monserrat");

            while (true) {
                char selection = keypad.getKey();
                if (selection == '1') {
                    lcd.clear();
                    lcd.print("Nouv. Code:");
                    code_Gayri = getInput();
                    saveCodeToEEPROM(0, code_Gayri);
                    break;
                } else if (selection == '2') {
                    lcd.clear();
                    lcd.print("Nouv. Code:");
                    code_Jourdes = getInput();
                    saveCodeToEEPROM(20, code_Jourdes);
                    break;
                } else if (selection == '3') {
                    lcd.clear();
                    lcd.print("Nouv. Code:");
                    code_Monserrat = getInput();
                    saveCodeToEEPROM(40, code_Monserrat);
                    break;
                }
            }
            break;
        } else if (customKey == '2') {
            lcd.clear();
            lcd.print("Nouv. code:");
            userCode = getInput();
            saveCodeToEEPROM(60, userCode);
            break;
        }
    }
}

void resetInput() {
    memset(code_saisi, 0, sizeof(code_saisi));
    compteur = 0;
    lcd.clear();
}

String getInput() {
    String input = "";
    while (input.length() < MAX_CODE_LENGTH) {
        char key = keypad.getKey();
        if (key) {
            lcd.print('X');
            input += key;
        }
    }
    return input;
}
