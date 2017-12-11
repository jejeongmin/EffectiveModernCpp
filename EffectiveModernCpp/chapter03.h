#pragma once

#include <vector>

/*********************************************************
여기서는 C+11 의 후행 반환 형식(trailing return type) 이 사용되었다.
다시 말해서 함수의 반환 형식을 이 위치가 아니라 매개변수 목록 다음에
-> 다음 위치에 선언하겠다고 나타내는 것이다.
이러한 후행 반환 형식 구문에는 반환 형식을 매개변수들을 이용해서
지정할 수 있다는 장점이 있다.
**********************************************************/
template<typename Container, typename Index>
auto authAndAccess1(Container& c, Index i) -> decltype(c[i])
{
	return c[i];
}

/*********************************************************
C++14 에서는 일반함수는 물론 람다 함수도 auto 반환 형식에 대해
형식 연역을 지원한다. 따라서 위와 같은 후행 반환 형식이 필요 없다.
**********************************************************/
template<typename Container, typename Index>
auto authAndAccess2(Container& c, Index i)
{
	return c[i];
}

/*********************************************************
위에서 auto 반환 형식을 활용한 형식 연역은 참조라던가 const 등
형식 연역에서 무시되는 속성으로 인해 형식 불일치가 있을 수 있다.
이를 decltype 도입을 통해 해결한다.
**********************************************************/
template<typename Container, typename Index>
decltype(auto) authAndAccess3(Container& c, Index i)
{
	return c[i];
}

/*********************************************************
극단적인 사용 예시이긴 하나, 첫번째 매개변수가 오른값일 경우,
전달될 수 없다. 이를 보편 참조 지정자 - && - 를 통해 해결한다.
**********************************************************/
template<typename Container, typename Index>
decltype(auto) authAndAccess4(Container&& c, Index i)
{
	return c[i];
}

/*********************************************************
아직 공부하지 않았지만 chapter25 의 내용에 따르면
보편 참조는 std::forward 를 적용하라고 한다.
**********************************************************/
template<typename Container, typename Index>
decltype(auto) authAndAccess5(Container&& c, Index i)
{
	return std::forward<Container>(c)[i];
}

/*********************************************************
decltype 은 주어진 이름이나 표현식의 양식을 알려준다.
**********************************************************/
class chapter03 : public chapter
{
	virtual int chapterMain()
	{
		const int	i = 0;
		decltype(i)	di = 0;			// const int
		decltype((di))	rdi = i;	// const int&, 소괄호로 묶은 것만으로도 변수가 아닌 표현식이 되어
									// 연역 변환 결과가 달라진다. decltype 은 대체적으로 직관적인 결과를
									// 주지만 때로는 이와 같이 예상외의 결과를 주기도 한다.

		decltype (1 == 1) boolean;	// 표현식이 그 대상이 되기도 한다.

		struct Point
		{
			int x;
			int y;
		};

		Point	xy;

		decltype(xy)	txy;	// Point
		decltype(Point::x)	tx;	// int

		std::vector<int>		v;
		v.push_back(0);

		decltype(v[0]) tv = xy.x;	// int& 를 반환하기 때문에 왼값 참조만 우변에 올 수 있음

		return 0;
	}
};


/*
	기억해 둘 사항들

	decltype 은 항상 변수나 표현식의 형식을 아무 수정 없이 보고한다.

	decltype 은 형식이 T 이고 이름이 아닌 왼값 표현식에 대해서는 항상 T& 형식을 보고한다.

	C++14 는 decltype(auto)를 지원한다. decltype(auto) 는 auto 처럼 초기치로부터 형식을 연역하지만
	그 형식 연역 과정에서 decltype 규칙들을 적용한다.

*/