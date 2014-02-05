// bit.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cstdint>
#include <vector>
#include <string>
#include <sstream>

#include <iostream>
#include "FitFeatureList.h"
#include "Configuration.h"
#include "SimpleCrypto.h"



int ParseOtherDesc(char *buff,char *&othertype )
{
	char *ptr1 = NULL, *ptr2 = NULL;
	othertype = buff;
	othertype = strchr(buff, ':');
	if ( NULL != othertype ) {
		*othertype = '\0';
		othertype++;
		ptr1 = ptr2 = othertype;

		while (NULL != *ptr1){
			if (('\\' == *ptr1) && (':' == *(ptr1 + 1))){
				*ptr2++ = ';';
				ptr1 += 2;
			}
			else{
				*ptr2++ = *ptr1++;
			}
		}
		*ptr2 = *ptr1;// add terminating NULL
	}
	return (int) (ptr2 - othertype);				
};



int _tmain(int argc, _TCHAR* argv[])
{
	char othertype[3333];
	char *Desc = othertype;
	char buff[3333]="Other :222222";

	int aa = ParseOtherDesc(buff,Desc);

	std::stringstream ss("1dasdasdasdasdasdasdasdasd2");
	std::vector<std::string> output;
	std::string r;
	while(std::getline(ss,r,';'))
		output.push_back(r);


	std::string bubba="111111112222222233333333";
	
	CSimpleCrypto mycryt;
	mycryt.encrypt(bubba);
	mycryt.decrypt(bubba);


	encryptConfigString(bubba);
	decryptConfigString(bubba);

	//Configuration myconfig;
	//myconfig.loadFile("c:\\other\\FlowCAM.cfgbt");
	//myconfig.SaveFile("c:\\great.cfg");

	//Configuration myconfig2;
	//myconfig2.loadFile("c:\\other\\FlowCAM.cfg");
	//myconfig2.SaveFile("c:\\other\\Flow2CAM.cfg");

	//Configuration myconfig3;
	//myconfig2.loadFile("c:\\other\\Flow2CAM.cfg");
	
	{
		Configuration gg;

		gg.enableFeature(FIT_FEATURE_2_21CFR);
		gg.enableFeature(FIT_FEATURE_2_AUTOFOCUS);
		gg.enableFeature(FIT_FEATURE_2_TESTFEATURES);
		gg.disableFeature(FIT_FEATURE_2_21CFR);
		gg.SaveFile("c:\\try.cfg");
	}
	{
		Configuration hh;
		hh.loadFile("c:\\try.cfg");
	}


	{
		Configuration myconfig2;
		myconfig2.loadFile("c:\\other\\FlowCAM.cfg");	
	}






}

