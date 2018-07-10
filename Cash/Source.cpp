#include <iostream>
#include <unordered_map>
#include <string>

template <typename ID, typename Obj, typename Creator>
class cache
{
public:
	using ptr_t = std::shared_ptr<Obj>;

private:
	std::unordered_map<ID, std::weak_ptr<Obj>> map_;
	Creator creator_;

	ptr_t get_by_id_impl(ID id)
	{
		ptr_t result = map_[id].lock();

		if (result == nullptr)
		{
			result = ptr_t(creator_(id));
			map_[id] = result;
		}

		return result;
	}

public: 
	cache(Creator creator) : creator_(creator) {}

	ptr_t operator[](ID id)
	{
		return get_by_id_impl(id);
	}

	ptr_t get_by_id(ID id)
	{
		return get_by_id_impl(id);
	}
};

struct TestData
{
	TestData(int id)
		: name("TestData object id=" + std::to_string(id))
	{
		std::cout
			<< name
			<< " created"
			<< std::endl;
	}

	~TestData()
	{
		std::cout
			<< name
			<< " destroyed"
			<< std::endl;
	}

	std::string name;
};

TestData* create_test_data(int id)
{
	return new TestData(id);
}

void test_print_ptr_info(std::shared_ptr<TestData> const& ptr)
{
	std::cout
		<< "Address="
		<< ptr
		<< ", value="
		<< ptr->name
		<< std::endl;
}

int main() {
	auto new_cache = cache<int, TestData, TestData*(*)(int) > (create_test_data);
	//cache<int, TestData, decltype(create_test_data)*> new_cache(create_test_data);

	{
		auto ptr1 = new_cache.get_by_id(5);
		test_print_ptr_info(ptr1);

		decltype(new_cache)::ptr_t ptr2 = new_cache.get_by_id(6);
		test_print_ptr_info(ptr2);

		auto ptr3 = new_cache.get_by_id(5);
		test_print_ptr_info(ptr3);

		auto ptr4 = new_cache.get_by_id(6);
		test_print_ptr_info(ptr4);
	}

	auto ptr1 = new_cache.get_by_id(5);
	test_print_ptr_info(ptr1);

	decltype(new_cache)::ptr_t ptr2 = new_cache.get_by_id(6);
	test_print_ptr_info(ptr2);

	return 0;
}