#pragma once


//The following enumeration if in order, you can add new features
//Above the comment "Add new Features above this line" 
// Take care when adding and never remove.
typedef enum 
{
	FIT_FEATURE_SATELLITE_VERSION = 0,	
	FIT_FEATURE_21CFR,					
	FIT_FEATURE_TEST,					
	FIT_FEATURE_AUTO_FOCUS,				
	FIT_FEATURE_DSP,					
	FIT_FEATURE_ADVANCED_PUMP,				
	//Add new Features above this line
	FIT_FEATURE_MAX						
}FitFeatures_e;

#define VALID_FIT_FEATURE(feat) (feat>=FIT_FEATURE_SATELLITE_VERSION && feat<FIT_FEATURE_MAX)


class CFitFeatureList
{

public:
	CFitFeatureList(void);
	~CFitFeatureList(void);

	bool operator==(const CFitFeatureList &rhs) const;
	bool operator!=(const CFitFeatureList &rhs) const;

	//! add a feature to the list.
	void addFeature(FitFeatures_e a_feature);
	//! remove a feature form the list.
	void removeFeature(FitFeatures_e a_feature);
	//! check to see if a feature is enabled.
	bool featureIsEnabled(FitFeatures_e a_feature);
	//! get Feature Buffer
	const char *serialization();
	//! re populate from buffer.
	int fromSerialization(const char *str);
	//! Clear All features from list.
	void clearFeatures();

private:

	char m_features[FIT_FEATURE_MAX+1];

};

