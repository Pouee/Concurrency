#include "iostream"
#include "thread"
// 线程标识符是std::thread::id 类型的,并且有两种获取方式
// 其一,线程的标识符可以通过从与之相关联的std::thread对象
// 中通过调用get_id()成员函数来获得.如果std::thread对象没
// 有相关联的执行线程,对get_id()的调用返回一个默认构造的
// std::thread::id对象,表示"没有线程".
// 另外,当前线程的标识符,可以通过调用std::this_thread::get_id()
// 获得,这也是定义在<thread>头文件中的

// std::thread::id类型的对象提供一套完整的比较运算符,提供了
// 所有不同值的总排序.这就允许它们在关系容器中作主键,或是被排
// 序,或者任何作为程序员的你认为合适的方式进行比较.
int main()
{
	std::thread t;
	std::thread::id tid = t.get_id(); 
	tid = std::this_thread::get_id();
	std::cout << tid << std::endl;
	std::cout << std::this_thread::get_id() << std::endl;
	system("pause");
	return 0; 
}