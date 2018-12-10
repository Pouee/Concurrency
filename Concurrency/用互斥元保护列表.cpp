#include "iostream"
#include "thread"
#include "string"
#include "list"
#include "mutex"
#include "algorithm"
// 尽管这种全局变量的使用偶尔也是恰当的,在大多数情况下,不用全局变量,而是
// 在类中将互斥元和受保护的数据组织在一起,是很普遍的.

// 不使用全局变量,你将敏锐的发现,如果其中一个成员函数返回对受保护数据的指
// 针或引用,那么所有成员函数都以良好顺序的方式锁定互斥元也是没关系的,因为
// 你已在保护中捅了个大窟窿.能够访问(并且可能修改)该指针或引用的任意代码现
// 在可以访问受保护的数据而无需锁定该互斥元.因此使用互斥元保护数据需仔细设
// 计接口,以确保在有任意对受保护的数据进行访问之前,互斥元已经被锁定,且不留
// 后门.

// 不要将对受保护数据的指针和引用传递到锁的范围之外,无论是通过从函数返回它们
// 将其存放在外部可见的内存中,还是作为参数传递给用户提供的函数.
std::list<int> shared_data;
std::mutex mutual_exclusion;
void add_to_list(int value) 
{
	std::lock_guard<std::mutex> lock(::mutual_exclusion);
	shared_data.push_back(value);
}
bool list_contains(int value) 
{
	std::lock_guard<std::mutex> lock(::mutual_exclusion);
	return shared_data.end()!= std::find(shared_data.begin(), shared_data.end(), value);
}
class some_data {
	int a{0};
	std::string b;
public:
	void do_something() 
	{
		a++;
		std::cout << "doing something..." << std::endl;
	}
};
class data_wrapper {
private:
	some_data data; 
	std::mutex m; 
public:
	template<typename F>
	void process_data(F f) 
	{
		std::lock_guard<std::mutex> lock(m);
		f(data); 
	}
};
some_data *unprotected = nullptr;
void malicious_function(some_data& some) 
{
	unprotected = &some; 
}

int main()
{
	
	data_wrapper x;
	x.process_data(malicious_function);
	unprotected->do_something();
	system("pause");
	return 0; 
}