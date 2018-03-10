#pragma once

#include <iostream>
#include <functional>
#include <memory>

/*********************************************************
**********************************************************/

using namespace std;

class chapter33 : public chapter
{
	class Widget
	{
	public:
		bool	isValidated() const	{ return true;  }
		bool	isArchived() const { return true;	}
	};

	template<typename T>
	T normalize(T x)
	{
		return x;
	}

	virtual int chapterMain()
	{
		// c++14 에서 가장 고무적인 기능은 일반적 람다, 즉 매개변수 명세에 auto 를 사용하는 람다이다.
		auto f1 = [&](auto x) { return normalize(x); };

		// 위의 코드를 제대로 동작하게 하려면, 람다가 x 를 normalize 에 완벽하게 전달해야 한다.
		// 그러려면 코드에서 바꿀 점이 두 가지인데, 첫째로 x 가 보편 참조이어야 하고, 둘째로 x 를 std::forward 를 통해서 normalize 에 전달해야 한다.
		// auto f2 = [&](auto&& x) { return normalize( std::forward<???>(x)); };

		// 이 람다에 주어진인수가 왼값인지, 오른값인지는 매개변수 x 의 형식을 조사해 보면 알 수 있다.
		// 그러한 조사에 사용할 수 있는 수단이 바로 decltype 이다.
		auto f3 = [&](auto&& x) { return normalize(std::forward<decltype(x)>(x)); };

		// c++14 의 람다는 가변수인수도 지원한다.
		auto f4 = [&](auto&&... xs) { return normalize(std::forward<decltype(x)>(xs)...); };

		return 0;
	}
};

/*
	std::forward 를 통해서 전달된 auto&& 매개변수에는 decltype 을 사용하라
*/