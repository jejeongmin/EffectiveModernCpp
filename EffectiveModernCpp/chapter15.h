#pragma once

#include <functional>
#include <memory>

/*********************************************************

**********************************************************/

using namespace std;

class chapter15 : public chapter
{
	// c++11 에서는 실행 가능 문장이 하나여야 한다. 그리고 반드시 void 를 제외한 literal 형식을 반환해야 한다.
	// 아래 함수를 보면 ? 연산자와 재귀를 이용해 return 문 한 줄로 함수를 구현했음을 알 수 있다.
	constexpr int pow(int base, int exp) noexcept
	{
		return (exp == 0 ? 1 : base * pow(base, exp - 1));
	}

	// c++ 14 에서는 c++11 에서의 제약이 모두 없어졌다고 책에 되어 있는데,
	// 어찌된 일이지 vs2015 에서는 컴파일되지 않는다.
	/*
	constexpr int pow(int base, int exp) noexcept
	{
		int result = 1;
		for (int i = 0; i < exp; i++)	result *= base;
		return result;
	}
	*/

	class Point
	{
	public:
		constexpr Point(double x = 0, double y = 0) noexcept
			: _x(x), _y(y)
		{}

		constexpr double x() const noexcept { return _x; }
		constexpr double y() const noexcept { return _y; }

		// c++14 에서는 constexpr 에 void 반환도 가능하다고 되어 있는데, vs2015 에서 안되네.
		/*
		constexpr void x(double x) noexcept { _x = x; }
		constexpr void y(double y) noexcept { _y = y; }
		*/

		constexpr Point midpoint(const Point& p1, const Point& p2) noexcept
		{
			return{ (p1.x() + p2.x()) / 2, (p1.y() + p2.y()) / 2 };
		}

	private:
		double	_x, _y;
	};

	virtual int chapterMain()
	{
		constexpr auto arraySize = 10;	// OK, 10은 확실히 컴파일 시점 상수
		std::array<int, arraySize> data;

		constexpr Point p1(9.4, 27.7);

		return 0;
	}
};

/*
	constexpr 객체는 const 이며, 컴파일 도중에 알려지는 값들로 초기화된다.

	constexpr 함수는 그 값이 컴파일 도중에 알려지는 인수들로 호출하는 경우에는 컴파일 시점 결과를 산출한다.

	constexpr 객체나 함수는 비constexpr 객체나 함수보다 광범위한 문맥에서 사용할 수 있다.

	constexpr 은 객체나 함수 인터페이스의 일부이다.
*/