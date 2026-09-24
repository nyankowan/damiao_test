#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "damiao.h"

static const int DM_MOTOR_NUM = 3;
static const dm_motor_config_t s_dm_configs[DM_MOTOR_NUM] = {
    {.master_id = 0x10, .slave_id = 0x11, .limits = DM_LIMITS_DEFAULT,},
    {.master_id = 0x10, .slave_id = 0x12, .limits = DM_LIMITS_DEFAULT,},
    {.master_id = 0x10, .slave_id = 0x13, .limits = DM_LIMITS_DEFAULT,},
};
static dm_motor_t s_dm_motors[DM_MOTOR_NUM];
void main_loop_task(void);
void app_main(void)
{
    const can_bus_config_t bus_config = CAN_BUS_DEFAULT_CONFIG(GPIO_NUM_21, GPIO_NUM_22);//TX, RX
    ESP_ERROR_CHECK(can_bus_init(&bus_config));
    for(int i = 0; i<DM_MOTOR_NUM; i++){Core_num
        ESP_ERROR_CHECK(dm_motor_init(&s_dm_motors[i],&s_dm_configs[i]));
        dm_motor_enable(&s_dm_motors[i], portMAX_DELAY);
    }
    xTaskCreatePinnedToCore(main_loop_task, "main_loop", 8192, NULL, 1, NULL, tskNO_AFFINITY);
    return;
}

void main_loop_task(void){
    while(1){
        dm_motor_mit(&s_dm_motors[0],  0.0f/*pos*/, 0.0f/*vel*/, 40.0f/*Kp*/, 3.0f/*Kd*/, 0.0f/*torque*/, 0 /*ticks_to_wait*/);
        dm_motor_mit(&s_dm_motors[1],  0.0f/*pos*/, 1.0f/*vel*/, 0.0f/*Kp*/, 3.0f/*Kd*/, 0.0f/*torque*/, 0 /*ticks_to_wait*/);
        dm_motor_torque(&s_dm_motors[2],  1.0f/*torque*/, 0 /*ticks_to_wait*/);
        
        dm_feedback_t fb;
        if (dm_motor_get_feedback(&s_motor1, &fb, NULL) == ESP_OK) {
            dm_dump_feedback(&fb);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}
