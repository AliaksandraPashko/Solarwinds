#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <assert.h>


template<typename Message, typename Function>
class Notificator
{
	std::queue<Message> que_;
	std::thread t_;
	Function f_;
	bool buffered_message_;
	bool time_to_die = false;
	std::mutex mtx;
	std::condition_variable cv_;
public:
	Notificator(Function f, bool buf_mes) : f_(f), buffered_message_(buf_mes),
		t_(&Notificator::listener, this)/*, time_to_die(false)*/ {}


	void notify(Message message)
	{
		std::lock_guard <std::mutex> lock_g(mtx);
 		if (!buffered_message_)
		{
			assert(que_.size() == 1);
			if (!que_.empty())
			{
				que_.pop();
				assert(que_.size() == 0);
			}
		} 
		que_.push(message);
		cv_.notify_all();
		std::cout << std::this_thread::get_id() << " works " << std::endl;
	}

	void listener()
	{
 		while (true)
		{
			std::queue<Message> new_empty_que;
			{
				std::unique_lock<std::mutex> uniq_lock(mtx);
				std::cout << std::this_thread::get_id() << " is listening " << std::endl;
				cv_.wait(uniq_lock, [this]() { return (!que_.empty()) || time_to_die; });
				
				if (time_to_die)
				{
					break;
				}
				std::swap(new_empty_que, que_);
			}
			while (!new_empty_que.empty())
			{
				f_(new_empty_que.front());
				new_empty_que.pop();
			}
		}
	}

	~Notificator()
	{
		std::lock_guard <std::mutex> lock_g(mtx);
		time_to_die = true;
		cv_.notify_all();
		t_.join();
		std::cout << " destructor \n";
	}
};


void f_output(int x)
{
	std::cout << x << " by " << std::this_thread::get_id() << std::endl;
}


using notif_parameters = Notificator<int, void(*)(int)>;

void worker(std::shared_ptr<notif_parameters> notificator_ptr)
{ 
	int i = 0;
//	std::this_thread::sleep_for(std::chrono::milliseconds(100));
	while (true)
	{
		notificator_ptr->notify(i++);
	}
}



int main()
{
	std::shared_ptr<notif_parameters> notif_ptr = std::make_shared<notif_parameters>(f_output, true);
	std::thread t1(worker, notif_ptr);
	std::thread t2(worker, notif_ptr);
	t1.join();
	t2.join();
	std::this_thread::sleep_for(std::chrono::seconds(2));
	return 0;
}