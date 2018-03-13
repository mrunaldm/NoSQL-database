////////////////////////////////////////////////////////////////////////
// TestCore.h														  //	
//Ver 1.0                                                             //
// Application: Demonstration for CSE687 OOD Project 1                //
// Language:    C++ Visual Studio 2015                                //
// Platform:    Linux, Windows 10(dual boot)					      //
// Source:      Jim Fawcett, CST 4-187, Syracuse University           //
//              (315) 443-3948, jfawcett@twcny.rr.com                 //
// Author:      Mrunal Maniar, Syracuse University                    //
//              mdharmen@syr.edu                                      //
///////////////////////////////////////////////////////////////////////

/*
* Package Operations:
* -------------------
* Test executive demonstrates all the functional requirements
*
* Maintenance:
* ------------
* Required packages: NoSqlDb,Query,XmlDocument,Persistance
*
* Maintenance History:
* --------------------
* ver 1.0
** - first release
*/
#pragma once
#include "../DbCore/DbCore.h"
#include "../DateTime/DateTime.h"
#include "../Query/Query.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"
#include "../Persistance/XmlWrapper.h"

using namespace NoSqlDb;
auto putLine = [](size_t n = 1, std::ostream& out = std::cout)
{
	Utilities::putline(n, out);
};


class DbProvider
{
public:
	DbCore<NoSqlDb::Payload>& db() { return db_; }
private:
	static DbCore<NoSqlDb::Payload> db_;
};
DbCore<NoSqlDb::Payload> DbProvider::db_;

void addElements()
{
	DbProvider dbp;
	DbCore<NoSqlDb::Payload> db = dbp.db();
	std::cout << " \n Adding few more elements \n\n";
	DbElement<NoSqlDb::Payload> demoElem;
	demoElem.name("Jianan");
	demoElem.descrip("TA for Jim Fawcett");
	demoElem.dateTime(DateTime().now());
	demoElem.payLoad(Payload("Name : Jianan Sun"));
	db["Sun"] = demoElem;
	demoElem.payLoad(Payload("Name : Nikhil Prashar"));
	demoElem.name("Nikhil");
	demoElem.dateTime(DateTime().now());
	demoElem.descrip("TA for Jim Fawcett");
	db["Prashar"] = demoElem;
	demoElem.payLoad(Payload("Name : Pranjul Arora"));
	demoElem.name("Pranjul");
	demoElem.dateTime(DateTime().now());
	demoElem.descrip("TA for Jim Fawcett");
	db["Arora"] = demoElem;
	dbp.db() = db;
}
bool requirement1()
{
	Utilities::title("Demonstrating Requirement #1");
	std::cout << "\n  " << typeid(std::function<bool()>).name()
		<< ", declared in this function, "
		<< "\n  is only valid for C++11 and later versions.";
	putLine();
	return true; // would not compile unless C++11
}

bool requirement2()
{
	Utilities::title("Demonstrating Requirement #2");
	std::cout << "\n  A visual examination of all the submitted code "
		<< "will show only\n  use of streams and operators new and delete.";
	putLine();
	return true;
}

bool test3a()
{
	Utilities::title("Demonstrating Requirement #3a - creating DbElement");
	std::cout << "\n  Creating a db element with key \"Fawcett\":";
	DbCore<NoSqlDb::Payload> db;
	DbProvider dbp;
	dbp.db() = db;
	DbElement<NoSqlDb::Payload> demoElem;
	demoElem.name("Jim");
	demoElem.descrip("Instructor for CSE687");
	demoElem.dateTime(DateTime().now());
	demoElem.payLoad(NoSqlDb::Payload("The good news is ..."));
	if ((demoElem.name() != "Jim") && (demoElem.descrip() != "Instructor for CSE687") && 
		(demoElem.dateTime().now() != DateTime().now()) && (demoElem.payLoad().comparePayload(Payload("The good news is ..."))))
		return false;
	db["Fawcett"] = demoElem;
	showDb(db);
	putLine();
	return true;
}

bool test3b()
{
	Utilities::title("Demonstrating Requirement #3b - creating DbCore");
	DbProvider dbp;
	DbCore<NoSqlDb::Payload> db;
	dbp.db() = db;
	DbElement<NoSqlDb::Payload> demoElement;
	demoElement.name("Jim");
	demoElement.descrip("Instructor at SU");
	demoElement.payLoad(Payload("James Fawcett"));
	demoElement.dateTime(DateTime().now());
	demoElement.payLoad().category().push_back("OOD");
	demoElement.payLoad().category().push_back("Design Patterns");
	demoElement.payLoad().category().push_back("SMA");
	db["Fawcett"] = demoElement;
	DbElement<NoSqlDb::Payload> element;
	element.name("Ammar");
	element.descrip("TA for Jim Fawcett");
	element.dateTime(DateTime().now());
	element.payLoad(Payload("Ammar Salman"));
	element.payLoad().category().push_back("OOD");
	demoElement.payLoad().category().push_back("SMA");
	demoElement.payLoad().category().push_back("Distributed Objects");
	db["Salman"] = element;
	showDb(db);
	dbp.db() = db;
	if (db.size() != 2)
		return false;
	else
		return true;
}

bool test4a()
{
	Utilities::title("Demonstrating requirement 4a - Adding key-value pair");
	DbProvider dbp;
	DbCore<NoSqlDb::Payload> db = dbp.db();
	DbElement<NoSqlDb::Payload> element;
	element.name("Mrunal");
	element.descrip("Student of CSE687");
	element.payLoad(Payload("Mrunal Maniar"));
	element.dateTime(DateTime().now());
	element.payLoad().category().push_back("OOD");
	element.payLoad().category().push_back("SPFM");
	element.payLoad().category().push_back("SMDM");
	DbCore<NoSqlDb::Payload>::Key key = "Dharmendra";
	db.addKeyValue(element, key);
	
	dbp.db() = db;
	addElements();
	db = dbp.db();
	showDb(db);
	putLine();
	if ((db[key].name() != element.name()) && (db[key].descrip() != element.descrip()) && (db[key].payLoad().comparePayload(element.payLoad())))
		return false;
	else
		return true;
}

bool test4b()
{
	Utilities::title("Demonstrating requirement 4b - Removing key-value pair");
	DbProvider dbp;
	DbCore<NoSqlDb::Payload> db = dbp.db();
	DbCore<NoSqlDb::Payload>::Key key = "Dharmendra";
	showDb(db);
	std::cout << "\n\n After removing the key " << key << std::endl;
	db.removeKeyValue(key);
	showDb(db);
	dbp.db() = db;
	putLine();
	if (db.contains(key))
		return false;
	else
		return true;
}

bool test5a()
{
	Utilities::title("#Requirement 5a - Demonstrating editing text metadata");
	DbProvider dbp;
	DbCore<NoSqlDb::Payload> db = dbp.db();
	DbElement<NoSqlDb::Payload> element;
	element.name("James");
	element.descrip("Professor at EECS");
	element.payLoad(Payload("Jim Fawcett"));
	element.payLoad().category().push_back("Class on Tue/Thu");
	element.payLoad().category().push_back("Help session on Friday");
	element.payLoad().category().push_back("OOD");
	DbCore<NoSqlDb::Payload>::Key key = "Fawcett";
	std::cout << "\n Old values \n";
	showDb(db);
	std::cout << "\n Updated values \n";
	db.editValues(element, key);
	showDb(db);
	dbp.db() = db;
	putLine();
	if ((db[key].name() != element.name()) && (db[key].descrip() != element.descrip()) && (db[key].payLoad().comparePayload(element.payLoad())))
		return false;
	else
		return true;
}

bool test5b()
{
	Utilities::title("Requirement 5b - Adding and removing child keys");
	std::cout << "\n Adding child to the key Fawcett";
	DbCore<NoSqlDb::Payload>::Key childKey = "Dharmendra";
	DbCore<NoSqlDb::Payload>::Key child2 = "NY";
	DbCore<NoSqlDb::Payload>::Key key = "Fawcett";
	DbProvider dbp;
	DbCore<NoSqlDb::Payload> db = dbp.db();
	DbElement<NoSqlDb::Payload> el;
	DbElement<NoSqlDb::Payload> e2;
	e2.name("New York");
	e2.descrip("USA");
	e2.payLoad(Payload("State of New York"));
	e2.payLoad().category().push_back("NYC");
	e2.payLoad().category().push_back("Manhattan");
	el.name("Mrunal");
	el.descrip("Student of CSE687");
	el.payLoad(Payload("Requirement satisfied"));
	db.addKeyValue(el, childKey);
	db.addChild(key, childKey);
	db.addKeyValue(e2, child2);
	db.addChild(key, child2);
	showHeader();
	showElem(db[key]);
	if (!(std::find(db[key].children().begin(), db[key].children().end(), childKey) != db[key].children().end()))
		return false;
	std::cout << "\n\n Removing the added child with key "<<childKey<<std::endl;
	db.removeChild(key, childKey);
	showHeader();
	showElem(db[key]);
	if ((std::find(db[key].children().begin(), db[key].children().end(), childKey) != db[key].children().end()))
		return false;
	putLine();
	dbp.db() = db;
	return true;
}

bool test6a()
{
	Utilities::title("Requirement 6a - value of a specific key");
	Query<NoSqlDb::Payload> query;
	DbProvider dbp;
	DbCore<NoSqlDb::Payload> db = dbp.db();
	query.setDb(db);
	NoSqlDb::DbCore<NoSqlDb::Payload>::Key key = "Salman";
	showDb(db);
	std::cout << "\n Query for database with key :" << key << std::endl;
	if (query.select(key).keys().size() == 0)
		return false;
	else
		query.select(key).show();
	return true;
}

bool testLoadXML()
{
	Utilities::title("Loading project structure XML from database");
	DbProvider dbp;
	std::string fileName = "loadxmldatabase.xml";
	DbCore<NoSqlDb::Payload> db = dbp.db();
	XmlWrapper<NoSqlDb::Payload> xmlwrap(db.dbStore());
	xmlwrap.xmlLoadDatabase(fileName);
	db = xmlwrap.db();
	dbp.db() = db;
	return true;
}
bool test6b()
{
	Utilities::title("Query test - Requirement 6b - Metadata querying");
	DbProvider dbp;
	Query<NoSqlDb::Payload> query;
	query.setDb(dbp.db());
	std::cout << "\n Query for database with description : TA for Jim Fawcett " << std::endl;
	conditions check;
	check.description("TA for Jim Fawcett");
	query.select(check).show();
	return true;
}

bool test6c()
{
	Utilities::title("Query test - Requirement 6c - children of a specific key");
	DbProvider dbp;
	DbCore<NoSqlDb::Payload> db = dbp.db();
	Query<NoSqlDb::Payload> query;
	query.setDb(db);
	Query<NoSqlDb::Payload>::Key key = "Fawcett";
	std::cout << "\n Retrieve children of key :" << key << std::endl;
	Query<NoSqlDb::Payload>::Children children = query.retrieveChildren(key);
	for (auto child : children)
	{
		std::cout << child << std::endl;
	}
	if (children.size() == 0)
	{
		std::cout << "6c failed";
		return false;
		
	}
	else
		return true;
}

bool test6d()
{
	Utilities::title("Query test - Requirement 6d - query based on payload ");
	DbProvider dbp;
	DbCore<NoSqlDb::Payload> db = dbp.db();
	Query<NoSqlDb::Payload> query;
	query.setDb(db);
	conditions condition;
	std::cout << "\n Query datatbase for elements who have OOD as a category in payload" << std::endl;
	condition.payload().push_back("OOD");
	query.select(condition).show();
	if (query.keys().size()==0)
	{
		query.show();
		std::cout << "6d failed";
		return true;

	}
	else
		return true;
}

bool xmlAugmentation()
{
	Utilities::title("Demonstrating augmentation");
	DbProvider dbp;
	DbCore<NoSqlDb::Payload> db = dbp.db();
	XmlWrapper<NoSqlDb::Payload> xmlWrap(db.dbStore());
	DbElement<NoSqlDb::Payload> elem;
	elem.name("Requirement Augmentation");
	elem.descrip("Augmenting XML");
	elem.payLoad(Payload("new XML payload"));
	elem.payLoad().category().push_back("augment");
	NoSqlDb::DbCore<NoSqlDb::Payload>::Key key = "Augmentation";
	db.addKeyValue(elem, key);
	xmlWrap.db(db);
	std::string fileName = "data.xml";
	xmlWrap.xmlPersistDatabase(fileName);
	std::cout << " \n\n\n File named " << fileName << " created in debug package\n ";
	return true;
}

bool test6e()
{
	Utilities::title("Query test - Requirement 6g - Query based on dateTime ");
	DbProvider dbp;
	DbCore<NoSqlDb::Payload> db = dbp.db();
	Query<NoSqlDb::Payload> query;
	query.setDb(db);
	conditions condition;
	DateTime fromDate = DateTime(DateTime().now()) - DateTime().makeDuration(48, 0);
	condition.fromDate(fromDate).toDate(DateTime().now());
	std::cout << "\n Display elements created in last 48 hours";
	query.select(condition).show();
	if (query.keys().size() == 0)
	{
		std::cout << "6e failed";
		return false;

	}
	else
		return true;
}

bool test7a()
{
	Utilities::title("Query test - Requirement 7a - Anding Queries ");
	DbProvider dbp;
	DbCore<NoSqlDb::Payload> db = dbp.db();
	Query<NoSqlDb::Payload> query;
	query.setDb(db);
	Query<NoSqlDb::Payload> secondQuery;
	secondQuery.setDb(db);
	conditions condition1;
	std::cout << "\n \n Query for elements with description as 'TA for Jim Fawcett' and name as 'Pranjul' " << std::endl;
	condition1.description("TA for Jim Fawcett");
	conditions condition2;
	condition2.name("Pranjul");
	query.from(secondQuery.select(condition1).keys(), condition2).show();
	if (query.keys().size() == 0)
		return false;
	else
		return true;
}

bool test7b()
{
	Utilities::title("Query test - Requirement 7b - 'Or' - ing Queries ");
	DbProvider dbp;
	DbCore<NoSqlDb::Payload> db = dbp.db();
	Query<NoSqlDb::Payload> query;
	query.setDb(db);
	conditions condition;
	conditions condition2;
	conditions condition3;
	std::cout << "\n \n Query for values with name as 'Ammar' or 'Pranjul' or with description as 'Professor at EECS' " << std::endl;
	condition3.description("Professor at EECS");
	condition2.name("Ammar");
	condition.name("Pranjul");
	query.select(condition).or_(condition2).or_(condition3).show();
	if (query.keys().size() == 0)
		return false;
	else
		return true;
}

bool requirement3()
{
	if (test3a() && test3b())
		return true;
	else
		return false;
}

bool requirement4()
{
	if (test4a() && test4b())
		return true;
	else
		return false;
}

bool requirement5()
{
	if (test5a() && test5b())
		return true;
	else
		return false;
}

bool persistDatabase()
{
	Utilities::title("Persisting database to xml file");
	DbProvider dbp;
	std::string fileName = "database.xml";
	DbCore<NoSqlDb::Payload> db = dbp.db();
	XmlWrapper<NoSqlDb::Payload> xmlWrap(db.dbStore());
	xmlWrap.xmlPersistDatabase(fileName);
	std::cout << "\n\n\n File named " << fileName << " created in Debug folder";
	return true;
}

bool requirement6()
{
	if (test6a() && test6b() && test6c() && test6d() && test6e())
		return true;
	else
		return false;
}


bool requirement7()
{
	if (test7a() && test7b())
		return true;
	else
		return false;
}

bool TestStub1()
{
	TestExecutive ex;
	TestExecutive::TestStr testR1{ requirement1, "Use C++11" };
	TestExecutive::TestStr testR2{ requirement2,"Use streams and new and delete" };
	TestExecutive::TestStr testR3{ requirement3,"Creation of db elements and db core" };
	ex.registerTest(testR1);
	ex.registerTest(testR2);
	ex.registerTest(testR3);
	bool result = ex.doTests();
	return result;
}

bool TestStub2()
{
	TestExecutive ex;
	TestExecutive::TestStr testR4{ requirement4,"Addition and deletion of keys and values" };
	TestExecutive::TestStr testR5{ requirement5,"Updating data in database" };
	ex.registerTest(testR4);
	ex.registerTest(testR5);
	bool result = ex.doTests();
	return result;
}

bool TestStub3()
{
TestExecutive ex;
TestExecutive::TestStr testR6{requirement6 ,"Requirement 6 - Querying the database" };
TestExecutive::TestStr testR7{requirement7,"Requirement 7 - Handling complex queries" };
ex.registerTest(testR6);
ex.registerTest(testR7);
bool result = ex.doTests();
putLine(2);
return result;
}

bool TestStub4()
{
	TestExecutive ex;
	TestExecutive::TestStr testR8{ persistDatabase,"Saving database to XML" };
	TestExecutive::TestStr testR9{ testLoadXML,"Load database from XML" };
	TestExecutive::TestStr testR10{ xmlAugmentation,"Augmenting XML to a new file" };
	ex.registerTest(testR8);
	ex.registerTest(testR9);
	ex.registerTest(testR10);
	bool result = ex.doTests();
	putLine(2);
	return result;
}