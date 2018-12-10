#include "iostream"
#include "thread"
#include "string"
#include "list"
#include "mutex"
#include "algorithm"
// ��������ȫ�ֱ�����ʹ��ż��Ҳ��ǡ����,�ڴ���������,����ȫ�ֱ���,����
// �����н�����Ԫ���ܱ�����������֯��һ��,�Ǻ��ձ��.

// ��ʹ��ȫ�ֱ���,�㽫����ķ���,�������һ����Ա�������ض��ܱ������ݵ�ָ
// �������,��ô���г�Ա������������˳��ķ�ʽ��������ԪҲ��û��ϵ��,��Ϊ
// �����ڱ�����ͱ�˸������.�ܹ�����(���ҿ����޸�)��ָ������õ����������
// �ڿ��Է����ܱ��������ݶ����������û���Ԫ.���ʹ�û���Ԫ������������ϸ��
// �ƽӿ�,��ȷ������������ܱ��������ݽ��з���֮ǰ,����Ԫ�Ѿ�������,�Ҳ���
// ����.

// ��Ҫ�����ܱ������ݵ�ָ������ô��ݵ����ķ�Χ֮��,������ͨ���Ӻ�����������
// ���������ⲿ�ɼ����ڴ���,������Ϊ�������ݸ��û��ṩ�ĺ���.
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