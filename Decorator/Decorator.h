#include <iostream>
#include <numeric>
#include <vector>
#include <algorithm>

template<typename DataType, typename FilterFunc, typename LessFunc = std::less<DataType>>
class Decorator
{
	FilterFunc filter_;
	LessFunc less_;
public:
	Decorator(FilterFunc filter, LessFunc less = LessFunc())
		: filter_(filter), less_(less) {}

	bool operator ()(const DataType& obj1, const DataType& obj2)
	{
		bool ob1 = filter_(obj1);
		bool ob2 = filter_(obj2);

		if (ob1 && ob2)
			return less_(ob1, ob2);

		if (ob1) return true;

		return false;
	}
};

template<typename Iterator, typename Filter>
Iterator min_elem(Iterator first, Iterator last, Filter filter)
{
	Decorator<int, bool(*)(int)> decorator(filter);
	auto a = std::min_element(first, last, decorator);
	return (filter(*a)) ? a : last;
}
