#pragma once

#include <vector>
#include <list>
#include <atomic>
#include <memory>
#include <functional>

#include "thread.h"
#include "fiber.h"
namespace sylar {
	/**
		通过线程池和任务队列维护
		任务队列中可以放置协程和回调函数
	**/

	class schedule
	{
	public:
		typedef std::shared_ptr<schedule> ptr;
		typedef std::function<void()> Func;
		schedule(int threadNum,bool use_caller=true,const std::string &name="root");
		~schedule();
		void start();
		void stop();
		void run();
		void tickle();
		void ideal();
		void test_run();

		template<class T>
		void scheduler(T var, int id = -1) {
			bool needTickle=scheFuncAndFiber(var, id);
			if (needTickle)tickle();
		}

		template<class T>
		void multischeduler(T begin, T end) {
			for (auto it = begin;it != end;it++) {
				scheFuncAndFiber(*it, it->id);
			}
			tickle();
		}
		template<class T>
		bool scheFuncAndFiber(T var, int id) {
			//bool needTickle = m_func.empty();
			m_func.push_back(funcAndFiber(var, id));
			bool needTickle = !m_func.empty();
			return needTickle;
		}

		struct funcAndFiber
		{
			typedef std::shared_ptr<funcAndFiber> ptr;
			sylar::fiber::ptr m_fiber=nullptr;
			Func m_func=nullptr;
			int threadId=-1;

			funcAndFiber(fiber::ptr fiber, int id) {
				threadId = id;
				m_fiber.swap(fiber);
			}
			funcAndFiber(Func f, int id):threadId(id) {
				threadId = id;
				m_func.swap(f);
			}
			funcAndFiber() {
				m_fiber = nullptr;
				m_func = nullptr;
				threadId = -1;
			}
			void reser() {
				if (m_fiber)m_fiber = nullptr;
				if (m_func)m_func = nullptr;
				threadId = -1;
			}
			~funcAndFiber() {
				if (m_fiber)m_fiber = nullptr;
				if (m_func)m_func = nullptr;
				threadId = -1;
			}

		};

	private:
		std::atomic<int> threadNum{0};			//线程总数
		std::atomic<int> activeThreadNum{ 0 };	//活跃线程数
		std::atomic<int> m_funcNum{ 0 };		//任务数量
		std::vector<thread::ptr> vec;			//线程队列
		std::list<funcAndFiber> m_func;			//任务链表

		std::string m_name;

		bool isStoped;
		bool isAutoStoping;

		sylar::semaphore  m_sema;
		sylar::Mutex  m_mutex;
	};
}
