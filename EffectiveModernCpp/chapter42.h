#pragma once

#include <iostream>
#include <functional>
#include <memory>
#include <vector>
#include <list>
#include <regex>

/*********************************************************
emplace_back 함수는 주어진 인수를 이용해서 std::vector 안에서 직접 std::string 을 생성한다.
그 과정에 생겨나는 임시객체가 없다.

이런 생성 삽입(emplacement) 함수들이 삽입 함수들보다 성능이 뛰어난 비결은
인터페이스가 더 유연하다는 것이다. 삽입 함수들은 삽입할 객체를 받지만
생성 삽입 함수는 삽입할 객체의 생성자들을 위한 인수들을 받는다.
이러한 차이 때문에 생성 삽입 함수들은 삽입 함수에서는 꼭 필요할 수 있는
임시 객체의 생성과 파괴를 피한다.
**********************************************************/

using namespace std;

class chapter42 : public chapter
{
	class Widget
	{
	};

	virtual int chapterMain()
	{
		std::vector<std::string>	vs;
	
		// 아래 생성삽입 함수들은 생성 삽입함수가 삽입 함수보다 더 나은 선택일 수 있는 조건들을 만족한다.
		// 조건들은 아래 쪽의 요약을 참고할 것
		vs.emplace_back("xyzzy");
		vs.emplace_back(50, 'x');


		std::list<std::shared_ptr<Widget>>	ptrs;

		// push_back 하면서 shared_ptr 객체가 임시로 생성된다.
		// 삽입 과정에서 Widget 에 대한 메모리 할당이 실패하더라도 out of memory 예외가 전파되면서
		// 임시객체가 파괴되고 자동으로 관리되는 widget 객체도 해제된다.
		ptrs.push_back(std::shared_ptr<Widget>(new Widget, [](Widget* pWidget) {}));

		// 실제 list 에 삽입될 때까지, shared_ptr 은 생성되지 않는다.
		// Widget 객체가 먼저 생성되는데, 이때 out of memory 예외가 발생한다면,
		// Widget 객체를 정리해줄 그 무엇도 없다.
		// emplace 계열 함수는 out of memory 예외 안전성이 떨어지는 문제가 있다.
		ptrs.emplace_back(new Widget, [](Widget* pWidget) {});

		// 위 문제를 해결한 구현은 아래와 같다.
		// 결국 생성삽입 함수 본래의 용도로 쓰지 말고, 오른값을 써서 문제를 피해간건데..
		// 이럴 바엔 emplace_ 계열 함수는 out of memory 를 유발할 수 있는 사용자 지정 타입에 대해서는
		// 아예 안쓰는 게 낫지 않나.. 하긴 프로그램 짜면서 out of memory 겪는 일이 얼마나 된다고..
		std::shared_ptr<Widget> spw1(new Widget);
		ptrs.emplace_back(std::move(spw1));

		// 그리고 위의 구현 보다는 아래 구현이 더 안전하다.
		std::shared_ptr<Widget> spw2 = make_shared<Widget>();
		ptrs.emplace_back(std::move(spw2));

		std::vector<std::regex>	regexes;

		// 실행시점에서 c style 문자열로부터 std::regex 를 생성하려면 비교적 큰 비용이 필요할 수 있으므로
		// const char* 를 받는 std::regex 생성자는 explicit 로 선언되어 있다. 그래서 아래 코드는 컴파일이 안된다. 
		//std::regex	r = nullptr;
		//std::regex	r = "[A-Z]+";
		//regexes.push_back(nullptr);
		//regexes.push_back("[A-Z]+");

		// 아래 코드는 컴파일되지만, 동작하는 코드는 아니다.
		// emplace 의 특성상 regex 에 생성자 인수로 전달할 뿐이다.
		regexes.emplace_back(nullptr);
		regexes.emplace_back("[A-Z]+");

		// 생성자를 위한 직접초기화(direct initialization) 과 복사를 통한 복사 초기화(copy initialization)은
		// 서로 구분되어 있으며, explicit 는 복사 초기화만 제한한다.
		// emplace 계열 함수는 직접초기화를 하기 때문에 explicit 의 의한 제약을 받지 않고 컴파일되는 것이다.
		std::regex	r1("[A-Z]+");
		//std::regex	r1 = "[A-Z]+";

		return 0;
	}
};

/*
	이론적으로, 생성 삽입 함수들은 종종 해당 삽입 버전보다 더 효율적이어야 하며,
	덜 효율적인 경우는 절대로 없어야 한다. -> 그러나 표준 구현자들에 의해 그런 경우는 발생한다.

	실질적으로, 만일
		1) 추가하는 값이 컨테이너로 배정되는 것이 아니라 생성되고,
		2) 인수 형식(들)이 컨테이너가 담는 형식과 다르고
		3) 그 값이 중복된 값이어도 컨테이너가 거부하지 않는다면,
	생성 삽입함수가 삽입 함수보다 빠를 가능성이 아주 크다.

	생성 삽입 함수는 삽입 함수라면 거부당했을 형식 변환들을 수행할 수 있다.
*/