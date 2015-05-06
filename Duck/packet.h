/* -------------------------------------------------------------------------
//  File Name   :   packet.h
//  Author      :   Ma Jianfeng
//  Create Time :   2015-2-8 18:39:44
//  Description :
//
// -----------------------------------------------------------------------*/

#ifndef __PACKET_H__
#define __PACKET_H__

//-------------------------------------------------------------------------

#include <stdint.h>
#include <string.h>

//-------------------------------------------------------------------------

class Token
{
public:
	inline Token(char byte) { ::memset(m_data, byte, sizeof(m_data)); }
	inline Token(char data[4]) { operator =(data); }
	inline Token(const char* str) { operator =(str); }
	inline ~Token() {}
	inline Token(const Token& token) { operator =(token); }
	inline Token& operator =(const Token& token)
	{
		::memcpy(m_data, token.m_data, sizeof(m_data));
		return *this;
	}
	inline Token& operator =(char data[4])
	{
		::memcpy(m_data, data, sizeof(m_data));
		return *this;
	}
	inline Token& operator =(const char* str)
	{
		::memcpy(m_data, str, sizeof(m_data));
		return *this;
	}
	inline bool operator ==(const Token& token) const { return ::memcmp(m_data, token.m_data, sizeof(m_data)); }
	inline bool operator !=(const Token& token) const { return !operator ==(token); }
	inline const char* data() const { return m_data; }
	inline int8_t size() const { return sizeof(m_data); }
private:
    char m_data[4];					    	// Must be digital or charactor
};

// Composition structure of instruction
// Be careful to change it's size
class Instruction
{
public:
	inline Instruction() : m_token((char)0x00), m_type((char)0x00), m_value((char)0x00) {}
public:
    enum Type
    {
        SwitchMode			= 'm',			// Switch mode
        Stop			    = 's',			// Duck stop
        Advance			    = 'a',			// Duck advance
        Back			    = 'b',			// Duck Back
        TurnRight		    = 'r',			// Duck turn right
        TurnLeft		    = 'l',			// Duck turn left

        Response			= 'p',			// Response
    };
    enum Value
    {
        AutoMode		    = '0',			// Set auto mode
        ControlMode		    = '1',			// Set control mode

        ZeroSpeed		    = '3', 			// Zeor speed
        LowSpeed            = '4', 			// Low speed
        HigeSpeed		    = '5', 			// High speed

		InvalidArgResponse	= 'v',			// Invalided argument
        FailResponse 		= 'x',			// Response fail
        SwitchModeResponse 	= 'm',			// Switch mode success
        StopResponse 	    = 's',			// Stop success
        AdvanceResponse 	= 'a',			// Advance success
        BackResponse 	    = 'b',			// Back success
        TurnRightResponse 	= 'r',			// Turn right success
        TurnLeftResponse    = 'l',			// Turn left success
    };

    Token 	m_token;						// Token
    char 	m_type;							// Type
    char 	m_value;						// Speed level , mode value or response etc.
};

#define INSTRUCTION_SIZE 	sizeof(Instruction)

//--------------------------------------------------------------------------

class Packet
{
public:
    Packet(uint8_t size);					// The size must be larger than sizeof(Instruction) + 2
    ~Packet();
public:
    inline uint8_t size() const { return m_size; }
    const char* encode(Instruction* inst);
    Instruction* decode(char byte);
private:
    const char 		m_filler;               // The charactor for fill
    const char 		m_delimiter;            // The delimiter of a instruction
    char* 			m_recv;      			// Data buffer of recieve, see HC-08 bluetooth document
    const uint8_t 	m_size;					// Packet's size
    char* 			m_send;                 // Data buffer of send packet, see HC-08 bluetooth document
    char* 			m_cursor;               // Cursor for recieve buffer
    const uint8_t 	m_offset;				// The offset of instruction
};

//--------------------------------------------------------------------------
#endif /* __PACKET_H__ */
