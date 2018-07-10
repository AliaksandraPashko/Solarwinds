#include <iostream>
#include <unordered_map>
#include <string>
#include <thread>
#include <assert.h> 

static int destructor_call_count = 0;
static int constructor_call_count = 0;

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
			//    std::cout << std::this_thread::get_id();
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
		constructor_call_count++;
		std::cout
			<< name
			<< " created by "
			<< " by thread "
			<< std::this_thread::get_id()
			<< std::endl;
	}

	~TestData()
	{
		destructor_call_count++;
		std::cout
			<< name
			<< " destroyed by"
			<< " by thread "
			<< std::this_thread::get_id()
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
		<< " by thread "
		<< std::this_thread::get_id()
		<< std::endl;
}




void test_ptr_is_created_while_not_exist()
{
	auto new_cache = cache<int, TestData, TestData*(*)(int) >(create_test_data);
	auto ptr1 = new_cache.get_by_id(1);
	assert(ptr1 != nullptr);
}


void test_ptr_is_not_recreated_while_exist()
{
	auto new_cache = cache<int, TestData, TestData*(*)(int) >(create_test_data);
	auto ptr1 = new_cache.get_by_id(2);
	auto ptr2 = new_cache.get_by_id(2);

	assert(ptr2 == ptr1);
}

void test_last_ptr_is_destroyed()
{
	auto destructor = destructor_call_count;
	auto constructor = constructor_call_count;
	auto new_cache = cache<int, TestData, TestData *(*)(int)>(create_test_data);
	{
		std::shared_ptr<TestData> ptr1 = new_cache.get_by_id(3);
		{
			std::shared_ptr<TestData> ptr2 = new_cache.get_by_id(3);
		}
		assert(constructor_call_count == constructor + 1);
		assert(destructor_call_count == destructor);
	}
	assert(destructor_call_count == destructor + 1);
}





int main() {
	
	test_last_ptr_is_destroyed();
	test_ptr_is_created_while_not_exist();
	test_ptr_is_not_recreated_while_exist();
	


	auto new_cache = cache<int, TestData, TestData*(*)(int) >(create_test_data);
	//cache<int, TestData, decltype(create_test_data)*> new_cache(create_test_data);

	{
		auto ptr1 = new_cache.get_by_id(5);
	//	std::thread thread1(test_print_ptr_info, std::ref(ptr1));
	//	std::thread thread2(test_print_ptr_info, std::ref(ptr1));

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