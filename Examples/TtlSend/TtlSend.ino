/* ttl Send */


#include <TTL.h>

// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 13;
int led2 = 12;
int key1 = 53;

//serial port
TTL ttlMidi(2);
uint8_t counter = 0;

#define TIMEOUT_POLL 1000
unsigned long timeOutPoll;

// the setup routine runs once when you press reset:
void setup()
{
    // initialize the digital pin as an output.
    pinMode(led, OUTPUT);
    pinMode(led2, OUTPUT);

    pinMode(key1, INPUT_PULLUP);

    //start ttl midi
    ttlMidi.begin();

    ttlMidi.attachControlChange(handleControlChange);

}

// the loop routine runs over and over again forever:
void loop()
{

    // check for time out
    if ((long)(millis() - timeOutPoll) >= 0)
    {
        timeOutPoll = millis() + TIMEOUT_POLL;


        ttlMidi.sendDown();


    }

    if (!digitalRead(key1) )
    {
        ttlMidi.sendMemory(counter++);
    }



    //ttlMidi.sendMemory(counter++);
    //ttlMidi.sendUp();
    ttlMidi.read();
    if (ttlMidi.messageReceived())
    {

        //if (ttlMidi.getData1()== 0x62 ) {
        digitalWrite(led2, HIGH);
        delay(1);
        digitalWrite(led2, LOW);

    }
}


void handleControlChange()
{
    digitalWrite(led2, HIGH);
    delay(1);
    digitalWrite(led2, LOW);
}
