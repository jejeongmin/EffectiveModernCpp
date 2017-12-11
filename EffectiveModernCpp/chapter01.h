#pragma once

#include "stdafx.h"
#include <array>

/*********************************************
param type 이 포인터 또는 참조 형식이지만 보편 참조는 아님
*********************************************/
template<typename T>
void f1(T& param)
{
	// decltype 은 말 그대로 type declaration 에 사용하는 것으로,
	// 타입 체킹이나 타입 확인을 위해 사용하는 것은 아니다.
	decltype(param)	temp = param;

	// 원래 내가 의도 했던 compile time 에서의 타입 확인을 위해서는 typeid 를 사용해야 한다.
	// 그러나 이 소스의 결과가 보여주듯이, 실제 typeid 에 의한 결과가 템플릿 연역 변환에 대한
	// 정확한 결과를 보여주진 않는다. premitive type 만 반영될 뿐이다.
	std::cout << typeid(temp).name() << " : " << temp << std::endl;
}

/*********************************************
param type 이 보편 참조임
*********************************************/
template<typename T>
void f2(T&& param)
{
	std::cout << typeid(param).name() << " : " << param << std::endl;
}

/*********************************************
param type 이 포인터도 아니고 보편참조도 아님
*********************************************/
template<typename T>
void f3(T param)
{
	std::cout << typeid(param).name() << " : " << param << std::endl;
}

/*********************************************
배열 변수의 크기를 컴파일 시점 상수로서 돌려주는 템플릿 함수
배열 매개변수에 이름을 붙이지 않은 것은, 이 템플릿에
필요한 것은 배열에 담긴 원소의 개수뿐이기 때문이다.
*********************************************/
template<typename T, std::size_t N>
constexpr std::size_t arraySize(T(&)[N]) noexcept
{
	return N;
}

class chapter01 : public chapter
{
	virtual int chapterMain()
	{
		int	x = 27;
		const int	cx = x;
		const int&	rx = x;

		f1(x);
		f1(cx);
		f1(rx);

		f2(x);
		f2(cx);
		f2(rx);
		f2(27);

		f3(x);
		f3(cx);
		f3(rx);

		const char	name[] = "Je, JeongMin";

		// 배열 매개변수 선언이 포인터 매개변수처럼 취급되므로, 템플릿 함수에 값으로 전달되는
		// 배열의 형식은 포인터 형식으로 연역된다.
		f3(name);

		// 함수의 매개변수를 진짜 배열로 선언할 수 는 없지만, 배열에 대한 참조로 선언할 수는 있다.
		// 템필릿 함수가 인수를 참조로 받도록 하고, 함수에 배열을 전달하면, T 에 대해 연역된 형식은
		// 배열의 실제 형식이 된다. 그 형식은 실제 배열의 크기를 포함한다.
		f1(name);

		// 배열의 크기를 컴파일 타임에 알아내 참조할 수 있다.
		int keyVals[] = { 1, 3, 7, 9, 11, 22, 35 };
		int	mappedVals[arraySize(keyVals)];
		int	arraySize = sizeof(mappedVals) / sizeof(int);
		memset(mappedVals, 0, sizeof(mappedVals));

		std::cout << "array size : " << arraySize << std::endl;

		arrayTest();

		return 0;
	}

	/*
		MSDN : https://msdn.microsoft.com/ko-kr/library/bb983093.
		std::array 를 쓰는 상대적 장점 : https://duragon.gitbooks.io/c-11/content/chapter6.html
	*/
	void arrayTest()
	{
		std::array<int, 4> values = { 1, 2, 3 };

		for each(auto itor in values)
		{
			std::cout << itor << std::endl;
		}
	}

};
// std::array

/*
	기억해 둘 사항들

	템플릿 형식 연역 도중에 참조 형식의 인수들은 비참조로 취급된다.
	즉, 참조성이 무시된다.

	보편 참조 매개변수에 대한 형식 연역 과정에서 왼값 인수들은 특별하게 취급된다.

	값 전달 방식의 매개변수에 대한 형식 연역 과정에서 const 또는 volatile( 또는 그 둘 다인 )
	인수는 비 const, 비 volatile 인수로 취급된다.

	템플릿 형식 연역 과정에서 배열이나 함수 이름에 해당하는 인수는 포인터로 붕괴한다.
	단, 그런 인수가 참조를 초기화하는 데 쓰이는 경우에는 포인터로 붕괴하지 않는다.
*/