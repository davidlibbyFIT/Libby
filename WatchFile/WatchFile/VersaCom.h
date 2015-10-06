#ifndef VERSA_COM
#define VERSA_COM

#include "FileInformation.h"
#include <string>
#include <memory>


typedef enum VersaReturn
{
	VERSA_OK
	,VERSA_FAIL
	,VERSA_MISSING_INI_FILE
	,VERSA_THREAD_ALLREADY_RUNNING
	,VERSA_FAILED_TO_CREATE_THREAD
};


/*
"Status.ini" status codes:
	Status=0 -- idle status (updated by VERSA Appl. 110 when VERSA 110 instrument is initialized with home toolbar button or when the sequence starts running)
	Status=1 -- acknowledgement by Fluid Imager software (when Fluid Imager reads Status=3, Fluid Imager immediately updates Status=1)
	Status=3 -- VERSA Appl. 110 tells Fluid Imager to start reading a single sample that was just dispensed into injection port (after first delay if available)
	Status=4 -- Fluid Imager tells VERSA Appl. 110 that a single sample was successfully read (VERSA Appl. 110 proceeds to next sample or wash sequence after second delay if available)
	Status=5 -- Fluid Imager returns error or VERSA Appl. 110 returns hardware error (all execution is stopped; the 500 ms timer is not disabled; error	message is displayed by VERSA Appl. 110)
*/

typedef enum VersaStatus
{
	VERSA_STATUS_IDLE = 0
	,VERSA_STATUS_START_IMAGING
	,VERSA_STATUS_SAMPLE_READY
	,VERSA_STATUS_IMAGING_COMPLETE
	,VERSA_STATUS_ERROR
	,VERSA_VISUAL_SPREADSHEET_IDLE
	,VERSA_ALL_STOP
};

class VersaCom
{
public:
	VersaCom(void);
	~VersaCom(void);
	VersaReturn setFileName(const std::string &newFileName);
	bool fileExists(const std::string &newFileName);
	std::string convertStatusToStrig(VersaStatus status);
	VersaReturn startWorkingThread();
	VersaReturn stopWorkingThread();


private:
	static unsigned __stdcall VersaCom::workingThread(void* args);
	void theFileChanged();
	
	std::string m_fileName;
	std::unique_ptr<FileInformation> m_pFileInformation;

	bool m_grabThreadRunning;
	HANDLE m_hGrabThread;
	bool m_stopGrabThread;
};


#endif //VERSA_COM

