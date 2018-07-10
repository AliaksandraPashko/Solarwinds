#include "Notificator.h"

void f_output(int x)
{
	std::cout << x << " by " << std::this_thread::get_id() << std::endl;
}


using notif_parameters = Notificator<int, void(*)(int)>;


void worker(std::shared_ptr<notif_parameters> notificator_ptr, int x)
{ 
	for (int i=x; i < 10; i++)
	{
		notificator_ptr->notify(i);
		i++;
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

void test_threads_w_buffered_messages_synchronized()
{
	std::shared_ptr<notif_parameters> notif_ptr = std::make_shared<notif_parameters>(f_output, true);
	std::thread t1(worker, notif_ptr, 1);
	std::thread t2(worker, notif_ptr, 2);
	t1.join();
	t2.join();
}

void test_threads_w_unbuffered_messages_synchronized()
{
	std::shared_ptr<notif_parameters> notif_ptr = std::make_shared<notif_parameters>(f_output, false);
	std::thread t1(worker, notif_ptr, 1);
	std::thread t2(worker, notif_ptr, 2);
	t1.join();
	t2.join();
}

int main()
{
//	test_threads_w_buffered_messages_synchronized();
	test_threads_w_unbuffered_messages_synchronized();

	std::this_thread::sleep_for(std::chrono::seconds(1));
	return 0;
}