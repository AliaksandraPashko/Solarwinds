#include "Notificator.h"
#include <assert.h>

using notif_parameters = Notificator<int, void(*)(int)>;

void worker_push_unbuffered(std::shared_ptr<notif_parameters>, int);

void callback_func_for_unbuff(int);

std::set<int> data_;

class test_unbuffered_messages_are_overwrited
{
public:
	test_unbuffered_messages_are_overwrited()
	{
		std::shared_ptr<notif_parameters> notif_ptr = std::make_shared<notif_parameters>(callback_func_for_unbuff, false);
		std::thread t1(worker_push_unbuffered, notif_ptr, 1);
		std::thread t2(worker_push_unbuffered, notif_ptr, 2);
		t1.join();
		t2.join();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		std::set<int> need_set;
		int n = 1;
		std::generate_n(std::inserter(need_set, need_set.begin()), 10, [&n]() { return n++; });
		assert(data_ != need_set);
	}
};

static	test_unbuffered_messages_are_overwrited test_unbuf;


void callback_func_for_unbuff(int x)
{
	data_.insert(x);
	std::cout << x << " by " << std::this_thread::get_id() << std::endl;
}


void worker_push_unbuffered(std::shared_ptr<notif_parameters> notificator_ptr, int x)
{
	for (int i = 0; i < 5; i++)
	{
		notificator_ptr->notify(x);
		x += 2;
	}
}

