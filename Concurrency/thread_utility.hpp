#pragma once
#include "thread"
#include "algorithm"
class thread_utility final
{
public: 
	/*
		获取硬件所支持的并发CPU核心数量
		返回大于1等于1的值
	*/
	inline static unsigned int hardware_concurrency() {
		return std::max<unsigned long>(std::thread::hardware_concurrency(), 1);
	}
	/*
		将work_count数量的工作以min_work_count进行划分,
		获得该工作所需要的最优线程数量 返回大于1等于1的值
	*/
	inline static unsigned int num_threads
	(unsigned long work_count, unsigned long min_work_count) {
		return std::min<long>((work_count + min_work_count - 1) / min_work_count, hardware_concurrency());
	}
	template<typename It>
	inline static void join_threads(It begin, It end) {
		std::for_each(begin, end, std::mem_fn(&std::thread::join));
	}
	thread_utility() = delete ;
	~thread_utility() = delete;
};
