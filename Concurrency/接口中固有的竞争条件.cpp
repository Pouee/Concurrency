#include "iostream"
#include "thread"
#include "mutex"
#include "memory"
#include "stack"
#include "stdexcept"
// ������Ϊʹ���˻���Ԫ����������������������������,δ�ػ����ھ�������.
// ����Ȼ��Ҫȷ���������ʵ�������
template <typename T>
class threadsafe_stack {
private:
	std::stack<T> data; 
	std::mutex m; 
public:
	threadsafe_stack() {}
	threadsafe_stack(const threadsafe_stack& other)
	{
		// ����other����
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