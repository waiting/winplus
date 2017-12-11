//////////////////////////////////////////////////////////////////////////
// Purpose:   测试拷贝构造函数，赋值调用机制
// Desc:      
// Author:    WT
// Date:      2013-09-16
//////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
using namespace std;

class Tester
{
public:
	Tester()
	{
		cout << "default construct\n";
	}
	Tester( Tester const & other )
	{
		cout << "Tester() copy construct\n";
	}
	Tester & operator = ( Tester const & other )
	{
		cout << "operator = () assign operator\n";
		return *this;
	}
	void doSomeWork(){}
};

Tester GetTester1()
{
	return Tester();
}

Tester GetTester2()
{
	Tester t;
	t.doSomeWork();
	return t;
}

int main(int argc, char* argv[])
{
	{
		Tester t = Tester();
		Tester t2 = t;
		Tester t3( Tester() );
		Tester t4(t);
		t = t4;
		Tester t5 = GetTester1();
		Tester t6;
		t6 = GetTester1();
		Tester t7 = GetTester2();
		Tester t8;
		t8 = GetTester2();
	}
	//string s( string() );
	//s = "";
	//cout << s;
	return 0;
}
