#include <iostream>
#include <thread>
#include <mutex>

int value = 0;
std::mutex mtx;
std::condition_variable cv;
bool is_stopping = false;

bool value_reached()
{
	return value == 20;
}

void waiting()
{
	std::unique_lock<std::mutex> unique_lock(mtx);
	cv.wait(unique_lock, value_reached);
	std::cout << std::this_thread::get_id() << " is waiting " << std::endl;
	is_stopping = true;
}

void increment()
{
	while (true)
	{
		{
			std::lock_guard<std::mutex> lock(mtx);

			if (is_stopping) break;

			value++;
			std::cout << value << " incremented by " << std::this_thread::get_id() << std::endl;
		}

		cv.notify_all();

		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

int main() {
	std::thread t1(increment), t2(increment), t3(increment), t4(waiting), t5(waiting);
	t1.join();
	t2.join();
	t3.join();
	t4.join();
	t5.join();
	return 0;
}