/* -------------------------------------------------------------------------
//  File Name   :   packet.cpp
//  Author      :   Ma Jianfeng
//  Create Time :   2015-2-8 18:40:24
//  Description :  
//
// -----------------------------------------------------------------------*/

#include "packet.h"

#include <string.h>

//-------------------------------------------------------------------------

Packet::Packet(uint8_t size)
    : m_filler(0xFF)
    , m_delimiter(0x00)
	, m_recv(new char[INSTRUCTION_SIZE + 1])
	, m_size(size)
	, m_send(new char[size])
    , m_cursor(NULL)
	, m_offset((size - INSTRUCTION_SIZE) / 2)
{
    // <several bytes of Instruction>-<1-byte delimiter>
    ::memset(m_recv, m_delimiter, INSTRUCTION_SIZE + 1);

    // <several bytes filler>-<1-byte delimiter>-<several bytes of Instruction>-<1-byte delimiter>
    ::memset(m_send, m_filler, size);
    *(m_send + m_offset - 1) = m_delimiter;
    *(m_send + m_offset + INSTRUCTION_SIZE) = m_delimiter;
}

Packet::~Packet()
{
	delete[] m_recv;
	delete[] m_send;
}

const char* Packet::encode(Instruction* inst)
{
    char* data = m_send + m_offset;
    ::memcpy(data, inst->m_token.data(), inst->m_token.size());
    data += inst->m_token.size();

    *data++ = inst->m_type;
    *data++ = inst->m_value;
    return m_send;
}

Instruction* Packet::decode(char byte)
{
    Instruction* inst = NULL;
    if(m_delimiter == byte)
    {
        if(NULL == m_cursor)
            m_cursor = m_recv;
        else 
        {
            // The end, or discard current frame when occur error
            if(INSTRUCTION_SIZE == static_cast<uint8_t>(m_cursor - m_recv))
            {
                inst = reinterpret_cast<Instruction*>(m_recv);
            }
            m_cursor = NULL;
        }
    }
    else if(NULL != m_cursor)
    {
        if(static_cast<uint8_t>(m_cursor - m_recv) >= INSTRUCTION_SIZE)
            m_cursor = NULL;
        else
            *m_cursor++ = byte;
    }

    return inst;
}

//--------------------------------------------------------------------------
