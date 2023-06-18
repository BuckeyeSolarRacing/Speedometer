// A portion of this code was borrowed from a very helpful Youtube video, cited below:
// Bill, director. Using Rotary Encoders with Arduino. YouTube, YouTube, 23 Mar. 2019, https://www.youtube.com/watch?v=V1txmR8GXzE. Accessed 22 Apr. 2023. 

// ---------------------------------------------------
// SETUP BELOW
// ---------------------------------------------------

void setup()
{
  #define SER_CON 5   // Binary sequence is sent through this pin to be shifted in to shift register
  #define CLK_CON 4   // Controls when shift register shifts

  pinMode(SER_CON, OUTPUT);
  pinMode(CLK_CON, OUTPUT);

  // These pins control which digits light up. 0 means digit is illuminated
  #define FirstLetter PB13
  #define SecondLetter PB14
  #define ThirdLetter 2
  #define FourthLetter 3

  pinMode(FirstLetter, OUTPUT);
  pinMode(SecondLetter, OUTPUT);
  pinMode(ThirdLetter, OUTPUT);
  pinMode(FourthLetter, OUTPUT);

  // Binary sequences controlling which segments light up. There is a unique sequence for every number
  #define NumZero 0b00111111
  #define NumOne 0b00000110
  #define NumTwo 0b01011011
  #define NumThree 0b01001111
  #define NumFour 0b01100110
  #define NumFive 0b01101101
  #define NumSix 0b01111101
  #define NumSeven 0b00000111
  #define NumEight 0b01111111
  #define NumNine 0b01100111

  #define ENC_COUNT_REV 48   // CHANGE TO 48 pulses per rotation
  #define ENC_IN 15          // PWM motor speed input

  pinMode(ENC_IN, INPUT_PULLDOWN);    // Default value is 0
  attachInterrupt(digitalPinToInterrupt(ENC_IN), updateEncoder, RISING);

  Serial.begin(9600);
}

// ---------------------------------------------------
// MAIN LOOP BELOW
// ---------------------------------------------------

int analogInput;
volatile long encoderValue = 0;
int intervalLength = 250;        //quarter second "refresh rate"
long previousMillis = millis();
long currentMillis = 0;
int rpm = 0;
float mph = 0;

void loop()
{
  currentMillis = millis();
  if (currentMillis - previousMillis > intervalLength)
  {
    previousMillis = currentMillis;
    rpm = (float)(encoderValue * 60 * 4 / ENC_COUNT_REV);   // calculate speed in rpm
    mph = rpm * 0.047599888690754;                          // calculate speed in mph
    encoderValue = 0;
  }  

  Serial.print(encoderValue);
  Serial.print('\n');
  Serial.print(mph);
  Serial.print('\n');
  
  int CheckDigit = 0;
  for (int DigitIndex = 0; DigitIndex < 4; ++DigitIndex)
  {
    // All digits are initialized as being turned off
    digitalWrite(FirstLetter, 1);
    digitalWrite(SecondLetter, 1);
    digitalWrite(ThirdLetter, 1);
    digitalWrite(FourthLetter, 1);

    CheckDigit = ((int)mph / (int)pow(10, DigitIndex)) % 10;
    if (CheckDigit == 0)
    {
      shift_out(NumZero);
    }
    else if (CheckDigit == 1)
    {
      shift_out(NumOne);
    }
    else if (CheckDigit == 2)
    {
      shift_out(NumTwo);
    }
    else if (CheckDigit == 3)
    {
      shift_out(NumThree);
    }
    else if (CheckDigit == 4)
    {
      shift_out(NumFour);
    }
    else if (CheckDigit == 5)
    {
      shift_out(NumFive);
    }
    else if (CheckDigit == 6)
    {
      shift_out(NumSix);
    }
    else if (CheckDigit == 7)
    {
      shift_out(NumSeven);
    }
    else if (CheckDigit == 8)
    {
      shift_out(NumEight);
    }
    else if (CheckDigit == 9)
    {
      shift_out(NumNine);
    }

    // one digit is displayed at a time
    if (DigitIndex == 0)
    {
      digitalWrite(FourthLetter, 0);
    }
    else if (DigitIndex == 1)
    {
      digitalWrite(ThirdLetter, 0);
    }
    else if (DigitIndex == 2)
    {
      digitalWrite(SecondLetter, 0);
    }
    else if (DigitIndex == 3)
    {
      digitalWrite(FirstLetter, 0);
    }

    delay(1);
  }
}

// ---------------------------------------------------
// SUBROUTINES BELOW
// ---------------------------------------------------

// Shifts out 8-bit value with shift register
void shift_out(int data)
{
  int i;
  for(i = 0; i < 8; i++)
  {
    digitalWrite(SER_CON, ((data>>(7-i))&0x01));
    digitalWrite(CLK_CON, 1);
    digitalWrite(CLK_CON, 0);
  }
  digitalWrite(CLK_CON, 1);
  digitalWrite(CLK_CON, 0);
}

void updateEncoder()
{
  encoderValue++;
}
