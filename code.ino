#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

// Définition de l'écran LCD
LiquidCrystal_I2C lcd(0x26, 16, 2);

// Définition du clavier matriciel
#define ROWS 4
#define COLS 4
const char kp4x4Keys[ROWS][COLS]  = {{'1', '2', '3', 'A'}, {'4', '5', '6', 'B'}, {'7', '8', '9', 'C'}, {'*', '0', '#', 'D'}};
byte rowKp4x4Pin[ROWS] = {9, 8, 7, 6};
byte colKp4x4Pin[COLS] = {5, 4, 3, 2};
Keypad keypad = Keypad(makeKeymap(kp4x4Keys), rowKp4x4Pin, colKp4x4Pin, ROWS, COLS);

// Définition des LEDs
const int led_rouge = 11;
const int led_verte = 12;

// Variables pour le code
char code[5];
const char code_admin[4] = {'2', '4', '0', '9'};
const char code_Gayri[4] = {'8', '6', '2', '4'};
const char code_Jourdes[4] = {'2', '4', '8', '6'};
int compteur = 0;
int col = 0;

void setup() {
    // Initialisation du LCD
    lcd.init();
    lcd.backlight();
    
    // Initialisation des LEDs
    pinMode(led_rouge, OUTPUT);
    pinMode(led_verte, OUTPUT);
    digitalWrite(led_rouge, HIGH); // Coffre verrouillé par défaut
}

void loop() {
    lcd.setCursor(0, 0);
    lcd.print("Entrez le code : ");
    char customKey = keypad.getKey();
    if (customKey) {
        lcd.setCursor(col, 1);
        lcd.print("X");
        col++;
        code[compteur] = customKey;
        compteur++;

        if (customKey == 'A') { // Réinitialisation
            compteur = 0;
            col = 0;
            lcd.clear();
        }

        if (compteur == 4) { // Vérification du code
            if ((memcmp(code, code_admin, 4) == 0) || (memcmp(code, code_Gayri, 4) == 0) || (memcmp(code, code_Jourdes, 4) == 0)) {
                lcd.setCursor(4, 1);
                lcd.print(" ");
                lcd.setCursor(6, 1);
                lcd.print("Bonjour");
                delay(5000);
            } else {
                lcd.setCursor(5, 1);
                lcd.print("Code refusé");
                delay(2000);
            }
            compteur = 0;
            col = 0;
            lcd.clear();
        }
    }
}
