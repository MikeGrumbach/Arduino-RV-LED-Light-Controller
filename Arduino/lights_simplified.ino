#include <IRremote.h>

int irSig = 2; // ir input signal from ir receiver

uint32_t button = 0;
const uint32_t button_1 = 0xBA45FF00;  // IR Controller zone 1

/* Future Work */
/*
const uint32_t button_2 = 0xB946FF00;  // IR Controller zone 2
const uint32_t button_3 = 0xB847FF00;  // IR Controller zone 3
const uint32_t button_4 = 0xBB44FF00;  // IR Controller zone 4
const uint32_t button_5 = 0xBF40FF00;  // IR Controller zone 5
const uint32_t button_up = 0xE718FF00;  // IR Controller zone zones 3,4,5 all at once
const uint32_t button_down = 0xAD52FF00;  // IR Controller zones 1,2 both at once
const uint32_t button_left = 0xF708FF00;  // IR Controller zone 5,4,3,2,1 one at a time
const uint32_t button_right = 0xA55AFF00;  // IR Controller zone 1,2,3,4,5 one at a time
const uint32_t button_ok = 0xE31CFF00;  // IR Controller zone 1,2,3,4,5 all at once
*/

// one curcuit board controls each zone, or bank of ceiling lights.
// additioal boards are needed for each zone
// only zone 1 was used for this project, but easily expanded to multiple zones

int zone1_t2Pin = 3; // npn bjt which turns the lights off with a 15 second latch time
int zone1_t3Pin = 4; // npn bjt which turns the lights back on with a 0.25 second on signal
int zone1_ledPin = 5; // input signal from green led used to reset lightCircuitEnabled flag if light is turned on from wall switch rather than from the arduino

/* Future Work */
/*
int zone2_t2Pin = 6; // npn bjt which turns the lights off with a 15 second latch time
int zone2_t3Pin = 7; // npn bjt which turns the lights back on with a 0.25 second on signal
int zone2_ledPin = 8; // input signal from green led used to reset lightCircuitEnabled flag if light is turned on from wall switch rather than from the arduino

int zone3_t2Pin = 9; // npn bjt which turns the lights off with a 15 second latch time
int zone3_t3Pin = 10; // npn bjt which turns the lights back on with a 0.25 second on signal
int zone3_ledPin = 11; // input signal from green led used to reset lightCircuitEnabled flag if light is turned on from wall switch rather than from the arduino

int zone4_t2Pin = 12; // npn bjt which turns the lights off with a 15 second latch time
int zone4_t3Pin = 13; // npn bjt which turns the lights back on with a 0.25 second on signal
int zone4_ledPin = 14; // input signal from green led used to reset lightCircuitEnabled flag if light is turned on from wall switch rather than from the arduino

int zone5_t2Pin = 15; // npn bjt which turns the lights off with a 15 second latch time
int zone5_t3Pin = 16; // npn bjt which turns the lights back on with a 0.25 second on signal
int zone5_ledPin = 17; // input signal from green led used to reset lightCircuitEnabled flag if light is turned on from wall switch rather than from the arduino
*/

// circuit board led (Green LED in the schematic)
// when the circuit is powered up for the first time, the GREEN LED is ON, whether the LIGHT SWITCH is ON or OFF
// when the LIGHT SWITCH is ON and the arduino signals to shut the light OFF, lightCircuitEnabled = true and the GREEN LED is OFF
// if the arduino is activated again, lightCircuitEnabled = false and the GREEN LED is ON
// but if the light switch is toggled OFF then ON, the GREEN LED will turn back ON, but lightCiruitEnabled = true.
// so the next arduino signal sets lightCircuitEnabled = false, then to true, which turns the lights OFF again.

bool lightCircuitEnabled = false;

bool zone1_lightCircuitEnabled = false;
int zone1_ledPinValue = 0; // holds the read value of the ledPin

/* Future Work */
/*
bool zone2_lightCircuitEnabled = false;
int zone2_ledPinValue = 0; // holds the read value of the ledPin

bool zone3_lightCircuitEnabled = false;
int zone3_ledPinValue = 0; // holds the read value of the ledPin

bool zone4_lightCircuitEnabled = false;
int zone4_ledPinValue = 0; // holds the read value of the ledPin

bool zone5_lightCircuitEnabled = false;
int zone5_ledPinValue = 0; // holds the read value of the ledPin
*/

int loopCounter = 0; //for limiting the ledPin write statements from going wild

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  IrReceiver.begin(irSig, ENABLE_LED_FEEDBACK); 

  pinMode(zone1_t2Pin, OUTPUT); // turn the light off
  pinMode(zone1_t3Pin, OUTPUT); // turn the iight on 
  pinMode(zone1_ledPin, INPUT); // reset the lightCircuitEnabled Flag
 
  /* Future Work */
  /*
  pinMode(zone2_t2Pin, OUTPUT); // turn the light off
  pinMode(zone2_t3Pin, OUTPUT); // turn the iight on 
  pinMode(zone2_ledPin, INPUT); // reset the lightCircuitEnabled Flag

  pinMode(zone3_t2Pin, OUTPUT); // turn the light off
  pinMode(zone3_t3Pin, OUTPUT); // turn the iight on 
  pinMode(zone3_ledPin, INPUT); // reset the lightCircuitEnabled Flag

  pinMode(zone4_t2Pin, OUTPUT); // turn the light off
  pinMode(zone4_t3Pin, OUTPUT); // turn the iight on 
  pinMode(zone4_ledPin, INPUT); // reset the lightCircuitEnabled Flag

  pinMode(zone5_t2Pin, OUTPUT); // turn the light off
  pinMode(zone5_t3Pin, OUTPUT); // turn the iight on 
  pinMode(zone5_ledPin, INPUT); // reset the lightCircuitEnabled Flag
  */
}

void loop() {
  if(IrReceiver.decode()){  
    // put your main code here, to run repeatedly:
    zone1_ledPinValue = digitalRead(zone1_ledPin); // check if GREEN LED is ON

    /* Future Work */
    /*
    zone2_ledPinValue = digitalRead(zone2_ledPin); // check if GREEN LED is ON
    zone3_ledPinValue = digitalRead(zone3_ledPin); // check if GREEN LED is ON
    zone4_ledPinValue = digitalRead(zone4_ledPin); // check if GREEN LED is ON
    zone5_ledPinValue = digitalRead(zone5_ledPin); // check if GREEN LED is ON
    */

    // if the ceiling lights were turned off with the ir controller, then then the wall switch was shut off, this will cause the test voltage to rise above the reference voltage
    // this will raise the comparitor output and the led on the circuit board will turn on, ie ledPinValue == High
    // this causes the EM Switch to switch back to NO (Normally Open) position, ie the 12v power supply, not the light circuit
    // but the zoneX_lightCircuitEnabled == true, which is not correct
    // so now that the ir receiver received any signal, the lightCircuitEnabled variable will be set to match the state of the circuit board
    if(zone1_ledPinValue == HIGH && zone1_lightCircuitEnabled == true){ // on any ir received signal, reset the zoneX_lightCircuitEnabled variable if the wall switch was toggled off then on.
      zone1_lightCircuitEnabled = false;
      Serial.println("Reset zone1_lightCircuitEnabled to false");
    }

    /* Future Work */
    /*
    if(zone2_ledPinValue == HIGH && zone2_lightCircuitEnabled == true){ // on any ir received signal, reset the zoneX_lightCircuitEnabled variable if the wall switch was toggled off then on.
      zone2_lightCircuitEnabled = false;
    }

    if(zone3_ledPinValue == HIGH && zone3_lightCircuitEnabled == true){ // on any ir received signal, reset the zoneX_lightCircuitEnabled variable if the wall switch was toggled off then on. 
      zone3_lightCircuitEnabled = false;
    }

    if(zone4_ledPinValue == HIGH && zone4_lightCircuitEnabled == true){ // on any ir received signal, reset the zoneX_lightCircuitEnabled variable if the wall switch was toggled off then on. 
      zone4_lightCircuitEnabled = false;
    }

    if(zone5_ledPinValue == HIGH && zone5_lightCircuitEnabled == true){ // on any ir received signal, reset the zoneX_lightCircuitEnabled variable if the wall switch was toggled off then on. 
      zone5_lightCircuitEnabled = false;
    }
    */

    switch(IrReceiver.decodedIRData.decodedRawData){//determine which IR signal was received 
      case button_1:
        lightCircuitEnabled = zone1_lightCircuitEnabled;
        button = button_1;
         Serial.println("inside the switch: Button (1) pressed");
         Serial.println(String("zone1_lightCircuitEnabled = ") + zone1_lightCircuitEnabled);
      break;

      /* Future Work */
      /*
      case button_2:
        lightCircuitEnabled = zone2_lightCircuitEnabled;
        button = button_2;
        Serial.println("inside the switch: Button (2) pressed");
      break;

      case button_3:
        lightCircuitEnabled = zone3_lightCircuitEnabled;
        button = button_3;
      break;

      case button_4:
        lightCircuitEnabled = zone4_lightCircuitEnabled;
        button = button_4;
      break;

      case button_5:
        lightCircuitEnabled = zone5_lightCircuitEnabled;
        button = button_5;
        //Serial.println("inside the switch: Button (5) pressed");
      break;
      
      // code for below button functions not created yet - one day. ;)
      case button_up:  // IR Controller zone zones 3,4,5 all at once
        
      break;

      case button_down:  // IR Controller zones 1,2 both at once
        
      break;

      case button_left:  // IR Controller zone 5,4,3,2,1 one at a time
        
      break;

      case button_right:  // IR Controller zone 1,2,3,4,5 one at a time
        
      break;

      case button_ok:  // IR Controller zone 1,2,3,4,5 all at once
        
      break;
      */
    }
    
    //Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX); //test code used during development
    zone1_ledPinValue = digitalRead(zone1_ledPin); // check if GREEN LED is ON

    /* Future Work */
    /*
    zone2_ledPinValue = digitalRead(zone2_ledPin); // check if GREEN LED is ON
    zone3_ledPinValue = digitalRead(zone3_ledPin); // check if GREEN LED is ON
    zone4_ledPinValue = digitalRead(zone4_ledPin); // check if GREEN LED is ON
    zone5_ledPinValue = digitalRead(zone5_ledPin); // check if GREEN LED is ON
    */

    if(lightCircuitEnabled == false){//if ceiling lights have been commanded off, turn them back on. enable them  by raising the signal to T2 in circuit board Zone 2 and puts the light circuit into a state waiting for the next signal
      if(button == button_1){digitalWrite(zone1_t2Pin, HIGH);}

      /* Future Work */
      /*
      if(button == button_2){digitalWrite(zone2_t2Pin, HIGH);}
      if(button == button_3){digitalWrite(zone3_t2Pin, HIGH);}
      if(button == button_4){digitalWrite(zone4_t2Pin, HIGH);}
      if(button == button_5){digitalWrite(zone5_t2Pin, HIGH);}
      */

      Serial.println(String("zone1_ledPinValue = ") + zone1_ledPinValue);

      Serial.println("15 second delay start"); //Debounce Timer - wait 15 seconds for the Test Volage to drop below the Reference Voltage at the LM358 OpAmp
      delay(15000);
      Serial.println("15 second delay end");

      if(button == button_1){digitalWrite(zone1_t2Pin, LOW);}//release the enablement signal to T2 now that the circuit is latched and the light is off
      
      /* Future Work */
      /*
      if(button == button_2){digitalWrite(zone2_t2Pin, LOW);}
      if(button == button_3){digitalWrite(zone3_t2Pin, LOW);}
      if(button == button_4){digitalWrite(zone4_t2Pin, LOW);}
      if(button == button_5){digitalWrite(zone5_t2Pin, LOW);}
      */

      if(button == button_1){zone1_ledPinValue = digitalRead(zone1_ledPin);} // check if GREEN LED is ON

      /* Future Work */
      /*
      if(button == button_2){zone1_ledPinValue = digitalRead(zone2_ledPin);} // check if GREEN LED is ON
      if(button == button_3){zone1_ledPinValue = digitalRead(zone3_ledPin);} // check if GREEN LED is ON
      if(button == button_4){zone1_ledPinValue = digitalRead(zone4_ledPin);} // check if GREEN LED is ON
      if(button == button_5){zone1_ledPinValue = digitalRead(zone5_ledPin);} // check if GREEN LED is ON
      */

      //test code used during development
      /* Serial.println(String("after turning lights OFF, zone1_ledPinValue = ") + zone1_ledPinValue);
      Serial.println(String("after turning lights OFF, zone2_ledPinValue = ") + zone2_ledPinValue);
      Serial.println(String("after turning lights OFF, zone3_ledPinValue = ") + zone3_ledPinValue);
      Serial.println(String("after turning lights OFF, zone4_ledPinValue = ") + zone4_ledPinValue);
      Serial.println(String("after turning lights OFF, zone5_ledPinValue = ") + zone5_ledPinValue);*/

      if(button == button_1){zone1_lightCircuitEnabled = true;}

      /* Future Work */
      /*
      if(button == button_2){zone2_lightCircuitEnabled = true;}
      if(button == button_3){zone3_lightCircuitEnabled = true;}
      if(button == button_4){zone4_lightCircuitEnabled = true;}
      if(button == button_5){zone5_lightCircuitEnabled = true;}
      */

      Serial.println(String("zone1_lightCircuitEnabled = ") + zone1_lightCircuitEnabled);
      Serial.println(String("zone1_ledPinValue = ") + zone1_ledPinValue);

      //test code used during development
      //Serial.println("zone2_lightCircuitEnabled = true");

      IrReceiver.resume();
    }else{//if the lights were off, then raise T3 in circuit board Zone 5 for 250ms to turn them on
      //test code used during development
      //Serial.println(String("before turning lights ON, zone2_ledPinValue = ") + zone2_ledPinValue);
      if(button == button_1){digitalWrite(zone1_t3Pin, HIGH);}

      /* Future Work */
      /*
      if(button == button_2){digitalWrite(zone2_t3Pin, HIGH);}
      if(button == button_3){digitalWrite(zone3_t3Pin, HIGH);}
      if(button == button_4){digitalWrite(zone4_t3Pin, HIGH);}
      if(button == button_5){digitalWrite(zone5_t3Pin, HIGH);}
      */

      Serial.println(".25 second delay");
      delay(250);

      if(button == button_1){digitalWrite(zone1_t3Pin, LOW);}

      /* Future Work */
      /*
      if(button == button_2){digitalWrite(zone2_t3Pin, LOW);}
      if(button == button_3){digitalWrite(zone3_t3Pin, LOW);}
      if(button == button_4){digitalWrite(zone4_t3Pin, LOW);}
      if(button == button_5){digitalWrite(zone5_t3Pin, LOW);}
      */

      if(button == button_1){zone1_ledPinValue = digitalRead(zone1_ledPin);} // check if GREEN LED is ON

      /* Future Work */
      /*
      if(button == button_2){zone1_ledPinValue = digitalRead(zone2_ledPin);} // check if GREEN LED is ON
      if(button == button_3){zone1_ledPinValue = digitalRead(zone3_ledPin);} // check if GREEN LED is ON
      if(button == button_4){zone1_ledPinValue = digitalRead(zone4_ledPin);} // check if GREEN LED is ON
      if(button == button_5){zone1_ledPinValue = digitalRead(zone5_ledPin);} // check if GREEN LED is ON
      */

      Serial.println(String("zone1_ledPinValue = ") + zone1_ledPinValue);

      //test code used during development
      //Serial.println(String("after turning lights OFF, zone2_ledPinValue = ") + zone2_ledPinValue);

      if(button == button_1){zone1_lightCircuitEnabled = false;}

      /* Future Work */
      /*
      if(button == button_2){zone2_lightCircuitEnabled = false;}
      if(button == button_3){zone3_lightCircuitEnabled = false;}
      if(button == button_4){zone4_lightCircuitEnabled = false;}
      if(button == button_5){zone5_lightCircuitEnabled = false;}
      */
      
      Serial.println(String("zone1_lightCircuitEnabled = ") + zone1_lightCircuitEnabled);

      //test code used during development
      //Serial.println("zone2_lightCircuitEnabled = true");

      IrReceiver.resume();
    }
  }
}
