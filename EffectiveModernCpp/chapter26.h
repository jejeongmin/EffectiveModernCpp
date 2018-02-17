#pragma once

#include <iostream>
#include <functional>
#include <memory>

/*********************************************************
유사한 선언 형식을 가진 템플릿 함수가, 비템플릿 함수의 오버로딩을 먹어버릴 수 있다는 걸
보여주기 위한 예제이다.
책에서는 승격(short -> int ) 을 통한 오버로딩보다 템플릿 매개 변수 일치를 통한 오버로딩이
더 우선 순위가 높은데서 오는 잠재적인 버그를 보여주고 싶었던 듯 한데,
VS 2017 에서는 저자의 의도대로 실행되지 않는다.

심지어 템플릿 함수와 비템플릿 함수가 동일한 선언 형식으로 중복적재되어 있을 때는
비템플릿 함수가 우선한다는 규칙 마저 맞지 않는다.

--->

함수 이름 글자 타이핑이 잘못되어서 오버로딩이 되지 않았던 것이었다는 걸 뒤늦게 발견...
이 장을 읽는 내내, 이렇게 복잡하게 오버로딩을 처리할 바에는 오버로딩을 사용하지 말아야지. 라는 생각을 계속 했는데
오버로딩을 하지 않고, 함수 이름을 달리 해서 처리할 때는 함수 이름을 아주 확실하게 다르게
- postfix 로 명확하게 의도를 네이밍한다던가 - 만들어줘야 한다는 교훈을 얻음.
**********************************************************/

using namespace std;

class chapter26 : public chapter
{
	std::vector<std::string>	names;

	template<typename T>
	void logAndAdd(T&& name)
	{
		std::cout << "log and add" << std::endl;
		names.push_back(std::forward<T>(name));
	}

	std::string nameFromIdx(int idx)
	{
		return "";
	}

	void logAndAdd(int idx)
	{
		std::cout << "log and add" << std::endl;
		names.push_back( nameFromIdx(idx) );
	}

	void logAndAdd(std::string value)
	{
		std::cout << "log and add" << std::endl;
		names.push_back(value);
	}

	virtual int chapterMain()
	{
		std::string petName("Darla");

		logAndAdd(petName);
		logAndAdd(std::string("Persephone"));
		logAndAdd("Patty Dog");

		int value = 22;

		logAndAdd(22);

		return 0;
	}
};

/*
	보편 참조에 대한 중복적재는 거의 항상 보편 참조 중복적재 버전이 예상보다 자주 호출되는 상황으로 이어진다.

	완벽 전달 생성자들은 특히나 문제가 많다. 그런 생성자는 대체로 비 const 왼값에 대한 복사 생성자보다 더 나은 부합이며,
	기반 클래스 복사 및 이동 생성자들에 대한 파생 클래스의 호출들을 가로챌 수 있기 때문이다.
*/