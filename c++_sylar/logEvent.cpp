#include"logEvent.h"
namespace sylar {
	logEvent::logEvent(std::shared_ptr<logger> &m_logger,logLevel::Level m_level, std::string filename, uint32_t fileLine, time_t elaps_time, time_t local_time, pid_t thread_id, std::string threadName)
		:m_loggre(m_logger)
		,eventLevel(m_level)
		,m_file(filename)
		,m_line(fileLine)
		,elaps_time(elaps_time),
		local_time(local_time),
		thread_id(thread_id),
		thread_name(threadName){};

}