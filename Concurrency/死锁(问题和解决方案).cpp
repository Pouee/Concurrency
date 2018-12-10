#include "iostream"
#include "mutex"
#include "thread"
#include "thread_guard.h"
using namespace std;
// һ�����߳��е�ÿһ������Ҫͬʱ������������Ԫ��ִ��һЩ������
// ����ÿ���̶߳�ӵ����һ������Ԫ,ͬʱ�ȴ�����һ��.�̶߳��޷�
// ����,��Ϊÿ���̶߳��ڵȴ���һ���ͷ��以��Ԫ.�����龰��Ϊ��
// ��(deadlock).��������Ҫ������������໥��Ԫ��ִ�в���ʱ��
// ������


// Ϊ�˱�������,�����Ľ�����ʼ��ʹ����ͬ��˳����������������Ԫ
// �������������������ԪB֮ǰ��������ԪA,��ô����Զ��������.
// ��ʱ�����Ǻ�ֱ�۵�,��Щʱ��ȴ������ô��.
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

		// std::lock ����ͬʱ�������������Ļ���Ԫ,��û�������ķ���
		// �κ�һ������ʧ�ܶ��ͷ��Ѿ������Ļ�����
		std::lock(lhs.m, rhs.m);
		// �ṩһ���������std::adopt_lock ��֪�û���Ԫ�Ѿ�������
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