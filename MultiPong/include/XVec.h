#pragma once

#ifndef XVEC_H
#define XVEC_H

#include <vector>

template<typename T, unsigned int SIZE>
class XVec
{
public:
	XVec()
	{
		firstIndex = 0;
		vector.reserve(SIZE);
		for (int i = 0; i < SIZE; i++)
			vector.push_back(T());
	}

	~XVec()
	{

	}

private:
	unsigned int firstIndex;
	std::vector<T> vec;

public:

	T& operator [](size_t index)
	{
		return vec[(index + firstIndex) % SIZE];
	}

	void operator ++()
	{
		if (++firstIndex == SIZE)
			firstIndex = 0;
	}

	void operator ++(int)
	{
		if (++firstIndex == SIZE)
			firstIndex = 0;
	}

	void operator --()
	{
		if (--firstIndex > SIZE)
			firstIndex = SIZE - 1;
	}

	void operator --(int)
	{
		if (--firstIndex > SIZE)
			firstIndex = SIZE - 1;
	}
	

};

#endif
