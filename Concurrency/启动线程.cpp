#include "iostream"
#include "cstdlib"
#include "thread"
#include "fstream"

using namespace std;
class background_task {
public:
	void operator()() {
		cout << "OK" << endl;
	}
};
class Test {
public:
	Test() = default;
	Test(int) { cout << "Test(int)" << endl; }
	Test(const Test & t) {
		cout << "Test(const Test& )" << endl;
	}
	Test(const Test && t) {
		cout << "Test(const Test&& )" << endl;
	}
	Test& operator=(const Test& t) {
		cout << "operator=(const Test&)" << endl;
		return *this;
	}
	Test& operator=(const Test&& t) {
		cout << "operator=(const Test&&)" << endl;
		return *this;
	}
	~Test() {
		cout << "~Test()" << endl;
	}
};
int main()
{
	// C++ �����������Ϊ��������
	//thread my_thread(background_task());
	// C++11
	//thread my_thread{ background_task() };
	/*
	{
		int local_data = 10;
		struct func {
			int &i;
			func(int &i) :i(i) {}
			void operator()() {
				for (i = 0; i < 1000; i++) {
					cout << i << endl;
					
				}
			}

		} fn(local_data);
		thread my_thread1(fn);
		// һ����ʼ���߳�,����Ҫ��ʽ�ؾ�����Ҫ�ȴ�����ɻ���������������
		// ����join()����Ϊ��������������߳�������Ĵ洢��,����std::thread
		// ��������������ɵ��߳������,��Ҳ�����κ��߳������,�����ζ��,
		// ��ֻ�ܶ�һ���������̵߳���һ��join(),һ���������join(),��std::thread
		// �������ǿ����ӵ�,����joinable()������false
		my_thread1.join();
		
		//my_thread1.detach();
	}
	*/
	{
		// ���쳣�����µĵȴ�
		// ������ʹ�� try/catch
		/*
		thread t([]{
			for (int i = 0; i < 1000000000; i++);
		});
		// do something in current thread
		try 
		{
			int i = 0;
			throw i;
		}
		catch (...) 
		{
			cout << "an exception occurred" << endl;
			t.join();
			throw;
		}
		t.join();
		*/
		// RAII ��Դ��ȡ����ʼ��
		/*
		class thread_guard {
		private:
			std::thread &t;
		public:
			thread_guard(std::thread &t) :t(t) {};
			~thread_guard() {
				if (t.joinable()) {
					t.join();
				}
			}
			// ȷ�������������Զ��ϳ�
			// ��ֵ���߸�ֵ����һ�����������Σ�յ�,��Ϊ�����ܱ���Ҫ���
			// ���̵߳���������ڵĸ���.
			thread_guard(const thread_guard &) = delete;
			thread_guard& operator=(const thread_guard &) = delete;
		};
		thread t([] {
			for (int i = 0; i < 1000; i++) {
				cout << i << endl;
			}
		});
		thread_guard g(t);
		//t.detach();

		*/
	}
	/*
	{
		struct func {
			void operator()() {
				ofstream f;
				f.open("d:\\test.log");
				std::chrono::seconds dura(1);
				int i = 10;
				while (--i) {
					f << i << endl;
					std::this_thread::sleep_for(dura);
				}
			}
			~func(){
				cout << "~func()" << endl;
				//system("pause");
			}
		} ;
		func fn;
		// �ػ��߳�
		// ���߳��˳����߳�Ҳ���˳�
		thread *t = new thread(fn);
		// �����߳�
		t->detach();
	}
	*/
	
	{
		// ���ݲ������̺߳���
		// ��������Ĭ�ϻ��Ը��Ƶķ�ʽ���뵽�߳��ڲ��洢�ռ�,�������������
		// �����������߳��ڽ���ת��
		//thread t([](int a, const Test &str) {
		//	std::chrono::seconds dura(10);
		//	std::this_thread::sleep_for(dura);
		//},3,3);
		//system("pause");
		//t.join();
		
		
		//char buffer[1024];
		// ����bufferδ���ת��ǰ�ͱ��ͷ�
		//thread([](string &s) {},buffer);
		// ʹ��string��������ָ��
		//thread([](string &s) {},string(buffer));

		// ���ô��� std::ref
		/*
		std::chrono::seconds dura(10);
		int k = 10;
		thread t([](int &k) {
			while (k);
			cout << "shutdown..." << endl;
		}, ref(k));
		std::this_thread::sleep_for(dura);
		k = 0;
		t.join();
		*/
		//class X {
		//public:
		//	void work() {
		//		cout << "working..." << endl;
		//	}
		//
		//} x;
		//auto p = &X::work;
		//(x.*p)();
		//auto f = bind(&X::work, x);
		//f();
		// just like bind
		//thread t(&X::work,x);
		//t.detach();

		// �ṩ��������һ����Ȥ�ĳ�����,����Ĳ������ܱ�����
		// ֻ�ܱ��ƶ�
		cout << "--------" << endl;
		Test test;
		thread t([](Test t) {
			cout << "--------" << endl;
		}, test);
		t.join();
	}
	
	{
		
	}
	cout << "outer ..." << endl;
	system("pause");
	return 0;
}