#pragma once

#include <functional>
#include <memory>

/*********************************************************
**********************************************************/

class chapter13 : public chapter
{
	// 이 코드는 가장 일반적인 코드이지만, 엄밀하게 의미론적인 측면에서 개선이 필요하다.
	// 실제 iterator 가 가리키는 것을 전혀 수정하지 않았음에도 const_iterator 를 사용하지 않았다.
	void cpp_98()
	{
		std::vector<int> values;
		std::vector<int>::iterator it = std::find(values.begin(), values.end(), 1983);
		values.insert(it, 1998);
	}

	// 위의 코드를 정확하게 수정하면 아래와 같다.
	void cpp_14()
	{
		std::vector<int> values;
		auto it = std::find(values.cbegin(), values.cend(), 1983);
		values.insert(it, 1998);
	}
	
	virtual int chapterMain()
	{
		return 0;
	}
};

/*
	iterator 보다 const_iterator 를 선호하라.

	최대한 일반적인 코드에서는 begin, end, rbegin 등의 비멤버 버전들을 해당 멤버 함수들보다 선호하라.
*/