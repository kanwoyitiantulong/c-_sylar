#pragma once
/**
线程库：使用pthread
包含：线程id、线程名、线程函数、线程指针
**/
#include<memory>
#include<functional>
#include<string>
namespace sylar {
	class thread {
	public:
		//类智能指针
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
	private:
		pid_t m_id;
		std::string m_name;
		pthread_t m_thread;
		cb m_cb;
	};
}
