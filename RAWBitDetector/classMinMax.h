#pragma once

#include <cstddef>
#include <iomanip>
#include <iostream>
#include <limits>

template <typename ClassType>
class classMinMax
{
public:
	classMinMax()
	{
		Reset();
	}

	ClassType GetMaxValue()
	{
		return MaxValue;
	}

	ClassType GetMinValue()
	{
		return MinValue;
	}

	void Calculate(ClassType value)
	{
		if (MinValue > value)
		{
			MinValue = value;
		}

		if (MaxValue < value)
		{
			MaxValue = value;
		}
	}

	void SetMaxValue(ClassType value)
	{
		MaxValue = value;
	}

	void SetMinValue(ClassType value)
	{
		MinValue = value;
	}

	void Reset()
	{
		constexpr ClassType min = std::numeric_limits<ClassType>::lowest();
		constexpr ClassType max = std::numeric_limits<ClassType>::epsilon();

		MinValue = max;
		MaxValue = min;
	}

private:
	ClassType MinValue;
	ClassType MaxValue;
};

