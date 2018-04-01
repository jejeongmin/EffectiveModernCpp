#pragma once

#include <iostream>
#include <functional>
#include <memory>

/*********************************************************
	std::atomic 은 동시적 프로그래밍에 유용하나, 특별한 메모리의 접근에는 유용하지 않다.

	volatile 은 특별한 메모리의 접근에는 유용하나, 동시적 프로그래밍에는 유용하지 않다.
**********************************************************/

using namespace std;

class chapter40 : public chapter
{
	int	computeImportValue() { return 0; }

	virtual int chapterMain()
	{
		// atomic 으로 선언된 변수에 대해서 기본으로 제공되는 연산들은 thread-safe 하다.
		std::atomic<int> ai(0);
		ai = 10;
		std::cout << ai;	// ai 를 읽어들이는 동작 자체는 원자적이나, cout 으로 전달되는 과정은 원자성을 보장하지 않는다.
		++ai;
		--ai;

		std::atomic<int>	ac(0);
		volatile int vc(0);

		// thread 1
		{
			++ac;
			++vc;
		}

		// thread 2
		{
			++ac;
			++vc;
		}

		// ac 는 결과값 2를 보장하는 반면, vc 는 그렇지 않을 수도 있다.
		// 결과 연산에 대해 vc 에서 값 update 가 동시에 읽어나면 vc 의 최종 결과는 1이 될 수도 있다.
		// volatile 은 thread-safe 하지 않다.

		// 아래 코드의 논리적인 흐름에 문제가 없으려면 코드 순서가 바뀌면 안된다.
		// 그러나 atomic 이 아닌 변수들에 대해서는 최적화 컴파일 과정에서 코드 순서가 바뀔수 있다.
		// 또한 std::atomic 을 사용하면 컴파일러가 최적화를 위해 수행 가능한 코드의 순서 배치에
		// 대한 제약들이 생긴다.
		std::atomic<bool> valAvailable = false;
		auto imptValue = computeImportValue();
		valAvailable = true;

		int x = 0;

		// before compiler optimazation
		{
			auto y = x;
			y = x;
			x = 10;
			x = 20;
		}

		// after compiler optimazation
		{
			auto y = x;
			x = 20;
		}

		// 위의 코드의 최적화 전 과정의 코드에서 코드의 모든 라인이 의미를 가진다고 가정하자.
		// 책의 예로는 온도계의 온도 변수와 매핑된 변수를 주기적으로 설정해준다던가.
		// 특정 장비의 신호값에 매핑된 변수에 값 설정을 통해 주기적으로 신호를 전송한다던가.
		// 이럴 경우 컴파일러가 위 코드를 마음대로 최적화해버리면 문제가 발생한다.
		// volatile 은 해당 코드가 특별한 메모리를 다룬다는 점을 컴파일러에게 알려주는 수단이다.
		// 즉 컴파일러는 이를 "이 메모리에 대한 연산들에는 그 어떤 최적화도 수행하지 말라"는 지시로 생각한다.
		
		// std::atomic 에 대한 복사/배정 연산자는 제공되지 않는다.
		// atomic 에 대한 원자적인 연산은 hardware 명령어 레벨에서 제공되는데, 
		// 복사/배정 생성을 위한 일련의 연산(read 에 이은 write)는 hardware 에서 제공되지 않기 때문이다.
		// 여타의 다른 명령과 달리 hardware 에서 제공되지 않는 명령을 굳이 software 적으로 구현했을 때
		// 많은 문제가 예상되기 때문에( 남용으로 인한 성능 저하 ) 애초에 구현하지 않은 듯 하다.
		// 필요하면 직접 애플리케이션 프로그래머가 구현해서 쓰면 된다.
		std::atomic<int>	ax;
		//auto y = ax;

		volatile std::atomic<int> vai;	// 서로 용도가 다르므로 다음과 같이 동시에 사용할 수도 있다.

		return 0;
	}
};

/*
	일반적으로 std::atomic 변수에 접근하는 것은 std::atomic 이 아닌 변수에 접근하는 것보다 훨씬 느리며,
	앞에서 보았듯이, 컴파일러는 최적화를 위한 몇몇 코드 재배치 기법들을 적용할 수 없게 된다.
	그러므로 꼭 필요한 지점에만 사용해라.

	std::atomic 은 뮤텍스 보호 없이 여러 스레드가 접근하는 자료를 위한 것으로, 동시적 소프트웨어의 작성을 위한 도구이다.

	volatile 은 읽기와 기록을 최적화로 제거하지 말아야 하는 메모리를 위한 것으로, 특별한 메모리를 다룰 때 필요한 도구이다.
*/