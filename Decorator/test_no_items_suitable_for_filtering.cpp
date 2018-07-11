#include "Decorator.h"
#include <assert.h>

bool filter1(int);

class test_no_items_suitable_for_filtering
{
public:
	test_no_items_suitable_for_filtering()
	{
		std::vector<int> vec(10);
		iota(vec.begin(), vec.end(), 1);
		assert(min_elem(vec.begin(), vec.end(), filter1) == vec.end());
	}
};

static test_no_items_suitable_for_filtering test_no_items;


bool filter1(int x)
{
	return (x == 0);
}
