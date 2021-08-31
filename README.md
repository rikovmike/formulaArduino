# Formula Arduino
Simple racing game for Arduino and WH2004 LCD display with analogue steering controls and optional buzzer sound effects. There no buttons or other components for controls. Onle resistor for steering wheel emulation.

# Hardware
Theoretically, any Arduino with digital and analog ports sufficient to connect a symbolic LCD screen and a variable resistor for control is enough for you. You can connect buzzer for sound effects, but this is optional.
The finished sketch takes ~16 KB of flash memory, so any arduino with 32 KB is enough.

This project uses a standard **Winstar WH2004A** screen, 20x4 characters. You can use any other one that can work with the LiquidCrystal library. Please note that for screens of other dimensions, you will need to correct the source code.

For controls, use one **10kohm variable resistor**. 

# Connections
Use this simple cheme to build a game machine:

![FormulaArduinoScheme](https://user-images.githubusercontent.com/32895646/131463721-aac58252-73eb-4bbb-8e03-b4c0f70f7468.png)


# Any questions?
Please, feel free to contact me for any questions about this project. Thanks!
http://www.rikovmike.ru
mike@rikovmike.ru


