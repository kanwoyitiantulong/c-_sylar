#pragma once
/*
	负责决定将日志打印到什么位置
*/
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include <sstream>
#include "log.h"
#include "logFormat.h"


namespace sylar {
	class logger;
	class logEvent;
	class logAppender {
	public:
		typedef std::shared_ptr<logAppender> ptr;
		logAppender(std::shared_ptr<logFormat> format,logLevel::Level l=logLevel::Level::Debug):m_format(format),m_level(l) {}
		virtual ~logAppender() {}
		virtual void log(std::shared_ptr<logger> m_logger, logLevel::Level le,std::shared_ptr<logEvent> m_event) =0;

		std::string getName() { return appenderName; }
		void setName(std::string s) { appenderName = s; }
	protected:
		std::string appenderName;
		logLevel::Level m_level;
		std::shared_ptr<logFormat> m_format;
		std::ostringstream m_ostringstream;
	};



	class fileAppender :public logAppender {
	public:
		typedef std::shared_ptr<fileAppender> ptr;
	public:
		fileAppender(std::shared_ptr<logFormat> format, std::string name = "./log.txt", logLevel::Level l = logLevel::Level::Debug);
		~fileAppender() {

			if (wfile.is_open())wfile.close();
		}
	public:
		void log(std::shared_ptr<logger> m_logger, logLevel::Level le, std::shared_ptr<logEvent>  m_event) override;
	private:
		void Init();
		//std::string &filename = appenderName;

		std::ofstream wfile;
		std::ostringstream m_os;
		std::mutex m_mutex;
	};



	class stdoutAppender :public logAppender {
	public:
		typedef std::shared_ptr<stdoutAppender> ptr;
	public:
		stdoutAppender(std::shared_ptr<logFormat> format,std::string aname="fileappend", logLevel::Level l = logLevel::Level::Debug) :logAppender(format,l) {
			appenderName = "stdoutAppender";
		}
		void log(std::shared_ptr<logger> m_logger, logLevel::Level le, std::shared_ptr<logEvent>  m_event) override;

	private:
		std::mutex m_mutex;
		std::ostringstream m_os;
	};

}