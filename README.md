# HyperSerialEsp8266
  
Exposes USB high speed serial port at 2Mb baud for driving led strip using NeoPixelBus library. It's intended to replace slow Arduino solutions (level shifter 3.3V to 5v may be required). **Data integrity check (Fletcher's checksum) included in new 'Awa' protocol for HyperHDR. So no more random flashing caused by serial transmission errors.** That option must be checked in HyperHDR to make system works as on the screen below.  
  
Make sure that your serial chip on the ESP8266 can handle 2Mb speed: for example CP2102 can't as its max speed is 1Mb and you can compile a version for that speed but I think that's not the point, cheap CH340G can do it without any problems.  
  
There is also my new fork named HyperSerialWLED available with the support for the AWA serial protocol at @2Mb speed for both ESP8266 and ESP32: https://github.com/awawa-dev/HyperSerialWLED Can't guarantee it will work as stable as HyperSerialEsp8266 because WLED has a lot of other things to do in the backgrounds (ex. handling Wifi) and timing control for the serial port could be at danger for larger number of LEDs. But you don't need to abandon all the benefits offered by the WLED which can be a big advantage for some users.  
  
Tested with SK6812 RGBW and cheap 1.75$ noname clone of Wemos ESP8266 d1 mini CH340G. The data output is on the GPIO2 (D4 pin for Wemos mini d1), just like WLED for example. Should work with 3-channel RGB led system like WS8212b also.  
  
RGB to RGBW conversion is calibrated for the neutral white channel BTF SK6812 but it can be easily changed (for cool and warm temperature) in the code. Search for "color calibration". In HyperHDR use "1.5" gamma for red, blue and green for best effect in the "Image Processing" tab.

| LED strip / Device             | HyperSerialEsp8266 |
|--------------------------------|:------------------:|
| SK6812 cold white              |        yes         |
| SK6812 neutral white           |        yes         |
| WS281x                         |        yes         |
| SPI (APA102, SK9812, HD107...) |        yes         |
  
# Data integrity check
  
Why the data integrity check was introduced which causes incompatibility with other software? Because at 2Mb speed many chip-makers allow few percent error in the transmission. And we do not want to have any distracting flashes. Broken frames are abandon without showing them. At 100Hz for 250 leds approximately 1-5% of the frames are broken.
  
# Why not Arduino, Raspberry Pi or WLED
  
Because Arduino is slow, really slow even at @500000 baud. And no data integrity checking so random flashing may occure... Increasing serial port speed leads to problems with the LED library as most ATMega are at 16MHz only (for comparision Esp8266 is at 80/160MHz). For over 200leds and RGB channel we have merely ~20 frames. For RGBW is even worse. The only advantage of Arduinos is logic at 5V. For Esp8266 level shifter 3.3V to 5V may be required. You can find example of simply and efficient one here: <a href="https://hyperhdr.blogspot.com/2020/12/my-build-log-using-sk6812-rgbw-led.html#chapter3">Level shifter for 3.3V logic level</a>  
  
Rpi is very powerful device but for the SK6812 and WS2812b protocol timing of transmission is crucial. It's hard to maintain it in the multitasking environment.  
  
WLED is a brilliant app and it's preffered solution in most cases. But sometimes Wifi is not an option: there is a problem with the signal strenght/stability caused by EM interference or obstacles which results in disturbing effects from the LED strip.You can also try https://github.com/awawa-dev/HyperSerialWLED if you don't want to abandon WLED benefits or you are looking for a version for ESP32.  
  
**Arduino: 250 RGB leds, 500000 baud, 100 Hz output from HyperHDR, real output for the LED strip is around 20Hz.**  
**With such slow hardware driver you don't even need 30FPS from the grabber really:**  

<img src='https://user-images.githubusercontent.com/69086569/207443059-0ef8bc71-0a5d-4faa-bab7-f313d1cb6ed6.png' border='0' alt='p100' width="320"/>
  
  
With HyperSerialEsp8266 you can have over 100Hz refresh rate for 250 RGB and over 80HZ for 250 RGBW LED strips.  
<img src="https://i.postimg.cc/bvCwBYHJ/250-rgbw-latch0.jpg" width="320"/>&nbsp;<img src="https://i.postimg.cc/Kz18SXJ5/250-rgb-latch0.jpg" width="320"/>
  
  
Statistics are sent to serial port monitor when there is no data incoming. You can read it with any serial port client. This driver is not hiding any information from you.
  
# Flashing
  
Recommend to use [esphome-flasher](https://github.com/esphome/esphome-flasher/releases)  

For **RGBW LED strip** like RGBW SK6812 NEUTRAL white choose: *firmware_SK6812_RGBW_NEUTRAL.bin*  
  
For **RGBW LED strip** like RGBW SK6812 COLD white choose: *firmware_SK6812_RGBW_COLD.bin*  
  
For **RGB LED strip** like WS8212b or RGB SK6812 variant choose: *firmware_WS281x_RGB.bin*  
  
For **SPI driven RGB LED strip** APA102: *firmware_SPI_APA102_SK9822_HD107.bin*, WS8201: *firmware_SPI_WS2801.bin*  
  
If you want to disable your first LED because it's used as a sacrificial level shifter, please use [HyperHDR v19](https://github.com/awawa-dev/HyperHDR/pull/379)  

For the RGBW firmware the white channel is automatically calculated and R,G,B channels are corrected.  
  
Using esphome-flasher:  
<img src="https://i.postimg.cc/X7yb9FQs/flash.jpg" width="640"/>
  
# Usage in HyperHDR
  
Set `Refresh time` to zero, `Baudrate` to 2000000 and you enabled `HyperHDR's AWA protocol`.  
Enabling `White channel calibration` is optional, if you want to fine tune the white channel balance of your sk6812 RGBW LED strip.  
`ESP8266/ESP32 handshake` could help you to properly initialize the ESP device and enables statistics available in the logs (you must stop the LED device first to get them).  

![obraz](https://user-images.githubusercontent.com/69086569/207109594-0493fe58-3530-46bb-a0a3-31a110475ed6.png)
  
# Benchmarks


**Refresh rate depending on requested refresh rate/LED strip length**:
| RGBW LED strip / Device                        | WeMos D1 Mini Pro (CP2104)<br>HyperSerialEsp8266 v8 |
|------------------------------------------------|-----------------------------------------|
| 300LEDs<br>Refresh rate/continues output=100Hz |                  71-75                 |
| 600LEDs<br>Refresh rate/continues output=60Hz  |                  34-35                 |
| 900LEDs<br>Refresh rate/continues output=40Hz  |                  23                   |


**Logic level analyzer, RGB (250 leds, 100Hz)**  
<img src="https://i.postimg.cc/sjrQQ11Y/250-rgb-setup.jpg" width="640"/>  


**RGBW (250 leds, 100Hz)**  
<img src="https://i.postimg.cc/KZL38tcc/250-rgbw-setup.jpg" width="640"/>  
  
# Compiling
  
Currently we use PlatformIO to compile the project. Install [Visual Studio Code](https://code.visualstudio.com/) and add [PlatformIO plugin](https://platformio.org/).
This environment will take care of everything and compile the firmware for you.

But there is also an alternative and an easier way. Just fork the project and enable its Github Action. Use the online editor to make changes to the ```platformio.ini``` file, for example change default pin-outs/speed or enable multi-segments support, and save it. Github Action will compile new firmware automatically in the Artifacts archive. It has never been so easy!

Tutorial: https://github.com/awawa-dev/HyperSerialEsp8266/wiki
  
# Pinout
  
ESP8266:  
For non-SPI LED strip data pin is GPIO2 (usually D4).  
For SPI LED strip is: GPIO13 (MOSI, usually D7) and GPIO14 (SCLK/CLOCK, usually D5).  
  
# Disclaimer
  
You use it on your own risk.  
Don't touch these firmwares if you don't know how to put the device in the programming mode if something goes wrong.  
As per the MIT license, I assume no liability for any damage to you or any other person or equipment.  
