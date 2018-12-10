#include "iostream"
#include "thread"
#include "vector"
#include "thread_guard.h"
#include "windows.h"

// If all shared data is read-only,there's no problem,
// because the data read by one thread is unaffected by
// whether or not another thread is reading the same data
// 如果所有共享的数据都是只读的,就没有问题,因为一个线程所读取的
// 数据不受另一个线程是否正在读取相同的数据而影响.
// 然而,如果数据是在线程之间共享的,一个或多个线程开始修改数据,就
// 可能有很多的麻烦.在这种情况下,你必须要小心,确保一切安好.

// 在并发性中,竞争条件就是结果取决于两个或更多个线程上的操作执行的
// 相对顺序的一切事物.
// 在谈到并发时,术语竞争条件(race condition)通常用来表示有问题的
// (problematic)竞争条件


// 有几种方法来处理有问题的竞争条件.最简单的选择是用保护机制封装你的数据结构
// 以确保只有实际执行修改的线程能够在不变量损坏的地方看到中间数据.从其他访问
// 该数据结构的线程的角度看,这种操作要么还没开始要么已经完成.
// 另一个选择是修改数据结构的设计及其不变量,从而令修改作为一系列不可分割的变
// 更来完成,每个修改均保留其不变量,这通常被称之为无锁编程(lock-free programming)
// 另一种方式则是事务(transaction)
using namespace std;
int cnt = 0;
void fun() 
{
	for (int i = 0; i < 10000000; ++i) {
		// ++ 运算不是原子操作
		++::cnt;
	}
	cout << "cnt :" << ::cnt << endl;
}
int main()
{	
	DWORD start_time = GetTickCount();
	{
		vector<thread_guard> threads;
		for (int i = 0; i < 20; i++)
		{
			threads.push_back(thread(fun));
		}
	}
	DWORD end_time = GetTickCount();
	std::cout << "time elapse:" << end_time - start_time << std::endl;
	system("pause");
	return 0;
}
