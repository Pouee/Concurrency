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
// 从函数中返回std::thread
std::thread f(){
	return std::thread(some_function);
}
std::thread g() {
	std::thread t(some_function); 
	return t; 
}
// 将线程所有权转移到函数中
void u(std::thread t) {}
void v() {
	std::thread t(some_other_function);
	u(std::move(t));
}
int main() 
{
	/*
	// 构建一个线程与t1关联 
	std::thread t1(some_function);			
	// 将t1的所有权转移给t2 此刻t1不再拥有相关联的执行线程
	// 运行some_other_function 的线程现在与t2关联
	std::thread t2 = std::move(t1);	
	// t1 与新的临时对象线程相关联
	t1 = std::thread(some_other_function);	
	// t3 没有与任何线程相关联 
	std::thread t3;
	// t2 相关联的线程转移给t3
	t3 = std::move(t2);
	
	// t1 已经有自己的关联线程了 调用移动赋值将会调用std::terminate();
	t1.detach();
	t1 = std::move(t3);
	*/
	std::thread t1([] {});
	std::thread t2([] {});
	scoped_thread st1(std::move(t1));
	scoped_thread st2(std::move(t2));
	
	// 生成一批线程并等待它们完成
	std::vector<std::thread> threads; 
	for (int i = 0; i < 20; i++) {
		threads.push_back(std::thread(some_function));
	}
	std::for_each(threads.begin(), threads.end(),
		std::mem_fn(&std::thread::join));

	system("pause");
	return 0; 
}