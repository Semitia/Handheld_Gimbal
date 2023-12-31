/**
 * @file gimbal.h
 * @brief arduino nano 的手持云台库头文件
*/

#ifndef __GIMBAL_H__
#define __GIMBAL_H__
#include <Wire.h>
#include <Servo.h>
#include <MPU6050_tockn.h>

enum {
    PITCH = 0,
    YAW,
    ROLL
};

/* 云台初始化结构体 */
typedef struct __GimInit_t {
    // pins
    int led_pin;
    int button_pin;
    int servo_pin[3]; 
    int light_threshold;
    int light_sensor_pin;
    TwoWire *IIC;
    MPU6050 *IMU;
} GimInit_t;

/* 云台类 */
class Gimbal {
public:
    Gimbal();
    Gimbal(GimInit_t *init);
    ~Gimbal();
    void stateUpdate();
    void calculate();
    void execute();
    void servoTest();
    bool checkButton();
    void updateTar();
private:
    // 硬件相关
    int led_pin;
    int button_pin;
    int light_sensor_pin;
    Servo servo[3];
    TwoWire *IIC;
    MPU6050 *IMU;

    // 状态
    int servo_pos[3];
    bool button;
    float pitch, yaw, roll;                 // 当前姿态
    float pitch_tar, yaw_tar, roll_tar;     // 目标姿态
    float acc_x, acc_y, acc_z;              // 加速度计数据
    int light_threshold;                    // 补光阈值
    int light_val;                          // 光传感器数据，亮度越高，值越小
    int led_intensity;                      // 补光灯亮度0-255
};

Gimbal::Gimbal() {
    led_pin = 9;
    button_pin = 1;
    light_sensor_pin = A0;
    light_threshold = 900;

    pinMode(led_pin, OUTPUT);
    pinMode(button_pin, INPUT);
    pinMode(light_sensor_pin, INPUT);
}

Gimbal::Gimbal(GimInit_t *init) {
    led_pin = init->led_pin;
    button_pin = init->button_pin;
    light_sensor_pin = init->light_sensor_pin;
    light_threshold = init->light_threshold;
    led_intensity = 0;
    // servo[0].attach(init->servo_pin[0]);
    // servo[1].attach(init->servo_pin[1]);
    // servo[2].attach(init->servo_pin[2]);
    //Serial.print(init->servo_pin[0]);

    IIC = init->IIC;
    IMU = init->IMU;

    pinMode(led_pin, OUTPUT);
    pinMode(button_pin, INPUT);
    pinMode(light_sensor_pin, INPUT);
    Serial.print("Gimbal init.\r\n");
}

Gimbal::~Gimbal() {
    for(int i = 0; i < 3; i++) {
        servo[i].detach();
    }
}

void Gimbal::stateUpdate() {
    // IMU
    IMU->update();
    pitch = IMU->getAngleX();
    yaw = IMU->getAngleY();
    roll = IMU->getAngleZ();
    acc_x = IMU->getAccX();
    acc_y = IMU->getAccY();
    acc_z = IMU->getAccZ();
    // light sensor
    light_val = analogRead(light_sensor_pin);
    // button
    button = digitalRead(button_pin);

    //DEBUG
    Serial.print("pitch  |  yaw  |  roll | light_val | button \r\n");
    Serial.print(pitch); Serial.print(" | ");
    Serial.print(yaw); Serial.print(" | ");
    Serial.print(roll); Serial.print(" |    ");
    Serial.print(light_val); Serial.print("      |    ");
    Serial.print(button); Serial.print("\r\n \r\n");
    return;
}

void Gimbal::calculate() {
    if (button) {                   //按下按钮，停止姿态平稳，更新目标姿态
        pitch_tar = pitch;
        yaw_tar = yaw;
        roll_tar = roll;
        return;
    }
    servo_pos[PITCH] += (int)(pitch_tar - pitch);
    servo_pos[YAW] += (int)(yaw_tar - yaw);
    servo_pos[ROLL] += (int)(roll_tar - roll);

    //DEBUG
    Serial.print("pos[PITCH]  | pos[YAW]  | pos[ROLL] | pitch_tar | yaw_tar | roll_tar \r\n");
    Serial.print(servo_pos[PITCH]); Serial.print(" | ");
    Serial.print(servo_pos[YAW]); Serial.print(" | ");
    Serial.print(servo_pos[ROLL]); Serial.print(" | ");
    Serial.print(pitch_tar); Serial.print(" | ");
    Serial.print(yaw_tar); Serial.print(" | ");
    Serial.print(roll_tar); Serial.print("\r\n \r\n");
    return;
}

void Gimbal::execute() {
    if(light_val > light_threshold) {   // 补光
        digitalWrite(led_pin, HIGH);
    }
    else {
        digitalWrite(led_pin, LOW);
    }
    // if(button) return;
    // for(int i = 0; i < 3; i++) {
    //     servo[i].write(servo_pos[i]);
    // }
    return;
}

void Gimbal::updateTar() {
    pitch_tar = pitch;
    yaw_tar = yaw;
    roll_tar = roll;
    return;
}

bool Gimbal::checkButton() {
    return button; 
}

void Gimbal::servoTest() {
    servo[0].write(90);
    servo[1].write(90);
    servo[2].write(90);
    delay(1000);
    servo[0].write(0);
    servo[1].write(0);
    servo[2].write(0);
    delay(1000);
    return;
}

#endif