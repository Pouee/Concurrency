#include "thread_guard.h"

void thread_guard::destroy() 
{
	if (this->has_thread) {
		if (t.joinable()) {
			t.join();
		}
		this->has_thread = false;
	}
}

void thread_guard::move_thread(thread_guard & another)
{
	if (another.has_thread) {
		this->has_thread = true;
		this->t = std::move(another.t);
		another.has_thread = false;
	}
}

thread_guard::thread_guard(std::thread &&_t)noexcept :t(std::move(_t)),has_thread(true)
{

}
thread_guard::thread_guard(thread_guard && tg) noexcept
{
	if (&tg != this) {
		move_thread(tg);
	}
}
thread_guard & thread_guard::operator=(thread_guard && _t) noexcept
{
	// avoiding self asignment
	if (&_t != this) {
		this->destroy();
		move_thread(_t);
	}
	return *this; 
}
thread_guard::~thread_guard()
{
	destroy();
}
