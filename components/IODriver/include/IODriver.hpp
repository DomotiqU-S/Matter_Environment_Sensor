#ifndef IODRIVER_HPP
#define IODRIVER_HPP

#include <esp_err.h>
#include <esp_log.h>
#include <esp_matter.h>

#define MOTION_INTERVAL_CHECK 10
#define MOTION_TIME_THRESHOLD 300 * 1000
#define MOTION_PIN (gpio_num_t) 11
#define SDA_PIN (gpio_num_t) 9
#define SCL_PIN (gpio_num_t) 8

#if CHIP_DEVICE_CONFIG_ENABLE_THREAD
#include "esp_openthread_types.h"
#endif

typedef void* app_driver_handle_t;

/**
 * @brief Initialize the driver
 * 
 */
void app_driver_sensors();

/**
 * @brief Callback whne the attributes are updated
 * 
 * @param driver_handle 
 * @param endpoint_id 
 * @param cluster_id 
 * @param attribute_id 
 * @param val 
 * @return esp_err_t 
 */
esp_err_t app_driver_attribute_update(app_driver_handle_t driver_handle, uint16_t endpoint_id, uint32_t cluster_id,
                                      uint32_t attribute_id, esp_matter_attr_val_t *val);

/**
 * @brief Set the default value for the endpoint
 * 
 * @param endpoint_id 
 * @return esp_err_t 
 */
esp_err_t app_driver_set_default(uint16_t endpoint_id);

/**
 * @brief format the value for the attribute
 * 
 * @param value a float value
 * @param precision the precision of the value to round
 * @return int16_t 
 */
int16_t formatForAttribute(float value, uint8_t precision);

/**
 * @brief Decompose the number into an array of uint8_t
 * Each element of the array is a place value of the number
 * The size of the array should fit the size of the number
 * @param value 
 * @param result 
 */
void decomposeNumber(float value, uint8_t* result, uint8_t precision);

/**
 * @brief Read the sensor periodically
 * 
 */
void readSensor();

/**
 * @brief Start the task to read the sensor
 * 
 */
void startTask();

#if CHIP_DEVICE_CONFIG_ENABLE_THREAD
#define ESP_OPENTHREAD_DEFAULT_RADIO_CONFIG()                                           \
    {                                                                                   \
        .radio_mode = RADIO_MODE_NATIVE,                                                \
    }

#define ESP_OPENTHREAD_DEFAULT_HOST_CONFIG()                                            \
    {                                                                                   \
        .host_connection_mode = HOST_CONNECTION_MODE_NONE,                              \
    }

#define ESP_OPENTHREAD_DEFAULT_PORT_CONFIG()                                            \
    {                                                                                   \
        .storage_partition_name = "nvs", .netif_queue_size = 10, .task_queue_size = 10, \
    }
#endif

#endif // IODRIVER_HPP