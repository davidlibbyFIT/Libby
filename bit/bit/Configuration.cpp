#include "Configuration.h"
#include <fstream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <iomanip>
#include <stdint.h>
#include "C:\work\dev-35-hsfc-libby\VS\src\Preconditions.h"

// global scope, exported in Configuration.h
//Configuration gConfiguration;

using namespace std;

/*
=======================================================================================
function to 'scramble' the characters in a string.  it simple rearranges the bits in 
in each character in the same way given by the rule scramble_bit.  the rule is chosen 
so that when the scrambled characters are scrambled again the original string is obtained.
=======================================================================================
*/


/**
* FUNCTION Scramble*
* @brief scrambles the string in place Used version 1 files.
*
* @version 1.0 
*
* @author David Libby
* @date 1/2/2014 3:20:56 PM
*
* @param str 
*/
void Scramble(string &str)
{
	int scramble_bit[] = {7,6,3,2,5,4,1,0};
	int len = str.length();
	int before, after;

	for (int i = 0; i < len; ++i) {
		before = str[i];
		after = 0;
		for (int j = 0; j < 8; ++j) {
			if (before & (1 << scramble_bit[j])) {
				after = after | 1 << j;
			}
		}
		str[i] = (unsigned char) after;
	}
}

/**
* FUNCTION encryptConfigString*
* @brief scrambles the string in place Used version 2 files.
*
* @version 1.0 
*
* @author David Libby
* @date 1/2/2014 3:21:54 PM
*
* @param str 
*/
void encryptConfigString(std::string &str) 
{
	const int P = 2777;
	for (int i=0; i<(int)str.length(); i++)
	{
		int32_t x = str[i] - 0x20;
		x = (x + (i+1)*P);
		x = x % (0x7E - 0x20);
		if (x < 0)
			x += (0x7E - 0x20);
		str[i] = x + 0x20;
	}
}

/**
* FUNCTION decryptConfigString*
* @brief de scrambles the string in place Used version 2 files.
*
* @version 1.0 
*
* @author David Libby
* @date 1/2/2014 3:22:05 PM
*
* @param str 
*/
void decryptConfigString(std::string &str)
{
	const int P = 2777;
	int i = 0;

	for (int i=0; i<(int)str.length(); i++)
	{
		int32_t x = str[i] - 0x20;
		x = (x - (i+1)*P) % (0x7E - 0x20);
		if (x < 0)
			x += (0x7E - 0x20);
		str[i] = 0x20 + x;
	}
}


/**
* FUNCTION cfgChecksumCalc*
* @brief Calculate the checksum for a string. If you want to shorten the calculation then pass in how many places you want calculated.
*
* @version 1.0 
*
* @author David Libby
* @date 1/3/2014 9:11:22 AM
*
* @param a_CheckString 
* @param length 
*
* @return int 
*/
int cfgChecksumCalc(const string a_CheckString,int length/*=-1*/ )
{
	int checksum;
	size_t size = a_CheckString.length();
	
	//By Default it will check the entire string if the user provides the length then it will
	//Truncate the check.
	if(length>-1)
	{
		if(a_CheckString.length()<size)
			return -1;
		size=length;
	}
	if (size<1) 
		return -1;

	checksum = 0;
	for (size_t index=0;index<size;index++){
		checksum += a_CheckString[index];
	}
	checksum = 1 + checksum % 119;
	return checksum;

};


Configuration::Configuration(void):m_bConfigurationDirty(false)
{

	//Make sure you have a matching define in the Configure Header file
	InitConfigValues (FIT_FEATURE_2_21CFR, FIT_FEATURE_2_BOOL_FALSE);
	InitConfigValues (FIT_FEATURE_2_AUTOFOCUS, FIT_FEATURE_2_BOOL_FALSE);
	InitConfigValues (FIT_FEATURE_2_CAMERA, FIT_FEATURE_2_NOT_AVAILABLE);
	InitConfigValues (FIT_FEATURE_2_CASE, FIT_FEATURE_2_NOT_AVAILABLE);
	InitConfigValues (FIT_FEATURE_2_CFG_VERSION,"2.0");
	InitConfigValues (FIT_FEATURE_2_DSP, FIT_FEATURE_2_NOT_AVAILABLE);
	InitConfigValues (FIT_FEATURE_2_FCMODEL, FIT_FEATURE_2_NOT_AVAILABLE);
	InitConfigValues (FIT_FEATURE_2_FILTER, FIT_FEATURE_2_NOT_AVAILABLE);
	InitConfigValues (FIT_FEATURE_2_HLFDIR, "\\FONTS\\");
	InitConfigValues (FIT_FEATURE_2_LASER, FIT_FEATURE_2_NOT_AVAILABLE);
	InitConfigValues (FIT_FEATURE_2_OPTICAL_SENSOR, FIT_FEATURE_2_NOT_AVAILABLE);
	InitConfigValues (FIT_FEATURE_2_PUMP, FIT_FEATURE_2_NOT_AVAILABLE);
	InitConfigValues (FIT_FEATURE_2_SATELLITE, FIT_FEATURE_2_BOOL_FALSE);
	InitConfigValues (FIT_FEATURE_2_SERIALNUMBER, FIT_FEATURE_2_NOT_AVAILABLE);
	InitConfigValues (FIT_FEATURE_2_TESTFEATURES, FIT_FEATURE_2_BOOL_FALSE);

}


Configuration::~Configuration(void)
{
}


bool Configuration::featureIsEnabled(const std::string &a_key)
{
	precondition_throw(isValidKey(a_key)==true,"Invalid Key value");
	return(m_ConfigData[a_key].value==FIT_FEATURE_2_BOOL_TRUE);
}

/**
* FUNCTION addFeature*
* @brief Adds a new feature bit.
*
* @version 1.0 
*
* @author David Libby
* @date 12/27/2013 4:58:33 PM
*
* @param a_feature 
*/
void Configuration::enableFeature(const std::string &a_key)
{
	precondition_throw(isValidKey(a_key)==true,"Invalid Key value");
	setItem(a_key,FIT_FEATURE_2_BOOL_TRUE);
}
/**
* FUNCTION removeFeature*
* @brief Removes a feature bit
*
* @version 1.0 
*
* @author David Libby
* @date 12/27/2013 4:58:53 PM
*
* @param a_feature 
*/
void Configuration::disableFeature(const std::string &a_key)
{
	precondition_throw(isValidKey(a_key)==true,"Invalid Key value");
	setItem(a_key,FIT_FEATURE_2_BOOL_FALSE);
}

/**
* FUNCTION isDirty*
* @brief Checks to see if the current configuration is dirty.
*
* @version 1.0 
*
* @author David Libby
* @date 1/2/2014 3:22:35 PM
*
*
* @return bool 
*/
bool Configuration::isDirty()
{
	return m_bConfigurationDirty;
}

/**
* FUNCTION setClean*
* @brief Sets the dirty flags to clean.
*
* @version 1.0 
*
* @author David Libby
* @date 1/2/2014 3:23:09 PM
*
*/
void Configuration::setClean()
{
	m_bConfigurationDirty;
}

string Configuration::getItem(const string & a_key)
{
	precondition_throw(isValidKey(a_key)==true,"Invalid Key value");
	return m_ConfigData[a_key].value;
}
/**
* FUNCTION setItem*
* @brief Set value for given key.
*
* @version 1.0 
*
* @author David Libby
* @date 1/2/2014 3:55:34 PM
*
* @param a_key 
* @param a_value 
*/
void Configuration::setItem(const string & a_key,const string & a_value)
{
	precondition_throw(isValidKey(a_key)==true,"Invalid Key value");
	m_ConfigData[a_key].value = a_value;
	m_ConfigData[a_key].dirty = true;
	m_bConfigurationDirty=true;
}

/**
* FUNCTION getItemLength*
* @brief Get Length of value
*
* @version 1.0 
*
* @author David Libby
* @date 1/2/2014 3:55:07 PM
*
* @param a_key 
*
* @return int 
*/
int Configuration::getItemLength(const string & a_key)
{
	precondition_throw(isValidKey(a_key)==true,"Invalid Key value");
	return  m_ConfigData[a_key].value.length();
}

/**
* FUNCTION isValidKey*
* @brief Checks to insure the key passed in is valid.
*
* @version 1.0 
*
* @author David Libby
* @date 1/2/2014 3:40:11 PM
*
* @param a_key 
*
* @return bool 
*/
bool Configuration::isValidKey(const string &a_key)
{
	if ( m_ConfigData.find(a_key) != m_ConfigData.end() ) 
		return true;

	return false;
}

/**
* FUNCTION loadFile*
* @brief Load config file(s) will work with version 1 and 2 files.
*
* @version 1.0 
*
* @author David Libby
* @date 1/2/2014 2:28:06 PM
*
* @param a_Filename 
*/
void Configuration::loadFile(const string &a_Filename)
{

	ifstream in(a_Filename, ios::in | ios::binary);
	if (in)
	{
		string contents;
		in.seekg(0, ios::end);
		contents.resize((unsigned int)in.tellg());
		in.seekg(0, ios::beg);
		in.read(&contents[0], contents.size());
		in.close();

		//Check to see if it is plain text
		if(!isStringInStringNoCase(contents,FIT_FEATURE_2_CFG_VERSION))//If not
		{
			string OldString=contents;
			decryptConfigString(contents);//Try most recent  De scramble
			if(!isStringInStringNoCase(contents,FIT_FEATURE_2_CFG_VERSION))
			{
				contents=OldString;
				Scramble(contents);//Try Old De scramble.				
				if(!isStringInStringNoCase(contents,FIT_FEATURE_2_CFG_VERSION))
				{
					int WeHaveAHugeProblem=1;
				}


			}
		}

		int origsum = contents[contents.length()-1];
		//We subtract 1 from the checksum to account for the last char being the checksum.
		int checksum=cfgChecksumCalc(contents,contents.length()-1);

		if (checksum != origsum) 
		{
			precondition_throw(0,"The Configuration File is corrupted.");
			/*
			Log::write(LOG_LEVEL_ERROR, "The Configuration File is corrupted");
			if (!networkrun) {
				MessageBox(NULL, 
					"The Configuration File is corrupted.\n"
					"\n"
					"Please contact Fluid Imaging Technologies to obtain the\n" 
					"Configuration File for this instrument.", 
					"Corrupted Configuration File", 
					MB_OK | MB_APPLMODAL);
			}*/
			return ;
		}

		

		for(iConfigurationMap iData = m_ConfigData.begin(); iData != m_ConfigData.end(); iData++) 
		{
			setValueFromString(contents, iData->first);
		}
		setClean();
	}

}

/**
* FUNCTION SaveFile*
* @brief Save Data file.
*
* @version 1.0 
*
* @author David Libby
* @date 1/2/2014 2:27:26 PM
*
* @param a_Filename 
*/
void Configuration::SaveFile(const string &a_Filename)
{
	//If it is not dirty then don't save.
	if(!isDirty())
		return;

	//Build String
	std::string configString=createConfigurationString();	

	int checksum=cfgChecksumCalc(configString);
	configString+=checksum;

	encryptConfigString(configString);	



	fstream out(a_Filename,ios::out|ios::binary);
	if (out)
	{
		out << configString;
		out.close();
	}
	setClean();
}

/**
* FUNCTION setValueFromString*
* @brief Looks in content string got Key Value and Updates the data
*
* @version 1.0 
*
* @author David Libby
* @date 1/2/2014 3:53:48 PM
*
* @param a_contents 
* @param a_key 
*
* @return bool 
*/
bool Configuration::setValueFromString( string &a_contents/*in*/,const string &a_key/*in*/)
{
	precondition_throw(isValidKey(a_key)==true,"Invalid Key value");
	//Create Upper Case string to search for Keys.
	string upperString= a_contents;
	transform(upperString.begin(), upperString.end(), upperString.begin(), toupper);	

	bool ret=false;	
	size_t found = upperString.find(a_key);
	if( found != string::npos)
	{
		size_t currentPoint = found+a_key.length();

		//All lengths are encoded in a 3 number pattern 003 etc
		string lenStr=upperString.substr(currentPoint,3);
		currentPoint +=3;

		//Get the length
		istringstream ss(lenStr);		
		int length =0;
		ss >> length;
		
		//Get The Value
		setItem(a_key,a_contents.substr(currentPoint,length));
		ret = true;
		
	}
	return ret;
}

/**
* FUNCTION InitConfigValues*
* @brief Init pairs for key and default values.
*
* @version 1.0 
*
* @author David Libby
* @date 1/2/2014 2:18:17 PM
*
* @param a_key 
* @param a_Default 
*/
void Configuration::InitConfigValues (string a_key,const string &a_Default)
{
	m_ConfigData[a_key].value=a_Default;
	m_ConfigData[a_key].default=a_Default;
}

/**
* FUNCTION addDataToString*
* @brief Adds single data item to string
*
* @version 1.0 
*
* @author David Libby
* @date 1/2/2014 2:17:08 PM
*
* @param a_Config 
* @param a_key 
* @param a_value 
*/
void Configuration::addDataToString(string &a_Config,const string &a_key,const string &a_value )
{
	precondition_throw(isValidKey(a_key)==true,"Invalid Key value");
	a_Config+=a_key;
	int lenCount=a_value.length();
	std::stringstream strLenCount;
	strLenCount << setw(3) << setfill('0') << lenCount;
	a_Config+=strLenCount.str();
	a_Config+=a_value;	
}

/**
* FUNCTION createConfigurationString*
* @brief Builds the Raw Config String for serial use.
*
* @version 1.0 
*
* @author David Libby
* @date 1/2/2014 2:16:31 PM
*
*
* @return string 
*/
string Configuration::createConfigurationString()
{

	string configString;
	//We always want the Config version at the top of the file.
	addDataToString(configString,FIT_FEATURE_2_CFG_VERSION , m_ConfigData[FIT_FEATURE_2_CFG_VERSION].value);

	for(iConfigurationMap iData = m_ConfigData.begin(); iData != m_ConfigData.end(); iData++) 
	{
		//We don't want to write config version this twice.
		if(iData->first==FIT_FEATURE_2_CFG_VERSION )
			continue;
		//If the Item is not dirty then no need to save.
		if(iData->second.dirty==false)
			continue;
		//If it is set to the default value there is no need to save it.
		if(iData->second.default==iData->second.value)
			continue;

		addDataToString(configString, iData->first/*Key*/, iData->second.value/*value*/);
	}
	return configString;
}

/**
* FUNCTION isStringInStringNoCase*
* @brief Case Insensitive Search of string
*
* @version 1.0 
*
* @author David Libby
* @date 1/2/2014 1:18:21 PM
*
* @param a_Target 
* @param a_Search 
*
* @return bool 
*/
bool Configuration::isStringInStringNoCase( string a_Target, string a_Search )
{
	bool isStringInString=false;
	std::transform( a_Target.begin(), a_Target.end(), a_Target.begin(), ::toupper );
	std::transform( a_Search.begin(), a_Search.end(), a_Search.begin(), ::toupper );

	if (a_Target.find(FIT_FEATURE_2_CFG_VERSION)!=string::npos)
		isStringInString= true;	

	return isStringInString;
}

