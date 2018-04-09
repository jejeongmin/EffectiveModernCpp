#pragma once

#include <iostream>
#include <functional>
#include <memory>
#include <vector>

/*********************************************************	
**********************************************************/

using namespace std;

class chapter41 : public chapter
{
	class WidgetForOverload
	{
	public:
		void addName(const std::string& newName)
		{
			names.push_back(newName);
		}

		void addName(const std::string&& newName)
		{
			names.push_back(std::move(newName));
		}

	private:
		std::vector<std::string> names;
	};

	template<typename T>
	class WidgetForGeneralRef
	{
	public:
		void addName(T&& newName)
		{
			names.push_back(std::forward<T>(newName));
		}

	private:
		std::vector<T> names;
	};

	class WidgetForValue
	{
	public:
		void addName(const std::string newName)
		{
			names.push_back(std::move(newName));
		}

		/*
			값 전달은 항상 복사되는 매개 변수에 대해서만 고려하라는 이야기는 다음과 같다.
			매개변수로 기껏 값을 복사해서 전달했더니, 결국에는 어떤 조건에 의해서 사용하지 않게 되는 경우에는 쓰지 말라는 이야기.
		*/
		void addNameNotRecommended(const std::string newName)
		{
			int	minLength = 8, maxLength = 128;

			if (newName.length() < minLength && newName.length() > maxLength)
				return;

			names.push_back(std::move(newName));
		}

	private:
		std::vector<std::string> names;
	};

	virtual int chapterMain()
	{
		return 0;
	}
};

/*
	이동이 저렴하고 항상 복사되는 복사 가능 매개변수에 대해서는 값 전달이 참조 전달만큼이나
	효율적이고, 구현하기가 쉽고, 산출되는 목적 코드의 크기도 작다.

	왼값 인수의 경우 값 전달(즉, 복사 생성) 다음의 이동 배정은 참조 전달 다음의
	복사 배정보다 훨씬 비쌀 가능성이 있다.
	-> 이건 vector 와 같이 새로운 배정으로 인해 (기존 메모리 해제) -> ( 새로운 메모리 할당 ) 의
	동작이 쌍으로 일어나는 경우를 말한다. 이 경우는 매개 변수 이동 방식과 특정 자료구조의 재할당 방식이
	화학작용을 일으켜 발생하는 이슈인데, 책에서 매개 변수 이동의 특수 케이스로 설명하기에는
	예제로서 부적합하다. 책 늘려 쓰기다.
	41장과 42장은 찟어서 화장실 휴지로 써도 상관없음.

	값 전달에서는 잘림 문제가 발생할 수 있으므로, 일반적으로 기반 클래스 매개 변수 형식에
	대해서는 값 전달이 적합하지 않다.
	-> 객체 다형성으로 인해 발생할 수 있는 문제를 어렵게 설명해놨음...
*/