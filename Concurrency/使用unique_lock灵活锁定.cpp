#include "iostream"
#include "thread"
#include "mutex"

using namespace std;
mutex m;
void f() 
{
	// ��ʾlock����ʱӦ����δ������
	// unique_lock<mutex> lock(m, std::defer_lock);
	// ռ�ø���Ĵ洢�ռ�,ʹ��ʱ���ܻ�ռ�ø��������,һ��������Ƽ�ʹ��lock_guard
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