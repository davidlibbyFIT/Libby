#include "VersaCom.h"
#include<fstream>
#include <iostream>       // std::cout
#include <process.h>    /* _beginthread, _endthread */

VersaCom::VersaCom(void):
	m_fileName("")	
	,m_grabThreadRunning(false)
	,m_hGrabThread(NULL)
	,m_stopGrabThread(false)

{
	m_pFileInformation.reset();
}
VersaCom::~VersaCom(void)
{
	stopWorkingThread();
}

/**
* FUNCTION setFileName
*
* @brief Sets the filename of the ini file to watch.
*
* @version 1.0 
*
* @author David Libby
* @date 10/6/2015 4:14:03 PM
*
* @param newFileName 
*
* @return VersaReturn 
*/
VersaReturn VersaCom::setFileName(const std::string &newFileName)
{
	
	if(!fileExists(newFileName))
		return VERSA_MISSING_INI_FILE;	
	m_pFileInformation.reset( new FileInformation(newFileName));
	//m_pWatchingThread.reset( new std::thread(workingThread(this)));

	return VERSA_OK;
	
}
/**
* FUNCTION fileExists
*
* @brief Checks to see if the file exists..
*
* @version 1.0 
*
* @author David Libby
* @date 10/6/2015 4:14:27 PM
*
* @param newFileName 
*
* @return bool 
*/
bool VersaCom::fileExists(const std::string &newFileName)
{
	bool exists=false;
	std::ifstream file(newFileName.c_str());
	if(file)
		exists=true;
	return exists;
}
std::string VersaCom::convertStatusToStrig(VersaStatus status)
{

	/*
		Status=0 -- idle status (updated by VERSA Appl. 110 when VERSA 110 instrument is initialized with home toolbar button or when the sequence starts running)
		Status=1 -- acknowledgement by Fluid Imager software (when Fluid Imager reads Status=3, Fluid Imager immediately updates Status=1)
		Status=3 -- VERSA Appl. 110 tells Fluid Imager to start reading a single sample that was just dispensed into injection port (after first delay if available)
		Status=4 -- Fluid Imager tells VERSA Appl. 110 that a single sample was successfully read (VERSA Appl. 110 proceeds to next sample or wash sequence after second delay if available)
		Status=5 -- Fluid Imager returns error or VERSA Appl. 110 returns hardware error (all execution is stopped; the 500 ms timer is not disabled; error	message is displayed by VERSA Appl. 110)
	*/
	switch (status)
	{
		case VERSA_STATUS_IDLE:
			return "idle status (updated by VERSA Appl. 110 when VERSA 110 instrument is initialized with home toolbar button or when the sequence starts running)";
				break;
		case VERSA_STATUS_START_IMAGING:
			return "acknowledgement by Fluid Imager software (when Fluid Imager reads Status=3, Fluid Imager immediately updates Status=1)";
				break;
		case VERSA_STATUS_SAMPLE_READY:
			return "VERSA Appl. 110 tells Fluid Imager to start reading a single sample that was just dispensed into injection port (after first delay if available)";
				break;
		case VERSA_STATUS_IMAGING_COMPLETE:
			return "Fluid Imager tells VERSA Appl. 110 that a single sample was successfully read (VERSA Appl. 110 proceeds to next sample or wash sequence after second delay if available)";
				break;
		case VERSA_STATUS_ERROR:
			return "Fluid Imager returns error or VERSA Appl. 110 returns hardware error (all execution is stopped; the 500 ms timer is not disabled; error	message is displayed by VERSA Appl. 110)";
			break;
		case VERSA_VISUAL_SPREADSHEET_IDLE:
			return "Visual Spreadsheet is Idle and Waiting for a command.";
			break;
		case VERSA_ALL_STOP:
			return "THings have gone sideways everyone stop.";
			break;

	}

	return "Bad Parameter";
}

/**
* FUNCTION workingThread
*
* @brief Thread the watches the ini file.
*
* @version 1.0 
*
* @author David Libby
* @date 10/6/2015 4:16:34 PM
*
* @param args 
*
* @return unsigned 
*/
unsigned __stdcall VersaCom::workingThread(void* args)
{

	VersaCom *pVersa= (VersaCom *) args;		

	while (! pVersa->m_stopGrabThread)
	{
		pVersa->m_pFileInformation->waitForFileToChange(500);//500 is 1/2 seconds..
		pVersa->theFileChanged();
	}
	
	pVersa->m_stopGrabThread = true;

	//Log::write(LOG_LEVEL_DEBUG, "Sentech::grabThreadProc ending");

	_endthreadex(0);

	// code never reached

	return 0;
}

/**
* FUNCTION theFileChanged
*
* @brief Callback function that will alert when the file has changed.
*
* @version 1.0 
*
* @author David Libby
* @date 10/6/2015 4:16:49 PM
*
*/
void VersaCom::theFileChanged()
{
	static int ct=0;
	ct++;
	std::cout << "File Changed " << ct << " Time(s)\n";
}

/**
* FUNCTION startWorkingThread
*
* @brief Starts the thread.
*
* @version 1.0 
*
* @author David Libby
* @date 10/6/2015 4:17:20 PM
*
*
* @return VersaReturn 
*/
VersaReturn VersaCom::startWorkingThread()
{
	unsigned int threadId;

	if(m_pFileInformation==NULL)
	{
		return VERSA_MISSING_INI_FILE;
	}

	if (m_grabThreadRunning) {
		return VERSA_THREAD_ALLREADY_RUNNING;
	}

	m_stopGrabThread = false;

	m_hGrabThread = (HANDLE) _beginthreadex(NULL, 0,  VersaCom::workingThread, this,
		CREATE_SUSPENDED, &threadId);

	if (!m_hGrabThread) {
		//LogErrorMessage("Failed To Create Thread");
		return VERSA_FAILED_TO_CREATE_THREAD;
	}

	SetThreadPriority(m_hGrabThread, THREAD_PRIORITY_TIME_CRITICAL);

	ResumeThread(m_hGrabThread);
	m_grabThreadRunning=true;

	return VERSA_OK;
}

/**
* FUNCTION stopWorkingThread
*
* @brief Stop the thread.
*
* @version 1.0 
*
* @author David Libby
* @date 10/6/2015 4:17:40 PM
*
*
* @return VersaReturn 
*/
VersaReturn VersaCom::stopWorkingThread()
{
	if (m_grabThreadRunning) {
		m_stopGrabThread = true;

		while (WAIT_TIMEOUT == WaitForSingleObject(m_hGrabThread, 200)) 
		{

		}

		if (m_hGrabThread) {
			CloseHandle(m_hGrabThread);
			m_hGrabThread = NULL;
		}

		m_grabThreadRunning = false;
	}


	return VERSA_OK;
}
