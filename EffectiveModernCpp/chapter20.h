#pragma once

#include <functional>
#include <memory>


/*********************************************************
weak_ptr 는 자신을 생성하는 데 쓰인 std::shared_ptr 가 가리키는 것과 동일한 객체를
가리키나, 그 객체의 참조 횟수에는 영향을 주지 않는다.


효율성 면에서 std::weak_ptr 는 std::shared_ptr 와 본질적으로 동일하다.
std::weak_ptr 객체는 그 크기가 std::shared_ptr 객체와 같으며, std::shared_ptr 가 사용하는 것과 같은
제어블럭을 사용하며, 생성이나 파괴, 배정 같은 연산에 원자적 참조 횟수 조작이 관여한다.
앞에서 언급했듯이, 제어 블록에는 '두 번재' 참조 횟수가 있으며 그것이 바로 std::weak_ptr 가 조작하는 참조횟수이다.
**********************************************************/

using namespace std;

class chapter20 : public chapter
{
	class Widget
	{
	};

	virtual int chapterMain()
	{
		auto spw = std::make_shared<Widget>();	// ref count of spw is 1
		std::weak_ptr<Widget> wpw(spw);			// ref count of spw is still 1

		cout << "weak pointer expired : " << wpw.expired() << endl;

		auto spw1 = wpw.lock();					// 만약 expired 이면 null 을 반환
		std::shared_ptr<Widget> spw3(wpw);		// 만약 expired 이면 예외를 반환

		spw = nullptr;							// ref count of spw is 0

		cout << "weak pointer expired : " << wpw.expired() << endl;

		return 0;
	}
};

/*
	std::shared_ptr 처럼 작동하되, 대상을 잃을 수도 있는 포인터가 필요하면 std::weak_ptr 을 사용하라

	std::weak_ptr 의 잠재적인 용도로는 캐싱, 관찰자 목록, 그리고 std::shared_ptr 순환 고리 방지가 있다.
*/