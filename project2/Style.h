#pragma once
#include "pch.h"

template<typename DataType>
struct Style
{
	Style(size_t size, DataType data);

	size_t Size;
	DataType Data;
};

template<typename DataType>
inline Style<DataType>::Style(size_t size, DataType data) : Size(size), Data(data)
{
}