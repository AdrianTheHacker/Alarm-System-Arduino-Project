// Libraries used
#include <LiquidCrystal.h>

// Arming buttons variables
#define button 12
bool arm = false;
int buttonState = 0;

// Buzzer variables
#define buzzer A0

// LCD1602 module's global variables
const int rs = 7, en = 6, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// LED variables
#define armedLED 9 // Red LED
#define disarmedLED 10 // Green LED

// panic Button's variables
#define panicButton 13
int panicButtonState = 0;

// Ultrasonic distance sensor variables
#define echo 11
#define trig 8
int distance; 
long duration;

void setup() {
    Serial.begin(9600);
    lcd.begin(16, 2);

    // INPUTS
    pinMode(button, INPUT);
    pinMode(panicButton, INPUT);
    pinMode(echo, INPUT);

    // OUTPUTS
    pinMode(armedLED, OUTPUT);
    pinMode(disarmedLED, OUTPUT);
    pinMode(buzzer, OUTPUT);
    pinMode(trig, OUTPUT);
}

void loop() {
    turnLED();
}

void turnLED() {
    buttonState = digitalRead(button);

    // Toggles value of arm to its opposite value when button clicked
    if(buttonState == HIGH) {
        arm = !arm;
    }

    // Turn on Disarmed LED by defualt
    if(arm == false) {
        digitalWrite(armedLED, LOW);
        digitalWrite(disarmedLED, HIGH);
    }

    if(arm == true) {
        digitalWrite(disarmedLED, LOW);
        digitalWrite(armedLED, HIGH);
        panicButtonState = digitalRead(panicButton);

        if(panicButtonState == HIGH) {
            lcdDisplay();
            morseCodeMessage(buzzer);
            // digitalWrite(buzzer, HIGH);
            
            
        } 

        if (getDistance() <= 50) {
            digitalWrite(buzzer, HIGH); 
            lcdDisplay();
        }
    }
    lcd.clear();
    delay(200);
    digitalWrite(buzzer, LOW);
}

int getDistance() {
    digitalWrite(trig, LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);

    duration = pulseIn(echo, HIGH);
    distance = duration * 0.034 / 2;

    return distance;
}

void lcdDisplay() {
    lcd.write("Back away!!!");
    delay(200);
}

void morseCodeMessage(int outputDevice) {
    // Units of time
    int unit = 100;
    int unit3 = unit * 3;
    int unit7 = unit * 7;

    // "Back away criminal scum" in morse code
    // Website used: https://morsecode.world/international/translator.html
    String message = "-... .- -.-. -.- / .- .-- .- -.-- / -.-. .-. .. -- .. -. .- .-.. / ... -.-. ..- --";
    
    for(int i = 0; i < message.length(); i++) {
        Serial.println("Character: " + message.charAt(i));

        switch(message.charAt(i)) {
            case '.':
                digitalWrite(outputDevice, HIGH);
                delay(unit);
                digitalWrite(outputDevice, LOW);
                delay(unit);

                break;
            
            case '-':
                digitalWrite(outputDevice, HIGH);
                delay(unit3);
                digitalWrite(outputDevice, LOW);
                delay(unit);

                break;

            case ' ':
                delay(unit3);

                break;

            case '/':
                delay(unit7);

                break;

            default:
                Serial.println("Invalid Character");
        }
    }
}