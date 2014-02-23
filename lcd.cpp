

#include "lcd.h"


// Variables will change:

// LDC control
char space = B00000000;
char dash  = B01000000;
char dot   = B10000000;
//                  0         1         2         3         4         5         6         7         8         9         A         b         C         D         E         F
char lcdDigits[] = {B00111111,B00000110,B01011011,B01001111,B01100110,B01101101,B01111101,B00000111,B01111111,B01101111,B00110111,B01111100,B00111001,B01011110,B01111001,B01110001};
//                   A         b         C         d         E         F         G         H         I         J         K*NO_SUP  L         M*NO_SUP  n         o         p         q         r         S         t         u         v*NO_SUP  w*NO_SUP  x*NO_SUP  y         z
char lcdLetters[] = {B01110111,B01111100,B00111001,B01011110,B01111001,B01110001,B01111101,B01110110,B00000110,B00011111,B00000000,B00111000,B00110111,B01010100,B01011100,B01110011,B01100111,B01010000,B01101101,B01111000,B00011100,B00111110,B00000000,B00000000,B01101110,B01011011};


Lcd::Lcd(int updateRegisterPin, int clockSerialPin, int dataSerialPin) {
  this->_updateRegisterPin = updateRegisterPin;
  this->_clockSerialPin = clockSerialPin;
  this->_dataSerialPin = dataSerialPin;
  if (this->_updateRegisterPin >= 0) {  
    pinMode(this->_updateRegisterPin, OUTPUT);
  }
  if (this->_clockSerialPin >=0 ) {
    pinMode(this->_clockSerialPin   , OUTPUT);
  }
  if (this->_dataSerialPin >= 0) {
    pinMode(this->_dataSerialPin    , OUTPUT);
  }
}


void Lcd::sendBit(boolean active) {

    if (_clockSerialPin >= 0) {
      digitalWrite(_clockSerialPin, LOW);
    }
    if (_dataSerialPin >= 0) {
      if (active) {
        digitalWrite(_dataSerialPin, HIGH);
      }else {
        digitalWrite(_dataSerialPin, LOW);
      }
    }
    if (_clockSerialPin >= 0) {
      digitalWrite(_clockSerialPin, HIGH);
    }
}

void Lcd::sendByte(char character) {
    if (_updateRegisterPin >= 0) {
    digitalWrite(_updateRegisterPin, LOW);
  }
  internalSendByte(character);
  if (_updateRegisterPin >= 0) {
    digitalWrite(_updateRegisterPin, HIGH);
  }

}

void Lcd::internalSendByte(char character) {
  for (int i=7; i>=0; i--) {
    sendBit(bitRead(character, i));
  }
}


void Lcd::sendChar(char character) {

#ifdef _LCD_DEBUG_
  Serial.print("sendChar ");
  Serial.println(character);
#endif

  if (_updateRegisterPin >= 0) {
    digitalWrite(_updateRegisterPin, LOW);
  }
  internalSendChar(character);
  if (_updateRegisterPin >= 0) {
    digitalWrite(_updateRegisterPin, HIGH);
  }
}


void Lcd::internalSendChar(char character) {

  char lcdCharacter=0x00;
  if (character >= '0' && character <= '9') {
    lcdCharacter=lcdDigits[character-'0'];
  }
  if (character >= 'A' && character <= 'Z') {
    lcdCharacter = lcdLetters[character-'A'];
  }
  if (character >= 'a' && character <= 'z') {
    lcdCharacter = lcdLetters[character-'a'];
  }
  if (character == ' ') {
    lcdCharacter = space;
  }
  if (character == '-') {
    lcdCharacter = dash;
  }
  internalSendByte(lcdCharacter);
}

void Lcd::sendDigit(int number, int numberOfDigitsToSend, char fillChar, boolean lsb,int delayBetweenDigits) {

#ifdef _LCD_DEBUG_
  Serial.print("sendDigit ");
  Serial.println(number);
#endif

  int multiplier = 1;
  for (int i=0; i < numberOfDigitsToSend; i++) {
    multiplier = multiplier * 10;
  }

  char buffer[numberOfDigitsToSend + 1];
  buffer[numberOfDigitsToSend] = 0;
  
  if (number / multiplier == 0 && number >= 0) {
    int shiftedNum = number;
    for (int i = 0; i < numberOfDigitsToSend; i++) {
      int digit = shiftedNum % 10;
      if (shiftedNum > 0 || i == 0) {
        buffer[i] = '0' + digit;
      } else {
        buffer[i] = fillChar;
      }
      shiftedNum = shiftedNum / 10;
    } 
  } else if (number * 10 / multiplier == 0 && number <= 0) {
    int shiftedNum = number * -1;
    boolean sentMinusSign=false;
      for (int i=0; i < numberOfDigitsToSend; i++) {
      int digit = shiftedNum % 10;
      if (shiftedNum > 0 || i == 0) {
        buffer[i] = '0' + digit;
      } else if (!sentMinusSign) {
        buffer[i] = '-';
        sentMinusSign = true;
      } else {
        buffer[i] = fillChar;
      }
      shiftedNum = shiftedNum / 10;
    } 
  } else { 
      for (int i=0; i < numberOfDigitsToSend; i++) {
       if (i == numberOfDigitsToSend - 1) {
         buffer[i]= 'E';
       } else {
           buffer[i]= ' ';
       }
    }
  }
  sendCharString(buffer, lsb, delayBetweenDigits);
}

void Lcd::sendCharString(char zeroEndedCharPointer[], boolean lsb, int delayBetweenDigits) {
#ifdef _LCD_DEBUG_
  Serial.print("sendCharString ");
  Serial.println(zeroEndedCharPointer);
#endif
  char * currentChar = zeroEndedCharPointer;
  
  if (_updateRegisterPin >= 0) {
    digitalWrite(_updateRegisterPin, LOW);
  }
  if (lsb) {
    while (*currentChar) {
      currentChar++;
    } 
    while (currentChar != zeroEndedCharPointer) {  
      currentChar--;
      sendChar(*currentChar);
      if (delayBetweenDigits > 0) {
        delay(delayBetweenDigits);
      }
    }    
  } else {
    while (*currentChar) {  
      sendChar(*currentChar);
      if (delayBetweenDigits > 0) {
        delay(delayBetweenDigits);
      }
      currentChar++;
    }   
  }
  if (_updateRegisterPin >= 0) {
    digitalWrite(_updateRegisterPin, HIGH);
  }
}
