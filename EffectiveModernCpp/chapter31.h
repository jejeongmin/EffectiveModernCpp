#pragma once

#include <iostream>
#include <functional>
#include <memory>

/*********************************************************
**********************************************************/

using namespace std;

class chapter31 : public chapter
{
	using FilterContainer = std::vector<std::function<bool(int)>>;

	FilterContainer	_filters;
	int				_divisor = 5;

	int computeDivisor()
	{
		return 5;	// temporary value
	}

	/*
		아래 함수는 문제가 있다.
		람다는 지역변수 divisor 를 참조하는데, 그 변수는 addDivisorFilter 함수가 반환되면 더 이상 존재하지 않는다.
	*/
	void addDivisorFilter1()
	{
		auto divisor = computeDivisor();

		_filters.emplace_back(
			[&](int value) { return value % divisor == 0; }
		);
	}

	/*
		아래 함수는 람다 내부에서 사용할 변수를 명시적으로 캡쳐하였다.
		addDivisorFilter1() 함수가 가진 문제는 여전하지만, 표현식의 유효성이 divisor 의 수명에 의존한다는 점이 명확히 나타난다는 장점이 있다.
	*/
	void addDivisorFilter2()
	{
		auto divisor = computeDivisor();

		_filters.emplace_back(
			[&divisor](int value) { return value % divisor == 0; }
		);
	}

	/*
		위에서 언급하는 divisor 의 수명 문제는 캡쳐모드를 [by reference] 가 아닌, [by value]로 하면 해결된다.
	*/
	void addDivisorFilter3()
	{
		auto divisor = computeDivisor();

		_filters.emplace_back(
			[=](int value) { return value % divisor == 0; }
		);
	}

	/*
		캡쳐는 오직 람다가 생성된 범위 안에서 보이는, static 이 아닌 지역 변수(매개변수 포함)에만 적용된다.
		divisor 는 지역 변수가 아닌 클래스의 한 자료 멤버이므로 갈무리될 수 없다.
		그 이유는 자료 멤버를 람다에서 사용할 경우, this 가 암묵적으로 쓰이기 때문이다.
		람다가 클로저 안에 갈무리하는 것은 _divisor 가 아닌, Chapter31 의 this 포인터이다.
		이 점을 이해한다면 이 람다에서 만들어진 클로저의 유효성이 해당 Widget 객체의 수명에 의해 제한된다는 점도 이해할 수 있을 것이다.
	*/
	void addDivisorFilter4()
	{
		/*_filters.emplace_back(	// 빌드되지 않음.
			[](int value) { return value % _divisor == 0; }
		);*/

		_filters.emplace_back(
			[=](int value) { return value % _divisor == 0; }
		);

		// 바로 위의 함수를 컴파일러는 다음과 같이 바라본다.
		auto currentObjectPtr = this;
		_filters.emplace_back(
			[currentObjectPtr](int value) { return value % currentObjectPtr->_divisor == 0; }
		);
	}

	/*
		addDivisorFilter4 의 문제를 가장 깔끔하게 수정한 버전은 다음과 같다.
		그냥 [by value] 를 이용하도록 약간만 변형하면 된다.
	*/
	void addDivisorFilter5()
	{
		auto divisor = _divisor;

		_filters.emplace_back(
			[=](int value) { return value % divisor == 0; }
		);
	}

	/*
		아래 함수는 static 으로 선언된 divisor 가 기본값 캡쳐 되는 듯한 오해를 사게 한다.
		이 람다는 사실 아무 것도 갈무리 하지 않는다.
		실제로 이 람다는 divisor 를 참조로 갈무리한 것과 같으며 이는 기본값 갭쳐가 뜻하는 바와 직접적으로 모순이 된다.

		이 경우에도 해결책은 비슷하다.
		지역 변수에 값을 복사해서 그 지역 변수를 기본값 캡쳐로 넘기면 된다.
	*/
	void addDivisorFilter6()
	{
		static auto divisor = computeDivisor();

		_filters.emplace_back(
			[=](int value) { return value % divisor == 0; }
		);

		++divisor;
	}

	virtual int chapterMain()
	{
		return 0;
	}
};

/*
	기본 참조 캡쳐는 참조가 대상을 잃을 위험이 있다.

	기본 값 참조는 포인터(특히 this)가 대상을 잃을 수 있으며, 람다가 자기 완결적이라는 오해를 부를 수 있다.
*/