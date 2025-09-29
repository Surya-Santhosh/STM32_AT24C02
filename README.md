# STM32_AT24C02
The taske enables the interactive reading and writing of data to AT24C02 EEPROM using ESP32, STM32 via I2C Communication. When write command with address and data entered in the console, it sent via UART to ESP32, then it sent to STM32 over I2C and the STM32 writes the value to the specified EEPROM address.When a read command is entered with an address,then it sent to STM32 over I2C and STM32 reads the data from AT24C02 and sends it back to the console for display.Two threads are created for master and slave operation and use semaphore for synchronisation.





