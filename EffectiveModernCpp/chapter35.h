#pragma once

#include <iostream>
#include <functional>
#include <future>

/*********************************************************
thread 기반 처리에서는 예외가 발생하면 terminate 되는 것에 반해
task 기반 처리에서는 예외가 발생하면 future 객체를 통해 받아올 수 있다.
**********************************************************/

using namespace std;

class chapter35 : public chapter
{
	static int	doAsyncWork()
	{
		return 0;
	}
	
	virtual int chapterMain()
	{
		// thread-based
		std::thread	t(doAsyncWork);

		// task-based
		auto fut = std::async(doAsyncWork);

		// get 을 통해 전달한 비동기 함수의 리턴값을 얻어올 수 있다.
		// future 객체의 get(), wait() 를 실행하면 결과가 필요한 스레드에서 async 함수를 실행하게 된다.
		// 좀 더 추상하된, 유연한 설계를 도와주는 특성이다.
		int result = fut.get();

		return 0;
	}
};

/*
	std::thread API 에서는 비동기적으로 실행된 함수의 반환값을 직접 얻을 수 없으며, 만일 그런 함수가 예외를 던지면 프로그램이 종료된다.

	스레드 기반 프로그래밍에서는 스레드 고갈, 과다구독, 부하 균형화, 새 플랫폼으로의 적응을 독자가 직접 처리해야 한다.

	std::async 와 기본 시동 방침을 이용한 과제 기반 프로그래밍은 그런 대부분의 문제를 알아서 처리해준다.
*/