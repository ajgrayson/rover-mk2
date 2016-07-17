#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *motorL = AFMS.getMotor(3);
Adafruit_DCMotor *motorR = AFMS.getMotor(1);

#define DEBUG 

#ifdef DEBUG
#define DBG(message)    Serial.print(message)
#define DBGL(message)    Serial.println(message)
#else
#define DBG(message)
#define DBGL(message)
#endif

int MOTOR_SPEED = 150;
String LAST_CMD = "";

String motorSpeed() {
  return String(MOTOR_SPEED);
}

void runMotor(char m, int val) {
  uint8_t dir = RELEASE;

  if(val < 0){
    dir = BACKWARD;
  }
  else if (val > 0) {
    dir = FORWARD;
  }

  int spd = abs(val);
  
  if(m == 'L') {
    motorL->run(dir);
    motorL->setSpeed(spd);
  } else if(m == 'R') {
    motorR->run(dir);
    motorR->setSpeed(spd);
  }
  
  //delay(10);
}

void performDirectionalManuver(String cmd) {
  cmd.remove(0,1); // Remove the 'M'
  runMotor(cmd[0], cmd.substring(1).toInt());
}

void performCommand(String cmd) {
  if(cmd == "STOP") {
    runMotor('L', 0);
    runMotor('R', 0);
  }
  else if(cmd == "LEFT") {
    performDirectionalManuver("ML-" + motorSpeed());
    performDirectionalManuver("MR" + motorSpeed());
  }
  else if(cmd == "RIGHT") {
    performDirectionalManuver("ML" + motorSpeed());
    performDirectionalManuver("MR-" + motorSpeed());
  }
  else if(cmd == "FORWARD") {
    performDirectionalManuver("ML" + motorSpeed());
    performDirectionalManuver("MR" + motorSpeed());
  }
  else if(cmd == "BACKWARD") {
    performDirectionalManuver("ML-" + motorSpeed());
    performDirectionalManuver("MR-" + motorSpeed());
  }
  else if(cmd == "FAST") {
    MOTOR_SPEED = 255;
    if(LAST_CMD != "FAST") {
      performCommand(LAST_CMD);
    }
  }
  else if(cmd == "MEDIUM") {
    MOTOR_SPEED = 170;
    if(LAST_CMD != "MEDIUM") {
      performCommand(LAST_CMD);
    }
  }
  else if(cmd == "SLOW") {
    MOTOR_SPEED = 100;
    if(LAST_CMD != "SLOW") {
      performCommand(LAST_CMD);
    }
  }
  else if(cmd[0] == 'M') {
    performDirectionalManuver(cmd);
  }

  if(cmd != "FAST" && cmd != "MEDIUM" && cmd != "SLOW") {
    LAST_CMD = cmd;
  }
}

void receiveCommand() {
  if(Serial.available() > 0) {
    String cmd = Serial.readString();
    
    DBGL("Received:" + cmd);

    // multiple commands separated by pipes
    if(cmd.indexOf('|') > 0) {
      int sindx = cmd.indexOf('|');
      uint8_t limit = 0;
      
      while(sindx > 0 && limit < 10) {
        String subcmd = cmd.substring(0, sindx);
        performCommand(subcmd);
        cmd.remove(0, sindx + 1);
        sindx = cmd.indexOf('|');
        limit = limit + 1;
      }

      // perform the last one
      performCommand(cmd);
    } else {
      // single command
      performCommand(cmd);
    }
  }
}

void setup() {
  // ESP8266 should also be set to this rate
  Serial.begin(115200);
  
  AFMS.begin();  // create with the default frequency 1.6KHz
}

void loop() {
  receiveCommand();
}
