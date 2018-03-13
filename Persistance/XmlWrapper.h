////////////////////////////////////////////////////////////////////////
// XmlWrapper.h													      //	
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
* Wrapper on top of XmlDocument
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
#pragma once
#include "../XmlDocument/XmlDocument/XmlDocument.h"
#include "../XmlDocument/XmlElement/XmlElement.h"
#include "../DateTime/DateTime.h"
#include "../DbCore/DbCore.h"
#include <unordered_map>
#include <memory>
#include <vector>
#include <fstream>
#include <sstream>

using namespace XmlProcessing;
template<typename T>
class XmlWrapper
{
	using Sptr = std::shared_ptr<AbstractXmlElement>;
	using Key = std::string;
	using File = std::fstream;
	using Buffer = std::stringstream;
	using XmlString = std::string;
	using DbStore = std::unordered_map<Key, NoSqlDb::DbElement<T>>;
	using Records = std::vector<Sptr>;
	
public:
	XmlWrapper(const DbStore& dbStore)
	{
		DbStore_ = dbStore;
	}
	
	NoSqlDb::DbCore<T> db() { return db_; }
	void db(NoSqlDb::DbCore<T> database) { DbStore_ = database.dbStore(); }
	void xmlLoadDatabase(std::string&);
	void xmlPersistDatabase(std::string&);
	
private:
	NoSqlDb::DbCore<T> db_;
	DbStore DbStore_;
};

template<typename T>
void XmlWrapper<T>::xmlLoadDatabase(std::string& fileName)
{
	NoSqlDb::DbCore<NoSqlDb::Payload> db;
	Key key;
	NoSqlDb::Payload payload_;
	File file;
	file.open(fileName);
	Buffer buffer;
	buffer << file.rdbuf();
	XmlString Xml = buffer.str();
	XmlDocument newXDoc(Xml, XmlDocument::str);
	Records records = newXDoc.descendents("dbRecord").select();
	for (auto pRecord : records)
	{
		NoSqlDb::DbElement<NoSqlDb::Payload> elem;
		Records pChildren = pRecord->children();
		for (auto pChild : pChildren)
		{
			if (pChild->tag() == "key")
			{
				key = pChild->children()[0]->value();
			}
			else
			{
				Records pValueChildren = pChild->children();
				for (auto pValueChild : pValueChildren)
				{
					if (pValueChild->tag() == "name")
					{
						elem.name(pValueChild->children()[0]->value());
					}
					if (pValueChild->tag() == "description")
					{
						elem.descrip(pValueChild->children()[0]->value());
					}
					if (pValueChild->tag() == "dateTime")
					{
						elem.dateTime(pValueChild->children()[0]->value());
					}
					if (pValueChild->tag() == "payload")
					{
						Records pValuePayload = pValueChild->children();
						for (auto payloadType : pValuePayload)
						{
							if (payloadType->tag() == "path")
							{
								payload_.fpath(payloadType->children()[0]->value());
							}
							if (payloadType->tag() == "category")
							{
								payload_.category().push_back(payloadType->children()[0]->value());
							}
						}
					}
					if (pValueChild->tag() == "children")
					{
						Records pValueChildren = pValueChild->children();
						for (auto child : pValueChildren)
						{
							if (child->tag() == "child")
							{
								elem.children().push_back(child->children()[0]->value());
							}
						}
					}

				}
			}
		}
		elem.payLoad(payload_);

		if (db_.dbStore().find(key) == db_.dbStore().end())
			db_[key] = elem;
		payload_.category().clear();
	}
	std::cout << "\n\n Db imported from file :\n\n\n";
	showDb(db_);
}


template<typename T>
void XmlWrapper<T>::xmlPersistDatabase(std::string& fileName)
{
	Sptr pDb = makeTaggedElement("db");
	Sptr pDocElem = makeDocElement(pDb);
	XmlDocument xDoc(pDocElem);
	for (auto item : DbStore_)
	{
		Sptr pRecord = makeTaggedElement("dbRecord");
		pDb->addChild(pRecord);
		Sptr pKey = makeTaggedElement("key", item.first);
		pRecord->addChild(pKey);
		Sptr pValue = makeTaggedElement("value");
		pRecord->addChild(pValue);
		Sptr pName = makeTaggedElement("name", item.second.name());
		pValue->addChild(pName);
		Sptr pDescrip = makeTaggedElement("description", item.second.descrip());
		pValue->addChild(pDescrip);
		Sptr pDateTime = makeTaggedElement("dateTime", item.second.dateTime());
		pValue->addChild(pDateTime);
		Sptr pPayload = makeTaggedElement("payload");
		pValue->addChild(pPayload);
		Sptr pPath = makeTaggedElement("path", item.second.payLoad().fpath());
		pPayload->addChild(pPath);

		if (item.second.payLoad().category().size() != 0)
		{
			for (auto category : item.second.payLoad().category())
			{
				Sptr pCategory = makeTaggedElement("category", category);
				pPayload->addChild(pCategory);
			}
		}
		if (item.second.children().size() != 0)
		{
			Sptr pChildren = makeTaggedElement("children");
			pValue->addChild(pChildren);
			for (auto child : item.second.children())
			{
				Sptr pChild = makeTaggedElement("child", child);
				pChildren->addChild(pChild);
			}
		}
	}
	XmlString Xml = xDoc.toString();
	XmlDocument newXDoc(Xml, XmlDocument::str);
	//std::cout << newXDoc.toString();
	std::ofstream out(fileName);
	out << newXDoc.toString();
	out.close();
}


