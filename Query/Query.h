////////////////////////////////////////////////////////////////////////
//Query.h														      //	
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
* Defines functions for querying the database
*
* Maintenance:
* ------------
* Required packages: DbCore
*
* Maintenance History:
* --------------------
* ver 1.0
** - first release
*/
#pragma once
#include <iostream>
#include <string>
#include <regex>
#include "..\DbCore\DbCore.h"
#include "..\DateTime\DateTime.h"
#include <algorithm>
#include <vector>
#include <iomanip>


//Conditions class represents the condition used to query the database. Contains parameters to query based on metadata
//values - name,DateTime,description and payloadCategories 
class conditions
{
private:
	NoSqlDb::Payload::Category payLoadCategories;
	std::regex reName;
	std::regex reDesc;
	std::regex payLoad;
	DateTime fromDate_;
	DateTime toDate_;
	
public:
	conditions()
	{
		toDate_ = DateTime(DateTime().now());
		fromDate_ = DateTime(DateTime().now()) - DateTime().makeDuration(96, 0);
	}
	// Functions to get and set the paramaters of the condition class
	conditions& name(std::string);
	conditions& description(std::string);
	conditions& payload(std::string);
	conditions& fromDate(std::string);
	conditions& toDate(std::string);
	std::regex& name() { return reName; }
	std::regex& description() { return reDesc; }
	std::vector<std::string>& payload() { return payLoadCategories; }
	DateTime& toDate() { return toDate_; }
	DateTime& fromDate() { return fromDate_; }

};

conditions& conditions::fromDate(std::string fromDate)
{
	if (fromDate.empty())
		this->fromDate_ = DateTime(DateTime().now()) - DateTime().makeDuration(96, 0);
	else
		this->fromDate_ = DateTime(fromDate);
	return *this;
}

conditions& conditions::toDate(std::string toDate)
{
	if (toDate.empty())
		this->toDate_ = DateTime(DateTime().now());
	else
		this->toDate_ = toDate;
	return *this;
}

conditions& conditions::name(std::string name)
{
	reName = name;
	return *this;
}

conditions& conditions::description(std::string description)
{
	this->reDesc = description;
	return *this;
}

conditions& conditions::payload(std::string payload_)
{
	this->payLoadCategories.push_back(payload_);
	return *this;
}

conditions& helperForNullCheck(conditions& condition)
{
	if (condition.name()._Empty())
		condition = condition.name(".*");
	if (condition.description()._Empty())
		condition = condition.description(".*");
	return condition;
}

//Query class: Used to query the database based on conditions provided by the user
template<typename T>
class Query
{
public:
	using Key = std::string;
	using Keys = std::vector<Key>;
	using Children = Keys;
	Query() {}
	Query& setDb(NoSqlDb::DbCore<T> db);
	Children retrieveChildren(Key key);
	Query& select(conditions conditions);
	Query& select(Key key);
	Query& from(Keys& keys, conditions& conditions);
	Query& or_(conditions& conditions);
	void show();
	Keys& keys() { return this->keys_; }
	bool vectorMatchHelper(NoSqlDb::Payload::Category&,NoSqlDb::Payload::Category&);


	NoSqlDb::DbCore<T> db_;
	Keys keys_;
};

//Helper function for finding elements of one vector in another
template<typename T>
bool Query<T>::vectorMatchHelper(NoSqlDb::Payload::Category& c1,NoSqlDb::Payload::Category& c2)
{
	int count = 0;
	for (auto iter : c1 )
	{
		if (std::find(c2.begin(), c2.end(), iter) != c2.end())
		{
			count++;
		}
	}
	if (c1.size() == count)
		return true;
	else
		return false;
}

//Function to initialize the database to be queried 
template<typename T>
Query<T>& Query<T>::setDb(NoSqlDb::DbCore<T> db)
{
	db_ = db;
	return *this;
}

//Function to retrieve children of a particular key
template<typename T>
typename Query<T>::Children Query<T>::retrieveChildren(Key key)
{
	Children childKeys;
	if (db_.contains(key))
		childKeys = db_[key].children();
	return childKeys;
}

//Retrieve the value of a particualar key
template<typename T>
Query<T>& Query<T>::select(Key key)
{
	if (db_.contains(key)) 
	{
		if(std::find(this->keys_.begin(),this->keys_.end(),key)==this->keys_.end())
			this->keys_.push_back(key);
	}
		
	return *this;
}

//Retrieve keys whose values match a specified condition
template<typename T>
Query<T>& Query<T>::select(conditions conditions)
{
	conditions = helperForNullCheck(conditions);
	for (auto iterator : db_.dbStore())
	{
		if (conditions.payload().size() == 0) 
		{
			if (std::regex_search(iterator.second.name(), conditions.name()) &&
				std::regex_search(iterator.second.descrip(), conditions.description()) &&
				(iterator.second.dateTime() <= conditions.toDate()) &&
				(iterator.second.dateTime() > conditions.fromDate()))
					keys_.push_back(iterator.first);
		}
		else
		{
			if (std::regex_search(iterator.second.name(), conditions.name()) &&
				std::regex_search(iterator.second.descrip(), conditions.description()) &&
				(iterator.second.dateTime() <= conditions.toDate()) &&
				(iterator.second.dateTime() > conditions.fromDate()))
			{
				if (vectorMatchHelper(conditions.payload(), iterator.second.payLoad().category()))
					keys_.push_back(iterator.first);
			}
		}		
	}
	return *this;
}

//Used to find keys that match union of conditions
template<typename T>
Query<T>& Query<T>::or_(conditions& conditions)
{
	Keys keys = this->keys_;
	conditions = helperForNullCheck(conditions);
	Query query = select(conditions);
	for (auto iterator : query.keys())
	{
			if (!(std::find(keys.begin(), keys.end(), iterator) != keys.end()))
			{	
				keys.push_back(iterator);
			}
		
	}
	this->keys_.clear();
	this->keys_ = keys;
	return *this;
}

//Used to display the values of keys
template<typename T>
void Query<T>::show()
{
	Keys selectedKeys = this->keys_;
	showHeader();
	for (auto key : selectedKeys)
	{
		DbElement<T> elem = db_[key];
		showElem(elem);
	}
}

//Used to retrieve keys whose values match intersection of conditions
template<typename T>
Query<T>& Query<T>::from(Keys& key, conditions& conditions)
{
	Keys commonKeys;
	Query& query = select(conditions);
	for (auto k : query.keys_)
	{
		if (std::find(key.begin(), key.end(), k) != key.end())
			commonKeys.push_back(k);
	}
	this->keys_.clear();
	this->keys_ = commonKeys;
	return *this;
}

