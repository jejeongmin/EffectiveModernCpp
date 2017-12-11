#pragma once

#include <functional>
#include <memory>
#include <mutex>

/*********************************************************
동기화해야 할 내부 객체가 하나일 경우에는 std::atomic 을 활용하여 동기화 가능하지만
둘 이상이라면 mutex 를 써야 한다.

mutex 를 쓰건, atomic 을 쓰건, 동기화 객체를 내장하게 되면 객체의 이동/복사는 할 수 없다는 것을 알아두자. 
**********************************************************/

using namespace std;

class chapter16 : public chapter
{
	class Polynomial
	{
	public:
		using RootsType = std::vector<double>;

		RootsType roots1() const
		{
			if (false == rootsAreValid)
			{
				// calcurate roots values
				// rootVals = ...;
				rootsAreValid = true;
			}

			return rootVals;
		}

		RootsType roots2() const
		{
			std::lock_guard<std::mutex> scopeLock(m);

			if (false == rootsAreValid)
			{
				// calcurate roots values
				// rootVals = ...;
				rootsAreValid = true;
			}

			return rootVals;
		}

	private:
		mutable std::mutex m;
		mutable bool rootsAreValid{ false };
		mutable RootsType rootVals{};
	};

	virtual int chapterMain()
	{
		Polynomial	fomulars;

		// roots 함수가 const 이긴 하나, 내부적으로 캐시 갱신을 하면서 내부 멤버를 변경시킨다.
		// multi-thread safe 하지 않다.
		Polynomial::RootsType result1 = fomulars.roots1();

		// 내부적으로 mutex 를 사용하여 multi-thread safe 하다
		Polynomial::RootsType result2 = fomulars.roots2();

		return 0;
	}
};

/*
	동시적 문맥에서 쓰이지 않을 것이 확실한 경우가 아니라면, const 멤버 함수는 스레드에 안전하게 작성하라.

	std::atomic 변수는 뮤텍스에 비해 성능상의 이점이 있지만,. 하나의 변수 또는 메모리 장소를 다룰 때만 적합하다.
*/