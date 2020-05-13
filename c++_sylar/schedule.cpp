#include <sys/syscall.h>
#include <unistd.h> 
#include <iostream>
#include <assert.h>

#include "schedule.h"
#include "syncSignal.h"
namespace sylar {
	static thread_local schedule *thread_schedule;
//	static thread_local sylar::fiber::ptr	main_fiber;
//	static thread_local sylar::fiber::ptr	current_fiber;

	schedule::schedule(int threadNum, bool use_caller, const std::string &name):m_name(name), m_sema(0){
		if (use_caller) {//当前线程参与调度
			//semaphore se(1);

		//	m_sema=sylar::semaphore(1);
			sylar::fiber::getThis();
			thread_schedule = this;
			//main_fiber.reset(new sylar::fiber(std::bind(&schedule::run, this)));
			//current_fiber = main_fiber;
			threadNum--;
			this->threadNum = threadNum;
			//vec.push_back(syscall(SYS_gettid));
		}
		else {
		
		}
	}
	schedule::~schedule() {
	
	}
	void schedule::start() {
		vec.resize(threadNum);
		for (int i = 0;i < threadNum;i++) {//创建线程池
			vec[i].reset(new thread("thread_name_" + std::to_string(i), std::bind(&schedule::run, this)));
		}
		activeThreadNum.store(threadNum);
	}
	void schedule::stop() {
//		for (int i = 0;i < m_func.size();i++) {
//			tickle();
//		}
		for (int i = 0;i < vec.size();i++) {
			tickle();
		}
		isAutoStoping = true;
		if (isAutoStoping) {
			for(int i=0;i<vec.size();i++)
			vec[i]->join();
		}
	}
	void schedule::ideal() {
		std::cout << "thread is idealing" << std::endl;
		while (true) {
			m_sema.wait();
			std::cout << "got sem\t" << sylar::thread::GetThreadId() << std::endl;
			sylar::fiber::YaloutReady();
		}
	//	
	}
	void schedule::test_run() {
		while (1) {
			std::cout << "test_run" << std::endl;
			sleep(5);
		}
	}
	void schedule::run() {
		sylar::fiber::getThis();
		sylar::fiber::ptr idealFiber(new sylar::fiber(std::bind(&schedule::ideal, this)));
		funcAndFiber l_var;
		while (true) 
		{
			idealFiber->swapin();
			if (isAutoStoping)break;
			{
				MutexGuard<sylar::Mutex> mu(m_mutex);
				if (!m_func.empty()) {
					auto it = m_func.begin();
					while (it != m_func.end()) {//查找有指定线程或不指定线程的任务
						if (it->threadId == sylar::thread::GetThreadId() || it->threadId == -1) {
							l_var = *it;
							m_func.erase(it);
							break;
						}
						it++;
					}
				}
				else {
					std::cout << "not got work continue" << std::endl;
					continue;
				}
			}
			//找到任务
			assert(l_var.m_fiber || l_var.m_func);
			if (l_var.m_fiber) {//处理协程
				std::cout << "deal fiber" << std::endl;
				l_var.m_fiber->swapin();
			}
			else if (l_var.m_func) {
				std::cout << "deal func" << std::endl;
				l_var.m_func();
			}
		}
	}
	void schedule::tickle() {
		m_sema.notify();
	}
}

