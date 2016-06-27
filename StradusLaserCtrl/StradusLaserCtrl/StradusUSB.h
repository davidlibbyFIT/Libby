#pragma once

#include "CppService.h"
#include <windows.h>
#include <string>

typedef enum StratusStatus
{
	STRADUS_LASER_EMISSION_ACTIVE				= 0,
	STRADUS_STANDBY								= 1 << 0,
	STRADUS_WARMUP								= 1 << 1,
	STRADUS_VALUE_OUT_OF_RANGE					= 1 << 2,
	STRADUS_INVALID_COMMAND						= 1 << 3,
	STRADUS_INTERLOCK_OPEN						= 1 << 4,
	STRADUS_TEC_OFF								= 1 << 5,
	STRADUS_DIODE_OVER_CURRENT					= 1 << 6,
	STRADUS_DIODE_TEMPERATURE_FAULT				= 1 << 7,
	STRADUS_BASE_PLATE_TEMPERATURE_FAULT		= 1 << 8,
	STRADUS_BUFFER_OVERFLOW						= 1 << 9,
	STRADUS_EEPROM_ERROR						= 1 << 10,
	STRADUS_I2C_ERROR							= 1 << 11,
	STRADUS_COMMAND_TIME_OUT					= 1 << 12,
	STRADUS_WATCH_DOG_ERROR						= 1 << 13,
	STRADUS_FATAL_ERROR							= 1 << 14,
	STRADUS_DIODE_END_OF_LIFE_WARNING			= 1 << 15,
	
	//Below are non Stratus Errors Signals
	STRADUS_OK									= 1 << 20,
	STRADUS_FAIL								= 1 << 21,
	STRADUS_LASER_COUNT_INCORRECT				= 1 << 22,
	STRADUS_NOT_CONNECTED						= 1 << 23,
	STRADUS_BAD_RETURN_COMMAND					= 1 << 24,
	STRADUS_COMMAND_TIMEOUT						= 1 << 25

}StratusStatus;



class StradusUSB
{
public:
	StradusUSB(void);
	~StradusUSB(void);
	bool isConnected();

	int getLaserCount(){return m_LasertCount;};
	
	StratusStatus getBaseTempC(int &tempC);
	StratusStatus  getBaseTempF(int &tempF);
	StratusStatus  getInterlockStatus(bool &state);
	StratusStatus getLaserPower(float &powermW);
	StratusStatus getEmissionStatus(bool &emission);
	StratusStatus getLaserWavelength(std::string &waveLength);
	StratusStatus getMaxLaserPower(int &maxPowermW);
	StratusStatus getFaultCode(int &faultCode);
	std::string getLastFaultString();
	StratusStatus setLaserEmission(bool On);
	StratusStatus setLaserPower(int powermW);
	
private:
	StratusStatus getLaserRespone(std::string &returnCommand, DWORD msTimeout = 2000);
	StratusStatus sendCommand(std::string strCommand,std::string &returnCommand, DWORD msTimeout =2000);
	void parseReturn(const std::string &inStr,std::string &Command, std::string &Value);

	//! Basic Laser Member
	MixedCode::StradusService m_USBLaser;
	//! COunt of Laser(s) on the system
	int m_LasertCount;	
	//! Last fault code seen.
	int m_LastFaultCode;
	//! Internal Error for use in getting last error message.
	std::string m_InternalError;
};

