#pragma once
/////////////////////////////////////////////////////////////////////
// DbCore.h - Implements NoSql database prototype                  //
// ver 1.0                                                         //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2018       //
/////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides two classes:
* - DbCore implements core NoSql database operations, but does not
*   provide editing, querying, or persisting.  Those are left as
*   exercises for students.
* - DbElement provides the value part of our key-value database.
*   It contains fields for name, description, date, child collection
*   and a payload field of the template type.
* The package also provides functions for displaying:
* - set of all database keys
* - database elements
* - all records in the database

* Required Files:
* ---------------
* DbCore.h, DbCore.cpp
* DateTime.h, DateTime.cpp
* Utilities.h, Utilities.cpp
*
* Maintenance History:
* --------------------
* ver 1.1 : 19 Jan 2018
* - added code to throw exception in index operators if input key is not in database
* ver 1.0 : 10 Jan 2018
* - first release
*/

#include <unordered_map>
#include <string>
#include <vector>
#include <iostream>
#include <iomanip>
#include "../DateTime/DateTime.h"
#include <algorithm>

namespace NoSqlDb
{
	
	class Payload
	{
		
	public:
		using FilePath = std::string;
		using Category = std::vector<std::string>;
		Payload() {}
		Payload(FilePath file) { this->filePath_ = file; }
		Payload(Category cate) { this->category_ = cate; }
		Payload(FilePath file,Category cate)
		{
			this->category_ = cate;
			this->filePath_ = file;
		}
		FilePath& fpath() { return filePath_; }
		Category& category() { return category_; }
		void fpath(const FilePath& file) { filePath_ = file; }
		void category(const Category& c) { category_ = c; }
		void category(std::string& c) { category_.push_back(c); }
		bool comparePayload(const Payload&);

	private:
		FilePath filePath_;
		Category category_;
	};

	//Overloading to handle display of payload
	std::ostream& operator<<(std::ostream& os, Payload& p1)
	{
		os << p1.fpath() << "\t\t";
		for (auto cate : p1.category())
			os << cate << "\t";
		return os;
	}
	//Function to compare two payloads based on filepath and categories
	bool Payload::comparePayload(const Payload& p1)
	{
		if ((this->filePath_.compare(p1.filePath_) != 0) &&
			(this->category_.size() == p1.category_.size()) &&
			(std::equal(p1.category_.begin(), p1.category_.end(), this->category_.begin(), this->category_.end())))
			return true;
		else
			return false;
	}

	/////////////////////////////////////////////////////////////////////
	// DbElement class
	// - provides the value part of a NoSql key-value database
	template<typename T>
	class DbElement
	{
	public:
		using Key = std::string;
		using Children = std::vector<Key>;

		// methods to get and set DbElement fields

		std::string& name() { return name_; }
		std::string name() const { return name_; }
		void name(const std::string& name) { name_ = name; }
		DbElement<T>() {}
		std::string& descrip() { return descrip_; }
		std::string descrip() const { return descrip_; }
		void descrip(const std::string& name) { descrip_ = name; }

		DateTime& dateTime() { return dateTime_; }
		DateTime dateTime() const { return dateTime_; }
		void dateTime(const DateTime& dateTime) { dateTime_ = dateTime; }

		Children& children() { return children_; }
		Children children() const { return children_; }
		void children(const Children& children) { children_ = children; }

		T& payLoad() { return payLoad_; }
		T payLoad() const { return payLoad_; }
		void payLoad(const T& payLoad) { payLoad_ = payLoad; }

	private:
		std::string name_;
		std::string descrip_;
		DateTime dateTime_;
		Children children_;
		T payLoad_;
	};

	/////////////////////////////////////////////////////////////////////
	// DbCore class
	// - provides core NoSql db operations
	// - does not provide editing, querying, or persistance operations

	template <typename T>
	class DbCore
	{
	public:
		using Key = std::string;
		using Keys = std::vector<Key>;
		using Children = Keys;
		using DbStore = std::unordered_map<Key, DbElement<T>>;
		using iterator = typename DbStore::iterator;

		// methods to access database elements

		Keys keys();
		bool contains(const Key& key);
		size_t size();
		void throwOnIndexNotFound(bool doThrow) { doThrow_ = doThrow; }
		DbElement<T>& operator[](const Key& key);
		DbElement<T> operator[](const Key& key) const;
		typename iterator begin() { return dbStore_.begin(); }
		typename iterator end() { return dbStore_.end(); }
		bool addKeyValue(DbElement<T> dbElement, Key key);
		DbElement<T> editValues(DbElement<T>& dbElement, const Key& key);
		void removeKeyValue(Key key);
		void addChild(Key&, Key&);
		void removeChild(Key&, Key&);
		// methods to get and set the private database hash-map storage

		DbStore& dbStore() { return dbStore_; }
		DbStore dbStore() const { return dbStore_; }
		void dbStore(const DbStore& dbStore) { dbStore_ = dbStore; }
	private:
		DbStore dbStore_;
		bool doThrow_ = false;
	};

	//Function to add child to the element of a given key
	template<typename T>
	void DbCore<T>::addChild(Key& parentKey, Key& childKey)
	{
		if (!contains(parentKey))
			throw(std::exception("key does not exist in db"));
		DbElement<T>& dbElement = dbStore_[parentKey];
		dbElement.children().push_back(childKey);
	}

	//Function to remove child from the element of given key
	template<typename T>
	void DbCore<T>::removeChild(Key& parentKey, Key& childKey)
	{
		if (!contains(parentKey))
			throw(std::exception("key does not exist in db"));
		DbElement<T>& dbElement = dbStore_[parentKey];
		if (std::find(dbElement.children().begin(), dbElement.children().end(), childKey) != dbElement.children().end())
			dbElement.children().erase(std::remove(dbElement.children().begin(), dbElement.children().end(), childKey), dbElement.children().end());
		dbStore_[parentKey] = dbElement;
	}

	//Function to update values of given key
	template<typename T>
	DbElement<T> DbCore<T>::editValues(DbElement<T>& dbElement, const Key& key)
	{
		if (!contains(key))
			throw(std::exception("key does not exist in db"));
		else
		{
			DbElement<T> existingEntry = dbStore_[key];
			Payload payload = existingEntry.payLoad();
			if (!existingEntry.name()._Equal(dbElement.name()) && !dbElement.name().empty())
				existingEntry.name(dbElement.name());
			if (!existingEntry.descrip()._Equal(dbElement.descrip()) && !dbElement.name().empty())
				existingEntry.descrip(dbElement.descrip());
			if (existingEntry.children() == dbElement.children() && !dbElement.children().empty())
				existingEntry.children(dbElement.children());
			if(payload.category().size() != 0)
				existingEntry.payLoad(dbElement.payLoad());
			dbStore_[key] = existingEntry;
		}
		return dbStore_[key];
	}

	//Function to insert a key-value pair into the database
	template<typename T>
	bool DbCore<T>::addKeyValue(DbElement<T> element, Key key)
	{
		if (contains(key))
			return false;
		else
			dbStore_[key] = element;
		return true;
	}

	//Function to remove a key-value pair from the database
	template<typename T>
	void DbCore<T>::removeKeyValue(Key key)
	{
		if (contains(key))
			dbStore_.erase(key);
	}
	/////////////////////////////////////////////////////////////////////
	// DbCore<T> methods

	//----< does db contain this key? >----------------------------------

	template<typename T>
	bool DbCore<T>::contains(const Key& key)
	{
		iterator iter = dbStore_.find(key);
		return iter != dbStore_.end();
	}
	//----< returns current key set for db >-----------------------------

	template<typename T>
	typename DbCore<T>::Keys DbCore<T>::keys()
	{
		DbCore<T>::Keys dbKeys;
		DbStore& dbs = dbStore();
		size_t size = dbs.size();
		dbKeys.reserve(size);
		for (auto item : dbs)
		{
			dbKeys.push_back(item.first);
		}
		return dbKeys;
	}
	//----< return number of db elements >-------------------------------

	template<typename T>
	size_t DbCore<T>::size()
	{
		return dbStore_.size();
	}
	//----< extracts value from db with key >----------------------------
	/*
	*  - indexes non-const db objects
	*  - In order to create a key-value pair in the database like this:
	*      db[newKey] = newDbElement
	*    we need to index with the new key and assign a default element.
	*    That will be replaced by newDbElement when we return from operator[]
	*  - However, if we want to index without creating new elements, we need
	*    to throw an exception if the key does not exist in the database.
	*  - The behavior we get is determined by doThrow_.  If false we create
	*    a new element, if true, we throw. Creating new elements is the default
	*    behavior.
	*/
	template<typename T>
	DbElement<T>& DbCore<T>::operator[](const Key& key)
	{
		if (!contains(key))
		{
			if (doThrow_)
				throw(std::exception("key does not exist in db"));
			else
				return (dbStore_[key] = DbElement<T>());
		}
		return dbStore_[key];
	}
	//----< extracts value from db with key >----------------------------
	/*
	*  - indexes const db objects
	*/
	template<typename T>
	DbElement<T> DbCore<T>::operator[](const Key& key) const
	{
		if (!contains(key))
		{
			throw(std::exception("key does not exist in db"));
		}
		return dbStore_[key];
	}

	/////////////////////////////////////////////////////////////////////
	// display functions

	//----< display database key set >-----------------------------------

	template<typename T>
	void showKeys(DbCore<T>& db, std::ostream& out = std::cout)
	{
		out << "\n  ";
		for (auto key : db.keys())
		{
			out << key << " ";
		}
	}
	//----< show record header items >-----------------------------------

	inline void showHeader(std::ostream& out = std::cout)
	{
		out << "\n  ";
		out << std::setw(26) << std::left << "DateTime";
		out << std::setw(10) << std::left << "Name";
		out << std::setw(25) << std::left << "Description";
		out << std::setw(25) << std::left << "Payload";
		out << "\n  ";
		out << std::setw(26) << std::left << "------------------------";
		out << std::setw(10) << std::left << "--------";
		out << std::setw(25) << std::left << "-----------------------";
		out << std::setw(25) << std::left << "-----------------------";
	}
	//----< display DbElements >-----------------------------------------

	template<typename T>
	void showElem(const DbElement<T>& el, std::ostream& out = std::cout)
	{
		Payload payload = el.payLoad();
		Payload::Category categories = payload.category();
		out << "\n  ";
		out << std::setw(26) << std::left << std::string(el.dateTime());
		out << std::setw(10) << std::left << el.name();
		out << std::setw(25) << std::left << el.descrip();	
		out << std::setw(25) << std::left << payload.fpath();
		if (categories.size() > 0)
		{
			out << "\n Payload Categories : ";
			for (auto category : categories)
			{
				out << " \t " << category;
			}
		}
		typename DbElement<T>::Children children = el.children();
		if (children.size() > 0)
		{
			out << "\n    child keys: ";
			for (auto key : children)
			{
				out << " " << key;
			}
		}
	}
	//----< display all records in database >----------------------------

	template<typename T>
	void showDb(const DbCore<T>& db, std::ostream& out = std::cout)
	{
		showHeader(out);
		typename DbCore<T>::DbStore dbs = db.dbStore();
		for (auto item : dbs)
		{
			showElem(item.second, out);
			std::cout << "\n";
		}
	}
}
