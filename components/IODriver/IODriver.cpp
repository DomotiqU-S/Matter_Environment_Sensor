#include "IODriver.hpp"
#include <cmath>
#include <esp_log.h>
#include <stdlib.h>
#include <string.h>

#include <device.h>
#include <driver/gpio.h>
#include <esp_matter.h>
#include <esp_matter_console.h>
#include "SHT41.hpp"
#include "VEML7700.hpp"

using namespace esp_matter;
using namespace chip::app::Clusters;
using namespace esp_matter::cluster;

SHT41 sht41(SDA_PIN, SCL_PIN);
VEML7700 veml7700(SDA_PIN, SCL_PIN);

uint32_t counter_motion = 0;
bool previous_motion = false;
bool isMotion = false;


static void IRAM_ATTR mottionDetected(void* arg) {
    counter_motion = 0;
    isMotion = true;
}

void readSensor() {

    sht41.read();
    float temp = sht41.getTemperature();
    float hum = sht41.getHumidity();

    float lux = veml7700.readAlsLux();
    float mesured_lux = 10000 * log10(lux) + 1;

    esp_matter_attr_val_t val_temp;
    val_temp.type = esp_matter_val_type_t::ESP_MATTER_VAL_TYPE_INT16;
    val_temp.val.i16 = formatForAttribute(temp * 100, 1);
    esp_matter::attribute::update(0x1, 0x402, 0x0, &val_temp);

    esp_matter_attr_val_t val_hum;
    val_hum.type = esp_matter_val_type_t::ESP_MATTER_VAL_TYPE_INT16;
    val_hum.val.i16 = formatForAttribute(hum * 100, 0);
    esp_matter::attribute::update(0x2, 0x405, 0x0, &val_hum);

    esp_matter_attr_val_t val_light;
    val_light.type = esp_matter_val_type_t::ESP_MATTER_VAL_TYPE_INT16;
    val_light.val.i16 = formatForAttribute(mesured_lux, 1);
    esp_matter::attribute::update(0x4, 0x400, 0x0, &val_light);

}

void taskReadSensor(void *parameters) {
    while(1) {
        readSensor();
        vTaskDelay(60000 / portTICK_PERIOD_MS);
    }
}

void taskMotionSensor(void *parameters) {
    esp_matter_attr_val_t val_motion;
    val_motion.type = esp_matter_val_type_t::ESP_MATTER_VAL_TYPE_BOOLEAN;
    
    while(1) {
        if(isMotion != previous_motion) {
            val_motion.val.b = isMotion;
            esp_matter::attribute::update(0x3, 0x406, 0x0, &val_motion);
            previous_motion = isMotion;
        }

        // if no motion is detected for MOTION_TIME_THRESHOLD minutes, we set the motion attribute to false
        if(counter_motion * MOTION_INTERVAL_CHECK >= MOTION_TIME_THRESHOLD) {
            counter_motion = 0;
            isMotion = false;
        }

        counter_motion++;

        vTaskDelay(MOTION_INTERVAL_CHECK / portTICK_PERIOD_MS);
    }
}

void app_driver_sensors()
{   
    // Set the motion pin as input and interrupt on positive edge
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    io_conf.pin_bit_mask = (1ULL << MOTION_PIN);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    gpio_config(&io_conf);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(MOTION_PIN, mottionDetected, (void*) MOTION_PIN);

    sht41.begin();
    vTaskDelay(50 / portTICK_PERIOD_MS);
    veml7700.begin();
    vTaskDelay(50 / portTICK_PERIOD_MS);
}

esp_err_t app_driver_attribute_update(app_driver_handle_t driver_handle, uint16_t endpoint_id, uint32_t cluster_id,
                                      uint32_t attribute_id, esp_matter_attr_val_t *val)
{
    return ESP_OK;
}

esp_err_t app_driver_set_default(uint16_t endpoint_id)
{
    return ESP_OK;
}

void decomposeNumber(float value, uint8_t* result, uint8_t precision) {

    for(int i = 0; i < precision; i++) {
        result[i] = (uint8_t)(value / pow(10, i)) % 10;
    }
}

int16_t formatForAttribute(float value, uint8_t precision) {
    uint8_t decomposed[5];
    decomposeNumber(value, decomposed, 5);

    if(precision >= 0) {
        // if the decomposed number[length-1] is greater or equal to 5, we round up
        // else we round down
        if(decomposed[precision-1] >= 5) {
            decomposed[precision] += 1;
            // set all the digits after the precision to 0
            for(int i = precision-1; i >= 0; i--) {
                decomposed[i] = 0;
            }
        }
        else {
            // set all the digits after the precision to 0
            for(int i = precision-1; i >= 0; i--) {
                decomposed[i] = 0;
            }
        }
    }

    int16_t result = 0;
    for(int i = 4; i >= 0; i--) {
        result = result * 10 + decomposed[i];
    }

    return result;
}

void startTask() {
    xTaskCreate(taskReadSensor, "taskReadSensor", 4096, NULL, 5, NULL);
    xTaskCreate(taskMotionSensor, "taskMotionSensor", 4096, NULL, 5, NULL);
}