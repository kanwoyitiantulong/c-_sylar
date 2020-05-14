#include "IOManager.h"
#include <sys/epoll.h>
#include <fcntl.h>              /* Obtain O_* constant definitions */
#include <unistd.h>

#include <iostream>
#include <cstring>

namespace sylar {

	IOManager::IOManager(int threadNum, bool use_caller, const std::string &name)
		:schedule(threadNum, use_caller,name ){
		m_epfd=epoll_create(5000);
		FdResize(64);
		int ret=pipe2(pipefd, O_NONBLOCK);
		if (ret != 0) {
			std::cout << "creat pipe failed" << std::endl;
		}
		fcntl(pipefd[0], F_SETFD, O_NONBLOCK);
		int op = EPOLLET | EPOLLIN;
		newEpollEventCtrl(pipefd[0], op, nullptr);

	}
	IOManager::~IOManager() {
		close(m_epfd);
		close(pipefd[0]);
		close(pipefd[1]);
		for (int i = 0;i < m_fds.size();i++) {
			if (m_fds[i]) {
				delete m_fds[i];
				m_fds[i] = nullptr;
			}
		}
	}

	void IOManager::addEvent(int fd,event f_event,std::function<void()> cb) {
		FdChannel *ch = nullptr;
		if (fd > m_fds.size()) {
			FdResize(fd*1.5);
		}
		ch = m_fds[fd];
		if (ch->m_event & f_event) {
			std::cout << "event has been added:event:\t" << f_event << std::endl;
		}
		int op = ch->m_event ? EPOLL_CTL_MOD : EPOLL_CTL_ADD;
		ch->m_event = (event)(EPOLLET | f_event | ch->m_event);
		newEpollEventCtrl(fd, ch->m_event, ch);
		FdChannel::eventContext &m_ctx=ch->getEventContext(f_event);
		if (cb) {
			m_ctx.m_func.swap(cb);
		}
		else {
			m_ctx.m_fiber = sylar::fiber::getThis();
		}
	}
	void IOManager::delEvent(int fd, event f_event) {
		if (fd > m_fds.size())return;
		FdChannel* ch = m_fds[fd];
		ch->m_event = (event)(ch->m_event & ~f_event);
		int op=ch->m_event? EPOLL_CTL_MOD : EPOLL_CTL_DEL;
		newEpollEventCtrl(fd, op, ch);
		FdChannel::eventContext &m_ctx = ch->getEventContext(f_event);
		ch->reset(m_ctx);
	}
	void IOManager::cancelEvent() {
	
	}
	void IOManager::cancelAllEvent() {}
	void IOManager::FdResize(int size) {}

	FdChannel::eventContext& FdChannel::getEventContext(sylar::IOManager::event f_event) {}
	void IOManager::newEpollEventCtrl(int fd, int op, void *ptr) {
		struct epoll_event m_event;
		memset(&m_event, 0, sizeof(m_event));
		m_event.events = EPOLLET | op;
		m_event.data.fd = fd;
		m_event.data.ptr = ptr;
		epoll_ctl(m_epfd, op, fd, &m_event);
	}
	void FdChannel::reset(eventContext &m_ctx) {}

	void IOManager::stop() {}
	void IOManager::tickle() {
		write(pipefd[1], "T", 1);
	}
	void IOManager::ideal() {
		epoll_event *events_l = new epoll_event[64]();
		int timeout = 5000;
		int ret=epoll_wait(m_epfd, events_l, 64,timeout);


	}
	
}