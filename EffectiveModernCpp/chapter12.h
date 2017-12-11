#pragma once

#include <functional>
#include <memory>

/*********************************************************
**********************************************************/

class chapter12 : public chapter
{
	class Widget
	{
		using DataType = std::vector<double>;

		DataType	_values;

	public:
		DataType&	doWork() &		// doWork 의 이 버전은 *this 가 왼값일 때에만 적용된다.
		{
			std::cout << "widget::dowork - left reference call" << std::endl;
			return _values;
		}

		DataType&&	doWork() &&		// doWork 의 이 버전은 *this 가 오른값일 때에만 적용된다.
		{
			std::cout << "widget::dowork - right reference call" << std::endl;
			return std::move(_values);
		}

	};

	Widget makeWidget()
	{
		return Widget();
	}

	class Base
	{
	public:
		virtual void mf1() const;
		virtual void mf2(int x);
		virtual void mf3() &;
		void mf4() const;
	};

	// 작성자의 의도는 mf1~mf4 에 이르는 모든 함수의 재정의였겠지만, 약간의 차이로
	// 재정의 된 함수는 하나도 없다. 심지어 컴파일러는 아무런 오류를 뱉어내지 않는다.
	// 이럴 때 override 를 끝에 붙여주면 재정되지 않은 함수들에 대한 컴파일 오류를 발생시킨다.
	// 이러한 방침은 기반 클래스의 한 가상 함수의 서명(선언)을 변경했을 때 그 영향이 어느 정도인지
	// 쉽게 가늠하게 해준다.
	class Derived : public Base
	{
		virtual void mf1(); // override;
		virtual void mf2(unsigned int x); // override;
		virtual void mf3() &&; // override;
		void mf4() const; // override;

		// final 을 지정하면 아래의 파생 클래스에서 더 이상 이 함수를 재정의할 수 없게 된다.
		virtual void mf3() & final;
	};
	
	virtual int chapterMain()
	{
		Widget	val = makeWidget();
		val.doWork();

		makeWidget().doWork();

		return 0;
	}
};

/*
	재정의 함수는 override 로 선언하라.

	멤버 함수 참조 한정사를 이용하면 멤버 함수가 호출되는 객체(*this) 의 왼값 버전과
	오른값 버전을 다른 방식으로 처리할 수 있다.
*/