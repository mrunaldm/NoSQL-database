/////////////////////////////////////////////////////////////////////
// DbCore.cpp - Implements NoSql database prototype                //
// ver 1.0                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018       //
/////////////////////////////////////////////////////////////////////

#include "../DbCore/DbCore.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include "../Utilities/TestUtilities/TestUtilities.h"
#include <iostream>
#include <iomanip>
#include <functional>
#include "Query.h"

//----< reduce the number of characters to type >----------------------
using namespace NoSqlDb;
auto putLine = [](size_t n = 1, std::ostream& out = std::cout)
{
	Utilities::putline(n, out);
};

///////////////////////////////////////////////////////////////////////
// DbProvider class
// - provides mechanism to share a test database between test functions
//   without explicitly passing as a function argument.

class DbProvider
{
public:
	DbCore<NoSqlDb::Payload>& db() { return db_; }
private:
	static DbCore<NoSqlDb::Payload> db_;
};

DbCore<NoSqlDb::Payload> DbProvider::db_;


///////////////////////////////////////////////////////////////////////
// test functions

bool testR3a()
{
	Utilities::title("Demonstrating Requirement #3a - creating DbElement");
	std::cout << "\n  Creating a db element with key \"Fawcett\":";

	// create database to hold std::string payload

	NoSqlDb::DbCore<NoSqlDb::Payload> db;
	DbProvider dbp;
	dbp.db() = db;

	// create some demo elements and insert into db

	DbElement<NoSqlDb::Payload> demoElem;

	demoElem.name("Jim");
	demoElem.descrip("Instructor for CSE687");
	demoElem.dateTime(DateTime().now());
	demoElem.payLoad(NoSqlDb::Payload("The good news is ..."));

	if (demoElem.name() != "Jim")
		return false;
	if (demoElem.descrip() != "Instructor for CSE687")
		return false;
	if (demoElem.dateTime().now() != DateTime().now())
		return false;
	if (demoElem.payLoad().comparePayload(Payload("The good news is ...")))
		return false;

	showHeader();
	showElem(demoElem);

	db["Fawcett"] = demoElem;
	dbp.db() = db;
	putLine();
	return true;
}
//----< demo second part of requirement #3 >---------------------------

bool testR3b()
{
	Utilities::title("Demonstrating Requirement #3b - creating DbCore");

	DbProvider dbp;
	DbCore<NoSqlDb::Payload> db = dbp.db();

	DbElement<NoSqlDb::Payload> demoElem = db["Fawcett"];
	Payload pAmmar = Payload("You should try ...");
	pAmmar.category().push_back("TA");
	pAmmar.category().push_back("Friday help session");
	pAmmar.category().push_back("Phd candidate");
	demoElem.name("Ammar");
	demoElem.descrip("TA for CSE687");
	demoElem.payLoad(pAmmar);
	db["Salman"] = demoElem;
	if (!db.contains("Salman"))
		return false;

	demoElem.name("Jianan");
	demoElem.payLoad(Payload("Dr. Fawcett said ..."));
	db["Sun"] = demoElem;

	demoElem.payLoad(Payload("You didn't demonstrate Requirement #4"));
	demoElem.name("Nikhil");
	db["Prashar"] = demoElem;

	demoElem.payLoad(Payload("You didn't demonstrate Requirement #5"));
	demoElem.name("Pranjul");
	db["Arora"] = demoElem;

	demoElem.payLoad(Payload("You didn't demonstrate Requirement #6"));
	demoElem.name("Akash");
	db["Anjanappa"] = demoElem;

	if (db.size() != 6)
		return false;

	std::cout << "\n  after adding elements with keys: ";
	DbCore<NoSqlDb::Payload>::Keys keys = db.keys();
	showKeys(db);
	putLine();

	std::cout << "\n  make all the new elements children of element with key \"Fawcett\"";
	db["Fawcett"].children().push_back("Salman");
	db["Fawcett"].children().push_back("Sun");
	db["Fawcett"].children().push_back("Prashar");
	db["Fawcett"].children().push_back("Arora");
	db["Fawcett"].children().push_back("Anjanappa");

	showHeader();
	//showElem(db["Fawcett"]);

	db["Salman"].children().push_back("Sun");
	db["Salman"].children().push_back("Prashar");
	db["Salman"].children().push_back("Arora");
	db["Salman"].children().push_back("Anjanappa");

	// display the results
	dbp.db() = db;
	putLine();
	//std::cout << "\n  showing all the database elements:";
	showDb(db);
	putLine();
	return true;
}

bool test6a()
{
	Utilities::title("Query test - Requirement 6a");

	DbProvider dbp;
	DbCore<NoSqlDb::Payload> db = dbp.db();
	Query<NoSqlDb::Payload> query;
	query.setDb(db);
	NoSqlDb::DbCore<NoSqlDb::Payload>::Key key = "Fawcett";
	if (query.select(key).keys().size() == 0)
		return false;
	else
		query.select(key).show();
	return true;
}

bool test6b()
{
	Utilities::title("Query test - Requirement 6b - Metadata querying");
	DbProvider dbp;
	DbCore<NoSqlDb::Payload> db = dbp.db();
	Query<NoSqlDb::Payload> query;
	query.setDb(db);
	conditions condition;
	condition.description("TA for CSE687");
	query.select(condition).show();
	if (query.keys().size() == 0)
		return false;
	else
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
	Query<NoSqlDb::Payload>::Children children = query.retrieveChildren(key);
	if (children.size() == 0)
		return false;
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
	condition.payload().push_back("Friday help session");
	query.select(condition).show();
	if (query.keys().size() == 0)
		return false;
	else
		return true;
}

bool test6g()
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
		return false;
	else
		return true;
}

bool test6f()
{
	Utilities::title("Query test - Requirement 6f - /'Or/' - ing Queries ");
	DbProvider dbp;
	DbCore<NoSqlDb::Payload> db = dbp.db();
	Query<NoSqlDb::Payload> query;
	query.setDb(db);
	conditions condition;
	conditions condition2;
	conditions condition3;
	condition3.description("Instructor for CSE687");
	condition2.name("Ammar");
	condition.name("Pranjul");
	query.select(condition).or_(condition2).or_(condition3).show();
	if (query.keys().size() == 0)
		return false;
	else
		return true;
}

bool test6e()
{
	Utilities::title("Query test - Requirement 6d - Anding Queries ");
	DbProvider dbp;
	DbCore<NoSqlDb::Payload> db = dbp.db();
	Query<NoSqlDb::Payload> query;
	query.setDb(db);
	Query<NoSqlDb::Payload> secondQuery;
	secondQuery.setDb(db);
	conditions condition1;
	condition1.description("TA for CSE687");
	conditions condition2;
	condition2.name("Pranjul");
	query.from(secondQuery.select(condition1).keys(), condition2).show();
	if (query.keys().size() == 0)
		return false;
	else
		return true;
}

//----< test stub >----------------------------------------------------

#ifdef TEST_QUERY

using namespace Utilities;

int main()
{
	TestExecutive ex;
	// define test structures with test function and message
	TestExecutive::TestStr ts3a{ testR3a, "Create DbElement<std::string>" };
	TestExecutive::TestStr ts3b{ testR3b, "Create DbCore<std::string>" };
	TestExecutive::TestStr ts6a{ test6a ,"Query for a key" };
	TestExecutive::TestStr ts6b{ test6b,"Query based on metadata" };
	TestExecutive::TestStr ts6c{ test6c,"Retrieve children of a specified key" };
	TestExecutive::TestStr ts6d{ test6d,"Query based on payload" };
	TestExecutive::TestStr ts6e{ test6e,"Anding queries" };
	TestExecutive::TestStr ts6f{ test6f,"Or - ing queries" };
	TestExecutive::TestStr ts6g{ test6g,"QUery Elements in a time interval" };
	ex.registerTest(ts3a);
	ex.registerTest(ts3b);
	ex.registerTest(ts6a);
	ex.registerTest(ts6b);
	ex.registerTest(ts6c);
	ex.registerTest(ts6d);
	ex.registerTest(ts6e);
	ex.registerTest(ts6f);
	ex.registerTest(ts6g);
	bool result = ex.doTests();

	if (result == true)
		std::cout << "\n  all tests passed";
	else
		std::cout << "\n  at least one test failed";

	putLine(2);
	getchar();
	return 0;
}
#endif
