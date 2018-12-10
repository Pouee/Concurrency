#include "iostream"
#include "thread"
using namespace std;
void hello()
{
	int i = 100;
	while (--i) {
		cout << "hello concurrent world!" << endl;
	}	
}
int main()
{
	thread t(hello);
	t.join();

	return 0;
}


