/* ttl Send */
#include <TTL.h>

// Pin 13 has an LED connected on most Arduino boards.
int led = 13;
int led2 = 12;

#define TIMEOUT_POLL 1000
unsigned long timeOutPoll;

// serial port
TTL ttlMidi(2);
uint8_t counter = 0;

// the setup routine runs once when you press reset:
void setup()
{
    // initialize the digital pin as an output.
    pinMode(led, OUTPUT);
    pinMode(led2, OUTPUT);

    // attach callback functions
    ttlMidi.attachControlChange(handleControlChange);
    ttlMidi.attachProgramChange(handleProgramChange);

    // start ttl midi serial port
    ttlMidi.begin();

}

// the loop routine runs over and over again forever:
void loop()
{

    ttlMidi.read();

    // check for time out
    if ((long)(millis() - timeOutPoll) >= 0)
    {
        timeOutPoll = millis() + TIMEOUT_POLL;

        // send programchange data
        ttlMidi.sendMemory(counter++);

        // send controlchange data every 4th time
        if (counter % 4 == 0)
        {
            ttlMidi.sendDown();
        }
    }

}

void handleControlChange()
{
    digitalWrite(led, HIGH);
    delay(1);
    digitalWrite(led, LOW);
}

void handleProgramChange()
{
    digitalWrite(led2, HIGH);
    delay(1);
    digitalWrite(led2, LOW);
}
