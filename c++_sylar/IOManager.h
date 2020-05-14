#pragma once
#include "schedule.h"
/**
int epoll_create(int size);
int epoll_ctl(int epfd, int op, int fd, struct epoll_event *event);
int epoll_wait(int epfd, struct epoll_event *events,int maxevents, int timeout);

op:
	EPOLLLT
	EPOLLET
	EPOLL_CTL_ADD
	EPOLL_CTL_MOD
	EPOLL_CTL_DEL
	EPOLLIN
	EPOLLOUT
struct epoll_event:
	struct epoll_event {
		uint32_t     events;   //  Epoll events
		epoll_data_t data;      // User data variable
	};
	typedef union epoll_data {
		void    *ptr;
		int      fd;
		uint32_t u32;
		uint64_t u64;
	} epoll_data_t;


**/

namespace sylar {
	struct FdChannel;
	class IOManager:public schedule
	{
	public:
		enum event
		{
			NONE	= 0x00,
			READ	= 0x01,
			WRITE	= 0x04,
		};
	public:

		IOManager(int threadNum, bool use_caller = true, const std::string &name = "root");
		~IOManager();

		void addEvent(int fd, event f_event, std::function<void()> cb);
		void delEvent(int fd, event f_event);
		void cancelEvent();
		void cancelAllEvent();
		void FdResize(int size);
		void newEpollEventCtrl(int fd, int op, void *ptr);

		void stop() override;
		void tickle() override;
		void ideal() override;

	private:
		int m_epfd;
		std::vector<FdChannel *> m_fds;
		int pipefd[2];

	};
	struct FdChannel
	{
		Mutex m_mutex;

		struct eventContext
		{
			sylar::schedule* m_schedule;
			sylar::fiber::ptr m_fiber;
			std::function<void()> m_func;
		};
		eventContext& getEventContext(sylar::IOManager::event f_event);
		void reset(eventContext &m_ctx);
		eventContext Write;
		eventContext Read;
		int fd;
		sylar::IOManager::event m_event;
	};
	 
}
