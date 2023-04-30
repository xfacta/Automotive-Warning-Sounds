
// Warning Sounds
// for inputs from Speedometer and/or RPM meter
// and central Fuel/Temperature/Volts meter
// an amplified speaker should be connected
// input pins active LOW



#define Version "Warning Sounds V1"



// Using Leonardo Tiny
// Atmega32U4
// Available pins
// A0(18) A1(19) A2(20)
// D9 D10 D11
// Serial 1 TX(1) RX(0)
// I2C SCA(2) SCL(3)
// D13 for LED


// Pin definitions for inputs
// general warning pins could all be combined into one via a diode
// leaving only the Oil Pressure warning as special
const int Other_Warning_Pin = 9;               // Input from Speedometer or RPM meter
const int General_Warning_Pin = 10;            // Input from Fuel/Temp/Volts
const int Oil_Pressure_Warning_Pin = 11;       // Input specifically for Oil Pressure


// Pin definitions for outputs
const int Buzz_Pin = 18;                      // Speaker for warning sounds analog pin A0
//const int LED_BUILTIN = LED_BUILTIN;        // Builtin LED indicator


// Buzzer variables
const int Buzz_Loop = 3, Buzz_Step = 20;
int Buzz_Count, Buzz_Hz;


// Times of last important events
uint32_t Oil_Sound_Time, Warn_Time, Last_blink_Time;
uint32_t Warn_Interval = 15000;
bool Startup_Mode = true;



// ##################################################################################################################################################



void setup() {

  // Outputs
  pinMode(Buzz_Pin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  noTone(Buzz_Pin);

  // Digital inputs
  pinMode(Other_Warning_Pin, INPUT_PULLUP);
  pinMode(General_Warning_Pin, INPUT_PULLUP);
  pinMode(Oil_Pressure_Warning_Pin, INPUT_PULLUP);

  // Analog


  // LED triple blink to indicate correct startup
  delay(1000);
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(200);
  digitalWrite(LED_BUILTIN, LOW);
  delay(200);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(200);
  digitalWrite(LED_BUILTIN, LOW);
  delay(200);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(200);
  digitalWrite(LED_BUILTIN, LOW);
  Last_blink_Time = millis();

}


// ##################################################################################################################################################


void loop() {


  if ((digitalRead(Oil_Pressure_Warning_Pin) == LOW) && ((millis() - Oil_Sound_Time) > (Warn_Interval / 3)))
  {
    digitalWrite(LED_BUILTIN, HIGH);
    Oil_Sound_Time = millis();
    for (Buzz_Count = 0; Buzz_Count < Buzz_Loop; Buzz_Count++)
      // loop the required number of buzzer sounds
    {
      for (Buzz_Hz = 200; Buzz_Hz < 700; Buzz_Hz += Buzz_Step)
      {
        tone(Buzz_Pin, Buzz_Hz, Buzz_Step);
        // allows some time for each frequency to play
        delay(Buzz_Step - (Buzz_Step / 10));
      }
      // allows a gap between each alarm loop
      delay(Buzz_Step * 10);
    }
    noTone(Buzz_Pin);
    digitalWrite(LED_BUILTIN, LOW);
    Last_blink_Time = millis();
  }

  else if ((digitalRead(Other_Warning_Pin) == LOW) && ((millis() - Warn_Time) > Warn_Interval))
  {
    digitalWrite(LED_BUILTIN, HIGH);
    Warn_Time = millis();
    // Rising sound
    tone(Buzz_Pin, 200, 200); delay(190); tone(Buzz_Pin, 600, 200); delay(210);
    noTone(Buzz_Pin);
    digitalWrite(LED_BUILTIN, LOW);
    Last_blink_Time = millis();
  }

  else if ((digitalRead(General_Warning_Pin) == LOW) && ((millis() - Warn_Time) > Warn_Interval))
  {
    digitalWrite(LED_BUILTIN, HIGH);
    Warn_Time = millis();
    // Falling sound
    tone(Buzz_Pin, 600, 200); delay(190); tone(Buzz_Pin, 200, 200); delay(210);
    noTone(Buzz_Pin);
    digitalWrite(LED_BUILTIN, LOW);
    Last_blink_Time = millis();
  }

  // Blink every 10 seconds as proof of life

  if (millis() > (Last_blink_Time + 10000))
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(100);
    digitalWrite(LED_BUILTIN, LOW);
    Last_blink_Time = millis();
  }


} // end void loop



// ##################################################################################################################################################
