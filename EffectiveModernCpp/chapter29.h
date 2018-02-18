#pragma once

#include <iostream>
#include <functional>
#include <memory>

/*********************************************************
	대부분의 container 의 경우 이동연산을 할 경우, 복사 없이 포인터만 설정하게 되므로,
	상수 시간에 처리된다.

	하지만 일부 컨테이너 - array - 에서는 각 원소가 이동된다.
	이 경우에도 여전히 개별 원소의 이동이 복사보다 빠르다면,
	이동 연산이 더 빠르다는 믿음은 지켜지겠지만, 상수 시간에 처리되지는 않고, 선형 시간에 처리된다는 것은 알고 있어야 한다.

	또한 문자열 구현 중에 SSO ( small string optimazation ) 과 같이 객체의 버퍼 안에 최적화를 위한 작은 문자열을 포함하고 있는
	특수한 구현 들은 컴파일러의 최적화를 방해하기도 한다.
**********************************************************/

using namespace std;

class chapter29 : public chapter
{
	class Widget
	{};

	virtual int chapterMain()
	{
		std::vector<Widget>		vw1;
		for (int i = 0; i < 10000; ++i)
		{

			vw1.push_back(Widget());
		}
		auto vw2 = std::move(vw1);

		std::array<Widget, 10000> aw1;
		for (int i = 0; i < 10000; ++i)
		{
			aw1[i] = Widget();
		}
		auto aw2 = std::move(aw1);

		return 0;
	}
};

/*
	이동 연산들이 존재하지 않고, 저렴하지 않고, 적용되지 않을 것이라고 가정하라

	형식들과 이동 의미론 지원 여부를 미리 알 수 있는 경우에는 그런 가정을 둘 필요가 없다.
*/