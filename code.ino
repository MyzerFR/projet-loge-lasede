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

// Définition des LEDs
constexpr int led_rouge = 11, led_verte = 12;

// Codes d'accès
constexpr const char* codes[] = {"2409", "8624", "2486", "0911"};
constexpr int code_length = 4;

// Variables
char code_saisi[code_length + 1] = {0};  // +1 pour le caractère nul '\0'
int compteur = 0;

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
    lcd.print("Entrez le code: ");
    
    char customKey = keypad.getKey();
    if (customKey) {
        if (customKey == 'A') { // Réinitialisation
            resetInput();
            return;
        }

        if (compteur < code_length) {
            lcd.setCursor(compteur, 1);
            lcd.print('X');  // Affichage masqué du code
            code_saisi[compteur++] = customKey;
        }

        if (compteur == code_length) { // Vérification du code
            code_saisi[code_length] = '\0'; // Ajout du caractère nul
            bool code_valide = false;

            for (const char* code : codes) {
                if (strcmp(code_saisi, code) == 0) {
                    code_valide = true;
                    break;
                }
            }

            lcd.setCursor(0, 1);
            lcd.print(code_valide ? "Accès autorisé" : "Code refusé");
            delay(code_valide ? 5000 : 2000);

            resetInput();
        }
    }
}

void resetInput() {
    memset(code_saisi, 0, sizeof(code_saisi)); // Réinitialisation du buffer
    compteur = 0;
    lcd.clear();
}
