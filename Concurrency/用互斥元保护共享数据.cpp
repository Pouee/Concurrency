#include "iostream"
#include "thread"
#include "vector"
#include "mutex"
#include "thread_guard.h"
#include "windows.h"

using namespace std;
int cnt = 0;
mutex m;
// C++��׼�ṩ�ı����������ݵ�����������ǻ���Ԫ(mutex,mutual exclusion).
// �ڷ��ʹ�������֮ǰ,����(lock)���������صĻ���Ԫ,���������ݽṹ��ɺ�,
// ����(unlock)�û���Ԫ.�߳̿��ȷ��һ��һ���߳��Ѿ�����ĳ������Ԫ,��������
// ��ͼ������ͬ����Ԫ���̱߳���ȴ�,ֱ���ɹ������˸û���Ԫ���߳̽����˻���Ԫ.

// ��C++��,ͨ������std::mutex��ʵ����������Ԫ,���ó�Ա����lock()��������,
// ���ó�Ա����unlock()��������.Ȼ��,ֱ�ӵ��ó�Ա�����ǲ��Ƽ�������.��Ϊ���
// ��׼C++���ṩ��std::lock_guard��ģ��,ʵ���˻���Ԫ��RAII�����﷨:���ڹ���
// ʱ���������Ļ���Ԫ,������ʱ������Ԫ����,�Ӷ�ȷ���������Ļ���Ԫʼ�ձ���ȷ����
void fun()
{
	lock_guard<mutex> l(m);
	for (int i = 0; i < 10000000; ++i) {
		// ++ ����ԭ�Ӳ���
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
