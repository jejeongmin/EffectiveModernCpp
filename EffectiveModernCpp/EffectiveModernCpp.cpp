// EffectiveModernCpp.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "chapter.h"


int main()
{
	int		chaterNumber;

	std::cout << "input chater number : ";
	std::cin >> chaterNumber;

	auto chapter = chapter::makeChapter(chaterNumber);

	if (chapter != nullptr)
	{
		chapter->chapterMain();
	}
	
    return 0;
}

