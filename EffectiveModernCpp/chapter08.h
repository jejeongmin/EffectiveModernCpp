#pragma once

#include <functional>
#include <memory>
#include <mutex>


/*********************************************************
예제 코드에서 구현한 lockAndCall 템플릿 함수를 보면 함수에 대한
주소가 필요하기 때문에 f1, f2, f3 를 static 으로 선언하였다.
**********************************************************/

template<typename FuncType, typename MuxType, typename PtrType>
decltype(auto) lockAndCall(FuncType func, MuxType& mutex, PtrType ptr)
{
	using MuxGuard = std::lock_guard<MuxType>;

	MuxGuard	g(mutex);

	return func(ptr);
}

class chapter08 : public chapter
{
	class Widget
	{
	};

	static int f1(std::shared_ptr<Widget> spw)
	{
		return 0;
	}

	static double f2(std::unique_ptr<Widget> upw)
	{
		return 0.f;
	}

	static bool	f3(Widget* pw)
	{
		return true;
	}

	virtual int chapterMain()
	{
		std::mutex	f1m, f2m, f3m;

		using MuxGuard = std::lock_guard<std::mutex>;

		{
			MuxGuard	g(f1m);
			auto result = f1(0);		// 0 을 널 포인터로써 f1 에 전달

			//auto result1 = lockAndCall(f1, f1m, 0);	// 0을 shared_ptr<Widget> 로 연역할 수 없다.
		}

		{
			MuxGuard	g(f2m);
			auto result = f2(NULL);		// NULL 을 널 포인터로써 f1 에 전달
		
			//auto result2 = lockAndCall(f2, f2m, NULL); // NULL 을 unique_ptr<Widget> 로 연역할 수 없다.
		}

		{
			MuxGuard	g(f3m);
			auto result = f3(nullptr);	// nullptr 을 널 포인터로써 f1 에 전달

			auto result3 = lockAndCall(f3, f3m, nullptr);
		}

		return 0;
	}
};

/*
	0 과 NULL 보다 nullptr 을 선호하라
	
	정수형식과 포인터 형식에 대한 중복적재를 피하라.
*/