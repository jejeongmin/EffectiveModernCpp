#pragma once

#include <functional>
#include <memory>


/*********************************************************
raw pointer 가 충분히 작고 충분히 빠른 상황이라면, std::unique_ptr 역시 그럴 것이 거의 확실하다.

std::unique_ptr 은 독점적 소유권(exclusive ownership) 의미론을 체현하고 있다.
null 이 아닌 std::unique_ptr 은 항상 자신이 가리키는 객체를 소유한다.
std::unique_ptr 를 이동하면 소유권이 원본 포인터에서 대상 포인터로 옮겨진다.
이때 원본 포인터는 null 로 설정된다.
std::unique_ptr 의 복사는 허용되지 않는다.
그런 의미에서 std::unique_ptr 은 이동 전용 형식(move only type)이다.

std::unique_ptr 의 가장 흔한 용도 중 하나는, 계통 구조(hierarchy) 안의
객체를 생성하는 팩토리 함수의 반환 형식으로 쓰이는 것이다.
**********************************************************/

using namespace std;


class Investment {};
class Stock : public Investment {};
class Bond : public Investment {};
class RealEstate : public Investment {};

auto delInvmt = [](Investment* pInvestment)
{
	// custom action
	// ex) makeLogEntry();
	delete pInvestment;
};
/*********************************************************
반환값은 연역이 가능하므로 auto 로 대체하는 것이 훨씬 심플하다.

raw pointer 를 std::unique_ptr 에 배정하는 문장은 컴파일되지 않는다.
그런 문장을 허용한다면, raw pointer 에서 smart pointer 로의 암묵전인 변환이 성립하기 때문이다.
그런 암묵적인 변환에는 문제가 있으므로, c++11 의  smart pointer 들은 그런 변환을 금지한다.
이 때문에 아래 코드에서는 new 로 생성한 객체의 소유권을 pInv 에 부여하기 위해 reset 을 호출하였다.
**********************************************************/
template<typename T>
auto	// std::unique_ptr<Investment, decltype(delInvmt)>
makeInvestment(T&& param)
{
	std::unique_ptr<Investment, decltype(delInvmt)> pInv(nullptr, delInvmt);

	if ( true /* stock 을 생성해야 하는 경우 */)
	{
		pInv.reset(new Stock());
	}
	else if ( false /* Bond 을 생성해야 하는 경우 */)
	{
		pInv.reset(new Bond());
	}
	else if ( false /* RealEstate 을 생성해야 하는 경우 */)
	{
		pInv.reset(new RealEstate());
	}

	return pInv;
}

class chapter18 : public chapter
{
	
	virtual int chapterMain()
	{
		/* 
			팩토리 함수는 자신이 돌려준 객체를 호출자가 독점적으로 소유하려는 것인지,
			아니면 소유권을 공유하고자 하는지(std::shared_ptr 에 해당) 미리 알 수 없다.
			팩터리 함수가 unique_ptr 을 반환한다면 호출자는 유연하게 자신의 용도에 맞게
			변환할 수 있는 여지가 생긴다.
		*/ 
		std::shared_ptr<Investment> sp = makeInvestment<int>(0);

		return 0;
	}
};

/*
	std::unique_ptr 은 독점 소유권 의미론을 가진 자원의 관리를 위한, 작고 빠른 이동 전용 포인터이다.

	기본적으로 자원 파괴는 delete 를 통해 일어나나, 커스텀 삭제자를 지정할 수도 있다.
	상태 있는 삭제자나 함수 포인터를 사용하면 std::unique_ptr 객체의 크기가 커진다.

	std::unique_ptr 를 std::shared_ptr 로 손쉽게 변환할 수 있다.
*/