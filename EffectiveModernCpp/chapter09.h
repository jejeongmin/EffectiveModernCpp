#pragma once

#include <functional>
#include <memory>
#include <mutex>


/*********************************************************
**********************************************************/

class chapter09 : public chapter
{
	class Widget
	{
	};

	template<typename T>
	struct MyAllocList1 {
		typedef std::vector<T> type;
	};
	// -> 템플릿 내 선언이 훨씬 간단해진다.
	template<typename T>
	using MyAllocList2 = std::vector<T>;

	template<typename T>
	class Change_Trait
	{
		// 템플릿 메타 프로그래밍 시에 템플릿 형식 매개 변수를 받아
		// 적절히 변경한 형식으로 마들어 내야 하는 상황에서 아래 함수들을 활용할 수 있다.
		void change_trait()
		{
			std::remove_const_t<T>;			// const T -> T
			std::remove_reference_t<T>;		// T& / T&& -> T
			std::add_lvalue_reference_t<T>;	// T -> T&
		}

		T	_value;
	};

	virtual int chapterMain()
	{
		typedef std::unique_ptr<std::unordered_map<std::string, std::string>> UPtrMapSS1;
		// ->
		using UPtrMapSS2 = std::unique_ptr<std::unordered_map<std::string, std::string>>;

		typedef void(*FP1) (int, const std::string&);
		//-> 별칭 선언이 조금 더 직관적으로 보인다.
		using FP2 = void(*)(int, const std::string&);

		MyAllocList1<Widget>::type	lw1;
		MyAllocList2<Widget>		lw2;
		
		return 0;
	}
};

/*
	typedef 는 템플릿화를 지원하지 않지만, 별칭 선언은 지원한다.
	
	별칭 템플릿에서는 "::type" 접미어를 붙일 필요가 없다. 템플릿 안에서
	typedef 를 지정할 때에는 "typename" 접두사를 붙여야 하는 경우가 많다.

	C++14 는 C++11 의 모든 형식 특질 변환에 대한 별칭 템플릿들을 제공한다.
*/