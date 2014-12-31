#include "FeatureKeyHandler.h"
#include <time.h>


#define CHARS_TO_USE "123456789ABCDEFGHIJKLMNPQRSTUVWZYZ"


FeatureKeyHandler::FeatureKeyHandler()
{
	m_keyStorageVec.clear();
}
FeatureKeyHandler::~FeatureKeyHandler(void)
{
}


/**
* FUNCTION CheckExpireTime
*
* @brief Check to see if key expires
*
* @author DAVID.LIBBY
* @date 12/9/2014 2:34:49 PM
*
* @param KeyItem 
*
* @return FeatuerKeyStatus FEATURE_KEY_OK but be ready to react to FEATURE_KEY_EXPIRED and delete the file as needed.
*/
FeatuerKeyStatus FeatureKeyHandler::CheckExpireTime(FeatureKeyItem &KeyItem)
{
	if(KeyItem.vaildateFeatureKey()!=FEATURE_KEY_OK)
		return FEATURE_KEY_BAD_DATE;

	time_t t = time(0);   // get time now
	struct tm * now = localtime( & t );
	
	int month = std::stoi(KeyItem.getExpireDate().substr(0,2));
	int day = std::stoi(KeyItem.getExpireDate().substr(2,2));
	int year = std::stoi(KeyItem.getExpireDate().substr(4,2));
	//Check year and month only after the day in the expire time has passed do we alert..
	if(month <= now->tm_mon && year <= now->tm_year-100 && day < now->tm_mday)
	{
		return FEATURE_KEY_EXPIRED;
	}

	return FEATURE_KEY_OK;
}

/**
* FUNCTION addKeyItem
*
* @brief Add new Item to KeyList
*
* @author DAVID.LIBBY
* @date 12/9/2014 2:40:35 PM
*
* @param NewKeyItem 
*
* @return FeatuerKeyStatus FEATURE_KEY_OK but be ready to react to FEATURE_KEY_EXPIRED and delete the file as needed.
*/
FeatuerKeyStatus FeatureKeyHandler::addKeyItem(FeatureKeyItem &NewKeyItem)
{

	FeatuerKeyStatus ret = CheckExpireTime(NewKeyItem);
	if(ret != FEATURE_KEY_OK)
	{
		return ret;
	}	
	m_keyStorageVec.push_back(NewKeyItem);
	return FEATURE_KEY_OK;
}




/** @class FeatureKeyStructure the length of all these should not exceed 16 chars;
 *  @brief This structure blah blah blah...
 *  @var type This is used to hold the type of Key in use. it is only 3 chars long 
 *  @var expireDate is when the item should be invalidated. if is set in mmddyy so for 0ctober 1 2014 it would be expireDate="100114";
 *  @var setialNumber contains the device serial number or the case of sattelight the last 5 of the mac address.  setialNumber = "01000"
 *  @var Checksum checksum number 10
 */

FeatureKeyItem::FeatureKeyItem()
{
	m_feature = "NUL";
	m_expireDate = "000000";//Default is no expire date
	m_serialNumber = "00000" ;
	addChecksum();
}
FeatureKeyItem::~FeatureKeyItem(void)
{
}

/**
* FUNCTION vaildatePreChecksumString
*
* @brief Checks the size of the live fields to insure they are ok.
*
* @author DAVID.LIBBY
* @date 12/9/2014 11:10:42 AM
*
*
* @return FeatuerKeyStatus FEATURE_KEY_OK if things worked otherwise returns an error.
*/
FeatuerKeyStatus FeatureKeyItem::vaildatePreChecksumString()
{
	if(m_feature.length()>FEATURE_ITEM_FEATURE_MAX)
		return FEATURE_KEY_BAD_FEATURE;
	if(m_expireDate.length()>FEATURE_ITEM_EXPIRE_MAX)
		return FEATURE_KEY_BAD_DATE;
	if(m_serialNumber.length()>FEATURE_ITEM_SERIAL_MAX)
		return FEATURE_KEY_BAD_SERIAL;
	return FEATURE_KEY_OK;
}
FeatuerKeyStatus FeatureKeyItem::vaildateFeatureKey()
{
	FeatuerKeyStatus ret = vaildatePreChecksumString();
	if(ret!=FEATURE_KEY_OK)
	{
		return ret;
	}

	return addChecksum();

}

/**
* FUNCTION getPreChecksumString
*
* @brief Returns the string before the checksum is calculated.
*
* @author DAVID.LIBBY
* @date 12/8/2014 3:17:42 PM
*
* @param inOutString 
*
* @return FeatuerKeyStatus FEATURE_KEY_OK if things worked otherwise returns an error.
*/
FeatuerKeyStatus FeatureKeyItem::getPreChecksumString(std::string &inOutString)
{
	FeatuerKeyStatus ret = vaildatePreChecksumString();
	inOutString.clear();
	if(ret!=FEATURE_KEY_OK)
		 return ret;
	inOutString =  m_feature+m_expireDate+m_serialNumber;
	return FEATURE_KEY_OK;
}

/**
* FUNCTION calculateChecksum
*
* @brief Calculates the checksum of the 3 fields and returns a hex sum of there values truncated to the size of a unsigned char
*		It returns a 2 char hex value in upper case. such as FA
*
* @author DAVID.LIBBY
* @date 12/8/2014 3:18:30 PM
*
* @param inOutString 
*
* @return FeatuerKeyStatus FEATURE_KEY_OK if things worked otherwise returns an error.
*/
FeatuerKeyStatus FeatureKeyItem::calculateChecksum(std::string &inOutString)
{
	inOutString.clear();
	FeatuerKeyStatus ret = vaildatePreChecksumString();
	if(ret!=FEATURE_KEY_OK)
		return ret;	

	//Get the pre string
	std::string preCheckString;
	ret = getPreChecksumString(preCheckString);
	if(ret != FEATURE_KEY_OK)
	{
		return ret;
	}
	
	if(preCheckString.length()<1)
		return FEATURE_KEY_STRING_TOO_SHORT;

	//Calculate the crude checksum
	unsigned char chcksum=0;
	for(std::string::size_type i = 0; i < preCheckString.size(); ++i) {
		chcksum += preCheckString[i];
	}		
	//Convert it to Hex
	char hex[3];
	hex[0]='\0';
	sprintf_s(hex,"%02x",chcksum);
	hex[2]='\0';

	//Upper case the Hex
	hex[0]=toupper(hex[0]);
	hex[1]=toupper(hex[1]);

	inOutString =  hex;
	
	return FEATURE_KEY_OK;
}


/**
* FUNCTION setSerial
*
* @brief Sets the serial number
*
* @author DAVID.LIBBY
* @date 12/8/2014 3:22:24 PM
*
* @param newValue new string to use.
*
* @return FeatuerKeyStatus FEATURE_KEY_OK if things worked otherwise returns an error.
*/
FeatuerKeyStatus FeatureKeyItem::setSerial(std::string newValue)
{
	if(newValue.length()>FEATURE_ITEM_SERIAL_MAX)
		return FEATURE_KEY_BAD_SERIAL;

	m_serialNumber.clear();
	int lendiff=FEATURE_ITEM_SERIAL_MAX-newValue.length();
	for(int space=0;space<lendiff;space++)
	{
		m_serialNumber +=" ";
	}
	m_serialNumber += newValue;

	return addChecksum();
}
/**
* FUNCTION setFeature
*
* @brief Sets the feature code
*
* @author DAVID.LIBBY
* @date 12/8/2014 3:22:55 PM
*
* @param newValue new string
*
* @return FeatuerKeyStatus FEATURE_KEY_OK if things worked otherwise returns an error.
*/
FeatuerKeyStatus FeatureKeyItem::setFeature(std::string newValue)
{
	if(newValue.length()>FEATURE_ITEM_FEATURE_MAX)
		return FEATURE_KEY_BAD_FEATURE;

	m_feature.clear();
	
	int lendiff=FEATURE_ITEM_FEATURE_MAX-newValue.length();
	for(int space=0;space<lendiff;space++)
	{
		m_feature +=" ";
	}
	m_feature += newValue;
	return addChecksum();
}
/**
* FUNCTION setExpireDate
*
* @brief Sets the expire date in the format of mmddyy 112114
*
* @author DAVID.LIBBY
* @date 12/8/2014 3:23:33 PM
*
* @param newValue new string
*
* @return FeatuerKeyStatus FEATURE_KEY_OK if things worked otherwise returns an error.
*/
FeatuerKeyStatus FeatureKeyItem::setExpireDate(std::string newValue)
{
	if(newValue.length()>FEATURE_ITEM_EXPIRE_MAX)
		return FEATURE_KEY_BAD_DATE;
	
	m_expireDate.clear();
	
	int lendiff=FEATURE_ITEM_EXPIRE_MAX-newValue.length();
	for(int space=0;space<lendiff;space++)
	{
		m_expireDate +="0";
	}
	m_expireDate += newValue;

	return addChecksum();
}

/**
* FUNCTION getSerial
*
* @brief Returns serial number
*
* @author DAVID.LIBBY
* @date 12/8/2014 3:25:00 PM
*
*
* @return std::string 
*/
std::string FeatureKeyItem::getSerial()
{
	return m_serialNumber;
}
/**
* FUNCTION getFeature
*
* @brief Returns Feature Code
*
* @author DAVID.LIBBY
* @date 12/8/2014 3:25:17 PM
*
*
* @return std::string 
*/
std::string FeatureKeyItem::getFeature()
{
	return m_feature;
}
/**
* FUNCTION getExpireDate
*
* @brief Expire Date mmddyy
*
* @author DAVID.LIBBY
* @date 12/8/2014 3:25:29 PM
*
*
* @return std::string 
*/
std::string FeatureKeyItem::getExpireDate()
{
	return m_expireDate;
}
/**
* FUNCTION getChecksum
*
* @brief Returns checksum in hex FF
*
* @author DAVID.LIBBY
* @date 12/8/2014 3:25:45 PM
*
*
* @return std::string 
*/
std::string FeatureKeyItem::getChecksum()
{
	return m_Checksum;
}

/**
* FUNCTION getEncodeString
*
* @brief Return formated String as FFFD-DDDD-DSSS-SSCC
*
* FFF		= 3 char feature code
* DDDDDD	= 6 char expiration Date
* SSSSS		= 5 digit serial number
* CC		= 2 digit checksum in hex
*
* @author DAVID.LIBBY
* @date 12/8/2014 3:28:37 PM
*
* @param inOutString 
*
* @return FeatuerKeyStatus FEATURE_KEY_OK if things worked otherwise returns an error.
*/
FeatuerKeyStatus FeatureKeyItem::getEncodeString(std::string &inOutString)
{
	inOutString.clear();
	FeatuerKeyStatus ret = vaildatePreChecksumString();
	if(ret!=FEATURE_KEY_OK)
		return ret;

	ret = addChecksum();
	if(ret!=FEATURE_KEY_OK)
		return ret;


	inOutString+=m_feature;
	inOutString+=m_expireDate;
	inOutString+=m_serialNumber;
	inOutString+=m_Checksum;	

	if(inOutString.length()!= FEATURE_ITEM_SERIAL_MAX +
							FEATURE_ITEM_FEATURE_MAX +
							FEATURE_ITEM_EXPIRE_MAX +
							FEATURE_ITEM_CHECKSUM_MAX )
	{
		return FEATURE_KEY_STRING_TOO_LONG;
	}

	//Add Dashes ie FFFD-DDDD-DSSS-SSCC
	for(size_t index = FEATURE_ITEM_DIST_BETWEEN_DASH;index < inOutString.length();index+=FEATURE_ITEM_DIST_BETWEEN_DASH + 1)
	{
		inOutString.insert(index,"-");
	}
	
	
	//CSimpleCrypto::encrypt(newstring,CHARS_TO_USE);
	
	return FEATURE_KEY_OK;
}

/**
* FUNCTION getDecodeString
*
* @brief Pass in encoded String and it will decode it and add it to the class 
*
* @author DAVID.LIBBY
* @date 12/8/2014 5:03:29 PM
*
* @param instr 16 char string with 3 dashes FFFD-DDDD-DSSS-SSCC
*
* @return FeatuerKeyStatus FEATURE_KEY_OK if things worked otherwise returns an error.
*/
FeatuerKeyStatus FeatureKeyItem::DecodeString(std::string instr)
{

	//CSimpleCrypto::decrypt(instr,CHARS_TO_USE);

	//First Sanity check
	int tt=instr.length();
	if(instr.length() != FEATURE_ITEM_SERIAL_MAX +
		FEATURE_ITEM_FEATURE_MAX +
		FEATURE_ITEM_EXPIRE_MAX +
		FEATURE_ITEM_CHECKSUM_MAX + FEATURE_ITEM_DASH_COUNT /* for the dashes */ )
	{
		return FEATURE_KEY_STRING_TOO_LONG;
	}

	//! remove commas
	std::size_t found=instr.find('-');
	int commaCount =0;
	while (found !=  std::string::npos)
	{
		instr.erase(found,1);
		commaCount++;
		found=instr.find('-');
	}

	//! did we remover FEATURE_ITEM_DASH_COUNT (3) commas?
	if(commaCount!=FEATURE_ITEM_DASH_COUNT)
		return FEATURE_KEY_BAD_DASH_COUNT;

	//Break down the remaining string.
	//FFFDDDDDDSSSSSCC
	std::string feat= instr.substr(0,FEATURE_ITEM_FEATURE_MAX);
	std::string expire= instr.substr(FEATURE_ITEM_FEATURE_MAX,FEATURE_ITEM_EXPIRE_MAX);
	std::string serial= instr.substr(FEATURE_ITEM_FEATURE_MAX + FEATURE_ITEM_EXPIRE_MAX,FEATURE_ITEM_SERIAL_MAX);
	std::string ck= instr.substr(FEATURE_ITEM_FEATURE_MAX + FEATURE_ITEM_EXPIRE_MAX + FEATURE_ITEM_SERIAL_MAX,FEATURE_ITEM_CHECKSUM_MAX);

	//Scope the test checksum 
	{
		FeatureKeyItem TestItem;
		TestItem.setSerial(serial);
		TestItem.setExpireDate(expire);
		TestItem.setFeature(feat);

		if(ck!=TestItem.getChecksum())
			return FEATURE_KEY_BAD_CHECKSUM;
	}

	//Add all the new Items..
	std::string newchk;
	FeatuerKeyStatus ret = this->setSerial(serial);
	if(ret!=FEATURE_KEY_OK)
		return ret;
	ret = this->setExpireDate(expire);
	if(ret!=FEATURE_KEY_OK)
		return ret;
	ret = this->setFeature(feat);
	if(ret!=FEATURE_KEY_OK)
		return ret;
	ret = this->calculateChecksum(newchk);
	if(ret!=FEATURE_KEY_OK)
		return ret;

	if(newchk.length()!=FEATURE_ITEM_CHECKSUM_MAX)
	{
		return FEATURE_KEY_BAD_CHECKSUM;
	}

	return FEATURE_KEY_OK;

}


/**
* FUNCTION addChecksum
*
* @brief Adds a checksum to the current object.
*
* @author DAVID.LIBBY
* @date 12/8/2014 3:26:13 PM
*
*
* @return FeatuerKeyStatus FEATURE_KEY_OK if things worked otherwise returns an error.
*/
FeatuerKeyStatus FeatureKeyItem::addChecksum()
{
	std::string newCheck ;
	FeatuerKeyStatus ret = calculateChecksum(newCheck);
	
	if(ret!=FEATURE_KEY_OK)
		return ret;

	if(newCheck.length()<1)
		return FEATURE_KEY_BAD_CHECKSUM;
	
	m_Checksum = newCheck;
	return FEATURE_KEY_OK;
}
