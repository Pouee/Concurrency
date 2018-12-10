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
	// C++ 编译器会解析为函数声明
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
		// 一旦开始了线程,你需要显式地决定是要等待它完成还是让它自行运行
		// 调用join()的行为会清理所有与该线程相关联的存储器,这样std::thread
		// 对象不再与现已完成的线程相关联,它也不与任何线程相关联,这就意味着,
		// 你只能对一个给定的线程调用一次join(),一旦你调用了join(),此std::thread
		// 对象不再是可连接的,并且joinable()将返回false
		my_thread1.join();
		
		//my_thread1.detach();
	}
	*/
	{
		// 在异常环境下的等待
		// 不建议使用 try/catch
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
		// RAII 资源获取即初始化
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
			// 确保编译器不对自动合成
			// 赋值或者赋值这样一个对象可能是危险的,因为它可能比它要结合
			// 的线程的作用域存在的更久.
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
		// 守护线程
		// 主线程退出子线程也会退出
		thread *t = new thread(fn);
		// 分离线程
		t->detach();
	}
	*/
	
	{
		// 传递参数给线程函数
		// 参数传递默认会以复制的方式传入到线程内部存储空间,即便参数是引用
		// 参数将在新线程内进行转换
		//thread t([](int a, const Test &str) {
		//	std::chrono::seconds dura(10);
		//	std::this_thread::sleep_for(dura);
		//},3,3);
		//system("pause");
		//t.join();
		
		
		//char buffer[1024];
		// 可能buffer未完成转换前就被释放
		//thread([](string &s) {},buffer);
		// 使用string避免悬浮指针
		//thread([](string &s) {},string(buffer));

		// 引用传参 std::ref
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

		// 提供参数的另一个有趣的场景是,这里的参数不能被复制
		// 只能被移动
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