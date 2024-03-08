#include "IODriver.hpp"

#include <esp_log.h>
#include <stdlib.h>
#include <string.h>

#include <device.h>
#include <driver/gpio.h>
#include <esp_matter.h>
#include <esp_matter_console.h>
using namespace esp_matter;
using namespace chip::app::Clusters;
using namespace esp_matter::cluster;

bool isPressed = false;

uint16_t old_level = 0;
bool old_state = false;

static void IRAM_ATTR buttonCb(void* arg) {
    isPressed = !isPressed;
}

bool verifyLevel(uint8_t level)
{
    uint8_t scan_value = level;
    uint8_t ones_count = 0;
    while (scan_value > 0)
    {
        scan_value = scan_value >> 1;
        if (scan_value & 0x01)
        {
            ones_count++;
        }
    }
    return ones_count == 1;
}

void taskSensor(void *parameters) {
    while (1) {
        esp_matter_attr_val_t val_temp;
        val_temp.type = esp_matter_val_type_t::ESP_MATTER_VAL_TYPE_INT16;
        val_temp.val.i16 = formatForAttribute(2560, 1);
        esp_matter::attribute::update(0x1, 0x402, 0x0, &val_temp);

        esp_matter_attr_val_t val_hum;
        val_hum.type = esp_matter_val_type_t::ESP_MATTER_VAL_TYPE_INT16;
        val_hum.val.i16 = formatForAttribute(5000, 0);
        esp_matter::attribute::update(0x2, 0x405, 0x0, &val_hum);

        vTaskDelay(25000 / portTICK_PERIOD_MS);
    }
}

void app_driver_switch_init()
{   
    xTaskCreate(taskSensor, "slider_task", 4096, NULL, 5, NULL);
}

esp_err_t app_driver_attribute_update(app_driver_handle_t driver_handle, uint16_t endpoint_id, uint32_t cluster_id,
                                      uint32_t attribute_id, esp_matter_attr_val_t *val)
{
    if (endpoint_id == 1) {
        ESP_LOGI("IODriver", "Attribute update: endpoint_id: %u, cluster_id: %lu, attribute_id: %lu", endpoint_id, cluster_id, attribute_id);
    }
    else if(endpoint_id == 2) {
        ESP_LOGI("IODriver", "Attribute update: endpoint_id: %u, cluster_id: %lu, attribute_id: %lu", endpoint_id, cluster_id, attribute_id);
    }
    else {
        ESP_LOGI("IODriver", "Attribute update: endpoint_id: %u, cluster_id: %lu, attribute_id: %lu", endpoint_id, cluster_id, attribute_id);
    }
    return ESP_OK;
}

esp_err_t app_driver_set_default(uint16_t endpoint_id)
{
    return ESP_OK;
}

void decomposeNumber(int16_t value, uint8_t* result) {

    result[0] = value % 10;
    result[1] = (value / 10) % 10;
    result[2] = (value / 100) % 10;
    result[3] = (value / 1000) % 10;
}

int16_t formatForAttribute(int16_t value, uint8_t precision) {
    uint8_t decomposed[4];
    decomposeNumber(value, decomposed);

    if(precision >= 0) {
        // if the decomposed number[-1] is greater or equal to 5, we round up
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
    for(int i = 3; i >= 0; i--) {
        result = result * 10 + decomposed[i];
    }

    return result;
}