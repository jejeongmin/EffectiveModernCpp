#pragma once

#include <iostream>
#include <functional>
#include <memory>

/*********************************************************
	��κ��� container �� ��� �̵������� �� ���, ���� ���� �����͸� �����ϰ� �ǹǷ�,
	��� �ð��� ó���ȴ�.

	������ �Ϻ� �����̳� - array - ������ �� ���Ұ� �̵��ȴ�.
	�� ��쿡�� ������ ���� ������ �̵��� ���纸�� �����ٸ�,
	�̵� ������ �� �����ٴ� ������ ������������, ��� �ð��� ó�������� �ʰ�, ���� �ð��� ó���ȴٴ� ���� �˰� �־�� �Ѵ�.

	���� ���ڿ� ���� �߿� SSO ( small string optimazation ) �� ���� ��ü�� ���� �ȿ� ����ȭ�� ���� ���� ���ڿ��� �����ϰ� �ִ�
	Ư���� ���� ���� �����Ϸ��� ����ȭ�� �����ϱ⵵ �Ѵ�.
**********************************************************/

using namespace std;

class chapter29 : public chapter
{
	class Widget
	{};

	virtual int chapterMain()
	{
		std::vector<Widget>		vw1;
		for (int i = 0; i < 10000; ++i)
		{

			vw1.push_back(Widget());
		}
		auto vw2 = std::move(vw1);

		std::array<Widget, 10000> aw1;
		for (int i = 0; i < 10000; ++i)
		{
			aw1[i] = Widget();
		}
		auto aw2 = std::move(aw1);

		return 0;
	}
};

/*
	�̵� ������� �������� �ʰ�, �������� �ʰ�, ������� ���� ���̶�� �����϶�

	���ĵ�� �̵� �ǹ̷� ���� ���θ� �̸� �� �� �ִ� ��쿡�� �׷� ������ �� �ʿ䰡 ����.
*/