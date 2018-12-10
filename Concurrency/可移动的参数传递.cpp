#include "iostream"
#include "thread"
#include "memory"
using namespace std;
class M {
public:
	M() = default;
	M(const M&) {
		cout << "MM" << endl;
	}
	M(const M&&) {
		// 此处在新线程内执行
		while(true)
			cout << "gogogo" << endl;
	}
};
void f(M m) {}
int main()
{
	M m;
	thread t(f,m);
	while (true) cout << "111" << endl;
	t.join(); 
	system("pause");
	return 0; 
}