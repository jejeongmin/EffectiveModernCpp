#pragma once

template<typename T>
class TD;

/*********************************************************
	아래의 모든 수단들을 표준에서 유용한 정보를 출력하도록 권고할 뿐,
	정확한 정보를 반드시 출력할 것을 강제하고 있지는 않다.
	따라서 아래 정보를 100% 신뢰할 순 없으므로, 형식 연역에 대한 규칙은 여전히
	프로그래머가 정확히 알아야 할 지식이다.

	runtime 타입 정보 확인시 표준에 포함된 typeid 이외에도
	비표준이긴 하나 boost::typeindex::type_id_with_cvr 도 사용할 수 있다.
**********************************************************/
class chapter04 : public chapter
{
	class Widget
	{
	};

	template<typename T>
	void f(const T& param)
	{
		std::cout << "T = " << typeid(T).name() << std::endl;
		std::cout << "param = " << typeid(param).name() << std::endl;
	}


	std::vector<Widget> createVec()
	{
		std::vector<Widget>	vw;
		
		vw.push_back(Widget());
		vw.push_back(Widget());

		return vw;
	}

	virtual int chapterMain()
	{
		const int	theAnswer = 42;

		// 바로 아래 2줄과 같이 컴파일 되는 코드이면 intellisense 를 이용해 형식 정보가 보여진다.
		auto x = theAnswer;
		auto y = &theAnswer;

		// 바로 아래 코드와 같이 일부러 컴파일 오류를 일시 발생시켜, 컴파일러의 진단 메시지를 활용.
		//TD<decltype(x)> xType;

		// 런타임시, typeid 를 이용해 형식 정보를 확인할 수 있다.
		std::cout << " type x is " << typeid(x).name() << std::endl;

		const auto vw = createVec();

		if (!vw.empty())
		{
			f(&vw[0]);
		}

		return 0;
	}
};

/*
	컴파일러가 연역하는 형식을 IDE 편집기나 컴파일러 오류 메시지, Boost TypeIndex 라이브러리를
	이용해서 파악할 수 있는 경우가 많다.

	일부 도구의 결과는 유용하지도 않고 정확하지도 않을 수 있으므로,
	C++ 형식 연역 규칙들을 제대로 이해하는 것은 여전히 필요한 일이다.
*/