#include "stdafx.h"
#include <vector>
#include "chapter22.h"

struct chapter22::Widget::Impl
{
	std::string	name;
	std::vector<double>	data;
	int	value;
	// etc custom type..
};

chapter22::Widget::Widget()
{
	_impl = make_unique<Impl>();
}

/*
	소멸자를 구현하지 않으면, 컴파일러가 기본 소멸자를 생성하게 된다.
	그런데 소멸자 구현 내에 unique_ptr 이 있고, 이 unique_ptr 의 기본 할당해제자의 구현은
	품고 있는 raw pointer 에 대한 delete 동작인데, 
	chapter22:Widget::impl 의 경우 선언만 존재하는 in-complete type 이다.
	vs2017 구현에서는 이런 경우 컴파일러가 자동 생성을 하지 않게끔 구현된 것 같다.

	복사 생성자, 배정 연산자 모두 마찬가지 것 같다.
*/
chapter22::Widget::~Widget() = default;

chapter22::Widget::Widget(chapter22::Widget&& rhs) = default;
chapter22::Widget& chapter22::Widget::operator=(chapter22::Widget&& rhs) = default;

chapter22::Widget::Widget(const chapter22::Widget& rhs) : _impl(nullptr)
{
	if (rhs._impl)
	{
		_impl = std::make_unique<Impl>(*rhs._impl);
	}
}

chapter22::Widget& chapter22::Widget::operator=(const chapter22::Widget& rhs)
{
	if (nullptr == rhs._impl)
	{
		_impl.reset();
	}
	else if (nullptr == _impl)
	{
		_impl = std::make_unique<Impl>(*rhs._impl);
	}
	else
	{
		*_impl = *rhs._impl;
	}

	return *this;
}


int chapter22::chapterMain()
{
	chapter22::Widget	w1,w2;

	w1 = w2;

	return 0;
}


/*
	impl 관용구는 클래스 구현과 클래스 클라이언트 사이의 컴파일 의존성을 줄임으로써 빌드 시간을 감소한다.

	std::unique_ptr 형식의 impl 포인터를 사용할 때에는 특수 멤버 함수들을 클래스 헤더에 선언하고 구현 파일에서 구현해야 한다.
	컴파일러가 기본으로 작성하는 함수 구현들이 사용하기에 적합한 경우에도 그렇게 해야 한다.

	위의 조언들은 std::unique_ptr 에 적용될 뿐, std::shared_ptr 에는 적용되지 않는다.
*/