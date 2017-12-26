#pragma once

#include <functional>
#include <memory>


/*********************************************************
std::move 와 std::forward 는 그냥 캐스팅을 수행하는 함수(구체적으로는 함수 템플릿)이다.
std::move 는 주어진 인수를 무조건 오른값으로 캐스팅하고,
std::forward 는 특정 조건이 만족될 때에만 그런 캐스팅을 수행한다.
**********************************************************/

using namespace std;

class chapter23 : public chapter
{
	// std::move 구현은 대략 아래와 같음. 별 거 아님. 캐스팅이 전부
	template<typename T>
	decltype(auto) move(T&& param)
	{
		using ReturnType = remove_reference_t<T>&&;
		return static_cast<ReturnType>(param);
	}

	class Annotation
	{
	public:
		explicit Annotation()	{}

		explicit Annotation(const std::string text) : _value(std::move(text))
		{
			// 위는 작성자의 의도와는 다르게 _value 로 text 가 이동하는게 아니라 복사 된다.
			// 이유는 std::string 의 생성자를 보면 알수 있는데, const string 일 경우, 복사 생성자만 정의되어 있기 때문이다.
			// string&& 에 대한 이동 생성자가 정의되어 있긴 하지만, const string 복사 생성자가 더 오버로딩 조건에 부합하기 때문에 우선 호출된다.
			// 기본적으로 '이동'이란 동작 자체가 객체의 수정을 전제로 하기 때문에 const 에 대해서는 이동보다는 복사가 우선되는 게 타당하다.

			// 이 예에서 배울 점이 두 가지 있다.
			// 첫째, 이동을 지원할 객체는 const 로 선언하지 말아야 한다. const 객체에 대한 이동 요청은 소리없이 복사연산으로 변환된다.
			// 둘째, std::move 는 아무것도 실제로 이동하지 않을 뿐 아니라, 캐스팅되는 객체가 이동 자격을 갖추게 된다는 보장도 제공하지 않는다.
		}

	private:
		std::string		_value;
	};

	void process(const Annotation& lvalArg) {}
	void process(Annotation&& rvalArg) {}

	/*
		함수 매개변수는 결국 하나의 왼값이다. 그래서 std::forward 를 통해 전달하지 않으면
		void process( lvalArg ) 만 호출된다.
		std::forward 는 매개변수가 왼값이면 왼값으로, 오른값이면 오른값으로 그 형질 그대로 오른값으로 캐스팅해서 전달한다.
		그래서 std::forward 를 조건부 캐스팅을 수행한다고 하는 것이다.
	*/
	template<typename T>
	void logAndProcess(T&& param)
	{
		// log entry
		process(std::forward<T>(param));
	}

	virtual int chapterMain()
	{
		Annotation	w;

		logAndProcess(w);

		// std::move 와 std::forward 는 둘 다 결국 캐스팅만 수행하는 함수이고, 둘의 유일한 차이는 std::move 는 항상 캐스팅하지만 std::forward 는 조건에 따라서만 한다는 점을 생각하면,
		// std::move 는 아예 잊어버리고 항상 std::forward 만 사용하면 되지 않느냐는 의문이 들 수도 있다. 순수하게 기술적인 관점에서는 맞다.
		// 즉 std::forward 로 모든 것을 할 수 있다. 그러나 std::move 의 매력은 아래 코드에서 보드이 더 짧고, 오류의 여지가 적다는 것이다. 코드의 명확성도 높아진다
		logAndProcess(std::move(w));
		logAndProcess(std::forward<Annotation>(w));

		return 0;
	}
};

/*
	std::move 는 오른값으로의 무조건 캐스팅을 수행한다. std::move 자체는 아무 것도 이동하지 않는다.

	std::forward 는 주어진 인수가 오른값에 묶인 경우에만 그것을 오른값으로 캐스팅한다.

	std::move 와 std::forward 둘 다, 실행 시점에서는 아무 일도 하지 않는다.
*/