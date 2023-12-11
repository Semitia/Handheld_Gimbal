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
    int iic_SDA_pin, iic_SCL_pin, iic_num;
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
    int light_val;                          // 光传感器数据
};

Gimbal::Gimbal() {
    led_pin = 9;
    button_pin = 1;
    light_sensor_pin = A0;
    light_threshold = 100;
    for(int i = 0; i < 3; i++) {
        servo[i].attach(i + 3);
        servo_pos[i] = 90;
    }
    // IIC = Wire;
    // IIC.begin();
    // IMU = MPU6050(IIC);
    // Wire.begin();
    // IMU.begin();
    // IMU.calcGyroOffsets(true);

    pinMode(led_pin, OUTPUT);
    pinMode(button_pin, INPUT);
    pinMode(light_sensor_pin, INPUT);
}

Gimbal::Gimbal(GimInit_t *init) {
    led_pin = init->led_pin;
    button_pin = init->button_pin;
    light_sensor_pin = init->light_sensor_pin;
    light_threshold = init->light_threshold;
    for(int i = 0; i < 3; i++) {
        servo[i].attach(init->servo_pin[i]);
        servo_pos[i] = 90;
    }
    // IIC = TwoWire();
    // IIC.begin();
    // IMU = MPU6050(IIC);
    // Wire.begin();
    // IMU.begin();
    // IMU.calcGyroOffsets(true);

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
    Serial.print("pitch | yaw | roll | acc_x | acc_y | acc_z | light_val | button\n");
    Serial.print(pitch); Serial.print(" | ");
    Serial.print(yaw); Serial.print(" | ");
    Serial.print(roll); Serial.print(" | ");
    Serial.print(acc_x); Serial.print(" | ");
    Serial.print(acc_y); Serial.print(" | ");
    Serial.print(acc_z); Serial.print(" | ");
    Serial.print(light_val); Serial.print(" | ");
    Serial.print(button); Serial.print("\n");

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
    return;
}

void Gimbal::execute() {
    if(light_val < light_threshold) {   // 补光
        digitalWrite(led_pin, HIGH);
    }
    else {
        digitalWrite(led_pin, LOW);
    }
    if(button) return;
    for(int i = 0; i < 3; i++) {
        servo[i].write(servo_pos[i]);
    }
    return;
}

#endif