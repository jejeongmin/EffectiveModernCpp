#pragma once

#include <iostream>
#include <functional>
#include <memory>

/*********************************************************
	std::thread 의 합류 가능성이 중요한 이유 하나는, 만일 합류 가능한 스레드의
	소멸자가 호출되면 프로그램 실행이 종료된다는 것이다.
**********************************************************/

using namespace std;

constexpr auto tenMillion = 1'000'000;	// c++14 에서는 따옴표가 인식됨

class chapter37 : public chapter
{
	bool conditionalAreSatisfied() { return true;  }
	void performComputation(std::vector<int>&) {}

	bool doWork1(std::function<bool(int)> filter, int maxVal = tenMillion)
	{
		std::vector<int> goodVals;

		std::thread t([&filter, maxVal, &goodVals]
		{
			for (auto i = 0; i <= maxVal; ++i)
			{
				if (filter(i))
					goodVals.push_back(i);
			}
		});

		// t 의 핸들을 이용해 우선 순위 조정
		// task 기반에서는 thread handle 을 얻어올 수 없음.
		auto nh = t.native_handle();

		if (conditionalAreSatisfied())
		{
			t.join();
			performComputation(goodVals);
			return true;
		}
		
		// 만약 여기로 제어가 오면 지역객체인 t 의 소멸자가 불리고, 프로그램 실행이 종료된다.
		// std::thread 의 소멸자가 불리운다고 해서, 프로그램이 종료되는 것은 얼핏 들으면
		// 이상한 동작으로 보이지만, 충분히 합리적인 이유가 있다.
		// 이유를 설명하면 기니 272~273p 를 읽어보도록.
		// 여튼 그런 이유로 해서 표준 위원회는 std::thread 객체를 사용할 때 그 객체가 정의된
		// 범위 바깥의 모든 경로에서 합류 불가능으로 만드는 것은 프로그래머의 책임으로 하였다.
		return false;
	}

	/*
		scoped 객체의 특성을 이용해 std::thread 가 범위 안쪽에서 합류 가능한 상태로
		처리하기 위한 래퍼를 만든다.
		책에서는 RAII ( Resource Acquisision Is Initialization ) 이라고 소개하고 있다.
	*/
	class ThreadRAII
	{
	public:
		enum class DtorAction { join, detach };

		ThreadRAII(std::thread&& t, DtorAction a) : _action(a), _t(std::move(t)) {}

		~ThreadRAII()
		{
			// t 에 대한 멤버함수를 호출하기 전에 반드시 join 가능한지 검사해야 한다.
			// 합류 불가능한 thread 에 대해 join() 이나 detach() 를 실행하면 미정의 행동이 나오기 때문이다.
			if (_t.joinable())	
			{
				if (DtorAction::join == _action)
					_t.join();
				else
					_t.detach();
			}
		}

		std::thread& get() { return _t; }

	private:
		/*
			멤버 초기화는 선언 순서에 따른다.
			std::thread 가 가장 마지막에 선언된 것은 의도된 것이다.
			std::thread 가 초기화 되고 바로 실행될 수도 있는데,
			std::thread 가 다른 모든 멤버가 초기화된 이후에 초기화하면, 해당 비동기 스레드가
			다른 모든 멤버에 안전하게 접근할 수 있기 때문이다.
		*/
		DtorAction	_action;
		std::thread	_t;
	};

	/*
		join Action 을 취하도록 했다.
		detach Action 을 취하면 미정의 행동이 발생할 수 있기 때문이다.
		물론 join 도 성능이상, 그리고 약간의 미정의 행동을 유발하지만
		소멸자 호출로 인한 프로그램 종료, 미정의 해동, 성능 이상 중에서는
		그나마 성능 이상이 제일 낫다.
	*/
	bool doWork2(std::function<bool(int)> filter, int maxVal = tenMillion)
	{
		std::vector<int> goodVals;

		ThreadRAII t(
		std::thread([&filter, maxVal, &goodVals]
		{
			for (auto i = 0; i <= maxVal; ++i)
			{
				if (filter(i))
					goodVals.push_back(i);
			}
		}), ThreadRAII::DtorAction::join);

		auto nh = t.get().native_handle();

		if (conditionalAreSatisfied())
		{
			t.get().join();
			performComputation(goodVals);
			return true;
		}

		return false;
	}
	
	virtual int chapterMain()
	{
		return 0;
	}
};

/*
	모든 경로에서  std::thread 를 합류 불가능으로 만들어라.
	소멸 시 join 방식은 디버깅하기 어려운 성능 이상으로 이어질 수 있다.
	소멸 시 detach 방식은 디버깅하기 어려운 미정의 행동으로 이어질 수 있다.
	자료 멤버 목록에서 std::thread 객체를 마지막으로 선언하라.
*/