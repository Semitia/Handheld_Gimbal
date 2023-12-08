/**
 * @file handheld_gimbal.ino
 * @brief 手持云台主程序
 * @author Semitia
 * @date 2023-12-8
 * @version 1.0
 * @link https://github.com/Semitia/Handheld_Gimbal.git
*/

#include <SCoop.h>
#include "gimbal.h"

const int LED_pin=13;
Gimbal gimbal;

void setup() {
  GimInit_t gim_init;
  gim_init.led_pin = 9;
  gim_init.button_pin = 1;
  gim_init.servo_pin[0] = 3;
  gim_init.servo_pin[1] = 5;
  gim_init.servo_pin[2] = 6;
  gim_init.light_sensor_pin = A0;
  gim_init.light_threshold = 100;
  gim_init.iic_SDA_pin = A4;
  gim_init.iic_SCL_pin = A5;
  gim_init.iic_num = 0;
  gimbal = Gimbal(&gim_init);
  
  // 串口初始化
  Serial.begin(115200);
  while(Serial.read()>=0){}                   //clear buffer
  mySCoop.start();
}

void loop() {
  gimbal.stateUpdate();
  gimbal.calculate();
  gimbal.execute();
  return;
}

/**
 * 信息打印线程
*/
defineTaskLoop(info_Task){
  static bool LED_state = 1;
  sleep(2000); // 针对线程
  if(LED_state) {
    LED_state=0;
    digitalWrite(LED_pin,HIGH);
  }
  else {
    digitalWrite(LED_pin,LOW);
    LED_state=1;   
  }
  return;
}

/**
 * 串口数据处理函数，四种消息类型
 * @param buf 串口数据
 * byte 0: 电机编号/传感器编号
 * byte 1：类型标识
 *  0x01设置速度
 *    byte 2：方向
 *    byte 3~4：速度大小，现在其实是步进时间间隔
 *  0x02设置位置
 *    byte 2~3：坐标
 *  0x03读取速度
 *  0x04读取位置
 *  0x05 stop
 *  0x06 position reset
 *  0x07 read angle
*/  
// void buf_process(uint8_t *buf)
// {
//   uint8_t ID = buf[0];
//   uint8_t type = buf[1];
//   uint8_t resp_buf;
//   bool dir=buf[2];
//   int freq = buf[3]<<8|buf[4];
//   uint16_t target_pos=buf[2]<<8|buf[3];
//   Serial.print(ID);
//   switch(type) {
//     case 0x01:
//       M[ID].set_ctrl_mode(1,dir,freq);
//       Serial.print(" set speed: ");
//       Serial.print(dir);
//       Serial.print(" ");
//       Serial.println(freq);
//       break;
//     case 0x02:
//       M[ID].set_ctrl_mode(0,target_pos);
//       Serial.print(" set target position: ");
//       Serial.println(target_pos);
//       break;
//     case 0x03: {
//       Serial.println(" read speed: ");
//       Serial.println(M[ID].get_speed());
//       int speed = M[ID].get_speed();
//       resp_buf = 3;
//       Serial.print(resp_buf,HEX);
//       if(speed>0) Serial.print(1,HEX);
//       else {
//         Serial.print(0,HEX);
//         speed = -speed;
//       }
//       resp_buf = (speed&0xff00)>>8;
//       Serial.print(resp_buf,HEX);
//       resp_buf = speed&0xff;
//       Serial.print(resp_buf,HEX);
//       Serial.print("AI");
//     }
//       break;
//     case 0x04:
//       Serial.println(" read position: ");
//       Serial.println(M[ID].get_position());
//       break;
//     case 0x05:
//       Serial.println(" stop");
//       M[ID].set_ctrl_mode(0,target_pos);
//       break;
//     case 0x06:
//       Serial.println(" position reset");
//       M[ID].reset_position();
//       break;
//     case 0x07:
//       Serial.println(" read angle");
//       Serial.println(AS[ID].updateAngle(analogRead(ADC_pin[ID])));
//       break;
//     default:
//       Serial.println(" error cmd type");
//       Serial.println(type);
//       break;
//   }
//   return;
// }

/*
  serialEvent是一种在接收到串口数据时自动调用的函数，它可以让你在loop()函数执行完后处理串口数据。
  serialEvent会在每次loop()函数结束时被检查，如果串口缓冲区中有可用的数据，就会触发serialEvent。
  你可以使用Serial.read()等函数来读取串口数据，并进行相应的操作。
  如果你的板子有多个串口，你也可以使用serialEvent1()，serialEvent2()等函数来分别处理不同的串口数据。
*/
// void serialEvent(){
//   static int buf_index=0, end_flag=0;
//   static uint8_t buf[10];
//   while(Serial.available()>0)
//   {
//     uint8_t data=Serial.read();
//     buf[buf_index++]=data;

//     //结束标识符 0x41 0x49(星野爱呜呜呜)
//     if(end_flag == 0)
//     {
//       if(data == 0x41)
//       {
//         end_flag=1;
//       }
//     }
//     else if(end_flag == 1)
//     {
//       if(data == 0x49)
//       {
//         end_flag=0;
//         buf_index=0;
//         //for (int i = 0; i < 10; i++) {
//         //  Serial.print(buf[i]);
//         //}
//         //Serial.println();

//         buf_process(buf);
//       }
//       else {
//         end_flag=0;
//         buf_index=0;
//         Serial.println("error data");
//       }
//     }

//     if(buf_index > 9)
//     {
//       end_flag=0;
//       buf_index=0;
//       Serial.println("buf overflow");
//     }

//   }

// }
