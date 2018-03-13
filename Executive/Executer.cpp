////////////////////////////////////////////////////////////////////////
// Executive.cpp												      //	
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
* demonstrates all the functional requirements
*
* Maintenance:
* ------------
* Required packages:  DbCore,Query,XmlDocument,Persistance,Test
*
* Maintenance History:
* --------------------
* ver 1.0
** - first release
*/
#include "../Test/TestCore.h"

int main() 
{
	if (TestStub1() && TestStub2() && TestStub3() && TestStub4())
		std::cout << "All test passed"<<std::endl;
	else
		std::cout << "One or more tests failed" << std::endl;
	getchar();
	return 0;
}