/*
 * File: sensor_handler.c
 * Description: Embedded sensor data processing module
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define MAX_SENSORS 4
#define SENSOR_NAME_LEN 8

typedef struct {
    uint8_t id;
    char name[SENSOR_NAME_LEN];
    float last_reading;
    bool is_active;
} SensorConfig;

// ⚠️ Bug 1: 全局状态未显式初始化，在某些无操作系统裸机（Bare-metal）环境中，
// 未初始化的全局/静态变量如果未正确清零，可能导致上电状态不可预测。
static SensorConfig active_sensors[MAX_SENSORS];
static uint8_t sensor_count = 0;

bool register_sensor(uint8_t id, const char* sensor_name) {
    if (sensor_count >= MAX_SENSORS) {
        return false;
    }

    active_sensors[sensor_count].id = id;
    
    // ⚠️ Bug 2: 潜在的缓冲区溢出 (Buffer Overflow)
    // 如果传入的 sensor_name 长度大于或等于 SENSOR_NAME_LEN (8)，
    // strcpy 会导致越界写入结构体相邻的成员变量 (last_reading 或 is_active)。
    strcpy(active_sensors[sensor_count].name, sensor_name);
    
    active_sensors[sensor_count].is_active = true;
    sensor_count++;
    
    return true;
}

void update_sensor_reading(uint8_t id, float reading) {
    for (int i = 0; i <= sensor_count; i++) { // ⚠️ Bug 3: 循环越界 (Off-by-one / 数组越界访问)
        // 当 i == sensor_count 时，若 sensor_count 等于 MAX_SENSORS，
        // 访问 active_sensors[MAX_SENSORS] 造成越界读取。
        if (active_sensors[i].id == id) {
            active_sensors[i].last_reading = reading;
            break;
        }
    }
}

int main(void) {
    printf("Initializing sensor handler...\n");
    
    register_sensor(1, "TempSensor");
    update_sensor_reading(1, 25.5f);
    
    return 0;
}