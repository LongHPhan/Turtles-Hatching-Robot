#include <Arduino.h>
#include <TurtleReceiver.h>

//sensor stuff
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#define LED_PIN 4

NetController controller;

//Spinning thing
const int IN1F = 16;
const int IN2F = 17;
const int IN3F = 18;
const int IN4F = 19;

//Driver motor
const int IN1S = 26;
const int IN2S = 27;
const int IN3S = 32;
const int IN4S = 33;

Adafruit_MPU6050 mpu;

void setup() {
  Serial.begin(115200);
  controller.controllerSetup();


  pinMode(IN1F, OUTPUT);
  pinMode(IN2F, OUTPUT);
  pinMode(IN3F, OUTPUT);
  pinMode(IN4F, OUTPUT);
  pinMode(IN1S, OUTPUT);
  pinMode(IN2S, OUTPUT);
  pinMode(IN3S, OUTPUT);
  pinMode(IN4S, OUTPUT);

  //Sensor stuff
  pinMode(LED_PIN, OUTPUT);
  Wire.begin();  // SDA=21, SCL=22 by default on ESP32
  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1){
      delay(10);
      }
  }
  mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
  mpu.setGyroRange(MPU6050_RANGE_250_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  Serial.println("MPU6050 ready!");
}


void loop() {
    digitalWrite(LED_PIN, LOW);

    if(controller.getJoy1Y()>0.1){
      digitalWrite(IN1S, LOW);
      digitalWrite(IN2S, HIGH);
      digitalWrite(IN3S, LOW);
      digitalWrite(IN4S, HIGH);
    }
    else if(-.1 > controller.getJoy1Y()){
      digitalWrite(IN1S, HIGH);
      digitalWrite(IN2S, LOW);
      digitalWrite(IN3S, HIGH);
      digitalWrite(IN4S, LOW);
    }
    //Turn
    else if(controller.getJoy1X()>.1) {
      digitalWrite(IN1S, HIGH);
      digitalWrite(IN2S, LOW);
      digitalWrite(IN3S, LOW);
      digitalWrite(IN4S, HIGH);
    }
    else if(-.1>controller.getJoy1X()) {
      digitalWrite(IN1S, LOW);
      digitalWrite(IN2S, HIGH);
      digitalWrite(IN3S, HIGH);
      digitalWrite(IN4S, LOW);
    }
    else if(controller.getY()==true) {
      digitalWrite(IN3F, HIGH);
      digitalWrite(IN4F, LOW);
     
    }
    else if(controller.getA()==true) {
      digitalWrite(IN3F, LOW);
      digitalWrite(IN4F, HIGH);
    }


    else if(controller.getB()==true) {
      digitalWrite(IN1F, LOW);
      digitalWrite(IN2F, HIGH);
     
    }
    else if(controller.getX()==true) {
      digitalWrite(IN1F, HIGH);
      digitalWrite(IN2F, LOW);
    }
 
    else{
      // STOP motor
      digitalWrite(IN1S, LOW);
      digitalWrite(IN2S, LOW);
      digitalWrite(IN3S, LOW);
      digitalWrite(IN4S, LOW);


      digitalWrite(IN1F, LOW);
      digitalWrite(IN2F, LOW);
      digitalWrite(IN3F, LOW);
      digitalWrite(IN4F, LOW);
    }
   
  //sensor stuff
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Total acceleration magnitude
  float magnitude = sqrt(a.acceleration.x * a.acceleration.x +
                         a.acceleration.y * a.acceleration.y +
                         a.acceleration.z * a.acceleration.z);
  // 9.8 is gravity at rest, spike above ~15 means a shake
  if (magnitude > 18) {
    Serial.println("SHAKE DETECTED");
    digitalWrite(LED_PIN, HIGH);
    delay(2000);   
    digitalWrite(LED_PIN, LOW);
    delay(10);
  }
  delay(100);
}

