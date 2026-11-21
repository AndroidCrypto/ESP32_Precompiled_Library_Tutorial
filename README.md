# ESP32 Precompiled Library Tutorial

This repository accompanies the article **How to Create a Precompiled Library with the ESP32 (Expert Level)** published here: https://medium.com/@androidcrypto/how-to-create-a-precompiled-library-with-the-esp32-expert-level-b7466a508fd5?sharedUserId=androidcrypto

The tutorial is available here as [Markdown Version](ESP32_Precompiled_Library_Tutorial_EN.md).

The sketches [Esp32_S3_Library_Plain_v01](Esp32_S3_Library_Plain_v01) and [Esp32_C6_Library_Plain_v01](Esp32_C6_Library_Plain_v01) contain the SECRETMaths-Library in plain text. 

For testing the compiled library use the sketches [Esp32_S3_Library_Compiled_v01](Esp32_S3_Library_Compiled_v01) and [Esp32_C6_Library_Compiled_v01](Esp32_C6_Library_Compiled_v01)

The compiled (binary) library is [here](compiled_library) for reference. There are 3 zip-files:
- SECRETMaths_s3.zip is running on an ESP32-S3 only
- SECRETMaths_s3_c6.zip is running on ESP32-C6 and ESP32-S3
- SECRETMaths_s3_c6_c3.zip is running on ESP32-C3, ESP32-C6 and ESP32-S3

## Development Environment (Arduino)
````plaintext
Arduino IDE Version 2.3.10 (Windows)
arduino-esp32 boards Version 3.3.11 (https://github.com/espressif/arduino-esp32) that is based on Espressif ESP32 Version 5.5.5
````
