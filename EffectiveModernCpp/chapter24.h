#pragma once

#include <functional>
#include <memory>


/*********************************************************

**********************************************************/

using namespace std;

class chapter24 : public chapter
{
	class Widget
	{
	};

	// void f(Widget&& param);		오른값 참조
	// Widget&& var1 = Widget();	오른값 참조
	// auto&& var2 = var1;			오른값 참조 아님
	// template<typename T> void f(std::vector<T>&& param);		오른값 참조
	// template<typename T> void f(T&& param);					오른값 참조 아님

	// 위의 예에서 보듯 T&& 는 오른값 참조, 또는 왼 값 참조로 모두 해석될 수 있다. 이런 이중성 때문에
	// 이런 성격의 참조는 const, volatile 등 거의 모든 것과 묶일 수 있다.
	// 이런 참조를 보편 참조 ( universal reference ) 라고 부른다.

	// auto&& var2 = var1;			보편 참조는 반드시 초기화해야 한다. 초기치가 왼값이면 왼값, 오른값이면 오른값으로 타입이 결정된다.

	virtual int chapterMain()
	{
		return 0;
	}
};

/*
	함수 템플릿 매개변수의 형식이 T&& 형태이고, T 가 연역된다면, 또는 객체를 auto&& 로 선언한다면, 그 매개 변수나 객체는 보편참조이다.

	형식 선언의 형태가 정확히 [형식&&]가 아니면, 또는 형식 연역이 일어나지 않으면, [형식&&]는 오른값 참조를 뜻한다.

	오른값으로 초기화되는 보편 참조는 오른값 참조에 해당한다. 왼값으로 초기화되는 보편 참조는 왼값 참조에 해당한다.
*/