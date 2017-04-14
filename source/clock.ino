#include <FastLED.h>
#include <Wire.h>    
#include "RTClib.h" 

#define NUM_LEDS 34

CRGB ledsPos1[NUM_LEDS];
CRGB ledsPos2[NUM_LEDS];
CRGB ledsPos3[NUM_LEDS];
CRGB ledsPos4[NUM_LEDS];
CRGB ledBlink[NUM_LEDS];

int hour, minute;
RTC_DS1307 RTC; 

int blinkStatus = 0;

void setup() {
  // Setup each LED-7Segment block and save it to an array
  FastLED.addLeds<NEOPIXEL, 5>(ledsPos1, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 6>(ledsPos2, NUM_LEDS); 
  FastLED.addLeds<NEOPIXEL, 9>(ledsPos3, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 10>(ledsPos4, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 3>(ledBlink, 2);

  //Write the number 1 to each segment
  ledPrintNumber(1, 1);
  ledPrintNumber(1, 2);
  ledPrintNumber(1, 3);
  ledPrintNumber(1, 4);
  FastLED.show(); 

  //Setup the real time clock
  Wire.begin();
  RTC.begin();
  // Serielle Ausgabe starten 
  Serial.begin(9600);
  // Check if the RTC is running
  if (! RTC.isrunning()) {    
    // Set current Daten and Time if it was not running
    RTC.adjust(DateTime(__DATE__, __TIME__));
        Serial.println("Echtzeituhr wurde gestartet und auf Systemzeit gesetzt.");
  }
  else Serial.println("Echtzeituhr laeuft bereits.");
}

void loop() {

  //Toggle the seconds leds each second (nein - doch - ohhhh)
  if(blinkStatus==0)
  {
    ledBlink[0] = CRGB(50,0,0);
    ledBlink[1] = CRGB(50,0,0);
    blinkStatus = 1;
  }
  else
  {
     ledBlink[0] = CRGB::Black;
    ledBlink[1] = CRGB::Black;
    blinkStatus = 0;
  }
  FastLED.show(); 

  //Update Hour and Minute
  DateTime now=RTC.now(); // get the current time
  hour = now.hour(),DEC;
  minute = now.minute(),DEC;
  printHour(hour); //Print the current hour to 
  printMinute(minute);
  delay(1000);

}


void printHour(int hourInt)
{
  String hourStr = String(hourInt);
  if(hourStr.length()==1) //If the number includes only one char it will turn of the first 7segment block
  {
    ledPrintNumber(10, 1);
    ledPrintNumber(hourInt, 2);
  }
  else
  {
    ledPrintNumber(hourStr.substring(0,1).toInt(), 1);
    ledPrintNumber(hourStr.substring(1,2).toInt(), 2);
  }
}
void printMinute(int minuteInt)
{
  String minuteString = String(minuteInt);
  if(minuteString.length()==1) //If the number includes only one char it will turn the first 7segment block to a "0"
  {
    ledPrintNumber(0, 3);
    ledPrintNumber(minuteInt, 4);
  }
  else
  {
    ledPrintNumber(minuteString.substring(0,1).toInt(), 3);
    ledPrintNumber(minuteString.substring(1,2).toInt(), 4);
  }
}

void ledPrintNumber(int number, int pos)
{
  //Which LEDS are part of one Segment (start end end led number)
  int segments[7][2] = {
      {
        0, 4
      },
      {
        5,8
      },
      {
        9,13
      },
      {
        14,17
      },
      {
        19, 23
      },
      {
        23, 28
      },
      {
        28,33
      }
    };

  // Which segment should be turned on for each number (and number 10 to turn it off)
  int intseg[11][7] = 
  {
    {
      0,1,1,1,1,1,1 /*0*/
    },
    {
      0,0,0,1,1,0,0 /*1*/
    },
    {
      1,0,1,1,0,1,1 /*2*/
    },
    {
      1,0,1,1,1,1,0 /*3*/
    },
    {
      1,1,0,1,1,0,0 /*4*/
    },
    {
      1,1,1,0,1,1,0 /*5*/
    },
    { 
      1,1,1,0,1,1,1 /*6*/
    },
    {
      0,0,1,1,1,0,0 /*7*/
    },
    {
      1,1,1,1,1,1,1 /*8*/
    },
    {
      1,1,1,1,1,1,0 /*9*/
    },
    {
      0,0,0,0,0,0,0 /*off 10*/
    },
  };
  
  for(int i = 0; i < 7; i++) //Check for each segment if it turn on or off
  {
    int state = intseg[number][i]; //Save the state (on or off) in a varieable
    for(int s = segments[i][0]; s <= segments[i][1]; s++) //Run for each LED in the segment (from the start to the end led)
    {
      if(state==1) //turn it on
      {
        //Check which block (segment, number) should be turned on
        if(pos==1)
        {
          ledsPos1[s] = CRGB(50,0,0);
        }
        else if(pos==2)
        {
          ledsPos2[s] = CRGB(50,0,0);
        }
        else if(pos==3)
        {
          ledsPos3[s] = CRGB(50,0,0);
        }
        else if(pos==4)
        {
          ledsPos4[s] = CRGB(50,0,0);
        }
        else
        {
          
        }
      }
      else //turn it off
      {
        //Check which block (segment, number) should be turned on
        if(pos==1)
        {
          ledsPos1[s] = CRGB::Black; 
        }
        else if(pos==2)
        {
          ledsPos2[s] = CRGB::Black; 
        }
        else if(pos==3)
        {
          ledsPos3[s] = CRGB::Black; 
        }
        else if(pos==4)
        {
          ledsPos4[s] = CRGB::Black; 
        }
        else
        {
          
        }
      }
    }
  }
  FastLED.show(); 
} 
