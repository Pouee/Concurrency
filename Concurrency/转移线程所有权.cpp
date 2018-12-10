#include "iostream"
#include "thread"
#include "vector"
#include "algorithm"
#include "scoped_thread.h"
void some_function()
{
	std::cout << "some_function" << std::endl;
}
void some_other_function()
{
	std::cout << "some_other_function" << std::endl;
}
// �Ӻ����з���std::thread
std::thread f(){
	return std::thread(some_function);
}
std::thread g() {
	std::thread t(some_function); 
	return t; 
}
// ���߳�����Ȩת�Ƶ�������
void u(std::thread t) {}
void v() {
	std::thread t(some_other_function);
	u(std::move(t));
}
int main() 
{
	/*
	// ����һ���߳���t1���� 
	std::thread t1(some_function);			
	// ��t1������Ȩת�Ƹ�t2 �˿�t1����ӵ���������ִ���߳�
	// ����some_other_function ���߳�������t2����
	std::thread t2 = std::move(t1);	
	// t1 ���µ���ʱ�����߳������
	t1 = std::thread(some_other_function);	
	// t3 û�����κ��߳������ 
	std::thread t3;
	// t2 ��������߳�ת�Ƹ�t3
	t3 = std::move(t2);
	
	// t1 �Ѿ����Լ��Ĺ����߳��� �����ƶ���ֵ�������std::terminate();
	t1.detach();
	t1 = std::move(t3);
	*/
	std::thread t1([] {});
	std::thread t2([] {});
	scoped_thread st1(std::move(t1));
	scoped_thread st2(std::move(t2));
	
	// ����һ���̲߳��ȴ��������
	std::vector<std::thread> threads; 
	for (int i = 0; i < 20; i++) {
		threads.push_back(std::thread(some_function));
	}
	std::for_each(threads.begin(), threads.end(),
		std::mem_fn(&std::thread::join));

	system("pause");
	return 0; 
}