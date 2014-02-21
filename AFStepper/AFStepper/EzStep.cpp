#include "StdAfx.h"
#include "EzStep.h"
#include <sstream>

#define ALL_MOTION_TERMINATOR 13
#define MAXPAYLOAD 128



EzStep::EzStep(void)
{
	m_serialParams._baud = 9600;
	m_serialParams._data_bits = 8;
	m_serialParams._parity = 'N';
	strncpy_s(m_serialParams._port, sizeof(m_serialParams._port), "COM1", _TRUNCATE);
	m_serialParams._stop_bits = 1;
	m_PortOpen=false;
	m_CriticalSection = false;
	//m_pSerial.reset(new(Serial));

}


EzStep::~EzStep(void)
{
	SendCommand("/1TR\r");
}

EZStepRetType EzStep::OpenPort(const std::string Com)
{

	return CheckComPortForEZStepperAllMotion(Com);
}

EZStepRetType EzStep::SendCommand(const std::string command)
{
	if(!m_PortOpen)
		return EZ_PORT_NOT_OPEN;

	while(m_CriticalSection)
	{
	}
	m_CriticalSection=true;
	m_pSerial.write_sync((const unsigned char*)command.c_str(),command.length());

	// The reason fort the magic numbers is some leading Header data "ÿ/0`EZStepper AllMotion"
	unsigned char rxBuff[24];
	memset(rxBuff,0,sizeof(rxBuff));
	DWORD nBytes;
	const int Cmd_len = 128;//for receiving cmd responses



	char buff[MAXPAYLOAD] = {0};
	if (true == m_pSerial.read_sync(reinterpret_cast<unsigned char *>(buff), Cmd_len, &nBytes, 13))
	{
		char *pStr;
		if ((pStr = strstr(buff,"/0")) != NULL) {
			m_LastRead = buff;
			//m_LastRead.replace(m_LastRead.length()-2,1,"h");
			pStr += 2;
			if ( *pStr & 0x64 )
			{//bit set indicating ready to accept commands and ignore init error
				m_CriticalSection=false;
				return EZ_OK;
			}
		}
	}

	m_CriticalSection=false;
	return EZ_BAD_COMMAND;
}

std::string EzStep::ReadPort()
{
	
	if(!m_PortOpen)
		return "";

	while(m_CriticalSection)
	{
	}
	m_CriticalSection=true;

	const int Cmd_len = 128;//for receiving cmd responses
	DWORD nBytes;
	char buff[MAXPAYLOAD] = {0};
	if (true == m_pSerial.read_sync(reinterpret_cast<unsigned char *>(buff), Cmd_len, &nBytes, 13))
	{
		char *pStr;
		if ((pStr = strstr(buff,"/0")) != NULL) {
			pStr += 2;
			if ( *pStr & 0x64 )
			{
				//bit set indicating ready to accept commands and ignore init error
				m_CriticalSection=false;
				return buff;
			}
		}
	}
	m_CriticalSection=false;

	return "";
}
void EzStep::ReadPosition(std::string &strPos)
{
	if(!m_PortOpen)
		return ;

	SendCommand("/1?0\r");	
	strPos=m_LastRead;
	strPos.erase(0,4);	
}

unsigned int EzStep::ReadPosition()
{
	if(!m_PortOpen)
		return EZ_PORT_NOT_OPEN;

	unsigned int retpos=0;
	std::string StringRet; 		
	ReadPosition(StringRet);
	std::stringstream myStream(StringRet);
	myStream >> retpos;
	return retpos;
}

void EzStep::ReadSwitchState(std::string &strPos)
{
	if(!m_PortOpen)
		return ;

	SendCommand("/1?4\r");	
	strPos=m_LastRead;
	strPos.erase(0,4);	
}

int EzStep::ReadSwitchState()
{
	if(!m_PortOpen)
		return EZ_PORT_NOT_OPEN;

	unsigned int retpos=0;
	std::string StringRet; 		
	ReadSwitchState(StringRet);
	std::stringstream myStream(StringRet);
	myStream >> retpos;
	return retpos;
}

/**
* FUNCTION CheckComPortForEZStepperAllMotion*
* @brief Checks the port given to see if we have a "EZStepper AllMotion" 
*
* @version 1.0 
*
* @author David Libby
* @date 12/16/2013 10:51:51 PM
*
* @param comport 
*
* @return bool if we found the stepper return true.
*/
EZStepRetType EzStep::CheckComPortForEZStepperAllMotion(const std::string &comport)
{
	char buff[256];
	char *pStr;
	DWORD nBytes;
	EZStepRetType ret=EZ_OK;
	while(m_CriticalSection)
	{
	}
	m_CriticalSection=true;

	strcpy_s(m_serialParams._port, sizeof(m_serialParams._port), comport.c_str());

	m_pSerial.setParams(&m_serialParams);

	if (m_pSerial.openComm()) {
		//sprintf_s(buff,"/1&\r");

		std::string Command = "/1&\r";
		//std::string Command = "\1P0R\n";
		bool ret = m_pSerial.write_sync((const unsigned char*)Command.c_str(),Command.length());

		if (m_pSerial.write_sync((const unsigned char*)buff,4)) {

			char *LookForString="EZStepper AllMotion";		

			// The reason fort the magic numbers is some leading Header data "ÿ/0`EZStepper AllMotion"
			unsigned char rxBuff[24];
			memset(rxBuff,0,sizeof(rxBuff));

			if ( m_pSerial.read_sync(rxBuff, sizeof(rxBuff), &nBytes, ALL_MOTION_TERMINATOR ))
			{					
				rxBuff[sizeof(rxBuff)-1]='\0';
				if ((pStr = strstr(reinterpret_cast<char*>(rxBuff),"/0")) != NULL) {
					pStr += 3;
					if ( 0 != (*pStr & 0x60) )//No errors, except init which is ok
					{
						if( 0 == strncmp("EZStepper AllMotion",(const char *)pStr, 19))
						{
							m_pSerial.read_sync(rxBuff, sizeof(rxBuff), &nBytes, ALL_MOTION_TERMINATOR );
							m_PortOpen=true;
							m_CriticalSection=false;
							return EZ_OK;
						}

					}
				}
			}

		}
	}

	m_pSerial.closeComm();
	m_CriticalSection=false;
	return EZ_BAD_COM_PORT;
}
std::string EzStep::GetComPort()
{
	return m_serialParams._port;
}