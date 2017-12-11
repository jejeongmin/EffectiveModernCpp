#pragma once

#include <functional>
#include <memory>
#include <unordered_map>

class Widget6
{};

std::vector<bool> features(const Widget6& w)
{
	std::vector<bool> result;

	return result;
};

/*********************************************************
정리하자면 다음과 같은 코드 형태는 피하는게 좋다.

auto someVar = "보이지 않는" 대리자 클래스 형식의 표현식

명시적인 static_cast<> 를 활용해서 대리자 클래스 형식의 변환을 명시적으로 드러내고
프로그래머의 의도도 긴~ 타입 캐스팅 연산자 타이핑을 통해서 명시적으로 드러내자.

**********************************************************/
class chapter06 : public chapter
{
	virtual int chapterMain()
	{
		Widget6	w;

		// 아래 코드는 기대대로 잘 작동한다.
		{
			bool	highPriority = features(w)[5];
		}

		// 아래 코드는 잘 작동하지 않는다.
		// std::vector<bool> 의 [] 연산자는, 명세에 의하면 bool 로 변환을 암묵적으로 지원하긴 하나,
		// auto 로 연역하게 되면, 명시적으로 std::vector<bool>::reference 를 반환하게 되기 때문이다.
		// 컴파일러 구현에 따라 다른데, 대개 std::vector<bool>::reference 는 bool 을 표현하기 위해 word 값과
		// 그 word 내부에서 bool 값을 담고 있는 bit 상의 offset 이 포함된 구조체에 대한 포인터를 반환하는데
		// auto 에 포인터에 대한 임시값을 변환하게 되고, 해당 라인을 벗어나는 순간, 임시값이 파괴되어,
		// dangling pointer 가 되고 만다.
		{
			auto	highPriority1 = features(w)[5];

			// 위 코드를 개선한 아래 코드는 다음과 같다.
			auto	highPriority2 = static_cast<bool>(features(w)[5]);
		}

		return 0;
	}
};

/*
	"보이지 않는" 대리자 형식 때문에 auto 가 초기화 표현식의 형식을 "잘못" 연역할 수 있다.
	
	형식 명시 초기치 관용구는 auto 가 원하는 형식을 연역하도록 강제한다.
*/