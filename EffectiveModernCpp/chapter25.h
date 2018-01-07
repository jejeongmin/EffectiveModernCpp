#pragma once

#include <functional>
#include <memory>

/*********************************************************
보편 참조를 받는 하나의 템플릿을 왼값 참조와 오른값 참조들에 대해 중복적재한 두 개의 함수로 대체하면
실행시점의 추가 비용을 유발할 가능성이 큰 경우가 있다는 점은 사실이다.
더 큰 문제는 인수를 여러 개, 심지어 무한대로 받는 경우 사실상 중복 적재 구현이 유지보수 측면에서 좋은 선택이 아니라는 것이다, 타이핑 양도 그렇고.
결국 유일한 대안은 보편 참조이다.
**********************************************************/

using namespace std;

class chapter25 : public chapter
{
	class Widget
	{
	public:
		template<typename T>
		void setName(T&& newName)
		{
			_name = std::move(newName);
		}

	private:
		std::string _name;
	};

	/*
		함수가 결과를 값으로 돌려준다면( return by value ), 그리고 그것이 오른값 참조나 보편 참조에 묶인 객체라면,
		해당 참조를 돌려주는 return 문에서 std::move 나 std::forward 를 사용하는 것이 바람직하다.
	*/
	class Matrix
	{
		friend Matrix	operator+(Matrix&& lhs, const Matrix& rhs)
		{
			lhs._x += lhs._x;
			lhs._y += lhs._y;
			return std::move(lhs);
		}

	private:
		int		_x, _y;
	};
	
	/*
		아래 함수 구현은 구현 그대로라면 지역변수를 복사해서 반환하는 것처럼 보이나
		컴파일러의 반환값 최적화( RVO :  Return Value Optimazation ) 을 통해 복사를 피한다.
		컴파일러가 스스로 최적화를 하는 것이다.
	*/
	Widget makeWidget_right_implementation()
	{
		Widget	w;
		return w;
	}

	/*
		아래 함수는 지역변수의 복사를 피하기 위해, 나름 최적화를 시도한 것이나 의도대로 동작하지 않는다.
		컴파일러가 RVO 를 수행하기 위한 전제 조건이 반환값이 지역변수어야 하는데, 프로그래머의 최적화에 의해
		작성자의 의도와 달리 RVO 를 수행못하게 된다. RVO 에 의한 최적화가 std::move 에 의한 이동 비용보다 더 효율적이기 때문에
		가급적이면, RVO 를 활용하는 것을 권고한다.

	*/
	Widget makeWidget_wrong_implementation()
	{
		Widget	w;

		return std::move(w);
	}

	

	virtual int chapterMain()
	{
		Widget	w;
		auto	n = "jeongminje";

		std::cout << "" << n << std::endl;

		w.setName(n);

		std::cout << n << std::endl;

		return 0;
	}
};

/*
	오른값 참조나 보편 참조가 마지막으로 쓰이는 지점에서, 오른값 참조에는 std::move 를, 보편 참조에는 std::forward 를 적용하라

	결과를 값 전달 방식으로 돌려주는 함수가 오른값 참조나 보편 참조를 돌려줄 때에도 각각 std::move 나 std::forward 를 적용하라

	반환값 최적화의 대상이 될 수 있는 지역 객체에는 절대로 std::move 나 std::forward 를 적용하지 말아야 한다.
*/