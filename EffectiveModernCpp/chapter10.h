#pragma once

#include <functional>
#include <memory>
#include <mutex>


/*********************************************************
	이름공간 오염을 줄여준다는 것만으로도 범위 있는 enum 을 선호할 이유는 충분하다.
	또한 형식 검사 또한 철저히 되기 때문에 잠재적인 맥락에서 발생할 수 있는 문제를 줄여준다.

	책에는 범위 없는 enum 은 전방 선언이 안된다고 되어 있는데, 적어도 이 예제를 빌드한
	c++14 에서는 가능하다.
**********************************************************/

class chapter10 : public chapter
{
	virtual int chapterMain()
	{
		scopeless_enum();
		scope_enum();
		
		return 0;
	}

	enum class PreColor1;		// 전방 선언 가능
	enum PreColor2;				// c++98 에서는 전방 선언 불가능

	// enum 의 기본형식은 int 이나 다음과 같은 방법으로 재정의할 수 있다.
	enum class Status1 : std::uint32_t;
	enum Status2 : std::uint8_t;

	// enum 의 암묵적인 int 캐스팅을 허용하는 게 더 좋은 예..
	// 예를 들면 std::tuple 을 사용할 때, field index의 가독성을 위해 enum 의 묵시적 int 변환을 활용할 때..
	// type checking 이 주는 안정감을 포기하기 싫을 때는, 다소 타이핑이 많지만 아래와 같은 형식으로
	// enumerator 의  int 변환을 일반화할 수 있다.
	// 하지만 이게 왠 개뼉다귀 같은 코드인가... 이런 난잡한 신공 안부려도 해결할 방법은 많다.
	template<typename E>
	constexpr auto
		toUType(E enumerator) noexcept
	{
		return static_cast<std::underlying_type_t<E>>(enumerator;)
	}

	void scopeless_enum()
	{
		enum Color { black, white, red };
		//auto white = false;		// 오류. 범위에 이미 white 가 선언되어 있음

		Color c = red;

		if (c < 14.5)				// Color 를 double 과 비교!! 하지만 컴파일 된다.
		{
			//...
		}
	}

	void scope_enum()
	{
		enum class Color { black, white, red };

		auto white = false;			// 이 범위에 다른 white 는 없음

		//Color	c1 = white;			// 이 범위에 white 라는 이름의 열거자는 없음
		Color	c2 = Color::white;
		auto	c3 = Color::white;

		//if (c2 < 14.5)				// 강력한 type checking!! 물론 static_cast 로 강제로 형변환하면 동작시킬 순 있다.
		{
			//...
		}
	}
};

/*
	c++98 스타일의 enum 을 이제는 범위 없는 enum 이라고 부른다.

	범위 있는 enum 의 열거자들은 그 안에서만 보인다. 이 열거자들은 오직 캐스팅을
	통해서만 다른 형식으로 변환된다.

	범위 있는 enum 과 범위 없는 enum 모두 바탕 형식 지정을 지원한다.
	범위 있는 enum 의 기본 바탕 형식은 int 이다.
	범위 없는 enum 에는 기본 바탕 형식이 없다.

	범위 있는 enum 은 항상 전방 선언이 가능하다.
	
	범위 없는 enum 은 해상 선언에 바탕 형식을 지정하는 경우에만 전방 선언이 가능하다.
	( 아니다. 이 내용은 c++14, 적어도 vs2015 에서는 적용되지 않는다. 
	  vs2015 에는 바탕 형식을 지정하지 않아도 전방 선언이 가능하다 )
*/