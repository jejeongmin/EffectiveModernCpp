#pragma once

#include <iostream>
#include <functional>
#include <memory>

/*********************************************************
	예를 들어 Reactor 패턴을 구현할 때,
	일반적인 event / wait 방식으로 구현하게 된다 ( Normalway 함수 )

	그러나 future/promise 매커니즘을 활용하면 1회성 단발성 전달 방식에
	한해서 유용할 수 있다.
**********************************************************/

using namespace std;

class chapter39 : public chapter
{
	std::condition_variable cv;
	std::mutex	m;

	bool flag = false;

	void NormalWay()
	{
		// 여기서 특정 이벤트를 캐치한 후

		// 처리 함수에 알려준다.
		{
			std::lock_guard<std::mutex> g(m);
			flag = true;
		}
		cv.notify_one();

		// 위에서 통지 받은 이벤트를 처리한다.
		{
			std::unique_lock<std::mutex>	lk(m);
			cv.wait(lk, [&] { return flag;  });	// std::conditional_variable 은 정말 signal 된건지 검사할 수 있는 수단을 제공한다.
		}
	}

	std::promise<void>	p;
	void react() {};
	void NewWay()			// chapter 37 의 RAII 객체를 활용하면 좀 더 안전하게 thread 관리가 가능
	{
		std::thread t([&]
		{
			p.get_future().wait();		// signal 될때까지 대기
			react();
		});

		p.set_value();		// signal 을 준다.

		t.join();			// react 가 완료될 때까지 대기
	}

	virtual int chapterMain()
	{
		return 0;
	}
};

/*
	간단한 사건 통신을 수행할 때, 조건 변수 기반 설계에는 여분의 뮤텍스가 필요하고,
	검출 과제와 반응 과제의 진행 순서에 제약이 있으며, 사건이 실제로 발생했는지를 반응 과제가 다시 확인해야 한다.

	플래그 기반 설계를 사용하면 그런 단점들이 없지만, 대신 차단이아니라 폴링이 일어난다는 단점이 있다.

	조건 변수와 플래그를 조합할 수 있으나, 그런 조합을 이용한 통신 매커니즘은 필요 이상으로 복잡하다.

	std::promise 와 미래 객체를 사용하면 이러한 문제점들을 피할 수 있지만, 그런 접근 방식은 공유 상태에
	힙 메모리를 사용하며, 단발성 통신만 가능하다.
*/