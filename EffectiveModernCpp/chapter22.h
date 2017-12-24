#pragma once

#include <functional>
#include <memory>
#include "chapter.h"


/*********************************************************
	impl 기법을 활용하는 경우는,
	너무 긴 빌드 시간을 줄이거나,
	라이브러리 배포시 인터페이스 변경으로 인한 빌드를 방지하기 위해 주로 사용한다.
	impl 객체 관리에 unique_ptr 을 활용하라는 것이 chapter22 의 요점이다.
**********************************************************/

using namespace std;

class chapter22 : public chapter
{
public:
	class Widget
	{
	public:
		Widget();
		~Widget();

		Widget(Widget&& rhs);
		Widget& operator=(Widget&& rhs);

		Widget(const Widget& rhs);
		Widget& operator=(const Widget& rhs);

	private:
		struct Impl;		// 선언만 존재하는 불완전 형식( in-complete type )
		std::unique_ptr<Impl>	_impl;
	};

	virtual int chapterMain();
};

/*

*/