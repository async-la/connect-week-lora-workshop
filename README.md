## Connect Week Pasadena Hands on LoRa Workshop

## Slides 
https://docs.google.com/presentation/d/e/2PACX-1vRb2vZBMee971oruDp5ysN4Tk_J31Bu3k7uOItb6BPASJ0eNRNkNGFUsggvAUX0IOd_w6A2FqW2isL8/pub?start=false&loop=false&delayms=5000

## Hardware Setup

#### Development Board
https://www.tindie.com/products/TleraCorp/grasshopper-loralorawan-development-board/

#### Board Library (Includes LoRaWAN library)
https://github.com/GrumpyOldPizza/ArduinoCore-stm32l0 
https://github.com/GrumpyOldPizza/ArduinoCore-stm32l0/tree/master/libraries/LoRaWAN

#### Arduino Sketch
 1. Open `connect-week.ino` and update `devEui`, `appEui` and `appKey` to match values from your TTN device.

 ## Application Setup
 1. Run `npm install` to install depedencies
 2. Open `server.js` and add your app id and access key.
 3. Run `node server.js`


Note: The following serialization library is used in to encode/decode packets.
https://github.com/thesolarnomad/lora-serialization



 