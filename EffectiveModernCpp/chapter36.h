#pragma once

#include <iostream>
#include <functional>
#include <memory>

/*********************************************************
	TLS ( Thread Local Storage ) 란.

	thread 별 스택을 이야기하는 것이 아니다.
	개별 thread 의 스택, 전역, 정적 변수 등의 데이터를 저장하는 위치를 할당하는 방법을 말한다
	
	https://msdn.microsoft.com/en-us/library/windows/desktop/ms686749(v=vs.85).aspx
**********************************************************/

using namespace std;

class chapter36 : public chapter
{
	class Widget
	{};

	static int	doAsyncWork()
	{
		std::this_thread::sleep_for(100ms);
		return 0;
	}

	/*
		fut 은  기본 시동 정책으로 인해 deferred 실행이 가능하다.
		바로 아래 무한루프에서는 fut 가 최소한 한번은 실행될 것이라는 ( 그리고 종료될 것이라는 )
		전제를 깔고 작성된 구문이다.
		만약 fut 가 실행되지 않는다면 이 코드는 무한루프에 빠진다.
	*/
	void infinity_loop_example()
	{
		auto fut = std::async(doAsyncWork);

		while (std::future_status::ready != fut.wait_for(100ms))
		{
			// to do something
		}
	}

	/*
		wait_for(0s) 호출을 통해, task 의 deferred 속성을 알아낸 후,
		무한루프에 빠지지 않도록 필요한 처리를 한다.
	*/
	void fixed_infinity_loop_example_01()
	{
		auto fut = std::async(doAsyncWork);

		if (std::future_status::deferred == fut.wait_for(0s))
		{
			// fut 에 get 이나 wait 를 호출해서 doAyncWork 를 동기적으로 호출한다.
		}
		else
		{
			while (std::future_status::ready != fut.wait_for(100ms))
			{
				// to do something
			}
		}
	}

	/*
		문제를 가장 명료하게, 그리고 쉽게 해결하는 것은 시동 정책을 aync 로 명시하는 것이다.
		future 객체를 통한 동기호출을 아예 정책적으로 배제해버리면 아래 코드가 무한루프에 빠지는
		일은 막을 수 있다.
	*/
	void fixed_infinity_loop_example_02()
	{
		auto fut = std::async(std::launch::async, doAsyncWork);

		while (std::future_status::ready != fut.wait_for(100ms))
		{
			// to do something
		}
	}

	/*
		스캇 마이어스가 종종 하는 코드 딸딸이... 혹은 책 팔아먹기 위한 페이지 늘리기...
		그냥 기본정책 코드 쓰면 짧게 타이핑 몇 번이면 되는데, 템플릿은 이따 왜 쓰는겨...
	*/
	template<typename F, typename... Ts>
	inline auto reallyAsync(F&& f, Ts&&... params)
	{
		return std::async(std::launch::async, std::forward<F>(f), std::forward<Ts>(params)...);
	}
	
	virtual int chapterMain()
	{
		// default launch polity 는 async 와 deferred 를 모두 적용하는 것이다.
		// 아래 두 호출은 동일한 시동 정책을 가진다.
		auto fut1 = std::async(doAsyncWork);
		auto fut2 = std::async(std::launch::async | std::launch::deferred,  doAsyncWork);

		return 0;
	}
};

/*
	std::async 의 기본 시동 방침은 과제의 비동기적 실행과 동기적 실행을 모두 허용한다.

	그러나 이러한 유연성 때문에 thread_local 접근의 불확실성이 발생하고, 과제가 절대로 실행되지 않을 수도 있고,
	시간 만료 기반 wait 호출에 대한 프로그램 논리에도 영향을 미친다.

	과제를 반드시 비동기적으로 실행해야 한다면 std::launch::async 를 지정하라.
*/