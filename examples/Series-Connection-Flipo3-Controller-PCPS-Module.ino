/************************* CHANGE THIS ***************************************************/
#define how_many_flipo_modules  4     // How many Flipo #3 modules are connected in series
/*****************************************************************************************/

// Marcin Saj Flipo Project - https://Flipo.io
// https://flipo.io/project/controller-flip-disc-3
// https://github.com/marcinsaj/Flipo-Controller-Flip-Disc-3
// https://flipo.io/project/pulse-current-power-supply
// https://github.com/marcinsaj/Flipo-Pulse-Current-Power-Supply
//
// This example demonstrates how to control multiple Flipo#3 modules
//
// Hardware:
// Flipo#3 Module for 3 flip discs - https://flipo.io/project/controller-flip-disc-3
// Pulse Current Power Supply Module for flip discs - https://flipo.io/project/pulse-current-power-supply
// Arduino UNO


// Declaration of the controller control inputs
#define DIN_PIN     12    // A - pcb markings
#define EN_PIN      11    // B
#define CLK_PIN     10    // C

// Declaration of the PCPS module control input/output
#define EN_VF       A0    // Turn ON/OFF charging PCPS module
#define FB_VF       A1    // Measurement output for checking if the current pulse is ready

uint8_t *discModePointer;
uint16_t value = 0;

// Bit notation for Flipo#3 controller - set flip-discs
// Always active only two bits corresponding to control outputs 
// Check controller documentation and schematic
uint8_t setDisc[]=
{
  0b10000010,
  0b10010000,
  0b10100000  
};

// Bit notation for Flipo#3 controller - reset flip-discs
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

  ClearController();            // Clear all Flipo #3 Controllers

  PrepareCurrentPulse();        // Prepare current pulse
}

void loop()
{
// If you want flip first disc call SetDisc(0); - always count from "0"
// If you want flip disc no.10 just call SetDisc(9);    
  
  for(int i = 0; i < how_many_flipo_modules * 3; i++)
  {
    SetDisc(i);
    delay(500); 
  }

  for(int i = 0; i < how_many_flipo_modules * 3; i++)
  {
    ResetDisc(i);
    delay(500);  
  }
}

// Clear all Flipo #3 Controllers
void ClearController(void)
{
  ShiftOutDataStart();

  for(int i = 0; i < how_many_flipo_modules; i++)
  {
    ShiftOutData(0);
  }

  ShiftOutDataEnd();  
}

// First charging - setup call
void PrepareCurrentPulse(void)
{
  CurrentPulseON();
} 

void CurrentPulseON()
{
  digitalWrite(EN_VF, HIGH);            // Turn ON PCPS module- charging begin

  do {value = analogRead(FB_VF);}       // Measure the voltage of the accumulated charge
  while (value < 500);                  // ~2.5V this voltage means that the current pulse is ready
 
  digitalWrite(EN_VF, LOW);             // Turn OFF PCPS module- charging complete
}

void CurrentPulseOFF(void)
{
  ClearController();                    // Clear all Flipo #3 Controllers
}

void SetDisc(uint8_t discNumber)
{
  discModePointer = setDisc;            // Set pointer to the setDisc bit array
  ModeDisc(discNumber);
}

void ResetDisc(uint8_t discNumber)
{
  discModePointer = resetDisc;          // Set pointer to the resetDisc bit array
  ModeDisc(discNumber);
}

void ModeDisc(uint8_t discNumber)
{
  CurrentPulseON();                     // Prepare current pulse

  ShiftOutDataStart();                  // Transfer data begin   

  // Turn on flip-disc controller corrsponding outputs
  for(int i = (how_many_flipo_modules-1); i >= 0; i--)
  {
    if(discNumber / 3 == i) ShiftOutData(discModePointer[discNumber % 3]);
    else ShiftOutData(0);
  }

  ShiftOutDataEnd();                    // Transfer data complete
  
  delay(1);                             // Flip-disc required 1ms current pulse to flip
  CurrentPulseOFF();                    // Absolutely required!
                                        // This function here turns off the current pulse 
                                        // and cleans the controller outputs    
}

void ShiftOutDataStart(void)
{
  digitalWrite(EN_PIN, LOW);            // Transfer data begin    
}

void ShiftOutData(uint8_t discNumber)
{
  shiftOut(DIN_PIN, CLK_PIN, MSBFIRST, discNumber);   // Send data to the controller
}

void ShiftOutDataEnd(void)
{
  digitalWrite(EN_PIN, HIGH);           // Transfer data complete
}
