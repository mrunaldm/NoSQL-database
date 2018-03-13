////////////////////////////////////////////////////////////////////////
//XmlWrapper.cpp												      //	
//Ver 1.0                                                             //
// Application: Demonstration for CSE687 OOD Project 1                //
// Language:    C++													  //
// Platform:    Linux, Windows 10(dual boot)					      //
// Source:      Jim Fawcett, CST 4-187, Syracuse University           //
//              (315) 443-3948, jfawcett@twcny.rr.com                 //
// Author:      Mrunal Maniar, Syracuse University                    //
//              mdharmen@syr.edu                                      //
///////////////////////////////////////////////////////////////////////

/*
* Package Operations:
* -------------------
* Test stubs for xml operations
*
* Maintenance:
* ------------
* Required packages:  DbCore,XmlDocument
*
* Maintenance History:
* --------------------
* ver 1.0
** - first release
*/
#include "XmlWrapper.h"
#include "../DbCore/DbCore.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"
#include <iostream>
#include <iomanip>
#include <functional>

class DbProvider
{
public:
	NoSqlDb::DbCore<NoSqlDb::Payload>& db() { return db_; }
private:
	static NoSqlDb::DbCore<NoSqlDb::Payload> db_;
};

NoSqlDb::DbCore<NoSqlDb::Payload> DbProvider::db_;

bool TestPeristance() 
{
	DbProvider dbp;
	NoSqlDb::DbCore<NoSqlDb::Payload> db = dbp.db();

	NoSqlDb::DbElement<NoSqlDb::Payload> demoElem = db["Fawcett"];

	demoElem.name("Ammar");
	demoElem.descrip("TA for CSE687");
	demoElem.payLoad(NoSqlDb::Payload("You should try ..."));
	db["Salman"] = demoElem;
	if (!db.contains("Salman"))
		return false;

	demoElem.name("Jianan");
	demoElem.payLoad(NoSqlDb::Payload("Dr. Fawcett said ..."));
	db["Sun"] = demoElem;

	demoElem.payLoad(NoSqlDb::Payload("You didn't demonstrate Requirement #4"));
	demoElem.name("Nikhil");
	db["Prashar"] = demoElem;

	demoElem.payLoad(NoSqlDb::Payload("You didn't demonstrate Requirement #5"));
	demoElem.name("Pranjul");
	db["Arora"] = demoElem;

	demoElem.payLoad(NoSqlDb::Payload("You didn't demonstrate Requirement #6"));
	demoElem.name("Akash");
	db["Anjanappa"] = demoElem;
	XmlWrapper<NoSqlDb::Payload> XmlWrap(db.dbStore());
	std::string fileName = "test.xml";
	XmlWrap.xmlPersistDatabase(fileName);
	dbp.db() = db;
	return true;
}

bool testLoading()
{
	DbProvider dbp;
	NoSqlDb::DbCore<NoSqlDb::Payload> db = dbp.db();
	std::string fileName = "test.xml";
	XmlWrapper<NoSqlDb::Payload> xmlWrap(db.dbStore());
	xmlWrap.xmlLoadDatabase(fileName);
	NoSqlDb::showDb(xmlWrap.db());
	return true;
}

#ifdef TEST_XML
int main()
{
	Utilities::Title("Testing DbCore - He said, she said database");
	putLine();
	TestExecutive ex;
	// define test structures with test function and message
	TestExecutive::TestStr ts1{ TestPeristance , "Test persistance" };
	TestExecutive::TestStr ts2{ testLoading , "Restore database from xml" };
	ex.registerTest(ts1);
	ex.registerTest(ts2);
	bool result = ex.doTests();

	if (result == true)
		std::cout << "\n  all tests passed";
	else
		std::cout << "\n  at least one test failed";

	putLine(2);
	return 0;
}
#endif