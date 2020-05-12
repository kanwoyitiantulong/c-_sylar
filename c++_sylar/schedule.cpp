#include "schedule.h"
#include <sys/syscall.h>
#include <unistd.h> 
namespace sylar {
	static thread_local schedule *thread_schedule;
	static thread_local sylar::fiber::ptr	main_fiber;
	static thread_local sylar::fiber::ptr	current_fiber;

	schedule::schedule(int threadNum, bool use_caller, const std::string &name):m_name(name){
		if (use_caller) {//当前线程参与调度
			sylar::fiber::getThis();
			thread_schedule = this;
			main_fiber.reset(new sylar::fiber(std::bind(&schedule::run, this)));
			current_fiber = main_fiber;
			threadNum--;
			this->threadNum = threadNum;
			//vec.push_back(syscall(SYS_gettid));
		}
		else {
		
		}
	}
	schedule::~schedule() {}
	void schedule::start() {
		vec.resize(threadNum);
		for (int i = 0;i < threadNum;i++) {//创建线程池
			vec[i].reset(new thread("thread_name_" + std::to_string(i), std::bind(&schedule::run, this)));
		}
		activeThreadNum.store(threadNum);
	}
	void schedule::stop() {}
	void schedule::run() {
	
	}
	void schedule::tickle() {}
}

