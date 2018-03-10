#pragma once

#include <iostream>
#include <functional>
#include <memory>

/*********************************************************
**********************************************************/

using namespace std;

class chapter32 : public chapter
{
	class Widget
	{
	public:
		bool	isValidated() const	{ return true;  }
		bool	isArchived() const { return true;	}
	};

	virtual int chapterMain()
	{
		auto pw = std::make_unique<Widget>();

		// c++14 에서는 아래와 같이 간단히 객체를 클로저 안으로 이동 갈무리 할 수 있다.
		// 초기화 갈무리(init capture) 에서 좌변은 pw 는 클로저 클래스 안의 자료 멤버이고, 우변의 pw 는 람다가 정의되는 지점의 지역변수 pw 이다.
		auto func = [pw = std::move(pw)]
		{
			return pw->isArchived() && pw->isValidated();
		};

		std::vector<double> data;

		// c++11 에서 bind 객체를 이용해 init capture 를 흉내내기
		auto func2 = std::bind(
			[](const std::vector<double>& data)
			{ /* to implement function here */},
			std::move(data));

		// c++11 에서의 이동갈무리 구현 경우, c++14 보다는 한번의 복사가 불가피하다.
		// bind 에서 인자를 오른값으로 넘겼지만( std::move(data) )
		// 람다에서 매개 변수는 왼값이기 때문에 이동생성된 data 에 대한 복사본을 받을 수 밖에 없다 ( std::vector<double>& data )
		auto func3 = std::bind(
			[](std::vector<double>& data) mutable
			{ /* to implement function here */},
			std::move(data));

		return 0;
	}
};

/*
	객체를 클로저 안으로 이동할 때에는 c++14 의 초기화 갈무리를 사용하라

	c++11 에서는 직접 작성한 클래스나 std::bind 로 초기화 갈무리를 흉내낼 수 있다.
*/