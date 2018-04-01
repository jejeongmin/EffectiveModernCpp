#pragma once

#include <iostream>
#include <functional>
#include <memory>
#include <vector>
#include <future>

/*********************************************************
	std::thread 객체와 future 객체의 소멸자들이 불리우는 규칙은 다소 다르다.

	미래 객체는 피호출자가 호출자에게 결과를 전송하는 통신 채널의 한쪽 끝이다.
	대개 피호출자는 자신의 계산 결과를 std::promise 를 통해 전달한다.
	이 전달결과는 호출자 객체 혹은 피호출자 객체 어디에도 저장되지 않는다 ( 정확히는 저장될 수 없다. 둘 다 생명주기가 다르니까 )
	그래서 일종의 공유저장소를 만들어 그 곳에 저장한다.
	이 공유 저장소의 저장되는 객체는 그 값을 참조하는 미래 객체들에 대한 참조카운터를 가진다.
**********************************************************/

using namespace std;

class chapter38 : public chapter
{
	std::vector<std::future<void>> futs;

	// Widget 객체는 소멸자에서 차단될 수 있다. futs vector 컨테이너에 담긴 하나 이상의 미래 객체들이 std::async 를 통해 시동된 비지연 과제에 대한 공유 상태를 참조할 수도 있기 때문이다.
	class Widget
	{
	private:
		std::shared_future<double>	fut;
	};

	static int calcValues() { return 0;  }
	
	virtual int chapterMain()
	{
		std::packaged_task<int()> pt(calcValues);

		auto fut = pt.get_future();
		std::thread t(std::move(pt));

		// case 1 : t 에 아무 일도 일어나지 않는다 ->  t는 이 스코프의 끝에서 합류 가능한 스레드이다. 따라서 프로그램이 종료된다.
		// case 2 : t 에 대해 join 을 수행한다. -> 애초에 호출 코드에서 join 을 수행한다면, fut 의 소멸자에서는 join 을 수행할 필요가 없으며, 따라서 차단될 이유가 없다.
		// case 3 : t 에 대해 detach 를 수행한다. -> 마찬가지로, 애초에 호출코드에서 detach 를 수행한다면 fut 의 소멸자에서 그것을 수행할 필요가 없다.

		// 결론적으로 공유 상태를 참조하는 미래 객체가 있다면, 소멸자의 특별한 행동을 고려한 코드를 작성 필요가 없다는 것을 위에서는 보여준다.
		// 종료와 합류, 탈착에 관한 결정은 이미 해당 std::thread를 조작하는 코드에서 내려지기 때문이다.
		// 더 정확히 이야기하자면 thread 를 조작하는 코드에서 책임을 지도록 설계하고 코딩하는 것이 바람직하다.

		return 0;
	}
};

/*
	미래 객체의 소멸자는 그냥 미래 객체의 자료 멤버들을 파괴할 뿐이다.

	std::async 를통해 시동된 비지연된 과젱 대한 공유 상태를 참조하는 마지막 미래 객체의 소멸자는 그 과제가 완료될 때까지 기다린다.
*/