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
		bool ob1 = filter(obj1);
		bool ob2 = filter(obj2);

		if (ob1 && ob2)
			return less_(ob1, ob2);

		if (ob1) return true;
		if (ob2) return false;

		return false;
	}
};


bool filter(int x)
{
	return (x % 5 == 0);
}


int main()
{
	std::vector<int> vec(10);
	iota(vec.begin(), vec.end(), 1);
	Decorator<int, bool(*)(int)> decorator(filter);
	std::cout << *std::min_element(vec.begin(), vec.end(), decorator) << std::endl;

	return 0;
}
