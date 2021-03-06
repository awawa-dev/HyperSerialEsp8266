# HyperSerialEsp8266

Exposes USB high speed serial port at 2Mb baud for driving led strip using NeoPixelBus library. It's intended to replace slow Arduino solutions (level shifter 3.3V to 5v may be required). <b>Data integrity check (Fletcher's checksum) included in new 'Awa' protocol for HyperHDR. So no more random flashing caused by serial transmission errors.</b> That option must be checked in HyperHDR to make system works as on the screen below.<br/><br/>Make sure that your serial chip on the ESP8266 can handle 2Mb speed: for example CP2102 can't as its max speed is 1Mb and you can compile a version for that speed but I think that's not the point, cheap CH340G can do it without any problems.<br/><br/>

There is also my new fork named HyperSerialWLED available with the support for the AWA serial protocol at @2Mb speed for both ESP8266 and ESP32: https://github.com/awawa-dev/HyperSerialWLED Can't guarantee it will work as stable as HyperSerialEsp8266 because WLED has a lot of other things to do in the backgrounds (ex. handling Wifi) and timing control for the serial port could be at danger for larger number of LEDs. But you don't need to abandon all the benefits offered by the WLED which can be a big advantage for some users.<br/><br/>

Tested with SK6812 RGBW and cheap 1.75$ noname clone of Wemos ESP8266 d1 mini CH340G. The data output is on the GPIO2 (D4 pin for Wemos mini d1), just like WLED for example. Should work with 3-channel RGB led system like WS8212b also.<br/><br/>

RGB to RGBW conversion is calibrated for the neutral white channel BTF SK6812 but it can be easily changed (for cool and warm temperature) in the code. Search for "color calibration". In HyperHDR use "1.5" gamma for red, blue and green for best effect in the "Image Processing" tab.

# Data integrity check

Why the data integrity check was introduced which causes incompatibility with other software? Because at 2Mb speed many chip-makers allow few percent error in the transmission. And we do not want to have any distracting flashes. Broken frames are abandon without showing them. At 100Hz for 250 leds approximately 1-5% of the frames are broken.

# Why not Arduino, Raspberry Pi or WLED

Because Arduino is slow, really slow even at @500000 baud. And no data integrity checking so random flashing may occure... Increasing serial port speed leads to problems with the LED library as most ATMega are at 16MHz only (for comparision Esp8266 is at 80/160MHz). For over 200leds and RGB channel we have merely ~20 frames. For RGBW is even worse. The only advantage of Arduinos is logic at 5V. For Esp8266 level shifter 3.3V to 5V may be required. You can find example of simply and efficient one here: <a href="https://hyperhdr.blogspot.com/2020/12/my-build-log-using-sk6812-rgbw-led.html#chapter3">Level shifter for 3.3V logic level</a><br/><br/>Rpi is very powerful device but for the SK6812 and WS2812b protocol timing of transmission is crucial. It's hard to maintain it in the multitasking environment.<br/><br/>WLED is a brilliant app and it's preffered solution in most cases. But sometimes Wifi is not an option: there is a problem with the signal strenght/stability caused by EM interference or obstacles which results in disturbing effects from the LED strip.You can also try https://github.com/awawa-dev/HyperSerialWLED if you don't want to abandon WLED benefits or you are looking for a version for ESP32.<br/><br/><b>Arduino: 250 RGB leds, 500000 baud, 100 Hz output from HyperHDR, real output for the LED strip is around 20Hz<br/>With such slow hardware driver you don't even need 30FPS from the grabber really:</b><br/>
<img src='https://i.postimg.cc/1PD541LP/p100.jpg' border='0' alt='p100' width="320"/><br/><br/>
With HyperSerialEsp8266 you can have over 100Hz refresh rate for 250 RGB and over 80HZ for 250 RGBW LED strips.<br/>
<img src="https://i.postimg.cc/bvCwBYHJ/250-rgbw-latch0.jpg" width="320"/>&nbsp;<img src="https://i.postimg.cc/Kz18SXJ5/250-rgb-latch0.jpg" width="320"/><br/><br/>
Statistics are sent to serial port monitor when there is no data incoming. You can read it with any serial port client. This driver is not hiding any information from you.

# Flashing
For <b>RGBW LED strip</b> like RGBW SK6812 NEUTRAL white choose: <i>HyperSerialEsp8266.ino.d1_mini_RGBW_FIRSTLED_xxxxxx.bin</i><br/><br/>
For <b>RGBW LED strip</b> like RGBW SK6812 COLD white choose: <i>HyperSerialEsp8266.ino.d1_mini_RGBW_COLD_WHITE_FIRSTLED_xxxxxx.bin</i><br/><br/>
For <b>RGB LED strip</b> like WS8212b or RGB SK6812 variant choose: <i>HyperSerialEsp8266.ino.d1_mini_RGB_FIRSTLED_xxxxxx.bin</i><br/><br/><br/>
If the first LED in the strip should be enabled or set to black is your choice.<br/>
For the RGBW firmware the white channel is automatically calculated and R,G,B channels are corrected.<br/><br/>

Using esphome-flasher:<br/>
<img src="https://i.postimg.cc/X7yb9FQs/flash.jpg" width="640"/><br/>

# Usage in HyperHDR
<br/>
Watch out for latch time, don't go above 15ms as it will affect communication performance.<br/>
Switch to "Expert" level settings in the top-right corner to have following options visible.<br/><br/>
<img src="https://i.postimg.cc/7h0KZxnn/usage.jpg" width="640"/><br/>

# Result

RGB (250 leds, 100Hz)<br/>
<img src="https://i.postimg.cc/sjrQQ11Y/250-rgb-setup.jpg" width="640"/><br/><br/>

RGBW (250 leds, 100Hz)<br/>
<img src="https://i.postimg.cc/KZL38tcc/250-rgbw-setup.jpg" width="640"/><br/><br/>

# Compiling

Compile the sketch using arduin-ESP IDE and Makuna/NeoPixelBus <br/><br/>
<b>Options (first lines of the sketch):</b><br/><br/>
For RGB strip like WS8212b comment it, leave it for RGBW SK6812:<br/>
<i>#define   THIS_IS_RGBW</i><br/><br/>
Skip first led in the strip, that is used as level shifter:<br/>
<i>bool      skipFirstLed = true;</i><br/><br/>
Serial port speed:<br/>
<i>int       serialSpeed = 2000000;</i><br/><br/>
Don't change LED's count as it is dynamic.<br/>


# Disclaimer
You use it on your own risk.<br/>Don't touch these firmwares if you don't know how to put the device in the programming mode if something goes wrong.<br/>
As per the MIT license, I assume no liability for any damage to you or any other person or equipment.
