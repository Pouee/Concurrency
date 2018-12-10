#include "iostream"
#include "thread"
// 线程局部变量可以在程序中让你为每个线程拥有独立的变量实例,在声明变
// 量时可以用thread_local关键字进行标记,表明它是线程局部的.命名空间
// 范围的变量,类的静态数据成员都可以被声明为线程局部的,也就是说,具有
// 线程存储期(thread storage duration).

// 虽然线程局部变量的每一个变量有一个不同的地址,你仍可以用指针来指向它,
// 但是该对象若被释放,则执行将是未定义的.
namespace X {
	struct T {
		T() { std::cout << "T()" << std::endl; }
		~T() { std::cout << "~T()" << std::endl; }
	};
	// 并没有指定什么什么时候被构造,如果构造函数出错,线程将会终止(terminate())
	thread_local T t;
}
void f() 
{
	// 不使用线程时 和static的效果一样
	thread_local int k = 10;
	std::cout << k++ << std::endl;
}
int main()
{
	// 线程函数返回时,线程局部变量将会被析构,与构造函数的顺序相反.
	// 由于初始化的顺序是未指定的,因此确保这种变量的析构函数没有相
	// 互依存的关系是很重要的.如果一个线程局部变量的析构函数以异常
	// 退出,线程也会被终止(terminate())
	std::thread t([] { while (1);  });
	f();
	f();
	// 如果任何其他线程在应用程序退出时仍然在运行,那些线程的线程局部
	// 变量的析构函数不会被调用.
	t.detach();
	system("pause");
	//t.join();
	return 0; 
}