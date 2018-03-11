#pragma once

#include <iostream>
#include <functional>
#include <memory>

/*********************************************************
	c++11 에서는 람다가 거의 항상 std::bind 보다 나은 선택이다. 심지어 c++14 에서는 '거의'가 아니라 확고하게 나은 선택이다.
	그럼에도 불구하고 bind 를 알아둘 필요가 있는 것은, 그런 코드를 유지보수할 필요가 있기 때문.
	bind 를 항상 써 온 입장에서는, 람다가 왜 안되지.. 투덜대면서 일했기 때문에 34장 내용은 새삼스러울 것이 없네.
	다만 bind 에서는 안되는 줄 알았던 기법들을 여기서 뒤늦게 발견해서 안타까울 뿐.

**********************************************************/

using namespace std;
using namespace std::chrono;
using namespace std::placeholders;

class chapter34 : public chapter
{
	class Widget
	{};

	using Time = std::chrono::steady_clock::time_point;

	enum class Sound { Beep, Siren, Whistle };

	using Duration = std::chrono::steady_clock::duration;

	static void setAlarm(Time t, Sound s, Duration d)
	{}

	enum class CompLevel { Low, Normal, High };
	static Widget compress(const Widget&w, CompLevel lev)
	{
		return w;
	}

	class PolyWidget
	{
	public:
		template<typename T>
		void operator()(const T& param) const
		{}
	};

	virtual int chapterMain()
	{
		auto setSoundL1 = 
			[&](Sound s)
		{
			setAlarm(steady_clock::now() + hours(1), s, seconds(30));
		};

		// c++14 에서는 c++11 의 사용자 정의 리터럴 기능에 기초하는 표준 접미사(suffix)들을 이용해서 코드를 더욱 간결하게 만들 수 있다.
		// 이를테면 초는 s, 밀리초는 ms, 시간은 h 라는 접미사를 이용해서 지정하면 된다.
		auto setSoundL2 =
			[&](Sound s)
		{
			using namespace std::chrono;

			setAlarm(steady_clock::now() + 1h, s, 30s);
		};

		// 아래 코드에는 심각한 논리적 오류가 있다.
		// 작성 의도대로라면 setAlarm 이 호출되는 시점에 'steady_clock::now() + 1h' 이 평가되어야 한다.
		// 하지만 아래 코드에서는 bind 되는 시점ㅇ 'steady_clock::now() + 1h' 이 평가되게 된다.
		// 람다에서는 그런 문제가 없다.
		auto setSoundB1 =
			std::bind(chapter34::setAlarm, steady_clock::now() + 1h, _1, 30s);

		// 바로 위 버전을 개선한 bind 구문이다.
		// 위 문제를 바로 잡으려면, 첫 std::bind 안에 두 개의 함수 호출을 내포시켜야 한다.
		// c++14 에서는 std::plus<> 안의 steady_clock::time_point 은 생략 가능하다.
		auto setSoundB2 =
			std::bind(chapter34::setAlarm, 
				std::bind(std::plus<steady_clock::time_point>(), std::bind(steady_clock::now), 1h),
				_1, 30s);

		// setAlarm 을 아래와 같이 오버로딩 하게 되면 새로운 문제가 발생한다.
		// void setAlarm(Time t, Sound s, Duration d, Volume v);
		// 문제는 컴파일러로서는 두 setAlarm 함수 중 어떤 것을 std::bind 에 넘겨주어야 할 지 결정할 방법이 없다는 것이다.
		// 컴파일러가 알고 있는 것은 함수 이름뿐이며, 이름만으로는 중의성을 해소할 수 없다.
		// std::bind 가 호출되게 하려면 setAlarm 을 적절한 함수 포인터 형식으로 캐스팅 해야 한다.
		using SetAlarm3ParamType = void(*)(Time t, Sound s, Duration d);
		auto setSoundB3 =
			std::bind(static_cast<SetAlarm3ParamType>(chapter34::setAlarm),
				std::bind(std::plus<steady_clock::time_point>(), std::bind(steady_clock::now), 1h),
				_1, 30s);

		Widget w;

		// 람다에서는 w 가 by value 인지, by ref 인지 명시적으로 드러난다.
		auto compressRateL =
			[w](CompLevel lev)
		{
			return compress(w, lev);
		};

		// 하지만 bind 에서는 bind 내부 작동 방식을 알지 못하는 한 알 수 없다. 
		// 답을 말하자면 bind 에서는 by value 로 전달한다.
		using namespace std::placeholders;
		auto compressRateB = std::bind(compress, w, _1);

		// 인수 전달 방식도 마찬가지이다.
		compressRateL(CompLevel::Low);		// 람다에서는 코드 상에서 by value 인지, by ref 인지 드러난다.
		compressRateB(CompLevel::Low);		// bind 에서는 코드 상에서는 by value 인 듯 보이지만, 실제로는 모든 인수는 참조로 전달된다.

		// 이동 갈무리와 다형적 함수 객체등은 c++11 의 람다에서는 구현이 간단하지 않아, bind 를 쓰는 의미가 있었으나,
		// c++14 에서는 람다로 위 문제를 풀기가 훨씬 쉬워졌기 때문에 그나마 있던 당위성도 사라졌다

		PolyWidget pw;
		auto boundPW1 = std::bind(pw, _1);

		// 이제 boundPW 를 서로 다른 형식의 인수들로 호출할 수 있다.
		boundPW1(1930); 
		boundPW1(nullptr);
		boundPW1("Rosebud");

		// c++11 람다로는 위의 구현이 불가능했지만, c++14 에서는 아래와 같이 간단히 풀 수 있다.
		auto boundPW2 = 
			[pw](const auto& param)
		{
			pw(param);
		};

		boundPW2(1930);
		boundPW2(nullptr);
		boundPW2("Rosebud");

		return 0;
	}
};

/*
	std::bind 를 사용하는 것보다 람다가 더 읽기 쉽고 표현력이 좋다. 그리고 더 효율적일 수 있다.

	c++14가 아닌 c++11에서는 이동 갈무리를 구현하거나 객체를 템플릿화된 함수 호출 연산자에 묶으려 할 때 std::bind 가 유용할 수 있다.
*/