#include "StradusUSB.h"
#include <iostream>
#include <sstream>
#include <process.h> //THreads
#include "CppService.h"
#pragma comment(lib, "Debug/MixedCode.lib")



using namespace std;
using namespace MixedCode;

static int STRADUS_COMMAND_TIMEOUT_MS = 2000;

/*! \class StratusUSB
\brief This class is for talking to the stratus USB DLL 

Simple class for talking to the Stratus Laser It is designed to work with only one laser, if you have more installed the class will fail to connect.
*/
StradusUSB::StradusUSB(void):
m_LasertCount(0),
	m_LastFaultCode(0)	
{
	m_USBLaser.InitStradusHub();
	m_LasertCount = m_USBLaser.StradusCount();
}

StradusUSB::~StradusUSB(void)
{
}


/**
* FUNCTION isConnected
*
* @brief Checks to see that we have 1 and only one laser connected to this device.
*
* @author DAVID.LIBBY
* @date 1/26/2016 9:54:33 AM
*
*
* @return bool Returns true if only one laser is connected.
*/
bool StradusUSB::isConnected()
{
	if(m_LasertCount!=1)
	{
		m_LastFaultCode=STRADUS_LASER_COUNT_INCORRECT;
	}
	return m_LasertCount==1;
};//this should only work if we have one laser.


/**
* FUNCTION getBaseTempC
*
* @brief Gets the base temperature in celsius
*
* @author DAVID.LIBBY
* @date 1/26/2016 9:55:17 AM
*
* @param tempC Cureent baseplate temp
*
* @return StratusStatus 
*/
StratusStatus StradusUSB::getBaseTempC(int &tempC)
{

	if(!isConnected())
		return STRADUS_NOT_CONNECTED;

	string ret;
	StratusStatus err= sendCommand("?BPT\r",ret,STRADUS_COMMAND_TIMEOUT_MS);
	if(err!=STRADUS_OK)
	{
		m_LastFaultCode=err;
		return err;
	}

	m_InternalError.clear();
	if(ret.empty())
	{
		m_InternalError = __FUNCTION__;
		return STRADUS_BAD_RETURN_COMMAND;
	}

	string Command,Value;
	parseReturn(ret,Command,Value);
	if (Command=="?BPT")
	{
		tempC = stoi(Value.c_str());
		return STRADUS_OK;
	}
	return STRADUS_FAIL;
}
/**
* FUNCTION getLaserPower
*
* @brief Gets the current laser power in milliwatts
*
* @author DAVID.LIBBY
* @date 1/26/2016 9:58:04 AM
*
* @param powermW 
*
* @return StratusStatus 
*/
StratusStatus StradusUSB::getLaserPower(int &powermW)
{
	string ret;
	StratusStatus err= sendCommand("?LP\r",ret,STRADUS_COMMAND_TIMEOUT_MS);
	if(err!=STRADUS_OK)
	{
		m_LastFaultCode=err;
		return err;
	}

	m_InternalError.clear();
	if(ret.empty())
	{
		m_InternalError = __FUNCTION__;
		return STRADUS_BAD_RETURN_COMMAND;
	}

	string Command,Value;
	parseReturn(ret,Command,Value);
	if (Command=="?LP")
	{
		powermW = stoi(Value.c_str());
		return STRADUS_OK;
	}
	return STRADUS_FAIL;
}

/**
* FUNCTION setLaserEmission
*
* @brief Bool to turn the laser on or off.
*
* @author DAVID.LIBBY
* @date 1/26/2016 9:58:49 AM
*
* @param On True = on false = off
*
* @return StratusStatus 
*/
StratusStatus StradusUSB::setLaserEmission(bool On)
{
	m_InternalError.clear();
	bool currentStatus;

	if(getEmissionStatus(currentStatus)!=STRADUS_OK)
	{
		m_InternalError=__FUNCTION__;
		m_InternalError+=" Failed to change Emission State";
		return STRADUS_FAIL;
	}

	//If we already are in that state then we are ok.
	if(On==currentStatus)
		return STRADUS_OK;

	int emm=1;

	if(!On)
		emm=0;

	std::stringstream cmd;
	cmd<<"LE="<<emm<<"\r";


	string ret;
	StratusStatus err= sendCommand(cmd.str().c_str(),ret,STRADUS_COMMAND_TIMEOUT_MS);
	if(err!=STRADUS_OK)
	{
		m_LastFaultCode=err;
		return err;
	}


	m_InternalError.clear();
	if(ret.empty())
	{
		m_InternalError = __FUNCTION__;
		return STRADUS_BAD_RETURN_COMMAND;
	}

	string Command,Value;
	parseReturn(ret,Command,Value);
	bool status=false;
	if (Command=="LE")
	{
		int istatus = stoi(Value.c_str());		
		if(istatus==emm)
			STRADUS_OK;
		else
			STRADUS_FAIL;
	}

	return STRADUS_OK;


}
/**
* FUNCTION setLaserPower
*
* @brief Sets laser power in milliwatts, will check max power ofr laser if input exceeds it will be limited to MAX
*
* @author DAVID.LIBBY
* @date 1/26/2016 9:59:20 AM
*
* @param powermW New Power level to set in milliwatts limited to MAX power for laser.
*
* @return StratusStatus 
*/
StratusStatus StradusUSB::setLaserPower(int powermW)
{
	int maxPower;

	if(getMaxLaserPower(maxPower)!=STRADUS_OK)
		return STRADUS_FAIL;

	int currentPower;

	if(getLaserPower(currentPower)!=STRADUS_OK)
	{
		m_InternalError=__FUNCTION__;
		m_InternalError+=" Failed to change Power State";
		return STRADUS_FAIL;
	}

	if(powermW==(int)currentPower)
		return STRADUS_OK;

	if(powermW>maxPower)
		powermW = maxPower;

	std::stringstream cmd;
	cmd<<"LP="<<powermW<<"\r";


	string ret;
	StratusStatus err= sendCommand(cmd.str().c_str(),ret,STRADUS_COMMAND_TIMEOUT_MS);
	if(err!=STRADUS_OK)
	{
		m_LastFaultCode=err;
		return err;
	}
	Sleep(1000);

	m_InternalError.clear();
	if(ret.empty())
	{
		m_InternalError = __FUNCTION__;
		return STRADUS_BAD_RETURN_COMMAND;
	}

	string Command,Value;
	parseReturn(ret,Command,Value);
	bool status=false;
	if (Command=="LP")
	{
		int istatus = stoi(Value.c_str());		
		if(istatus==powermW)
			STRADUS_OK;
		else
			STRADUS_FAIL;
	}

	return STRADUS_OK;


}
/**
* FUNCTION getInterlockStatus
*
* @brief Gets the interlock status if true laser is "Locked"
*
* @author DAVID.LIBBY
* @date 1/26/2016 10:00:25 AM
*
* @param state True means "Locked"
*
* @return StratusStatus 
*/
StratusStatus StradusUSB::getInterlockStatus(bool &state)
{
	string ret;
	StratusStatus err= sendCommand("?IL\r",ret,STRADUS_COMMAND_TIMEOUT_MS);
	if(err!=STRADUS_OK)
	{
		m_LastFaultCode=err;
		return err;
	}


	m_InternalError.clear();
	if(ret.empty())
	{
		m_InternalError = __FUNCTION__;
		return STRADUS_BAD_RETURN_COMMAND;
	}

	string Command,Value;
	parseReturn(ret,Command,Value);
	bool status=false;
	if (Command=="?IL")
	{
		int istatus = stoi(Value.c_str());		
		if(istatus==1)
			state=true;
		else
			state=false;
	}

	return STRADUS_OK;
}

/**
* FUNCTION getMaxLaserPower
*
* @brief Returns the laser max power in milliwatts
*
* @author DAVID.LIBBY
* @date 1/26/2016 10:02:08 AM
*
* @param maxPowermW 
*
* @return StratusStatus 
*/
StratusStatus StradusUSB::getMaxLaserPower(int &maxPowermW)
{
	string ret;
	StratusStatus err= sendCommand("?RP\r",ret,STRADUS_COMMAND_TIMEOUT_MS);
	if(err!=STRADUS_OK)
	{
		m_LastFaultCode=err;
		return err;
	}


	m_InternalError.clear();
	if(ret.empty())
	{
		m_InternalError = __FUNCTION__;
		return STRADUS_BAD_RETURN_COMMAND;
	}

	string Command,Value;
	parseReturn(ret,Command,Value);
	if (Command=="?RP")
	{
		maxPowermW = stoi(Value.c_str());
		return STRADUS_OK;
	}
	return STRADUS_FAIL;
}
/**
* FUNCTION getFaultCode
*
* @brief Gets any fault codes for the laser these are bits set 
*
* @author DAVID.LIBBY
* @date 1/26/2016 10:02:29 AM
*
* @param faultCode see the below comment for bit placement.
*
* @return StratusStatus 
* 
* From The API doc
*
* Note  Value  Description
* 0  LASER EMISSION ACTIVE
* 1  STANDBY
* 2  WARMUP
* 4  VALUE OUT OF RANGE
* 8  INVALID COMMAND
* 16  INTERLOCK OPEN
* 32  TEC OFF
* 64  DIODE OVER CURRENT
* 128 DIODE TEMPERATURE FAULT
* 256 BASE PLATE TEMPERATURE FAULT
* 512  BUFFER OVERFLOW
* 1024  EEPROM ERROR
* 2048  I2C ERROR
* 4096  COMMAND TIME OUT
* 8192  WATCH DOG ERROR
* 16384  FATAL ERROR
* 32768  Diode End of Life Warning Indicator
*/
StratusStatus StradusUSB::getFaultCode(int &faultCode)
{
	string ret;
	StratusStatus err= sendCommand("?FC\r",ret,STRADUS_COMMAND_TIMEOUT_MS);

	m_InternalError.clear();
	if(ret.empty())
	{
		m_InternalError = __FUNCTION__;
		return STRADUS_BAD_RETURN_COMMAND;
	}

	string Command,Value;
	parseReturn(ret,Command,Value);
	if (Command=="?FC")
	{
		m_LastFaultCode = stoi(Value.c_str());
		faultCode=m_LastFaultCode;
		return STRADUS_OK;
	}
	return STRADUS_FAIL;
}


/**
* FUNCTION getLastFaultString
*
* @brief Converts last fault to human readable string
*
* @author DAVID.LIBBY
* @date 1/26/2016 10:04:52 AM
*
*
* @return string 
*/
string StradusUSB::getLastFaultString()
{
	string ret = "Laser emission active ";

	if(m_LastFaultCode!=0)
	{
		ret="Laser: ";

		if(m_LastFaultCode & STRADUS_STANDBY)
			ret += "Standby, ";

		if(m_LastFaultCode & STRADUS_WARMUP)
			ret += "Warm up, ";

		if(m_LastFaultCode & STRADUS_VALUE_OUT_OF_RANGE)
			ret += "Value Out Of Range, ";

		if(m_LastFaultCode & STRADUS_INVALID_COMMAND)
			ret += "Invalid Command, ";

		if(m_LastFaultCode & STRADUS_INTERLOCK_OPEN)
			ret += "Interlock Open, ";

		if(m_LastFaultCode & STRADUS_TEC_OFF)
			ret += "Tec Off, ";

		if(m_LastFaultCode & STRADUS_DIODE_OVER_CURRENT)
			ret += "Diode Over Current, ";

		if(m_LastFaultCode & STRADUS_DIODE_TEMPERATURE_FAULT)
			ret += "Diode Temperature Fault, ";

		if(m_LastFaultCode & STRADUS_BASE_PLATE_TEMPERATURE_FAULT)
			ret += "Base Plate Temperature Fault, ";

		if(m_LastFaultCode & STRADUS_BUFFER_OVERFLOW)
			ret += "Buffer Overflow, ";

		if(m_LastFaultCode & STRADUS_EEPROM_ERROR)
			ret += "Eeprom Error, ";

		if(m_LastFaultCode & STRADUS_I2C_ERROR)
			ret += "I2c Error, ";

		if(m_LastFaultCode & STRADUS_COMMAND_TIME_OUT)
			ret += "Command Time Out, ";

		if(m_LastFaultCode & STRADUS_WATCH_DOG_ERROR)
			ret += "Watch Dog Error, ";

		if(m_LastFaultCode & STRADUS_FATAL_ERROR)
			ret += "Fatal Error, ";

		if(m_LastFaultCode & STRADUS_DIODE_END_OF_LIFE_WARNING)
			ret += "Diode End Of Life Warning, ";

		if(m_LastFaultCode & STRADUS_NOT_CONNECTED)
			ret += "STRATUS NOT CONNECTED, ";

		if(m_LastFaultCode & STRADUS_BAD_RETURN_COMMAND)
			ret += "STRADUS_BAD_RETURN_COMMAND, " + m_InternalError;


		if(m_LastFaultCode & STRADUS_LASER_COUNT_INCORRECT)
		{ 
			ostringstream error;
			error << "You should only have 1 laser attached to the system, you have " << getLaserCount() << " laser(s) attached at this time." ;
			ret += error.str();
		}

		if(m_LastFaultCode & STRADUS_COMMAND_TIMEOUT)
			ret += "STRADUS_BAD_RETURN_COMMAND, " + m_InternalError;


	}
	//Remove any trailing nonesense.
	ret.erase(ret.find_last_not_of(", \n\r\t")+1);
	return ret;

}

/**
* FUNCTION getEmissionStatus
*
* @brief Is the laser on or off
*
* @author DAVID.LIBBY
* @date 1/26/2016 10:05:37 AM
*
* @param emission 
*
* @return StratusStatus 
*/
StratusStatus StradusUSB::getEmissionStatus(bool &emission)
{
	string ret;
	StratusStatus err= sendCommand("?LE\r",ret,STRADUS_COMMAND_TIMEOUT_MS);
	if(err!=STRADUS_OK)
	{
		m_LastFaultCode=err;
		return err;
	}

	m_InternalError.clear();
	if(ret.empty())
	{
		m_InternalError = __FUNCTION__;
		return STRADUS_BAD_RETURN_COMMAND;
	}

	string Command,Value;
	parseReturn(ret,Command,Value);
	bool status=false;
	if (Command=="?LE")
	{
		int istatus = stoi(Value.c_str());
		if(istatus==1)
			emission=true;
		else
			emission=false;

	}
	return STRADUS_OK;
}


/**
* FUNCTION getLaserWavelength
*
* @brief Returns the laser wavelength in nm (Nanometers)
*
* @author DAVID.LIBBY
* @date 1/26/2016 10:05:50 AM
*
* @param waveLengthnm 
*
* @return StratusStatus 
*/
StratusStatus StradusUSB::getLaserWavelength(string &waveLengthnm)
{
	string ret;
	StratusStatus err= sendCommand("?LW\r",ret,STRADUS_COMMAND_TIMEOUT_MS);
	if(err!=STRADUS_OK)
	{
		m_LastFaultCode=err;
		return err;
	}


	m_InternalError.clear();
	if(ret.empty())
	{
		m_InternalError = __FUNCTION__;
		return STRADUS_BAD_RETURN_COMMAND;
	}

	string Command;
	parseReturn(ret,Command,waveLengthnm);
	if (Command=="?LW")
	{
		return STRADUS_OK;
	}
	waveLengthnm = "";
	return STRADUS_FAIL;

}



/**
* FUNCTION getBaseTempF
*
* @brief Gets the base temp in farenheit
*
* @author DAVID.LIBBY
* @date 1/26/2016 10:07:26 AM
*
* @param tempF 
*
* @return StratusStatus 
*/
StratusStatus StradusUSB::getBaseTempF(int &tempF)
{
	int celsius;
	if(getBaseTempC(celsius)!=STRADUS_OK)
	{
		return STRADUS_FAIL;
	}
	tempF = (int)((celsius * 9.0) / 5.0 + 32);

	return STRADUS_OK;
}

/**
* FUNCTION parseReturn
*
* @brief Will parse the return string into it's 2 parts so ?LP=120 becomes command = ?LP Value = 120
*
* @author DAVID.LIBBY
* @date 1/26/2016 10:07:58 AM
*
* @param inStr Input string from laser 
* @param Command Command used
* @param Value Value returned.
*/
void StradusUSB::parseReturn(const string &inStr, string &Command, string &Value)
{
	istringstream iss(inStr);
	getline(iss, Command, '=');
	Command.erase(Command.find_last_not_of(" \n\r\t")+1);
	getline(iss, Value, '=');
	Value.erase(Value.find_last_not_of(" \n\r\t")+1);

}

/**
* FUNCTION sendCommand
*
* @brief Sends single command to laser.
*
* @author DAVID.LIBBY
* @date 1/26/2016 10:09:16 AM
*
* @param strCommand user command to laser.
* @param returnCommand The result of the command sent
* @param msTimeout Default of 2 seconds for commands We have found setting laser power can take some time.
*
* @return StratusStatus 
*/
StratusStatus StradusUSB::sendCommand(string strCommand,string &returnCommand, DWORD msTimeout/*=2000*/)
{


	m_InternalError.clear();
	if(!isConnected())
	{
		m_InternalError = __FUNCTION__;
		return STRADUS_BAD_RETURN_COMMAND;
	}

	m_USBLaser.StradusSend(0, strCommand.c_str());	

	StratusStatus ret = getLaserRespone(returnCommand,msTimeout);
	if(ret==STRADUS_COMMAND_TIMEOUT)
	{
		m_InternalError = __FUNCTION__;
		m_InternalError += "Timed out on Command " + strCommand;
	}

	return ret;

}
/**
* FUNCTION getLaserRespone
*
* @brief Retrieve laser response to command.
*
* @author DAVID.LIBBY
* @date 1/26/2016 10:10:40 AM
*
* @param returnCommand 
* @param msTimeout 
*
* @return StratusStatus 
*/
StratusStatus StradusUSB::getLaserRespone(string &returnCommand,DWORD msTimeout/*=2000*/)
{
	DWORD start = GetTickCount();

	m_InternalError.clear();
	if(!isConnected())
	{
		m_InternalError = __FUNCTION__;
		return STRADUS_BAD_RETURN_COMMAND;
	}

	string myReply; 

	while (start + msTimeout>GetTickCount())
	{
		returnCommand = m_USBLaser.StradusGetReply(0);

		if(returnCommand=="EMPTY")
			continue;

		int WeHaveAResponse=1;

		stringstream TempStream(returnCommand);
		string SingleLine;
		while(getline(TempStream,SingleLine,'\n'))
		{
			SingleLine.erase(SingleLine.find_last_not_of(" \n\r\t")+1);
			if(SingleLine.empty())
				continue;
			returnCommand = SingleLine;
			return STRADUS_OK;
		}

	}
	return STRADUS_COMMAND_TIMEOUT;
}




StradusObserver::StradusObserver(void):
	m_grabThreadRunning (false),
	m_stopGrabThread (false),
	m_hGrabThread (0),
	m_ptrToCallbackFunc(NULL),
	m_emegenctStopLaser(false)
{
	InitializeCriticalSection(&m_csGetStats);	
	startlaserGrabThread();
}

StradusObserver::~StradusObserver(void)
{
	stoplaserThreadProc();
	DeleteCriticalSection(&m_csGetStats);
}

bool StradusObserver::stoplaserThreadProc()
{
	if (m_grabThreadRunning) {
		m_stopGrabThread = true;

		while (WAIT_TIMEOUT == WaitForSingleObject(m_hGrabThread, 200)) {}

		if (m_hGrabThread) {
			CloseHandle(m_hGrabThread);
			m_hGrabThread = NULL;
		}

		m_grabThreadRunning = false;
	}
	return true;
}
bool StradusObserver::startlaserGrabThread()
{
	unsigned int threadId;

	if(!m_UsbConnection.isConnected())
	{
		return false;
	}

	if (m_grabThreadRunning) {
		return true;
	}


	m_stopGrabThread = false;

	m_hGrabThread = (HANDLE) _beginthreadex(NULL, 0, StradusObserver::laserThreadProc, this,
		CREATE_SUSPENDED, &threadId);

	if (!m_hGrabThread) {
		//LogErrorMessage("Failed To Create Thread");
		return false;
	}

	SetThreadPriority(m_hGrabThread, THREAD_PRIORITY_NORMAL);

	ResumeThread(m_hGrabThread);

	return true;
}


unsigned __stdcall StradusObserver::laserThreadProc(void* args)
{

	StradusObserver *Observer=NULL;
	Observer = (StradusObserver *) args;

	if(Observer==NULL)
		return false;

	Observer->m_grabThreadRunning = true;

	if(!Observer->m_UsbConnection.isConnected())
	{
		Observer->m_stopGrabThread =true;
	}

	StradusData startData,currentData;

	//Wavelength should not change.
	if(Observer->m_UsbConnection.isConnected())		
	{
		std::string waveNM;
		if(Observer->m_UsbConnection.getLaserWavelength(waveNM)==STRADUS_OK)
		{
			currentData.laserWavelength=waveNM;
		}
	}

	while (!Observer->m_stopGrabThread ) 
	{
		if( Observer->m_stopGrabThread)
		{
			break;
		}

		if(Observer->m_emegenctStopLaser)		
		{
			Observer->m_UsbConnection.setLaserEmission(false);
		}


		StradusRetriveFromLaser(Observer, currentData);
		Sleep(1000);

		
		if(startData!=currentData)		
		{
			startData=currentData;
			Observer->dataCopy(currentData);

		}
	}

	Observer->m_stopGrabThread = true;

	_endthreadex(0);

	// code never reached

	return 0;

}

void StradusObserver::dataCopy(StradusData newData)
{
	EnterCriticalSection(&m_csGetStats);
	m_currentData=newData;
	if(m_ptrToCallbackFunc)
	{
		m_ptrToCallbackFunc(newData);

	}
	LeaveCriticalSection(&m_csGetStats);
}

void StradusObserver::getData(StradusData &currentData)
{
	EnterCriticalSection(&m_csGetStats);

	currentData = m_currentData;

	LeaveCriticalSection(&m_csGetStats);
}


void StradusObserver::StradusObserverRegisterCallback(ptrSoToFunc MyFunc)
{	
	m_ptrToCallbackFunc=MyFunc;
}

void StradusObserver::emergencyStopLaser()
{	
	m_emegenctStopLaser=true;
}

void StradusObserver::StradusRetriveFromLaser(StradusObserver * Observer, StradusData &currentData)
{
	if(Observer==NULL)
		return;
	
	if(Observer->m_UsbConnection.isConnected())		
	{			
		int LaserPower=0;
		if(Observer->m_UsbConnection.getLaserPower(LaserPower)==STRADUS_OK)
		{
			currentData.powermW=(int)LaserPower;
		}

		bool emission=false;
		if(Observer->m_UsbConnection.getEmissionStatus(emission)==STRADUS_OK)
		{
			currentData.laserOn=emission;
		}

		int TempC;
		if(Observer->m_UsbConnection.getBaseTempC(TempC)==STRADUS_OK)
		{		
			currentData.baseTempCelsius=TempC;
		}


		int faultCode;
		if(Observer->m_UsbConnection.getFaultCode(faultCode)==STRADUS_OK)
		{		
			currentData.faultCode=faultCode;
		}

		currentData.statusText=Observer->m_UsbConnection.getLastFaultString();

	}
}
