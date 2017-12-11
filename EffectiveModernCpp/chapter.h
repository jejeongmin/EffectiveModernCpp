#pragma once

class chapter
{
public:
	virtual int		chapterMain() = 0;

	static std::shared_ptr<chapter> makeChapter(int index);
};