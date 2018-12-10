#include "iostream"
#include "thread"
#include "mutex"
#include "memory"
#include "stack"
#include "stdexcept"
// 仅仅因为使用了互斥元或其他其他机制来保护共享数据,未必会免于竞争条件.
// 你仍然需要确定保护了适当的数据
template <typename T>
class threadsafe_stack {
private:
	std::stack<T> data; 
	std::mutex m; 
public:
	threadsafe_stack() {}
	threadsafe_stack(const threadsafe_stack& other)
	{
		// 锁定other对象
		std::lock_guard<std::mutex> lock(other.m);
		data = other.data; 
	}
	void push(const T& value) 
	{
		std::lock_guard<std::mutex> lock(m);
		data.push(value);
	}
	void pop(T& value)
	{
		std::lock_guard<std::mutex> lock(m);
		if (data.empty()) 
			throw std::out_of_range("pop on a empty stack");
		value = std::move(data.top());
		data.pop();
	}
	std::shared_ptr<T> pop() 
	{
		std::lock_guard<std::mutex> lock(m);
		if (data.empty())
			throw std::out_of_range("pop on a empty stack");
		std::shared_ptr<T> ret(std::make_shared<T>(std::move(data.top())));
		data.pop();
		return ret; 
	}
	bool empty() const
	{
		std::lock_guard<std::mutex> lock(m);
		return data.empty();
	}
	threadsafe_stack& operator=(const threadsafe_stack&) = delete;
};
using namespace std;

int main()
{
	threadsafe_stack<int> sk;
	sk.push(1);
	shared_ptr<int> ptr = sk.pop();
	sk.push(2);
	cout << *ptr << endl;
	system("pause");
	return 0; 
}