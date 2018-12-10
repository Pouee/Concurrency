#include "iostream"
#include "thread"
#include "mutex"
using namespace std;
mutex m1;
mutex m2;
void delay_lock() 
{
	unique_lock<mutex> lock_a(m1, defer_lock);
	unique_lock<mutex> lock_b(m2, defer_lock);
	// 锁定lock之后会在析构时释放锁
	lock(lock_a, lock_b);
	
	//lock_a.unlock();
	//lock_a.unlock();
}
int main()
{
	delay_lock();
	delay_lock();
	return 0;
}
