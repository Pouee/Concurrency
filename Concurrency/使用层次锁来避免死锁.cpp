#include "iostream"
#include "stdexcept"
#include "thread"
#include "mutex"
#include "climits"
#include "thread_guard.h"
#include "queue"
using namespace std;

class hierarchical_mutex {
private:
	std::mutex internal_mutex;
	std::mutex data_mutex;
	unsigned long const hierarchy_value;
	// 保存上一次的层次值,该锁不能重入,所以只需要一个简单数据类型来保存
	unsigned long previous_hierarchy_value;
	// 当前层次值
	static thread_local unsigned long this_thread_hierarchy_value;
	void check_for_hierarchy_violation() 
	{
		// 当前线程锁定了更低等级的或者是同等级的锁
		if (this_thread_hierarchy_value <= hierarchy_value)
		{
			throw std::logic_error("mutex hierarchy violated");
		}
	}
	void update_hierarchy_value() 
	{
		// 保存当前层次值
		previous_hierarchy_value = this_thread_hierarchy_value; 
		// 改变当前层次值
		this_thread_hierarchy_value = hierarchy_value; 
	}
public:
	explicit hierarchical_mutex(unsigned long value) 
		:hierarchy_value(value),previous_hierarchy_value(0){}
	void lock()
	{
		check_for_hierarchy_violation();
		internal_mutex.lock();
		lock_guard<std::mutex> lock(data_mutex);
		update_hierarchy_value();
	}
	void unlock()
	{
		internal_mutex.unlock();
		lock_guard<std::mutex> lock(data_mutex);
		this_thread_hierarchy_value = previous_hierarchy_value;
		
	}
	bool try_lock() 
	{
		check_for_hierarchy_violation();
		if (!internal_mutex.try_lock())
			return false;
		lock_guard<std::mutex> lock(data_mutex);
		update_hierarchy_value();
		return true; 
	}
};
thread_local unsigned long hierarchical_mutex::this_thread_hierarchy_value = ULONG_MAX;

struct Data {
	hierarchical_mutex m;
	int data; 
	Data(unsigned long hierarchical, int data) 
		:m(hierarchical), data(data) {}
	void swap(Data & d) 
	{
		lock_guard<hierarchical_mutex> lock1(m);
		std::chrono::seconds dura(1);
		std::this_thread::sleep_for(dura);
		lock_guard<hierarchical_mutex> lock2(d.m);
		std::swap(d.data, data);
	}
};

int main()
{
	Data d1(10000,1), d2(1000,2);
	{
		thread_guard t1(thread([&] {
			try
			{
				d1.swap(d2);
			}
			catch (const std::exception&)
			{
				cout << "operation not be permitted!" << endl;
			}
			
		}));
		thread_guard t2(thread([&] {
			try
			{
				d2.swap(d1);
			}
			catch (const std::exception&)
			{
				cout << "operation not be permitted!" << endl;
			}
		}));
	}
	cout << d1.data << endl;
	system("pause");
	return 0;
}