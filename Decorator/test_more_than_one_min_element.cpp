#include "Decorator.h"
#include <algorithm>
#include <assert.h>

bool filter2(int);

class test_more_than_one_min_element
{
public:
	test_more_than_one_min_element()
	{
		std::vector<int> vec(10, 5);
		std::fill_n(vec.begin(), 3, 2);
		assert(min_elem(vec.begin(), vec.end(), filter2) == std::find_if(vec.begin(), vec.end(), filter2));
	}
};

static test_more_than_one_min_element test_min_more_than_one;

bool filter2(int x)
{
	return (x % 2 == 0);
}