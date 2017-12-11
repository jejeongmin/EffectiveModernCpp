#pragma once

#include <functional>
#include <memory>

/*********************************************************
	delete 를 이용하면 member function 뿐 아니라 일반 function 도 지정할 수 있다.
	또한 의미론 적으로도 access level 을 통해 제어하는 게 아니라
	실제 delete 라는 의미를 부여해 접근을 제어할 수 있다.

	음.. 하지만 좀 더 테스트해보니 함수에 delete 를 지정하면, 해당 함수의 구현 코드도 제거해야 하는데,
	어차피 함수 구현 코드를 제거하면, 빌드 때 링킹 에러가 나서 그 함수는 사용 못한다.
	delete 는 단지 링킹 타임의 에러를 컴파일 타임의 에러로 옮기고, delete 라는 의미만 부여한 거네.
	별 쓸모 없어보이는데..
	어차피 구현 코드도 지울 거.. 그냥 delete 붙이지 말고 지워버리면 되잖아..
**********************************************************/

class chapter11 : public chapter
{
	class Widget
	{
	public:
		void	public_test() = delete;
	private:
		void	private_test() = delete;

		friend class chapter11;
	};

	void isLucky(int number);
	void isLucky(char number) = delete;	
	void isLucky(bool number) = delete;
	void isLucky(double number) = delete;

	// 삭제된 함수로 수행할 수 있는 유일한 장점은 원치 않은 템플릿의 인스턴스화를 막을 수 있다는 점으로 보인다.
	template<typename T>
	void processPointer(T* ptr);

	template<typename T>
	void processPointer(void* ptr) = delete;
	template<typename T>
	void processPointer(const void* ptr) = delete;
	template<typename T>
	void processPointer(char* ptr) = delete;
	template<typename T>
	void processPointer(const char* ptr) = delete;
	
	virtual int chapterMain()
	{
		Widget	test;

		//test.public_test();
		//test.private_test();
		
		isLucky(10);
		//isLucky('a');
		//isLucky(true);
		//isLucky(3.5);

		return 0;
	}
};

void chapter11::isLucky(int number)
{
}

//void chapter11::isLucky(char number)
//{
//}

//void chapter11::isLucky(bool number)
//{
//}
//
//void chapter11::isLucky(double number)
//{
//}

/*
	정의되지 않은 비공개 함수보다 삭제된 함수를 선호하라.

	비멤버 함수와 템플릿 인스턴스를 비롯한 그 어떤 함수도 삭제할 수 있다.
*/