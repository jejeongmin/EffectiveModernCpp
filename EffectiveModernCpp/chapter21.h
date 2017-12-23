#pragma once

#include <functional>
#include <memory>


/*********************************************************
**********************************************************/

using namespace std;

class chapter21 : public chapter
{
	class Widget
	{
	};

	void processedWidgets(std::shared_ptr<Widget> spw, int priority)
	{
	}

	int computePriority()
	{
		// occur exception
		return 0;
	}

	virtual int chapterMain()
	{
		std::shared_ptr<Widget> spw2(new Widget);	// Widget 타입을 두 번 명시해야 하고, 타이핑 양도 더 많다.
		auto spw1(make_shared<Widget>());			// 위 방식에 비해 훨씬 짧고 명료하다.

		processedWidgets(std::shared_ptr<Widget>(new Widget), computePriority());	// 할당이 진행되었지만 아직 shared_ptr 생성이 되지 않았을 때, 예외가 발생하면 자원 누수
		processedWidgets(std::make_shared<Widget>(), computePriority());			// 자원 누수 위험이 없다.

		return 0;
	}
};

/*
new 의 작접 사용에 비해, make 함수를 사용하면 소스 코드 중복의 여지가 없어지고, 예외 안전성이 향상되고, std::make_shared 와 std::allocate_shared 의 경우 더 작고 빠른 코드가 산출된다.

make 함수의 사용이 불가능 또는 부적합한 경우로는 커스텀 삭제자를 지정해야 하는 경우와 중괄호 초기치를 전달해야 하는 경우가 있다.

std::shared_ptr 에 대해서 make 함수가 부적합 경우가 더 있는데, 두 가지 예를 들자면
	1) 커스텀 메모리 관리 기능을 가진 클래스를 다루는 경우와
	2) 메모리가 넉넉하지 않은 시스템에서 큰 객체를 자주 다루어야 하고 std::weak_ptr 들보다 오래 살아남는 경우이다.
*/