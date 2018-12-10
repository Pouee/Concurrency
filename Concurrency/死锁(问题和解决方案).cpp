#include "iostream"
#include "mutex"
#include "thread"
#include "thread_guard.h"
using namespace std;
// 一个对线程中的每一个都需要同时锁定两个互斥元来执行一些操作，
// 并且每个线程都拥有了一个互斥元,同时等待另外一个.线程都无法
// 继续,因为每个线程都在等待另一个释放其互斥元.这种情景称为死
// 锁(deadlock).它是在需要锁定两个或更多互斥元以执行操作时最
// 大问题


// 为了避免死锁,常见的建议是始终使用相同的顺序锁定这两个互斥元
// 如果你总是在锁定互斥元B之前锁定互斥元A,那么你永远不会死锁.
// 有时候这是很直观的,有些时候却并不那么简单.
class X 
{
private:
	int data;
	mutex m;
public:
	X(int _data) :data(_data) { }
	void lock() { m.lock(); }
	void unlock() { m.unlock(); }
	void showData()
	{
		cout << data << endl;
	}
	friend void swap(X& lhs, X& rhs) 
	{
		if (&lhs == &rhs)
		{
			return;
		}
		/*
		lock_guard<mutex> lock_a(lhs.m);

		cout << "locked" << lhs.data << endl;
		std::chrono::seconds dura(1);
		std::this_thread::sleep_for(dura);

		lock_guard<mutex> lock_b(rhs.m);
		cout << "locked" << rhs.data << endl;
		*/

		// std::lock 可以同时锁定两个或更多的互斥元,而没有死锁的风险
		// 任何一个锁定失败都释放已经锁定的互斥量
		std::lock(lhs.m, rhs.m);
		// 提供一个额外参数std::adopt_lock 告知该互斥元已经被锁定
		lock_guard<mutex> lock_a(lhs.m, adopt_lock);
		lock_guard<mutex> lock_b(rhs.m, adopt_lock);
		swap(lhs.data, rhs.data);
		
	}
};
int main()
{
	X a(1), b(2);
	
	{
		thread_guard t1(thread([&a, &b] {
			swap(a, b);
			cout << "...." << endl;
		}));
		thread_guard t2(thread([&a, &b] {
			std::chrono::seconds dura(3);
			std::this_thread::sleep_for(dura);
			swap(b, a);
		}));
		thread_guard t3(thread([&a, &b] {
			b.lock();
			std::chrono::seconds dura(1);
			std::this_thread::sleep_for(dura);
			b.unlock();
		}));
	}

	a.showData();
	b.showData();
	
	system("pause");
	return 0; 
}