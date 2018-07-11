#pragma once
#include <iostream>
#include <iterator>
#include <mutex>
#include <queue>
#include <set>
#include <thread>


template<typename Message, typename Function>
class Notificator
{
	std::queue<Message> que_;
	Function f_;
	bool buffered_message_;
	bool is_finishing_ = false;
	std::mutex mtx;
	std::condition_variable cv_;
	std::thread t_;
public:
	Notificator(Function f, bool buf_mes) : f_(f), buffered_message_(buf_mes),
		t_(&Notificator::listener, this) {}

	void notify(Message message)
	{
		std::lock_guard <std::mutex> lock_g(mtx);
		if (!buffered_message_)
		{
			while (!que_.empty())
			{
				que_.pop();
			}
		}
		que_.push(message);
		cv_.notify_all();
	}

	void listener()
	{
		while (true)
		{
			std::queue<Message> new_empty_que;
			{
				std::unique_lock<std::mutex> uniq_lock(mtx);
				std::cout << std::this_thread::get_id() << " is listening " << std::endl;
				cv_.wait(uniq_lock, [this]() { return (!que_.empty()) || is_finishing_; });

				if (is_finishing_)
				{
					break;
				}

				std::swap(new_empty_que, que_);
			}
			while (!new_empty_que.empty())
			{
				{
					std::lock_guard<std::mutex> lock_g(mtx);
					f_(new_empty_que.front());
				}
				new_empty_que.pop();
			}
		}
	}

	~Notificator()
	{
		is_finishing_ = true;
		cv_.notify_all();
		t_.join();
		std::cout << " destructor \n";
	}
};
