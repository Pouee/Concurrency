#include "iostream"
#include "thread"
// �̱߳�ʶ����std::thread::id ���͵�,���������ֻ�ȡ��ʽ
// ��һ,�̵߳ı�ʶ������ͨ������֮�������std::thread����
// ��ͨ������get_id()��Ա���������.���std::thread����û
// ���������ִ���߳�,��get_id()�ĵ��÷���һ��Ĭ�Ϲ����
// std::thread::id����,��ʾ"û���߳�".
// ����,��ǰ�̵߳ı�ʶ��,����ͨ������std::this_thread::get_id()
// ���,��Ҳ�Ƕ�����<thread>ͷ�ļ��е�

// std::thread::id���͵Ķ����ṩһ�������ıȽ������,�ṩ��
// ���в�ֵͬ��������.������������ڹ�ϵ������������,���Ǳ���
// ��,�����κ���Ϊ����Ա������Ϊ���ʵķ�ʽ���бȽ�.
int main()
{
	std::thread t;
	std::thread::id tid = t.get_id(); 
	tid = std::this_thread::get_id();
	std::cout << tid << std::endl;
	std::cout << std::this_thread::get_id() << std::endl;
	system("pause");
	return 0; 
}