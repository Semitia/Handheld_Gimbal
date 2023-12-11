// // /**
// //  * @file handheld_gimbal.ino
// //  * @brief 手持云台主程序
// //  * @author Semitia
// //  * @date 2023-12-8
// //  * @version 1.0
// //  * @link https://github.com/Semitia/Handheld_Gimbal.git
// // */

#include <Servo.h>
#include <SCoop.h>
#include "gimbal.h"

Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  //串口初始化
  Serial.begin(115200);
  while(Serial.read()>=0){}                   //clear buffer

  myservo.attach(5);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  Serial.println("loop\r\n");
  for (pos = 0; pos <= 180; pos += 5) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(100);                       // waits 15 ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 5) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(100);                       // waits 15 ms for the servo to reach the position
  }
}


// Gimbal gimbal;
// MPU6050 mpu6050(Wire);
// const int LED_pin = 13;

// void setup() {

  
//   // Wire.begin();
//   // mpu6050.begin();
//   // mpu6050.calcGyroOffsets(true);

//   // GimInit_t gim_init;
//   // gim_init.led_pin = 9;
//   // gim_init.button_pin = 11;
//   // gim_init.servo_pin[0] = 3;
//   // gim_init.servo_pin[1] = 5;
//   // gim_init.servo_pin[2] = 6;
//   // gim_init.light_sensor_pin = A0;
//   // gim_init.light_threshold = 900;
//   // gim_init.IIC = &Wire;
//   // gim_init.IMU = &mpu6050;
//   //gimbal = Gimbal(&gim_init);
//   myservo.attach(5); 
//   //mySCoop.start();
// }

// /**
//  * 信息打印线程
// */
// // defineTaskLoop(info_Task){
// //   static bool LED_state = 1;
// //   while(1){
// //     Serial.print("cnt: ");
// //     Serial.println();

// //     sleep(2000); // 针对线程
// //     if(LED_state) {
// //       LED_state=0;
// //       digitalWrite(LED_pin,HIGH);
// //     }
// //     else {
// //       digitalWrite(LED_pin,LOW);
// //       LED_state=1;   
// //     }
// //   }
// // }

// void loop() {
//   // gimbal.stateUpdate();
//   // gimbal.calculate();
//   // //gimbal.execute();
//   // gimbal.servoTest();
//     for (pos = 0; pos <= 180; pos += 45) { // goes from 0 degrees to 180 degrees
//     // in steps of 1 degree
//     myservo.write(pos);              // tell servo to go to position in variable 'pos'
//     delay(1000);                       // waits 15 ms for the servo to reach the position
//   }
//   for (pos = 180; pos >= 0; pos -= 45) { // goes from 180 degrees to 0 degrees
//     myservo.write(pos);              // tell servo to go to position in variable 'pos'
//     delay(1000);                       // waits 15 ms for the servo to reach the position
//   }
//   // sleep(1000);
// }
