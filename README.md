# ESP32 Neural Network Avoid Robot

This work is based on AI_delta_float_v1.2 by Tim Kälin 14-04-2020.  
https://www.the-diy-life.com/running-an-artificial-neural-network-on-an-arduino-uno/  
https://www.youtube.com/watch?v=SL0z8HkPOmg  

My version uses a Banana PI BPI-Bit (ESP32) based controller, and an Adeept Starry:Bit robot Chassis  
 
The robot is much smaller then the original, the LED lights were changed from using Adafruit neopixel library to using FastLED library (limited success here). The big change was to the drive portion of the code since the two robots use different drivers.
Other small changes, a lot of the small sketches from the original project were placed back into one sketch.  

Another small change was to add in an averaging of the sensor readings (I'm not sure it made much of a difference honestly).  

Video of mine working (a work in progress): https://youtu.be/6gSUvaeQpYA  

Code is a little rough around the edges (a lot rough) and needs cleaned up a bit.  
There are three directories included:  
Neural Net Avoid - code without an averaging applied (closest to original project)  
Neural Net Avoid Average - code with an averaging applied (not sure it made much difference)  
Test Code - contains various tests for motors, leds, sensors ect.  

The Starry:Bit Chassis was a nice choice for this (at least for me), Not only did I have a BPI-Bit which is an ESP32 based replacement for the Micro:bit - This robot has a pinout broken out on it so adding sensors is easy enough. Unfortuntally it is only a 3.3v board, so the Ultrasonics I used came from Adafruit and are 3v they are a little more then the standard HC-SR04s. They work the same way however, so more or less drop in replacements. (They are smaller, so you will need to make arrangements to mount them differently). A small breadboard was used to distrub power. The motor driver for this chassis is slightly different and changes to the original code were made for this. However the chassis is setup with driving the motors with a PIN and PWM setup, so just some minor changes were needed.  The original code used 6 pins, the starry:bit uses 4.  

Other then that it was fairly simple to get this to work, some minor changes, some tweaks to the motor drive - but I think it is working in a large open area pretty well. It doesn't seem to do so well in a closed in area.  

## Contributing

1. Fork it!
2. Create your feature branch: `git checkout -b my-new-feature`
3. Commit your changes: `git commit -am 'Add some feature'`
4. Push to the branch: `git push origin my-new-feature`
5. Submit a pull request

## Support Me

If you find this or any of my projects useful or enjoyable please support me.  
Anything I do get goes to buy more parts and make more/better projects.  
https://www.patreon.com/kd8bxp  
https://ko-fi.com/lfmiller  
https://www.paypal.me/KD8BXP  

## Other Projects

https://www.youtube.com/channel/UCP6Vh4hfyJF288MTaRAF36w  
https://kd8bxp.blogspot.com/  


## Credits

Copyright (c) 2022 LeRoy Miller

## License

This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses>
