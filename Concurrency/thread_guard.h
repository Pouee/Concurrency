#pragma once
#include "thread"
class thread_guard
{
private: 
	std::thread t;
	bool has_thread{ false };
	void destroy();
	void move_thread(thread_guard& another);
public:
	thread_guard() = default ;
	thread_guard(std::thread &&) noexcept;
	thread_guard(thread_guard &&) noexcept; // do not forget this 
	thread_guard& operator=(thread_guard&&) noexcept;
	thread_guard(const thread_guard&) = delete;
	thread_guard& operator=(const thread_guard&) = delete;
	~thread_guard();
};


