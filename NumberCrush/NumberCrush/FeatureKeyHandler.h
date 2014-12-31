#ifndef FEAT_KEY_HANDLER_H
#define FEAT_KEY_HANDLER_H

#include <string>
#include <vector>
#include "SimpleCrypto.h"

typedef enum FeatuerKeyStatus 
{
	FEATURE_KEY_OK
	,FEATURE_KEY_BAD_SERIAL
	,FEATURE_KEY_BAD_FEATURE
	,FEATURE_KEY_BAD_DATE
	,FEATURE_KEY_BAD_CHECKSUM
	,FEATURE_KEY_IO_FAILURE
	,FEATURE_KEY_STRING_TOO_SHORT
	,FEATURE_KEY_STRING_TOO_LONG
	,FEATURE_KEY_BAD_DASH_COUNT
	,FEATURE_KEY_EXPIRED
};

#define FEATURE_ITEM_SERIAL_MAX 5
#define FEATURE_ITEM_FEATURE_MAX 3
#define FEATURE_ITEM_EXPIRE_MAX 6
#define FEATURE_ITEM_CHECKSUM_MAX 2
#define FEATURE_ITEM_DASH_COUNT 3
#define FEATURE_ITEM_DIST_BETWEEN_DASH 4

//Foward declare the class
class FeatureKeyItem;

/*! 
	@class FeatureKeyHandler
    @brief A class for handling the user Key codes for visual spreadsheet. 

    THis will provide io etc for the key class
*/

class FeatureKeyHandler
{
	typedef std::vector<FeatureKeyItem> KeyStorageVectorType; //this is fine	

public:
	FeatureKeyHandler();
	~FeatureKeyHandler(void);
	FeatuerKeyStatus CheckExpireTime(FeatureKeyItem &KeyItem);
public:
	//! Sets the key directory for use.
	FeatuerKeyStatus addKeyItem(FeatureKeyItem &NewKeyItem);
private:
	
	KeyStorageVectorType m_keyStorageVec;

};


/** @class FeatureKeyStructure the length of all these should not exceed 16 chars;
 *  @brief This structure blah blah blah...
 *  @var type This is used to hold the type of Key in use. it is only 3 chars long 
 *  @var expireDate is when the item should be invalidated. if is set in mmddyy so for 0ctober 1 2014 it would be expireDate="100114";
 *  @var setialNumber contains the device serial number or the case of sattelight the last 5 of the mac address.  setialNumber = "01000"
 *  @var Checksum checksum number 10
 */
class FeatureKeyItem  
{
public:
	FeatureKeyItem(void);
	~FeatureKeyItem(void);
	

	//! Sets the serial value
	FeatuerKeyStatus setSerial(std::string newValue);
	//! Sets the Feature code
	FeatuerKeyStatus setFeature(std::string newValue);
	//! sets the expire date.
	FeatuerKeyStatus setExpireDate(std::string newValue);

	//! Gets the Setial number
	std::string getSerial();
	//! Gets the Geature Code
	std::string getFeature();
	//! Gets the expire date
	std::string getExpireDate();
	//! Gets the Checksum
	std::string getChecksum();
	//! Returns formatedString;
	FeatuerKeyStatus getEncodeString(std::string &inOutString);
	//! DeCode the current string.
	FeatuerKeyStatus DecodeString(std::string instr);
	FeatuerKeyStatus vaildateFeatureKey();


private:

	//! CHecks to insure all components of the item are valid.
	FeatuerKeyStatus vaildatePreChecksumString();	
	
	//! Gets the PreCheck String
	FeatuerKeyStatus getPreChecksumString(std::string &inOutString);	
	//! Calculates the Checksum
	FeatuerKeyStatus calculateChecksum(std::string &inOutString);
	//! Addes the checksum to the end of the string.
	FeatuerKeyStatus addChecksum();

	//! Feature code 3 chars long
	std::string m_feature;
	//! Expire Date in the format mmddyy 112114 6 chars long
	std::string m_expireDate;
	//Serial number of machine 5 chars long
	std::string m_serialNumber;
	//! Checksum 2 chars long in hex
	std::string m_Checksum;


	
};




#endif // FEAT_KEY_HANDLER_H