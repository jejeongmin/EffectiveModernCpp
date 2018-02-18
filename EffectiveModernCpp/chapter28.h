#pragma once

#include <iostream>
#include <functional>
#include <memory>

/*********************************************************

왼값 참조일 경우 T&& 에서 T 는 Widget& 으로 연역되며, 템플릿에서 치환된 코드는
void func(Widget& && param ) 이다.
참조(&)에 대한 참조(&&) 가 덧붙어 있는데, 기본적으로 C++ 에서 참조에 대한 참조는 위법이다.
하지만 특정 문맥에서는 참조 축약을 통해 일시적으로 허용되기도 한다.

28장은 참조축약에 대한 규칙과 함께 그 일시적 문맥이 어떤 것인지 설명한다.
**********************************************************/

using namespace std;

class chapter28 : public chapter
{
	template<typename T>
	void func(T&& param)
	{
		std::cout << "func()" << std::endl;
	}

	class Widget
	{
	};

	Widget widgetFactory()
	{
		return Widget();		
	}

	virtual int chapterMain()
	{
		Widget	w;

		func(w);
		func(widgetFactory());

		return 0;
	}
};

/*
	참조 축약은 템플릿 인스턴스화, auto 형식 연역, typedef 와 별칭 선언의 지정 및 사용, decltype 의 지정 및 사용 이라는 네 가지 문맥에서 일어난다.

	컴파일러가 참조 축약 문맥에서 참조에 대한 참조를 만들어 내면, 그 결과는 하나의 참조가 된다.
	원래의 두 참조 중 하나라도 왼값 참조이면 결과는 왼값 참조이고, 그렇지 않으면 오른값 참조이다.

	형식 연역이 왼값과 오른값을 구분하는 문맥과 참조 축약이 일어나는 문맥에서 보편 참조는 오른값 참조이다.
*/