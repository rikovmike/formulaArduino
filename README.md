![screenshot](https://user-images.githubusercontent.com/32895646/131472930-fa562ab3-0102-4e1c-ab85-b0b49a422d69.png)

# Formula Arduino
Simple racing game for Arduino and WH2004 LCD display with analogue steering controls and optional buzzer sound effects. There no buttons or other components for controls. Only one resistor for steering wheel emulation.

![fa_gif2](https://user-images.githubusercontent.com/32895646/131472952-5126ce3b-271d-4012-9911-aab0b0fed8c0.gif)

# Hardware
Theoretically, any Arduino with digital and analog ports sufficient to connect a symbolic LCD screen and a variable resistor for control is enough for you. You can connect a buzzer for sound effects, but this is optional.
The finished sketch takes ~16 KB of flash memory, so any Arduino board with 32 KB will be enough.

This project uses a cheap and simple non-i2c **Winstar WH2004A** screen, 20x4 characters. You can use any other one that can work with the [LiquidCrystal](https://www.arduino.cc/en/Reference/LiquidCrystal) library. Please note that for screens of other dimensions, you will need to correct the source code.

For controls, use one **10kohm variable resistor**. 

# Connections
Use this simple scheme to build a game machine:

![FormulaArduinoScheme](https://user-images.githubusercontent.com/32895646/131463721-aac58252-73eb-4bbb-8e03-b4c0f70f7468.png)


# Emulator Demo (some screen glithes)

https://wokwi.com/arduino/projects/321213469665788500

# Any questions?
Please, feel free to contact me for any questions about this project. 

Thanks!

[https://rikovmike.ru/](https://rikovmike.ru/)

mike@rikovmike.ru
