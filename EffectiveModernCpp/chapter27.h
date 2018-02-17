#pragma once

#include <iostream>
#include <functional>
#include <memory>

/*********************************************************
**********************************************************/

using namespace std;

class chapter27 : public chapter
{
	std::vector<std::string>	names;

	template<typename T>
	void logAndAdd(T&& name)
	{
		logAndAddImpl(std::forward<T>(name), std::is_integral<std::remove_reference_t<T>>::value);		
	}

	template<typename T>
	void logAndImpl(T&& name, std::true_type)
	{
		std::cout << "log and add for integral" << std::endl;
	}

	template<typename T>
	void logAndImpl(T&& name, std::false_type)
	{
		std::cout << "log and add for non integral" << std::endl;
	}

	/*
		보편 참조를 받는 템플릿의 생성을 여러가지 수단으로 제한해서 오버로딩을 안전하게 구현할 수도 있다.

		std::enable_if 를 이용하면 지정된 조건이 만족될 때만, 해당 템플릿이 활성화된다.
		std::is_same 은 두 형식이 같은 지 판정한다.
		std::decay 는 모든 참조와 모든 const, volatile 한정자를 제거한 형식에 해당한다.
		std::is_base_of<T1, T2> 는 만일 T2 가 T1 에서 파생된 형식이면 참이다. std::is_base_of<T1, T1> 은 만일 T1 이 사용자 정의 형식이라면 참이고, 내장 형식일 때는 거짓이다.
		std::is_constructible 은 한 형식의 객체를 다른 한 형식의 객체로부터 생성할 수 있는 지 컴파일 시점에서 판정한다.
	*/
	class Person
	{
	public:
		template<typename T,
				 typename = std::enable_if_t<
								!std::is_base_of<Person, std::decay_t<T>>::value
								&&
								!std::is_integral<std::remove_reference_t<T>>::value>>
			explicit Person(T&& n)
			: name(std::forward<T>(n))
		{
			static_assert(std::is_constructible<std::string, T>::value, "Paameter n can't be used to construct a std::string");
		}

	private:
		std::string	name;
	};
	
	virtual int chapterMain()
	{
		return 0;
	}
};

/*
	보편 참조와 중복적재의 조합에 대한 대안으로는 구별되는 함수 이름 사용, 매개변수를 const 에 대한 왼값 참조로 전달, 매개변수를 값으로 전달,
	꼬리표 배분 사용등이 있다.

	std::enable_if 를 이용해서 템플릿의 인스턴스화를 제한함으로써 보편 참조와 중복적재를 함께 사용할 수 있다.
	std::enable_if 는 컴파일러가 보편 참조 중복적재를 사용하는 조건을 프로그래머가 직접 제어하는 용도로 쓰인다.

	보편 참조 매개변수는 효율성 면에서 장점인 경우가 많지만, 대체로 사용성 면에서는 단점이 된다.
*/