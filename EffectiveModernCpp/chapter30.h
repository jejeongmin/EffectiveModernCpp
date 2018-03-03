#pragma once

#include <iostream>
#include <functional>
#include <memory>

/*********************************************************
	완벽 전달은 단순히 객체들을 전달하는 것만이 아니라, 그 객체들의 주요 특징, 즉 그 형식, 왼값 또는 오른값 여부,
	const 나 volatile 여부까지도 전달하는 것을 말한다.

	아래에서는 완벽전달이 실패하는 c++ 에서의 특수한 케이스들을 보여주고, 그것을 우회하는 방법을 보여준다.
**********************************************************/

using namespace std;

class chapter30 : public chapter
{
	void	f(const std::vector<int>& v) {}

	template <typename T>
	void fwd(T&& param)
	{
		f(std::forward<T>(param));
	}

	class Widget
	{
	public:
		static constexpr size_t MinVals = 32;
	};

	void	f2(size_t val) {}

	template <typename T>
	void fwd2(T&& param)
	{
		f2(std::forward<T>(param));
	}

	void	f3(int pf(int)) {}

	template <typename T>
	void fwd3(T&& param)
	{
		f3(std::forward<T>(param));
	}

	static int	processVal(int value) { return 0;  }
	static int	processVal(int value, int priority) { return 0; }

	struct IPv4Header
	{
		uint32_t	version : 4,
					IHL : 4,
					DSCP : 6,
					ECN : 2,
					totalLength : 16;				
	};
	
	virtual int chapterMain()
	{
		/*
			중괄호 초기치 실패 사례

			템플릿 연역은 중괄호 초기치를 std::initializer_list 로 인식할 수 없다.
			반면 auto 는 std::initializer_list 연역이 가능하다 ( chapter2 내용 참고 )
		*/
		//fwd({1, 2, 3});	
		auto il = { 1, 2, 3 };
		fwd(il);

		/*
			널 포인터를 뜻하는 0 또는 NULL

			NULL 은 완벽전달되지 않는다. 당연히 이 값은 0 이기 때문이다.
			0 이나 NULL 대신 nullptr 을 사용하면 된다 ( chapter 8 내용 참고 )
		*/

		/*
			선언만 된 정수 static const 및 constexpr 자료 멤버

			참조로 전달한다는 건 주소 전달을 의미한다. static MinVals 는 선언만 있을 뿐, 정의가 없다. 이런 경우 컴파일은 되지만 링크에서 실패한다.
			예전 컴파일러에서는 static 변수에 대한 정의를 구현 파일에서 별도로 해주었어야 했으나
			이 코드를 테스트한 vs2017 에서는 MinVals 에 대한 정의가 없어도 컴파일러가 알아서 정의를 하는 듯 하다.
		*/
		f2(Widget::MinVals);
		fwd2(Widget::MinVals);
		
		/*
			중복적재된 함수 이름과 템플릿 이름

			함수 템플릿에서의 processVal 을 넘겼을 때 무슨 일이 일어나는지 이해해야 한다.
			processVal 은 단지 컴파일러에서 인식하기 위한 이름값일 뿐, 어떤 형식을 가진 값이 아니다.
			따라서 템플릿에서 인자추론에 대한 연역이 일어나지 않고 실패한다.

			이를 해결하는 방법은 위의 이미 언급되어 있다. 형식을 알려주면 된다.
		*/
		f3(processVal);
		//fwd3(processVal);
		using ProcessFuncType = int(*)(int);
		fwd3(static_cast<ProcessFuncType>(processVal));

		return 0;

		/*
			비트필드

			c++ 표준은 non-const 참조는 절대로 비트필드에 묶이지 않아야 한다고 명시되어 있다.
			비트 필드는 결국 참조인데, 임의의 비트필드를 가르키는 참조는 없기 때문이다.
			
			이에 대한 해결책 역시 문제에 나와 있다.
			임의의 비트필드를 가르키는 참조가 없으므로 그냥 비트필드값이 복사된 보통 객체에 넣어주면 된다.
		*/
		IPv4Header		header;
		f2(header.totalLength);
		//fwd2(header.totalLength);	

		auto length = static_cast<uint16_t>(header.totalLength);
		fwd2(length);
	}
};

/*
	완벽 전달은 템플릿 형식 연역이 실패하거나 틀린 형식을 연역했을 때 실패한다.

	인수가 중괄호 초기치이거나 0 또는 NULL 로 표현된 널 포인터, 선언만 된 정수 static const 및 constexpr 자료 멤버,
	템플릿 및 중복적재된 함수 이름, 비트필드이면 완벽 전달이 실패한다.
*/