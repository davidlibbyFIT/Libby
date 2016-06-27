// LittleClass.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <string>
struct Record
{
	std::string Name;
	std::string Phone;
	Record::Record()
	{
		Name="N\A";
		Phone="N\A";
	}
};


class BaseDbClass
{
	public:	
	BaseDbClass()
	{

	};
	~BaseDbClass()
	{
	};
	virtual Record ReadRecord()=0;
	private:

};

class Textclass : public BaseDbClass
{
public:
	Record ReadRecord()
	{
		Record myRecord;
		int doSomeTestStuff=0;
		myRecord.Name="David Libby";
		myRecord.Phone="555-1313";
		return myRecord;
	}
};

class Xmlclass : public BaseDbClass
{
public:
	Record ReadRecord()
	{
		Record myRecord;
		int doSomeTestStuff=0;
		myRecord.Name="XML David Libby";
		myRecord.Phone="555-1313XML";
		return myRecord;
	}
};


int _tmain(int argc, _TCHAR* argv[])
{
	BaseDbClass * myDb=NULL;

	int classType=1;


	switch (classType)
	{
		case 0:
			myDb = new Textclass;
		break;
		case 1:
			myDb = new Xmlclass;
			break;
	}

	if(myDb)	
	{
		Record result= myDb->ReadRecord();
		delete myDb;
	}
	

	return 0;
}

