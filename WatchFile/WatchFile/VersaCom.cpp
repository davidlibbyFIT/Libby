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

VersaReturn VersaCom::setFileName(const std::string &newFileName)
{
	
	if(!fileExists(newFileName))
		return VERSA_MISSING_INI_FILE;	
	m_pFileInformation.reset( new FileInformation(newFileName));
	//m_pWatchingThread.reset( new std::thread(workingThread(this)));

	return VERSA_OK;
	
}
bool VersaCom::fileExists(const std::string &newFileName)
{
	bool exists=false;
	std::ifstream file(newFileName.c_str());
	if(file)
		exists=true;
	return exists;
}
std::string VersaCom::getStatusString(VersaStatus status)
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

	}

	return "Bad Parameter";
}

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

void VersaCom::theFileChanged()
{
	static int ct=0;
	ct++;
	std::cout << "File Changed " << ct << " Time(s)\n";
}

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
