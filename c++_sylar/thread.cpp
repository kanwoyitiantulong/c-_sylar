#include"thread.h"
#include<pthread.h>
#include <sys/syscall.h>
#include <unistd.h> 
#include<iostream>
namespace sylar {
	thread_local pthread_t local_pthread_t;
	thread_local std::string  local_thread_name;

	thread::thread(const std::string &threadName, cb fun) {
		m_name = threadName;
		m_cb = fun;
		int ret = pthread_create(&m_thread, NULL, thread_run, this);
		if (ret) {
			std::cout << "creat thread error;" << std::endl;
		}
	}
	thread::~thread() {
		pthread_detach(m_thread);
	}

	void thread::setThreadName(std::string str) {
		m_name = str;
	}
	std::string thread::getThreadName() {
		return m_name;
	}
	pid_t thread::getThreadId() {
		return m_id;
	}

	void thread::setThreadCb(cb &f) {
		m_cb = f;
	}

	void thread::join() {
		if (m_thread)pthread_join(m_thread, nullptr);
		m_thread = 0;
	}

	void* thread::thread_run(void *arg) {
		thread *t_thread = (thread*)arg;
		local_pthread_t = t_thread->m_thread;
		local_thread_name = t_thread->m_name;
		t_thread->m_id= syscall(SYS_gettid);
		pthread_setname_np(t_thread->m_thread, t_thread->m_name.substr(0,15).c_str());
		thread::cb func;
		func.swap(t_thread->m_cb);
		func();
		return 0;
	}

}