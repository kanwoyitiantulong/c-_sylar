#pragma once
//这个类用来创建event和logger
#include "logEvent.h"
#include "logger.h"

#define  SYLA_LOG_BASE(lg,level) if(lg->getLevel()<level) \ 
										sylar::eventWar(sylar::logEvent::ptr(new sylar::logEvent(lg, \
										sylar::logLevel::Debug,std::string(__FILE__),__LINE__,0,0,1,std::string("root")))).getMss()

#define  SYLA_LOG_FMT_BASE(lg,level,fmt,...) if(lg->getLevel()<level) \ 
											sylar::eventWar(sylar::logEvent::ptr(new sylar::logEvent(lg, \
												sylar::logLevel::Debug, std::string(__FILE__), __LINE__, 0,\
												0, 1, std::string("root")))).getEvent()->setMss(fmt, __VA_ARGS__)

namespace sylar {
	class eventWar
	{
	public:
		eventWar(logEvent::ptr event):m_event(event) {}
		~eventWar() { m_event->getLogger()->log(m_event->getLogger(),m_event->getLevel(),m_event); }
		std::ostringstream& getMss() { return m_event->getMss(); }
		logEvent::ptr getEvent() { return m_event; }
	private:
		logEvent::ptr m_event;
	};
}
