/*
 * FORMULA ARDUINO
 * Car setup header file
 * 
 * Using PROGMEM directive to store this massive data on flash, not in RAM
 * 
 * Developed by rikovmike 
 * mike@rikovmike.ru
 * 
 * ver a1.0
 * 
 */



const byte carFrames[6][8] PROGMEM = {
  

    {
    B11010,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000
    },


    {
    B01111,
    B01101,
    B11010,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000
    },



    {
    B11010,
    B01101,
    B01111,
    B01101,
    B11010,
    B00000,
    B00000,
    B00000
    },

    {
    B00000,
    B00000,
    B11010,
    B01101,
    B01111,
    B01101,
    B11010,
    B00000
    },
    
    {
    B00000,
    B00000,
    B00000,
    B00000,
    B11010,
    B01101,
    B01111,
    B01101
    },


    {
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B00000,
    B11010,
    B01101
    } 


};
