#pragma once

#include <functional>
#include <memory>


/*********************************************************
	std::shared_ptr 의 크기는 raw pointer 의 2 배이다.
	참조 횟수를 담을 메모리를 반드시 동적으로 할당해야 한다.
	참조 회수의 증가와 감소가 반드시 원자적 연산이어야 한다.
**********************************************************/

using namespace std;

class Widget : public std::enable_shared_from_this<Widget>
{
public:
	void process();
};

class chapter19 : public chapter
{
	virtual int chapterMain()
	{
		auto longgingDel = [](Widget *pw)
							{
								// make log entry
								delete pw;
							};

		// 커스텀 삭제자의 형식이 포인터 형식의 일부를 이룬다.
		std::unique_ptr<Widget, decltype(longgingDel)> upw(new Widget, longgingDel);

		// 커스텀 삭제자의 형식이 포인터 형식의 일부가 아니다.
		std::shared_ptr<Widget> spw(new Widget, longgingDel);

		// shared_ptr 은 커스텀 삭제자의 형식이 포인터 형식에 포함되지 않기 때문에, 아래와 같이 서로 다른
		// 커스텀 삭제자를 가진 두 포인터 타입을 같은 컨테이너에 담는 것이 가능하다. 즉 더 유연하다.
		auto customDeleter1 = [](Widget* pw) { /* doing A */ };
		auto customDeleter2 = [](Widget* pw) { /* doing B */ };

		std::shared_ptr<Widget> pw1(new Widget, customDeleter1);
		std::shared_ptr<Widget> pw2(new Widget, customDeleter2);

		std::vector<std::shared_ptr<Widget>> vpw{ pw1, pw2 };

		// 참조 회수는 제어 블록(control block)이라고 부르는 더 큰 자료구조의 일부이다.
		// std::shared_ptr 가 관리하는 객체당 하나의 제어블록이 존재한다.
		// std::shared_ptr 생성 시 커스텀 삭제자를 지정했다면, 참조 횟수와 함께 
		// 그 커스텀 삭제자의 복사본이 제어 블록에 담긴다.

		// 제어 블록은 아래와 같은 규칙에 의해 생성된다고 구현이 유추된다.
		// 1. std::make_shared 는 항상 제어 블록을 생성한다.
		// 2. 고유 소유권 포인터 ( std::unique_ptr 이나 std::auto_ptr ) 로부터 std::shared_ptr 객체를 생성하면 제어 블록이 생성된다.
		// 3. raw pointer 로 std::shared_ptr 생성자를 호출하면 제어 블록이 생성된다.

		// 위 규칙에 의하면 아래 코드는 아주 잘못되었다.
		auto aw = new Widget;

		std::shared_ptr<Widget> spw1(aw);
		std::shared_ptr<Widget> spw2(aw);

		// 위 코드에 의하면 aw 는 spw1, spw2 에 의해 두 개의 제어블록을 생성한다.
		// 하나의 객체에 대해 두 개의 참조 회수가 있으며, aw 의 파괴가 2번 시도된다.
		// 이는 명백히 정의되지 않은 행동이다.
		// 이를 피하기 위해서는 std::shared_ptr 생성자에 raw pointer 를 넘겨주는 일을 피하거나,
		// 이를 피할 수 없다면, 아래의 코드와 같이 차라리 new 의 결과를 직접 전달하라는 것이다.
		std::shared_ptr<Widget> spw3(new Widget);

		return 0;
	}
};

std::vector<std::shared_ptr<Widget>> processedWidgets;

/*
	std::shared_ptr 로 관리되는 컨테이너에 this 를 이용해서, emplace_back - 생성 후 삽입 - 하는 방식은 잘못된 방식이다.
	이 코드는 컴파일은 되지만, raw pointer 인 this 를 이용해서 std::shared_ptr 들의 컨테이너에 넘겨준다는 문제점이 있다.
	이 때문에 피지칭된 Widget 객체 대한 새 제어 블록이 만들어지게 되고, 이는 잘못된 참조 카운터의 원인이 된다.

	shared_ptr API 에는 바로 이런 상황에 특정한 수단을 포함하고 있다.
	std::enable_shared_from_this 라는 템플릿인데, 그 클래스의 this 포인터로부터 std::shared_ptr 을 안전하게 생성하려면
	이 템플릿을 그 클래스의 기반 클래스로 삼으면 된다.
	std::enable_shared_from_this 는 현재 객체를 가르키는 std::shared_ptr 을 생성하되 제어 블록을 복제하지는 않는 멤버 함수 하나를 정의한다.
	그 멤버 함수의 이름은 shared_from_this() 이다.
*/
void Widget::process()
{
	//processedWidgets.emplace_back(this);				// 잘못된 예
	processedWidgets.emplace_back(shared_from_this());	// 바른 예
}

/*
	shared_ptr 로 인한 비용은 어느 정도 있지만, 이득을 생각하면 그 비용은 충분히 감내할만하다.
	그럼에도 불구하고 비용이 걱정된다면, 우선은 소유권 공유가 꼭 필요한지부터 생각해볼 필요가 있다.
	독점 소유권(unique_ptr) 로 충분하다면, 심지어는 반드시 충분하지 않다하더라고
	unique_ptr 로 해두면 shared_ptr 로 '업그레이드' 하기는 쉽다.
	다만 그 반대는 참이 아니므로, 주의할 필요가 있다.

	그리고 shared_ptr 로 할 수 없는 일로는 배열 관리가 있다.
	std::unique_ptr 와의 또 다른 차이점은, std::shared_ptr 의 API 가 단일 객체를 가르키는 포인터만
	염두에 두고 설계되었다는 사실이다. std::shared_ptr<T[]> 같은 건 없다.
	이럴 경우 내장 배열에 대한 다양한 대안 - 이를테면 array, vector, string - 을 제공한다.
	array 가 어디에 필요할 까 싶었더니, shared_ptr 과 결부해 사용하는 경우에는 배열대신 좋은
	용례가 될 수 있겠다.

	std::shared_ptr 는 임의의 공유 자원의 수명을 편리하게 (가비지컬렉션에 맡길 때만큼이나) 괸리할 수 있는 수단을 제공한다.

	대체로 std::shared_ptr 객체는 그 크기가 std::unique_ptr 객체의 두 배이며, 제어 블록에 관련된 추가 부담을 유발하며,
	원자적 참조 횟수 조작을 요구한다.

	자원은 기본적으로 delete 를 통해 파괴되나, 커스텀 삭제자도 지원된다. 삭제자의 형식은 std::shared_ptr 의
	형식에는 아무런 영향도 미치지 않는다.

	raw pointer 형식의 변수로부터 std::shared_ptr 를 생성하는 일은 피해야 한다.

	by jeongminje
	
	shared_ptr 을 by ref 로 전달할 때는 ref count 가 증감하지 않는다.
	단순 전달하는 경우에는 속도를 위해서는 이가 최선이나, 소유권을 공유하기 위해서 넘길 때는
	반드시 by copy 에 의해 넘겨야 한다.

	shared_ptr 은 참조 카운터 자체(더 정확히는 제어블럭에 대해서)에 대해서는 멀티스레드를 지원하나,
	참조되는 객체 자체에 대한 멀티스레드 접근을 지원하는 건 아니다. ( 2017.12 현재 시점, c++20 에서는 지원 예정 )

	https://www.slideshare.net/zzapuno/multithread-sharedptr
	http://en.cppreference.com/w/cpp/memory/shared_ptr
	https://stackoverflow.com/questions/44552738/race-condition-in-shared-ptr-doesnt-happen
*/