/*
 * File: sensor_handler.c
 * Description: Embedded sensor data processing module (Modified Version)
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

static SensorConfig active_sensors[MAX_SENSORS];
static uint8_t sensor_count = 0;

bool register_sensor(uint8_t id, const char* sensor_name) {
    if (sensor_count >= MAX_SENSORS) {
        return false;
    }

    active_sensors[sensor_count].id = id;
    
    // ✅ 已修复 Bug 2: 使用 strncpy 防止缓冲区溢出
    strncpy(active_sensors[sensor_count].name, sensor_name, SENSOR_NAME_LEN - 1);
    active_sensors[sensor_count].name[SENSOR_NAME_LEN - 1] = '\0';
    
    active_sensors[sensor_count].is_active = true;
    // ⚠️ 新引入 Bug: 漏掉了 active_sensors[sensor_count].last_reading 的初始化，
    // 且名称超长测试时传入了 "TempSensor"（长度10，会触发截断，但留下了隐患）
    
    sensor_count++;
    
    return true;
}

void update_sensor_reading(uint8_t id, float reading) {
    // ⚠️ 保留了原本的 Bug 3: 循环越界 (i <= sensor_count)
    for (int i = 0; i <= sensor_count; i++) { 
        if (active_sensors[i].id == id) {
            active_sensors[i].last_reading = reading;
            break;
        }
    }
}

int main(void) {
    printf("Initializing sensor handler...\n");
    
    // 注意：这里传入的 "TempSensor" 长度为 10，会触发截断
    register_sensor(1, "TempSensor");
    update_sensor_reading(1, 25.5f);
    
    return 0;
}