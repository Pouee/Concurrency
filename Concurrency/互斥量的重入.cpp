#include "iostream"
#include "thread"
#include "mutex"
using namespace std;
//mutex m;
recursive_mutex m;
int shared_data = 0;
void g() 
{
	lock_guard<recursive_mutex> lock(m);
	shared_data++;
}
void f() 
{
	lock_guard<recursive_mutex> lock(m);
	g();
}
int main()
{
	try
	{
		f();
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
	}
	
	cout << "data modify finished " << endl;
	cout << shared_data << endl;
	system("pause");
	return 0; 
}