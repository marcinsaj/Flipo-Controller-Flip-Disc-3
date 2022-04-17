/************************* CHANGE THIS ***************************************************/
#define how_many_flipo_modules  2     // How many Flipo #3 modules are connected in series
/*****************************************************************************************/

#define DIN_PIN     10  // A
#define EN_PIN      9   // B
#define CLK_PIN     8   // C

#define EN_VF       A0
#define FB_VF       A1

uint8_t *discModePointer;

uint8_t setDisc[]=
{
  0b10000010,
  0b10010000,
  0b10100000
};

uint8_t resetDisc[]=
{
  0b00000101,
  0b01000100,
  0b00001100
};

void setup() 
{
  pinMode(DIN_PIN, OUTPUT);
  digitalWrite(DIN_PIN, LOW);
 
  pinMode(EN_PIN, OUTPUT);
  digitalWrite(EN_PIN, LOW);
 
  pinMode(CLK_PIN, OUTPUT);
  digitalWrite(CLK_PIN, LOW);

  pinMode(EN_VF, OUTPUT);
  digitalWrite(EN_VF, LOW);  

  pinMode(EN_VF, OUTPUT);
  digitalWrite(EN_VF, LOW); 

  PrepareFlipoController();

  CurrentPulseON();
}

void loop()
{
  for(int i = 0; i < how_many_flipo_modules * 3; i++)
  {
    SetDisc(i); 
    delay(1000); 
  }

  for(int i = 0; i < how_many_flipo_modules * 3; i++)
  {
    ResetDisc(i);
    delay(1000);  
  }
}

void CurrentPulseON()
{
  digitalWrite(EN_VF, HIGH);  // Charging ON
  int value = 0;

  do 
  {
    value = analogRead(FB_VF);
  } while (value < 500);
 
  digitalWrite(EN_VF, LOW);   // Charging OFF, current pulse is ready to power the flip disc 
}

void PrepareFlipoController(void)
{
  CurrentPulseOFF(); 
}

void CurrentPulseOFF(void)
{
  ShiftOutDataStart();

  for(int i = 0; i < how_many_flipo_modules; i++)
  {
    ShiftOutData(0);
  }

  ShiftOutDataEnd();  
}

void SetDisc(uint8_t discNumber)
{
  discModePointer = setDisc;
  ModeDisc(discNumber);
}

void ResetDisc(uint8_t discNumber)
{
  discModePointer = resetDisc;
  ModeDisc(discNumber);
}

void ModeDisc(uint8_t discNumber)
{
  CurrentPulseON();

  ShiftOutDataStart();

  for(int i = (how_many_flipo_modules-1); i >= 0; i--)
  {
    if(discNumber / 3 == i) ShiftOutData(discModePointer[discNumber % 3]);
    else ShiftOutData(0);
  }

  ShiftOutDataEnd();
  
  delay(1);
  CurrentPulseOFF();    
}

void ShiftOutDataStart(void)
{
  digitalWrite(EN_PIN, LOW);    
}

void ShiftOutData(uint8_t discNumber)
{
  shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, discNumber);
}

void ShiftOutDataEnd(void)
{
  digitalWrite(EN_PIN, HIGH);   
}
