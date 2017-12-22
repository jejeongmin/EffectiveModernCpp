#pragma once

#include <functional>
#include <memory>
#include <unordered_map>

/*********************************************************
std::function 을 사용하는 것과 auto 를 사용하는 것 사이에는,
전자의 경우 구문이 장황하고 매개 변수를 반복해서 지정해야 한다는 외견상의 차이보다
더욱 중요한 차이가 있다.

첫째, auto 로 선언된, 그리고 그 클로저를 담는 변수는 그 클로저와 같은 형식이며,
따라서 그 클로저에 요구되는 만큼의 메모리만 사용한다.
그러나 클로저를 담는 std::function 으로 선언된 변수의 형식은 std::function 템플릿의 한 인스턴스이며,
그 크기는 임의의 주어진 서명에 대해 고정되어 있다. 그런데 그 크기가 클로저 요구된 클로저를 저장하기에
부족할 수 있으며, 그런 경우 std::function 객체는 힙 메모리를 할당해서 클로저를 저장한다.
결과적으로, 대체로 std::function 객체는 auto 로 선언된 객체보다 메모리를 더 많이 소비한다.
그리고 인라인화를 제한하고, 간접 함수 호출을 산출하는 구현 세부사항 때문에 거의 항상 더 느리다.

둘째, type shortcut 되는 것을 막을 수 있다.
예를 들어 vector::size() 는 std::vector<T>::size_type 을 반환하는데
이는 x86 /x64 에 따라 크기가 달라질 수 있다.
명시적으로 형을 사용하면, platform build target 에 따라 값이 잘릴 수 있는데,
auto 를 사용하면 그런 문제를 미연에 해결할 수 있다.

세째, 잘못된 암묵적인 변환으로 인한 성능저하를 방지할 수 있다

**********************************************************/
class chapter05 : public chapter
{
	class Widget
	{};

	virtual int chapterMain()
	{
		int	x1;
		//auto x2;		auto 변수는 반드시 초기치가 제공되어야, 형식 연역이 가능합니다.
		auto x3 = 0;

		auto x4 = 0ull;

		// C++14 에서는 람다 표현식의 반환값, 매개변수 모두에 auto 적용이 가능합니다.
		auto derefLess = [](const auto& p1, const auto& p2) { return *p1 < *p2; };

		// 첫째 사례 예시
		// 람다 이전에는 std::function, std::bind 등을 통해 호출 가능한 객체를 저장했다.
		// 더 이전에는, 함수 포인터를 활용하곤 했다.
		std::function<bool(const std::unique_ptr<Widget>&, const std::unique_ptr<Widget>&)> func;

		// 둘째 사례 예시
		std::vector<int> v;
		unsigned sz1 = v.size();
		auto sz2 = v.size();

		// 세째 예시
		std::unordered_map<std::string, int> m;
		for (const std::pair<std::string, int>& p : m)
		{
			// 위 해시테이블에서 key 에 해당하는 부분은 const 라고 명세가 정해져 있다.
			// 그런데 loop 내, std::pair 에서는 const 가 아닌 string 이기 때문에
			// 컴파일러는 매 루프마다 이를 정확히 타입을 맞추려 한다.
			// 이 과정에서 암묵적 변환으로 인한, 임시 객체가 생성될 수 있으며,
			// 이는 루프에서 container 를 참조로 접근하려 했던 프로그래머의 의도와는 명백히 다르다.
		}

		for (const auto& p : m)
		{
			// auto 를 활용하면, 위의 문제를 깔끔히 해결할 수 있다.
		}

		return 0;
	}
};

/*
	사실 형식을 명시적으로 표기하는 것은 그냥 정확성 또는 효율성 면에서 미묘한 실수의 여지가 될 뿐,
	별다른 득이 없다. 더 나아가서, auto 형식들은 초기화 표현식의 형식이 변하면 자동으로 변한다.
	이는 auto 를 사용하면 리팩토링이 어느 정도 수월해짐을 의미한다.

	auto 변수는 반드시 초기화해야 하며, 이식성 또는 효율성 문제를 유발할 수 있는 형식 불일치가
	발생하는 경우가 거의 없으며, 대체로 변수의 형식을 명시적으로 지정할 때보다 더 적다.
	
	auto 로 형식을 지정한 변수는 항목 2와 항목 6에서 설명한 문제점들을 겪을 수 있다.
*/