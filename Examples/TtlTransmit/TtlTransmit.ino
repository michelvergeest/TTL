/* theater technisch lab
   remote midi transmitter
   baudrate 600 8N1
*/
#include <TTL.h>
 
// Pin 13 has an LED connected on most Arduino boards.
int led = 13;

#define TIMEOUT_POLL 1000
unsigned long timeOutPoll;

// serial port 0 
TTL ttlMidi(0);
uint8_t counter = 0;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);  

  // start ttl midi serial port
  ttlMidi.begin();
    
}

// the loop routine runs over and over again forever:
void loop() {
   
  // check for time out 
  if ((long)(millis() - timeOutPoll) >= 0)  {
    timeOutPoll = millis() + TIMEOUT_POLL; 
      
    // send programchange data   
    ttlMidi.sendMemory(counter);
    
    // send controlchange data 
   if (counter < 10) {
      ttlMidi.sendUp();
    }
    
    if (counter >= 10) {
      ttlMidi.sendDown();
    }
    
    if (counter == 20) {
      ttlMidi.sendGo();
      counter = 0;
    }
    
    // increase testcounter
    counter++;
    if (counter > 20)  counter = 0;
  }
  
}



