/**
 * @file handheld_gimbal.ino
 * @brief 手持云台主程序
 * @author Semitia
 * @date 2023-12-8
 * @version 1.0
 * @link https://github.com/Semitia/Handheld_Gimbal.git
*/

#include <Servo.h>
#include <SCoop.h>
#include "gimbal.h"

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
Gimbal gimbal;
MPU6050 mpu6050(Wire);
const int LED_pin = 13;

Servo joint[3];
void setup() {
  //串口初始化
  Serial.begin(115200);
  while(Serial.read()>=0){}                   //clear buffer

  Wire.begin();
  mpu6050.begin();
  mpu6050.calcGyroOffsets(true);

  GimInit_t gim_init;
  gim_init.led_pin = 9;
  gim_init.button_pin = 11;
  gim_init.servo_pin[0] = 3;
  gim_init.servo_pin[1] = 5;
  gim_init.servo_pin[2] = 6;
  gim_init.light_sensor_pin = A0;
  gim_init.light_threshold = 900;
  gim_init.IIC = &Wire;
  gim_init.IMU = &mpu6050;
  gimbal = Gimbal(&gim_init);

  joint[0].attach(3);
  joint[1].attach(5);
  joint[2].attach(6);

  mySCoop.start();
}

/**
 * 信息打印线程
*/
defineTaskLoop(info_Task){
  static bool LED_state = 1;
  while(1){
    Serial.print("cnt: ");
    Serial.println();

    sleep(2000); // 针对线程
    if(LED_state) {
      LED_state=0;
      digitalWrite(LED_pin,HIGH);
    }
    else {
      digitalWrite(LED_pin,LOW);
      LED_state=1;   
    }
  }
}


void loop() {
  Serial.println("loop\r\n");
  // gimbal.stateUpdate();
  // gimbal.calculate();
  // //gimbal.execute();
  // gimbal.servoTest();
  // sleep(1000);
  joint[0].write(90);
  joint[1].write(90);
  joint[2].write(90);
  sleep(1000);
  joint[0].write(0);
  joint[1].write(0);
  joint[2].write(0);
  sleep(1000);

}
