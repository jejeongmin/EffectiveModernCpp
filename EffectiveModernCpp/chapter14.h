#pragma once

#include <functional>
#include <memory>
#include <iostream>
#include <string>

/*********************************************************
이 장의 주제와는 다르지만 c++ 에서 예외 및 스택 해제를 복습(?)하기 위한 코드이다.

예외를 던지지 않는다는 사실이 명백한 함수들은 - 내부에서 직간접적으로 호출하는 함수들도 예외를 던지지 않아야 한다
- noexcept 를 지정해 컴파일러가 최적화할 수 있도록 해야 한다.
성능 향상을 위해 무리하게 구현을 수정해서 noexcept 로 만들 필요는 없다. 어디까지나 자연스럽게...
**********************************************************/

using namespace std;

class chapter14 : public chapter
{

	int		mf1(int x)	throw();		// 예외를 던지지 않는다. c++98 방식.
	int		mf2(int x)	throw(...);		// 예외를 던진다.
	int		mf3(int x)	noexcept;		// 예외를 던지지 않는다. 컴파일러 최적화의 여지가 가장 크다.

	class MyException {};

	class Dummy
	{
	public:
				Dummy(string s) : MyName(s) { PrintMsg("Created Dummy:"); }
				Dummy(const Dummy& other) : MyName(other.MyName) { PrintMsg("Copy created Dummy:"); }
				~Dummy() { PrintMsg("Destroyed Dummy:"); }
		void	PrintMsg(string s) { cout << s << MyName << endl; }
		
	public:
		string	MyName;
		int		level;
	};

	void C(Dummy d, int i)
	{
		cout << "Entering FunctionC" << endl;
		d.MyName = " C";
		throw MyException();

		cout << "Exiting FunctionC" << endl;
	}

	void B(Dummy d, int i)
	{
		cout << "Entering FunctionB" << endl;
		d.MyName = "B";
		C(d, i + 1);
		cout << "Exiting FunctionB" << endl;
	}

	void A(Dummy d, int i)
	{
		cout << "Entering FunctionA" << endl;
		d.MyName = " A";
		//  Dummy* pd = new Dummy("new Dummy"); //Not exception safe!!!  
		B(d, i + 1);
		//   delete pd;   
		cout << "Exiting FunctionA" << endl;
	}

	virtual int chapterMain()
	{
		cout << "Entering main" << endl;
		try
		{
			Dummy d(" M");
			A(d, 1);
		}
		catch (MyException& e)
		{
			cout << "Caught an exception of type: " << typeid(e).name() << endl;
		}

		cout << "Exiting main." << endl;
		char c;
		cin >> c;

		return 0;
	}
};

/*
	noexcept 는 함수의 인터페이스의 일부이다. 이는 호출자가 noexcept 여부에 의존할 수 있음을 뜻한다.

	noexcept 함수는 비 noexcept 함수보다 최적화의 여지가 크다.

	noexcept 는 이동 연산들과 swap, 메모리 해제 함수들, 그리고 소멸자들에 특히나 유용하다.

	대부분의 함수는 noexcept 가 아니라 예외에 중립적이다.
*/