
#include <SoftwareSerial.h>
#include <Servo.h>

#define trigpin1 12
#define echopin1 13
#define trigpin2 10
#define echopin2 11
#define led1 5
#define led2 6
#define led3 7
#define led4 8

const int DISTANCE_THRESHOLD = 50;

float duration_us, distance_cm;

SoftwareSerial Gsm(2, 3);
Servo Servo1;

int pirPin = 4;
int pirState = 0;
int calibrationTime = 30;

long Distance1, Duration1;

const unsigned long eventTime_1 = 1000;
const unsigned long eventTime_2 = 5000;

unsigned long previousTime_1 = 0;
unsigned long previousTime_2 = 0;

bool flag = false;
bool flag1 = false;

void setup()
{
    Serial.begin(9600);
    Gsm.begin(9600);

    pinMode(trigpin2, OUTPUT);
    pinMode(echopin2, INPUT);

    pinMode(trigpin1, OUTPUT);
    pinMode(echopin1, INPUT);

    Servo1.attach(9);

    pinMode(pirPin, INPUT);

    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(led3, OUTPUT);
    pinMode(led4, OUTPUT);

    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);

    delay(calibrationTime * 1000);

    Serial.println("SMART DUSTBIN IS Ready TO WORK");
}

void loop()
{
    digitalWrite(trigpin2, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigpin2, LOW);

    duration_us = pulseIn(echopin2, HIGH);
    distance_cm = 0.17 * duration_us;

    unsigned long currentTime = millis();

    if (distance_cm < DISTANCE_THRESHOLD)
    {
        Servo1.write(100);
        delay(1000);
    }
    else
    {
        Servo1.write(0);
    }

    Servo1.write(0);

    Serial.print("distance: ");
    Serial.print(distance_cm);
    Serial.println(" cm");

    delay(500);

    Serial.println("Ultrasonic");

    digitalWrite(trigpin1, LOW);
    delayMicroseconds(2);

    digitalWrite(trigpin1, HIGH);
    delayMicroseconds(10);

    digitalWrite(trigpin1, LOW);

    Duration1 = pulseIn(echopin1, HIGH);
    Distance1 = (Duration1 / 2) / 29.1;

    Serial.print("Distance:");
    Serial.println(Distance1);

    if ((Distance1 >= 21) && (Distance1 <= 28))
    {
        Serial.println("level 1");

        digitalWrite(led1, HIGH);
        digitalWrite(led2, LOW);
        digitalWrite(led3, LOW);
        digitalWrite(led4, LOW);
    }

    else if ((Distance1 >= 15) && (Distance1 <= 20))
    {
        Serial.println("level 2");

        digitalWrite(led1, HIGH);
        digitalWrite(led2, HIGH);
        digitalWrite(led3, LOW);
        digitalWrite(led4, LOW);

        previousTime_2 = currentTime;
    }

    else if ((Distance1 >= 7) && (Distance1 <= 14))
    {
        Serial.println("level 3");

        digitalWrite(led1, HIGH);
        digitalWrite(led2, HIGH);
        digitalWrite(led3, HIGH);
        digitalWrite(led4, LOW);
    }

    else if ((Distance1 >= 0) && (Distance1 <= 6))
    {
        Serial.println("level 4");

        digitalWrite(led1, HIGH);
        digitalWrite(led2, HIGH);
        digitalWrite(led3, HIGH);
        digitalWrite(led4, HIGH);

        if (flag == false)
        {
            Servo1.attach(9);
            send_msg();
            flag = true;
        }
    }
}

void send_msg()
{
    Servo1.detach();

    delay(100);

    Gsm.println("AT+CMGF=1");
    delay(1000);

    Gsm.println("AT+CMGS=\"+91xxxxxxxxxx\"");
    delay(1000);

    Gsm.println("DUST BIN IS FULL");
    delay(100);

    Gsm.println((char)26);
    delay(1000);
}

