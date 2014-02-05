#ifndef CONFIGURATION_H
#define CONFIGURATION_H

//#include <version.h>
#include <string>
#include <map>

#define START_TIME_FOR_SERIAL_NUMBERS 1307540280//__time64() at 6/8/2011T9:30AM EST
#define HLF_HASH_VALUE 1979
#define HIDDEN_FILE_MOD_NUMBER 1667
#define CONFIGURATION_FORMAT_VERSION "2.0"

//Pre Defined Default Types.
#define FIT_FEATURE_2_BOOL_FALSE		"NO"
#define FIT_FEATURE_2_BOOL_TRUE			"YES"
#define FIT_FEATURE_2_NOT_AVAILABLE		"NA"

//Make sure you init the default value in the constructor.
//InitConfigValues (FIT_FEATURE_2_21CFR, "NO");
//THese Key values should be upper case.
#define FIT_FEATURE_2_21CFR				"21CFR"
#define FIT_FEATURE_2_AUTOFOCUS			"AUTOFOCUS"
#define FIT_FEATURE_2_CAMERA			"CAMERA"
#define FIT_FEATURE_2_CASE				"CASE"
#define FIT_FEATURE_2_CFG_VERSION		"CFGFORMATVERSION"
#define FIT_FEATURE_2_DSP				"DSP"
#define FIT_FEATURE_2_FCMODEL			"FCMODEL"
#define FIT_FEATURE_2_FILTER			"FILTER"
#define FIT_FEATURE_2_HLFDIR			"HLFDIR"
#define FIT_FEATURE_2_LASER				"LASER"
#define FIT_FEATURE_2_OPTICAL_SENSOR	"OPTICAL SENSOR"
#define FIT_FEATURE_2_PUMP				"PUMP"
#define FIT_FEATURE_2_SATELLITE			"SATELLITE"
#define FIT_FEATURE_2_SERIALNUMBER		"SERIALNUMBER"
#define FIT_FEATURE_2_TESTFEATURES		"TESTFEATURES"


void Scramble(std::string &str);
void decryptConfigString(std::string &str);
void encryptConfigString(std::string &str);
int cfgChecksumCalc(const std::string a_CheckString,int length=-1);

//extern Configuration gConfiguration;

class Configuration
{
	
	typedef struct FeatureElement
	{

		std::string value;
		std::string default;
		bool dirty;
		FeatureElement()
		{
			default.clear();
			value.clear();
			dirty = false;
		}


	}FeatureElement;

	typedef std::map <std::string,FeatureElement> ConfigurationMap;
	typedef ConfigurationMap::iterator iConfigurationMap;

public:
	Configuration(void);
	~Configuration(void);

	bool featureIsEnabled(const std::string &a_key);
	void enableFeature(const std::string &a_key);
	void disableFeature(const std::string &a_key);
	std::string getItem(const std::string & a_key);
	void setItem(const std::string & a_key,const std::string & a_value);
	int getItemLength(const std::string & a_key);

	bool isDirty();
	void loadFile(const std::string &a_Filename);
	void SaveFile(const std::string &a_Filename);

private:
	bool isValidKey(const std::string &a_key);
	void setClean();
	bool isStringInStringNoCase( std::string a_Target, std::string a_Search);
	std::string createConfigurationString();
	void addDataToString( std::string &a_Config,const std::string &a_key,const std::string &a_value );
	bool setValueFromString( std::string &a_contents/*in*/, const std::string &a_key/*in*/);
	void InitConfigValues (std::string a_keyString,const std::string &a_Default);

private:

	ConfigurationMap m_ConfigData;
	bool m_bConfigurationDirty;

};



#endif // ifndef CONFIGURATION_H