#pragma once
#include "log.h"
#include <vector>
#include <unordered_map>
#include <memory>
namespace sylar {
	class logEvent;
	class logAppender;
	class logger:public std::enable_shared_from_this<logger>
	{
	public:
		typedef std::shared_ptr<logger> ptr;
	public:

		logger(std::string loggername="root",logLevel::Level l = logLevel::Debug);

		~logger();

		std::string getName();

		void log(logger::ptr m_logger,logLevel::Level le,std::shared_ptr<logEvent> event);

		bool addAppender(std::shared_ptr<logAppender> appender);
		bool delAppender(std::string name);
		void listAppender();
		std::shared_ptr<logAppender> getAppender(std::string name);

		std::string getFormat();
		void setFormat(std::string s);

		logLevel::Level getLevel() { return m_level; }

	private:
		logLevel::Level m_level=logLevel::Debug;
		std::string loggerName;
		std::string fmt;
		//std::shared_ptr<logEvent> m_logEvent;
		//logFormat::ptr m_logFormat;
		//std::vector<logAppender::ptr> m_logAppender;
		std::unordered_map<std::string, std::shared_ptr<logAppender>> m_map;
		ptr m_root;
	};
	
}
