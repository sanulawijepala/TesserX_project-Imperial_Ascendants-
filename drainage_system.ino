#include <Servo.h>

Servo Valve;//creates the servo object for the valve

int WL_start ;
int WL_current ;
int Time; 
int checkdelay = 1000 ;
bool flood = 0 ; //this value is whether the system is in a flood state or not

int SPKR = 3 ;//this is a PWM pin and is used for the speaker
int DrnVlv = 2 ;//this is the output pin used to control the supply of electricity to the drainege valve 
int DrnVlv_pos = 0 ; // used to control the servo in the drainage valve
int pitch = 1000 ; //the pitch of the alarm

int id = 0 ; //id of the device in the network


void setup() {

  pinMode(SPKR , OUTPUT); 
  pinMode(DrnVlv , OUTPUT);
  Valve.attach(9);
    
  pinMode(A0, INPUT); // Set analog pin A0 as the water level input
  Serial.begin(9600); // Initialize serial communication
  
  // First read (discarded for accuracy)
  analogRead(A0);
  
  // Second read (more accurate)
  WL_start = analogRead(A0);
  

}

void loop() {

  if (Time >= 65000) {

    Time = 0 ;// prevents the int from overflowing at 65535 and changing the program time from the true time 
    
    }
  WL_current = analogRead(A0);

  if (Time % checkdelay == 0) {
    
    if (WL_current > WL_start) {
      
      digitalWrite(DrnVlv , HIGH); //turns on the drainage valve
      
      DrnVlv_pos = 90;
      Valve.write(DrnVlv_pos);
      
      tone(SPKR , pitch ); //turns on the alarm
      
      flood = 1 ; // a flood is underway
      
      }
     if (flood == 1) {

      Serial.print("Flood is happening");//sends "a flood is happening" signal the master controller
      
      }
     else {
      Serial.println((String)"No."+ id + " is alive.");//periodicly checks connectivity with the master controller and keepalive 

      if (DrnVlv_pos == 90){// checks whether the valve is engaged and if it is then disengages it
        
        DrnVlv_pos = 0;
        Valve.write(DrnVlv_pos);
        
        }
      }
    }
  

  Time =+ 1 ;//increments the time each cycle
  
  delay(1); // I didn't know the clock speed so i just made a delay of 1 ms making the program repeat at a rate of 1000Hz
}
