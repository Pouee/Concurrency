#include "iostream"
#include "thread"
#include "vector"
#include "numeric"
#include "algorithm"
#include "thread_utility.hpp"
#include "windows.h"
template <typename Iterator,typename T>
struct accumulate_block {
	void operator()(Iterator first, Iterator last, T & result) {
		result = std::accumulate<Iterator,T>(first, last, result);
	}
};
template <typename Iterator, typename T>
T parallel_accumulate(Iterator first, Iterator last, T init) {
	const auto length  = std::distance(first, last);
	if (length == 0) return init; 
	unsigned long const min_per_thread = 25;
	unsigned long const max_thread = (length + min_per_thread - 1) / min_per_thread;
	unsigned long const hardware_threads = std::thread::hardware_concurrency(); 
	unsigned long const num_threads = std::min<unsigned long const>(hardware_threads != 0 ? hardware_threads : 2, max_thread);
	unsigned long const block_size = length / num_threads;
	std::vector<T> results(num_threads);
	std::vector<std::thread> threads(num_threads - 1);
	Iterator block_start = first; 
	for (unsigned long i = 0; i < num_threads - 1; ++i) {
		Iterator block_end = block_start;
		std::advance(block_end, block_size);
		threads[i] = std::thread(accumulate_block<Iterator, T>(), block_start, block_end, std::ref(results[i]));
		block_start = block_end;
	}
	accumulate_block<Iterator, T>()(block_start, last, results[num_threads - 1]);
	std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
	return std::accumulate(results.begin(),results.end(),init);
}
long fast_accumulate(int *begin, int *end, long init) {
	unsigned long count = end - begin;
	if (count == 0) return init;
	unsigned int threads_count = thread_utility::num_threads(count, 100) - 1 ;
	if (threads_count == 0) {
		return std::accumulate<int*,long>(begin, end, init);
	}
	std::vector<long> results(threads_count);
	std::vector<std::thread> threads(threads_count);
	unsigned long block_size = count / threads_count;
	for (unsigned int i = 0; i < threads_count - 1; ++i) {
		threads[i] = std::thread(accumulate_block<int *,long>(), begin, begin + block_size, std::ref(results[i]));
		begin += block_size;
	}
	threads[threads_count - 1] = std::thread(accumulate_block<int *, long>(), begin, end, std::ref(results[threads_count - 1]));
	//std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));
	thread_utility::join_threads(threads.begin(), threads.end());
	return std::accumulate(results.begin(), results.end(), init);
}
long thread_try_accumulate(int *begin, int *end, long init , unsigned int num_threads) {
	unsigned long count = end - begin;
	if (count == 0) return init;
	unsigned int threads_count = num_threads;
	if (threads_count == 0) {
		return std::accumulate<int*, long>(begin, end, init);
	}
	std::vector<long> results(threads_count);
	std::vector<std::thread> threads(threads_count);
	unsigned long block_size = count / threads_count;
	for (unsigned int i = 0; i < threads_count - 1; ++i) {
		threads[i] = std::thread(accumulate_block<int *, long>(), begin, begin + block_size, std::ref(results[i]));
		begin += block_size;
	}
	threads[threads_count - 1] = std::thread(accumulate_block<int *, long>(), begin, end, std::ref(results[threads_count - 1]));
	thread_utility::join_threads(threads.begin(), threads.end());
	return std::accumulate(results.begin(), results.end(), init);
}
int arr[200000000];
int main()
{
	for (auto &k : arr) {
		k = 256;
	}
	
	
	{
		std::cout << "single thread accumulate..." << std::endl;
		DWORD start_time = GetTickCount();
		long ret = std::accumulate(std::begin(arr), std::end(arr), (long)0);
		DWORD end_time = GetTickCount();
		std::cout << "time elapse:" << end_time - start_time << std::endl;
		std::cout << "result:" << ret << std::endl;
	}
	std::cout << "---------------------" << std::endl;
	{
		std::cout << "multithread accumulate..." << std::endl;
		DWORD start_time = GetTickCount();
		long ret = fast_accumulate(std::begin(arr), std::end(arr), 0);
		DWORD end_time = GetTickCount();
		std::cout << "time elapse:" << end_time - start_time << std::endl;
		std::cout << "result:" << ret << std::endl;
	}
	
	for(int i = 1 ; i < 100 ;++i)
	{
		std::cout << "thread count:" << i;
		DWORD start_time = GetTickCount();
		long ret = thread_try_accumulate(std::begin(arr), std::end(arr), 0,i);
		DWORD end_time = GetTickCount();
		std::cout << " time elapse:" << end_time - start_time << std::endl;
	}
	system("pause");
	return 0; 
}