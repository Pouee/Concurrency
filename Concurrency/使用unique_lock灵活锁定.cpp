#include "iostream"
#include "thread"
#include "mutex"

using namespace std;
mutex m;
void f() 
{
	// 表示lock构造时应该是未锁定的
	// unique_lock<mutex> lock(m, std::defer_lock);
	// 占用更多的存储空间,使用时可能会占用更多的性能,一般情况下推荐使用lock_guard
	unique_lock<mutex> lock(m);
	lock.lock();
	//lock.lock();
	//lock.unlock();
}
int main()
{
	f();
	return 0;
}