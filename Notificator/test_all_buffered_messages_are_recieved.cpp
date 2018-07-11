#include "Notificator.h"
#include <assert.h>

using notif_parameters = Notificator<int, void(*)(int)>;

void worker_push_buffered(std::shared_ptr<notif_parameters>, int);

void callback_func_for_buff(int);

std::set<int> data;
std::mutex mtx;

class test_all_buffered_messages_are_recieved
{
public:
	test_all_buffered_messages_are_recieved()
	{
		std::shared_ptr<notif_parameters> notif_ptr = std::make_shared<notif_parameters>(callback_func_for_buff, true);
		std::thread t1(worker_push_buffered, notif_ptr, 1);
		std::thread t2(worker_push_buffered, notif_ptr, 2);
		t1.join();
		t2.join();
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		std::set<int> need_set;
		int n = 1;
		std::generate_n(std::inserter(need_set, need_set.begin()), 10, [&]() { return n++; });
		assert(data == need_set);
	}
};

static	test_all_buffered_messages_are_recieved  test_buf;

void callback_func_for_buff(int x)
{
	data.insert(x);
	std::cout << x << " by " << std::this_thread::get_id() << std::endl;
}

void worker_push_buffered(std::shared_ptr<notif_parameters> notificator_ptr, int x)
{
	for (int i = 0; i < 5; i++)
	{
		notificator_ptr->notify(x);
		x += 2;
	}
}

