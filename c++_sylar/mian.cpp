#include "logEvent.h"
#include "logFormat.h"
#include "logger.h"
#include "log.h"
#include "logger.h"
#include "logAppender.h"
#include "eventWar.h"
#include<iostream>
/*
int main() {
	sylar::logger::ptr m_logger(new sylar::logger);
	sylar::logFormat::ptr logformat(new sylar::logFormat);
	sylar::stdoutAppender::ptr stdoutApd(new sylar::stdoutAppender(logformat));
	sylar::fileAppender::ptr fileappd(new sylar::fileAppender(logformat));
	
	m_logger->addAppender(stdoutApd);
	m_logger->addAppender(fileappd);
	sylar::logEvent::ptr event(new sylar::logEvent(m_logger, sylar::logLevel::Info,std::string(__FILE__),__LINE__,0,0,1,std::string("root")));
	//std::cout << logformat->getFmt() << std::endl;
	//std::cout << logformat->getMsg(logger, event->getLevel(), event) << std::endl;

	m_logger->log(m_logger, event->getLevel(), event);
	SYLA_LOG_BASE(m_logger, sylar::logLevel::Info) << "hello log!!!!";
	SYLA_LOG_FMT_BASE(m_logger, sylar::logLevel::Info,"%s","hellpg log fmt!!!!");



	return 0;
}*/