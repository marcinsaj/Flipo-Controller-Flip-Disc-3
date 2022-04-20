#define DIN_PIN     10   // A
#define EN_PIN      9  // B
#define CLK_PIN     8  // C
#define EN_VF       A0
#define FB_VF       A1

#define Button1     9
#define Button2     8
#define Button3     7


int value;

uint16_t setDot[]=
{
  0b0000110000000000,
  0b0100010000000000,
  0b0000010100000000,
  0b0000000000001100,
  0b0000000001000100,
  0b0000000000000101
};

uint16_t resetDot[]=
{
  0b1010000000000000,
  0b1001000000000000,
  0b1000001000000000,
  0b0000000010100000,
  0b0000000010010000,
  0b0000000010000010
};

void setup() 
{
  pinMode(DIN_PIN, OUTPUT);
  digitalWrite(DIN_PIN, LOW);
 
  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, LOW);
 
  pinMode(CLK_PIN, OUTPUT);
  digitalWrite(CLK_PIN, LOW);

  pinMode(A0, OUTPUT);
  digitalWrite(A0, LOW); 

  ShiftOutData(0);

  CurrentPulse();
}

// the loop function runs over and over again forever
void loop()
{
  Animation();

  
}

void CurrentPulse()
{
  digitalWrite(A0, HIGH);

  do
  {
    value = analogRead(A1);
  //delayMicroseconds(10);
  } while (value < 500);
 
  digitalWrite(A0, LOW);
}




void ShiftOutDatax(void)
{
  digitalWrite(EN_PIN, LOW);  
  shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, 0); 
  shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, 0);
  digitalWrite(EN_PIN, HIGH);
}


void ShiftOutData(uint16_t dotNumber)
{
  digitalWrite(EN_PIN, LOW);  
  shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, highByte(dotNumber));
  shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, lowByte(dotNumber));
  digitalWrite(EN_PIN, HIGH);
}

void ResetDisc(uint16_t discNumber)
{
  CurrentPulse();
  ShiftOutData(setDot[discNumber]);
  delay(1);

  ShiftOutData(0);    
}

void SetDisc(uint16_t discNumber)
{
  CurrentPulse();
  ShiftOutData(resetDot[discNumber]);
  delay(1);  

  ShiftOutData(0);  
}

void Animation(void)
{

  for(int i = 0; i < 6; i++)
  {
    SetDisc(i);
    delay(500);   
  }

  for(int i = 0; i < 6; i++)
  {
    ResetDisc(i);
    delay(500); 
  }


  for(int j = 1; j <= 10; j++)
  {
    for(int i = 5; i >= 0; i--)
    {
      SetDisc(i);
      delay(500/(j));   
    } 
    
    for(int i = 0; i < 6; i++)
    {
      ResetDisc(i);
      delay(500/(j)); 
    } 
  }


for(int j = 3; j < 10; j++)
{

    for(int i = 0; i < 5; i++)
    {
      SetDisc(i);
      delay(1000/j);
      ResetDisc(i); 
      delay(10);  
    }

      SetDisc(5);
      delay(1000/j);
      ResetDisc(5); 
      delay(10);  
    

    for(int i = 4; i > 0; i--)
    {
      SetDisc(i);
      delay(1000/j);
      ResetDisc(i); 
      delay(10);
    }
}    


}

