#ifndef _LCD_
#define _LCD_


#include "Arduino.h"

//#define _LCD_DEBUG_

class Lcd {

public:
  Lcd(int updateRegisterPin, int clockSerialPin, int dataSerialPin);
  
  
  void sendByte(char character);
  void sendChar(char character);
  // if number not representable an E is printed
  void sendDigit(int number, int numberOfDigitsToSend=1, char fillChar = ' ', boolean lsb=false,int delayBetweenDigits = 0);
  void sendCharString(char zeroEndedCharPointer[], boolean lsb=false, int delayBetweenDigits = 0);

private:

  void sendBit(boolean active);
  void internalSendByte(char character);
  void internalSendChar(char character);
  
  int _updateRegisterPin;
  int _clockSerialPin;
  int _dataSerialPin;
    

};

#endif /* _LCD_ */
