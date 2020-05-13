#pragma once
/**
�߳̿⣺ʹ��pthread
�������߳�id���߳������̺߳������߳�ָ��
**/
#include<memory>
#include<functional>
#include<string>

#include "syncSignal.h"
#include <memory>
namespace sylar {
	class thread:std::enable_shared_from_this<thread> {
	public:
		//������ָ��
		typedef std::shared_ptr<thread> ptr;
		typedef std::function<void()> cb;


		thread(const std::string &threadName,cb fun);
		~thread();

		void setThreadName(std::string str);
		std::string getThreadName();
		pid_t getThreadId();

		void setThreadCb(cb &f);
		void join();

		static void* thread_run(void *arg);
		static pid_t GetThreadId();
	private:
		pid_t m_id;
		std::string m_name;
		pthread_t m_thread;
		cb m_cb;

		semaphore m_semaphore=semaphore(0);
	};
}
