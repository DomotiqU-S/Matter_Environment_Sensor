#ifndef SPI_CONTROLLER_H
#define SPI_CONTROLLER_H

#include "BusController.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define HOST (spi_host_device_t)SPI1_HOST

class SPIController : public BusController
{
private:
    gpio_num_t mosi_pin;
    gpio_num_t miso_pin;
    gpio_num_t sclk_pin;
    gpio_num_t cs_pin;
    uint32_t clk_speed;
    spi_device_handle_t spi;

    spi_transaction_t transaction;
    spi_bus_config_t buscfg;
public:
    /**
     * @brief Constructor for the SPIController class.
     * 
     * This constructor initializes the SPI controller with the given parameters.
     * 
     * @param mosi_pin The GPIO pin number for the MOSI line.
     * @param miso_pin The GPIO pin number for the MISO line.
     * @param sclk_pin The GPIO pin number for the SCLK line.
     * @param cs_pin The GPIO pin number for the CS line.
     * @param clk_speed The clock speed of the SPI bus.
     * 
     * @return N/A
    */
    SPIController(gpio_num_t mosi_pin, gpio_num_t miso_pin, gpio_num_t sclk_pin, gpio_num_t cs_pin, uint32_t clk_speed = 1000000);
   
    /**
     * @brief Destructor for the SPIController class.
     * 
     * This destructor frees the memory allocated for the SPI controller.
     * 
     * @return N/A
    */
    ~SPIController();
    
    /**
     * @brief Initializes the SPI controller.
     * 
     * This function initializes the SPI controller with the given parameters.
     * 
     * @return ESP_OK if the SPI controller is initialized successfully, ESP_FAIL otherwise.
    */
    esp_err_t begin();
    
    /**
     * @brief Reads a byte from the SPI bus.
     * 
     * This function reads a byte from the SPI bus.
     * 
     * @param rx_buffer The buffer to store the received byte.
     * @param reg The register to read from.
     * @param restart Whether to restart the transaction.
     * 
     * @return ESP_OK if the byte is read successfully, ESP_FAIL otherwise.
    */
    esp_err_t readByte(uint8_t *rx_buffer, uint8_t reg, bool restart = false);
    
    /**
     * @brief Reads a word from the SPI bus.
     * 
     * This function reads a word from the SPI bus.
     * 
     * @param rx_buffer The buffer to store the received word.
     * @param reg The register to read from.
     * @param restart Whether to restart the transaction.
     * 
     * @return ESP_OK if the word is read successfully, ESP_FAIL otherwise.
    */
    esp_err_t readWord(uint8_t *rx_buffer, uint8_t reg, bool restart = false);
    
    /**
     * @brief Reads data from the SPI bus.
     * 
     * This function reads data from the SPI bus.
     * 
     * @param rx_buffer The buffer to store the received data.
     * @param reg The register to read from.
     * @param len The number of bytes to read.
     * @param restart Whether to restart the transaction.
     * 
     * @return ESP_OK if the data is read successfully, ESP_FAIL otherwise.
    */
    esp_err_t read(uint8_t *rx_buffer, uint8_t reg, uint8_t len, bool restart = false);
    
    /**
     * @brief Writes a byte to the SPI bus.
     * 
     * This function writes a byte to the SPI bus.
     * 
     * @param tx_buffer The buffer containing the byte to write.
     * @param reg The register to write to.
     * 
     * @return ESP_OK if the byte is written successfully, ESP_FAIL otherwise.
    */
    esp_err_t writeByte(uint8_t *tx_buffer, uint8_t reg);
    
    /**
     * @brief Writes a word to the SPI bus.
     * 
     * This function writes a word to the SPI bus.
     * 
     * @param tx_buffer The buffer containing the word to write.
     * @param reg The register to write to.
     * 
     * @return ESP_OK if the word is written successfully, ESP_FAIL otherwise.
    */
    esp_err_t writeWord(uint8_t *tx_buffer, uint8_t reg);
    
    /**
     * @brief Writes data to the SPI bus.
     * 
     * This function writes data to the SPI bus.
     * 
     * @param tx_buffer The buffer containing the data to write.
     * @param reg The register to write to.
     * @param len The number of bytes to write.
     * 
     * @return ESP_OK if the data is written successfully, ESP_FAIL otherwise.
    */
    esp_err_t write(uint8_t *tx_buffer, uint8_t reg, uint8_t len);
};

#endif
