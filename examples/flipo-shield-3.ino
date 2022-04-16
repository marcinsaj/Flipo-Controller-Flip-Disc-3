#define DIN_PIN     12   // A
#define EN_PIN      11  // B
#define CLK_PIN     10  // C
#define EN_VF       A0
#define FB_VF       A1

#define Button1     9
#define Button2     8
#define Button3     7


int value;

uint8_t setDot[]=
{
  0b00001100,
  0b01000100,
  0b00000101
};

uint8_t resetDot[]=
{
  0b10100000,
  0b10010000,
  0b10000010
};

void setup() {
  Serial.begin(9600);
  delay(3000);
 
  pinMode(DIN_PIN, OUTPUT);
  digitalWrite(DIN_PIN, LOW);
 
  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, LOW);
 
  pinMode(CLK_PIN, OUTPUT);
  digitalWrite(CLK_PIN, LOW);

    pinMode(A0, OUTPUT);
  digitalWrite(A0, LOW); 

    pinMode(Button1, INPUT_PULLUP);


    pinMode(Button2, INPUT_PULLUP);


    pinMode(Button3, INPUT_PULLUP);


ShiftOutData(0);


  pinMode(A1, INPUT);
 // CurrentPulse();
}

// the loop function runs over and over again forever
void loop()
{

/*
  int buttonState1 = digitalRead(Button1);
    int buttonState2 = digitalRead(Button2);
      int buttonState3 = digitalRead(Button3);


 if (buttonState1 == LOW) {
    CurrentPulse();
    ShiftOutData(resetDot[1]);
    delay(1);  
    ShiftOutDatax();

    delay(1000); 

  }

   if (buttonState2 == LOW) {
    CurrentPulse();
    ShiftOutData(resetDot[2]);
    delay(1);  
    ShiftOutDatax();

    delay(1000); 

  }

   if (buttonState3 == LOW) {
    CurrentPulse();
    ShiftOutData(setDot[1]);
    delay(1);  

    ShiftOutDatax();

    delay(1000); 

  }
 */
 
 for(int i = 0; i < 3; i++)
  {
    CurrentPulse();
    ShiftOutData(setDot[i]);
    delay(1);    

             ShiftOutData(0);

    delay(1000);    
  }

    for(int i = 0; i < 3; i++)
  {
    CurrentPulse();
    ShiftOutData(resetDot[i]);
    delay(1);  

         ShiftOutData(0);
    delay(1000);  
  }

  
}

void CurrentPulse()
{
  digitalWrite(A0, HIGH);

  do
  {
    value = analogRead(A1);

 // delayMicroseconds(10);
  Serial.println(value);
  } while (value < 800);
 
 Serial.println("ok");
//  delay(fliptime);
  digitalWrite(A0, LOW);
}




void ShiftOutDatax(void)
{
  digitalWrite(EN_PIN, LOW);  
  shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, 0); 
  shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, 0);
  shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, 0);
  shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, 0);

 
 
  digitalWrite(EN_PIN, HIGH);
}


void ShiftOutData(uint8_t dotNumber)
{
  digitalWrite(EN_PIN, LOW);  
  shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, 0); 
  shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, 0);
    shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, 0);
  shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, dotNumber);

 
 
  digitalWrite(EN_PIN, HIGH);
}

/*
  for(int i = FLIPO3_MOD; i > 0; i--)
  {
    shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, 0);  
  }
*/
