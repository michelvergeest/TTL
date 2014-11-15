#include "TTL.h"
#include "HardwareSerial.h"



/* _____GLOBAL VARIABLES_____________________________________________________ */
HardwareSerial TtlSerial = Serial; ///< Pointer to Serial class object


TTL::TTL(uint8_t serial = 0)
{

  switch(serial)
  {
#if defined(UBRR1H)
    case 1:
      TtlSerial = Serial1;
      break;
#endif
      
#if defined(UBRR2H)
    case 2:
      TtlSerial = Serial2;
      break;
#endif
      
#if defined(UBRR3H)
    case 3:
      TtlSerial = Serial3;
      break;
#endif
      
    default:
      TtlSerial = Serial;
  }

	setChannel(1);

	_messageReceived = false;
	_cbControlChange = NULL;
  _cbProgramChange = NULL;
  _state = WAIT_FOR_STATUS_BYTE;  
}

void TTL::begin()
{
	TtlSerial.begin(TTL_BAUDRATE);
}

void TTL::setChannel(uint8_t channel)
{
	_channelTx = constrain(channel, 1, 16) - 1;
}

// C0 00
void TTL::sendMemory(uint8_t memory) {
   TtlSerial.write(MIDI_PROGRAM_CHANGE | _channelTx);
   TtlSerial.write(memory & 0x7F);
}

// B0 61 00
void TTL::sendGo()
{
   sendControlChange(TTL_CC_GO);
}

// B0 62 00
void TTL::sendUp()
{
   sendControlChange(TTL_CC_UP);
}

// B0 63 00
void TTL::sendDown()
{
   sendControlChange(TTL_CC_DOWN);
}

void TTL::sendControlChange(uint8_t command)
{
  TtlSerial.write(MIDI_CONTROL_CHANGE | _channelTx);
  TtlSerial.write(command & 0x7F);
  TtlSerial.write(0);
}  


// 2014
void TTL::read()
{
  if (TtlSerial.available() == 0) return;  // No data available

  uint8_t rxdata = TtlSerial.read();

  switch (_state) {
  	case WAIT_FOR_STATUS_BYTE:
  		_messageReceived = false;
  		// check if status byte is received: bit7 = high
  		if (rxdata & (1<<7)) {
  			 // Channel message, remove channel nibble.
  			_channelMessage = rxdata & 0xF0;

  			// channel in the range 1-16
  			_channelRx = (rxdata & 0x0F)  + 1;

  			// next state
  			_state = WAIT_FOR_DATA1;
  		}
  		break;

  	case WAIT_FOR_DATA1:
  		switch (_channelMessage) {
  			case MIDI_PROGRAM_CHANGE: // memory 0..63	
          _data1 = rxdata;
  				_state = WAIT_FOR_STATUS_BYTE;
  				_messageReceived = true;
  				if (_cbProgramChange) _cbProgramChange();
  				break;
			  case MIDI_CONTROL_CHANGE:  // up, down, go
				  _data1 = rxdata;
          _state = WAIT_FOR_DATA2;
   			 break;
  			default:
  				_state = WAIT_FOR_STATUS_BYTE;
  		}
  		break;

  	case WAIT_FOR_DATA2:
  	   	_data2 = rxdata;
  		if (_channelMessage == MIDI_CONTROL_CHANGE) {
   			_messageReceived = true;
  			if (_cbControlChange) _cbControlChange();
  			_state = WAIT_FOR_STATUS_BYTE;	
  		}
  		break;

    // should not happen, just in case
    default:
      _state = WAIT_FOR_STATUS_BYTE;
      _messageReceived = false;
  }
} 


bool TTL::messageReceived()
{
	if ( _messageReceived) {
    _messageReceived = false;
    return true;
  }
  return false;
}


uint8_t TTL::getData1()
{
	return _data1;
}


uint8_t TTL::getData2()
{
	return _data2;
}


uint8_t TTL::getChannel()
{
	return _channelRx;
}


void TTL::attachControlChange(func_t cb)
{
  _cbControlChange = cb;
}


void TTL::attachProgramChange(func_t cb)
{
  _cbProgramChange = cb;
}

