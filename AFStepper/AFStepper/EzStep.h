#pragma once
#include "Serial.h"
#include <memory>
#include <string>



typedef enum 
{ 
	EZ_OK,
	EZ_BAD_COM_PORT,
	EZ_BAD_COMMAND,
	EZ_PORT_NOT_OPEN

} EZStepRetType;

class EzStep
{
public:
	EzStep(void);
	~EzStep(void);
	EZStepRetType EzStep::OpenPort(const std::string Com);
	EZStepRetType SendCommand(const std::string command);
	EZStepRetType CheckComPortForEZStepperAllMotion(const std::string &comport);
	std::string ReadPort();
	unsigned int ReadPosition();
	void ReadPosition(std::string &strPos);
	void ReadSwitchState(std::string &strPos);
	int ReadSwitchState();

	std::string GetComPort();
private:
	SerialParams m_serialParams;
	Serial m_pSerial;
	std::string m_LastRead;
	bool m_PortOpen;
	bool m_CriticalSection;
};

