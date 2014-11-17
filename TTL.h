#ifndef _TTL_H_
#define _TTL_H_

#include <Arduino.h>


#define TTL_BAUDRATE        600

#define MIDI_PROGRAM_CHANGE 0xC0
#define MIDI_CONTROL_CHANGE 0xB0

#define TTL_CC_GO           0x60
#define TTL_CC_UP           0x61
#define TTL_CC_DOWN         0x62

enum { WAIT_FOR_STATUS_BYTE,  WAIT_FOR_DATA1, WAIT_FOR_DATA2 };


//TTL CONTROLCHANGE
//1e byte xxxxN kanaalnumber
//2e byte; go 0x60, masterup 0x62, mastedown 0x62
//3e byte nothing

//TTL PROGRAMCHANGE
//1e byte xxxxN kanaalnumber
//2e byte; databyte 0..99

class TTL
{
    typedef void (*func_t)();

private:
    uint8_t _channelTx;

    uint8_t _channelRx;
    uint8_t _channelMessage;
    uint8_t _data1;
    uint8_t _data2;

    bool    _messageReceived;
    uint8_t _state;

    func_t  _cbControlChange; // call back pointer
    func_t  _cbProgramChange; // call back pointer

    void sendControlChange(uint8_t command);

public:
    TTL(uint8_t serial);
    void begin();

    // transmit message
    void setChannel(uint8_t channel);
    void sendGo();
    void sendUp();
    void sendDown();
    void sendMemory(uint8_t memory);

    // receive message
    void read();
    bool messageReceived();
    uint8_t getData1();
    uint8_t getData2();
    uint8_t getChannel();

    void attachControlChange(func_t);
    void attachProgramChange(func_t);
};

#endif // _TTL_H_
