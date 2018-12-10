#include "iostream"
#include "thread"
#include "vector"
#include "mutex"
#include "thread_guard.h"
#include "windows.h"

using namespace std;
int cnt = 0;
mutex m;
// C++标准提供的保护共享数据的最基本机制是互斥元(mutex,mutual exclusion).
// 在访问共享数据之前,锁定(lock)与该数据相关的互斥元,当访问数据结构完成后,
// 解锁(unlock)该互斥元.线程库会确保一旦一个线程已经锁定某个数据元,所有其他
// 试图锁定相同互斥元的线程必须等待,直到成功锁定了该互斥元的线程解锁此互斥元.

// 在C++中,通过构造std::mutex的实例创建互斥元,调用成员函数lock()来锁定它,
// 调用成员函数unlock()来解锁它.然而,直接调用成员函数是不推荐的做法.作为替代
// 标准C++库提供了std::lock_guard类模板,实现了互斥元的RAII惯用语法:它在构造
// 时锁定所给的互斥元,在析构时将互斥元解锁,从而确保被锁定的互斥元始终被正确解锁
void fun()
{
	lock_guard<mutex> l(m);
	for (int i = 0; i < 10000000; ++i) {
		// ++ 不是原子操作
		//lock_guard<mutex> l(m);
		++::cnt;
	}
	cout << "cnt :" << ::cnt << endl;
}
int main()
{
	{
		DWORD start_time = GetTickCount();
		for (size_t i = 0; i < 10000000; i++)
		{
			lock_guard<mutex> l(m);
		}
		DWORD end_time = GetTickCount();
		std::cout << "time elapse:" << end_time - start_time << std::endl;

	}
	
	DWORD start_time = GetTickCount();
	{
		vector<thread_guard> threads;
		for (int i = 0; i < 4; i++)
		{
			threads.push_back(thread(fun));
		}
	}
	DWORD end_time = GetTickCount();
	std::cout << "time elapse:" << end_time - start_time << std::endl;
	system("pause");
	return 0;
}
