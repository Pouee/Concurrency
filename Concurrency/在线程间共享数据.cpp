#include "iostream"
#include "thread"
#include "vector"
#include "thread_guard.h"
#include "windows.h"

// If all shared data is read-only,there's no problem,
// because the data read by one thread is unaffected by
// whether or not another thread is reading the same data
// ������й�������ݶ���ֻ����,��û������,��Ϊһ���߳�����ȡ��
// ���ݲ�����һ���߳��Ƿ����ڶ�ȡ��ͬ�����ݶ�Ӱ��.
// Ȼ��,������������߳�֮�乲���,һ�������߳̿�ʼ�޸�����,��
// �����кܶ���鷳.�����������,�����ҪС��,ȷ��һ�а���.

// �ڲ�������,�����������ǽ��ȡ���������������߳��ϵĲ���ִ�е�
// ���˳���һ������.
// ��̸������ʱ,���ﾺ������(race condition)ͨ��������ʾ�������
// (problematic)��������


// �м��ַ���������������ľ�������.��򵥵�ѡ�����ñ������Ʒ�װ������ݽṹ
// ��ȷ��ֻ��ʵ��ִ���޸ĵ��߳��ܹ��ڲ������𻵵ĵط������м�����.����������
// �����ݽṹ���̵߳ĽǶȿ�,���ֲ���Ҫô��û��ʼҪô�Ѿ����.
// ��һ��ѡ�����޸����ݽṹ����Ƽ��䲻����,�Ӷ����޸���Ϊһϵ�в��ɷָ�ı�
// �������,ÿ���޸ľ������䲻����,��ͨ������֮Ϊ�������(lock-free programming)
// ��һ�ַ�ʽ��������(transaction)
using namespace std;
int cnt = 0;
void fun() 
{
	for (int i = 0; i < 10000000; ++i) {
		// ++ ���㲻��ԭ�Ӳ���
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
