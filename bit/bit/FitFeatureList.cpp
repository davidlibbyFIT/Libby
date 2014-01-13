#include "FitFeatureList.h"
#include <string.h>
#include "C:\work\dev-35-hsfc-libby\VS\src\Preconditions.h"

CFitFeatureList::CFitFeatureList(void)
{
	clearFeatures();
}

CFitFeatureList::~CFitFeatureList(void)
{
}

/**
* FUNCTION operator==*
* @brief 
*
* @version 1.0 
*
* @author David Libby
* @date 12/30/2013 4:52:54 PM
*
* @param rhs 
*
* @return bool 
*/
bool CFitFeatureList::operator==(const CFitFeatureList &rhs) const
{
	return (strcmp(m_features, rhs.m_features) == 0);
}

/**
* FUNCTION operator!=*
* @brief 
*
* @version 1.0 
*
* @author David Libby
* @date 12/30/2013 4:53:04 PM
*
* @param rhs 
*
* @return bool 
*/
bool CFitFeatureList::operator!=(const CFitFeatureList &rhs) const
{
	return  (strcmp(m_features, rhs.m_features) != 0);
}

/**
* FUNCTION fromSerialization*
* @brief given a string of features it will enable and disable them
*
* @version 1.0 
*
* @author David Libby
* @date 12/30/2013 4:53:14 PM
*
* @param str 
*
* @return int 
*/
int CFitFeatureList::fromSerialization(const char *str)
{
	clearFeatures();

	int count = 0;

	precondition_throw(str,"Invalid Feature in config file.");
	precondition_throw(strlen(str) <= FIT_FEATURE_MAX,"Invalid Feature in config file.");
	
	for (int i=0; i<FIT_FEATURE_MAX; i++)
	{
		if (str[i] == '\0')
			break;

		if (str[i] == '1')
		{
			count++;
			m_features[i] = '1';
		}else if (str[i] == '0')
		{
			//We really don't need to do this but I want to throw if it is not 1 or 0
			//m_features[i] = '0'
		}
		else
		{
			precondition_throw(0,"Invalid Feature in config file.");	
			return false;
		}
	}
	return(count);
}

/**
* FUNCTION addFeature*
* @brief Add single feature.
*
* @version 1.0 
*
* @author David Libby
* @date 12/30/2013 4:53:58 PM
*
* @param a_feature 
*/
void CFitFeatureList::addFeature(FitFeatures_e a_feature)		
{
	precondition_throw(VALID_FIT_FEATURE (a_feature),"Invalid Feature");	
	m_features[a_feature] = '1'; 
}

/**
* FUNCTION removeFeature*
* @brief Remove single feature
*
* @version 1.0 
*
* @author David Libby
* @date 12/30/2013 4:54:09 PM
*
* @param a_feature 
*/
void CFitFeatureList::removeFeature(FitFeatures_e a_feature)	
{ 

	precondition_throw(VALID_FIT_FEATURE (a_feature),"Invalid Feature");	

	m_features[a_feature] = '0'; 
}

/**
* FUNCTION featureIsEnabled*
* @brief Check to see if feature is enabled.
*
* @version 1.0 
*
* @author David Libby
* @date 12/30/2013 4:54:29 PM
*
* @param a_feature 
*
* @return bool 
*/
bool CFitFeatureList::featureIsEnabled(FitFeatures_e a_feature)	
{ 
	precondition_throw(VALID_FIT_FEATURE (a_feature),"Invalid Feature");	
	return (m_features[a_feature] == '1'); 
}

/**
* FUNCTION serialization*
* @brief Return feature string.
*
* @version 1.0 
*
* @author David Libby
* @date 12/30/2013 4:54:49 PM
*
*
* @return const char * 
*/
const char *CFitFeatureList::serialization()
{ 
	return m_features; 
}

/**
* FUNCTION clearFeatures*
* @brief Clear all features.
*
* @version 1.0 
*
* @author David Libby
* @date 12/30/2013 4:54:59 PM
*
*/
void CFitFeatureList::clearFeatures()
{
	memset(m_features,'0',sizeof(m_features));
	m_features[FIT_FEATURE_MAX]='\0';
}

