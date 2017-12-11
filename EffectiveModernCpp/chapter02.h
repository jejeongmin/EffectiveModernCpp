#pragma once


void sumFunc(int, double)	{}

template<typename T> void f1(T param)
{
}

template<typename T> void f2(std::initializer_list<T> initList)
{
}

/***************************************
	auto 의 형식 연역 역시 세 가지 경우로 나뉜다.

	경우 1 : 형식 지정자가 포인터나 참조 형식이지만 보편 참조(universal reference) 가 아닌 경우
	경우 2 : 형식 지정자가 보편 참조인 경우
	경우 3 : 형식 지정자가 포인터도 아니고 참조도 아닌 경우
***************************************/
class chapter02 : public chapter
{
	virtual int chapterMain()
	{
		auto x = 27;			// 경우 3, x 는 포인터도 아니고 참조도 아님
		std::cout << typeid(x).name() << " : " << x << std::endl;

		const auto cx = x;		// 경우 3, cx 역시 둘다 아님
		std::cout << typeid(cx).name() << " : " << cx << std::endl;

		const auto& rx = x;		// 경우 1, rx 는 보편 참조가 아닌 참조
		std::cout << typeid(rx).name() << " : " << rx << std::endl;

		auto&& uref1 = x;		// x 는 int 이자 왼값이므로 uref1 의 형식은 int&
		std::cout << typeid(uref1).name() << " : " << uref1 << std::endl;

		auto&& uref2 = cx;		// cx 는 const int 이자 왼값이므로 uref2 의 형식은 const int&
		std::cout << typeid(uref2).name() << " : " << uref2 << std::endl;

		auto&& uref3 = 27;		// 27 은 int 이자 오른값이므로 uref3 의 형식은 int&&
		std::cout << typeid(uref3).name() << " : " << uref3 << std::endl;

		const char name[] = "Je, JeongMin";
		auto arr1 = name;		// const char*
		auto& arr2 = name;		// const char (&)[13]

		auto func1 = sumFunc;	// void (*)(int, double)
		auto& func2 = sumFunc;	// 책에는 void (&)(int, double) 일거라고 하나, 실제로는 void (*)(int, double)

		/*
			템플릿 인자 연역추론과 auto 인자 연역추론의 차이점 중 하나가 아래 드러난다.
			auto 로 선언된 변수의 초기치(initializer)가 중괄호 쌍으로 감싸인 형태이면, 연역된 형식은
			std::initialzer_list 이다.
		*/
		auto	x1 = 27;		// int
		auto	x2(27);			// int
		auto	x3 = { 27 };		// std::initializer_list<int>
		auto	x4{ 27 };			// int
		//auto	x5 = { 1, 2, 3.0 };		// std::initializer_list<T> 의 T 를 일관되게 유추할 수 없음

		//f1({ 11, 23, 9 });	// T 에 대한 형식을 연역할 수 없음
		f2({ 11, 23, 9 });		// T 는 int 로 연역되며, initList 의 형식은 std::initializer_list<int>로 연역된다.

		return 0;
	}

	/*******************************************************
	c++14 에서는 함수의 반환 형식을 auto 로 지정해서 컴파일러가 연역하게 할 수 있으며,
	람다의 매개변수 선언에 auto 를 사용하는 것도 가능하다.
	그러나 auto 의 그러한 용법들에는 auto 형식 연역이 아니라 템플릿 형식 연역의 규칙들이 적용된다.
	따라서 아래와 같이 리턴문에 auto 를 적용해도 컴파일러는 std::initializer_list<int> 를 연역할 수 없다.
	*******************************************************/
	auto createInitList()
	{
		return	0;
		//return	{ 1, 2, 3 };	compile error
	}
};

/*
	기억해 둘 사항들

	auto 형식 연역은 대체로 템플릿 형식 연역과 같지만, auto 형식 연역은 중괄호 초기치가
	std::initializer_list 를 나타낸다고 가정하는 반면, 템플릿 형식 연역은 그렇지 않다는 차이가 있다.

	함수의 반환 형식이나 람다 매개변수에 쓰인 auto 에 대해서는 auto 형식 연역이 아니라
	템플릿 형식 연역이 적용된다.
*/