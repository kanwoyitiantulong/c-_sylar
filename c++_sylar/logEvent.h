#pragma once
#include<string>
#include<sstream>
#include<thread>
#include<stdarg.h>

#include <unistd.h>
#include <sys/syscall.h>

#include"log.h"


namespace sylar {
	class logger;
	class logEvent {
	public:
		typedef std::shared_ptr<logEvent> ptr;
		logEvent(std::shared_ptr<logger> &m_logger,logLevel::Level m_level, std::string filename, uint32_t fileLine, time_t elaps_time, time_t local_time, pid_t thread_id, std::string threadName); 
	public:
		//获取信息
		pid_t getThreadId() { return thread_id; }
		std::string getThreadName() { return thread_name; }

		std::string getFileName() { return m_file; }
		uint32_t getFileLine() { return m_line; }

		uint64_t getTime() { return elaps_time; }
		time_t getLocalTime() { return local_time; }

		std::ostringstream& getMss() { return m_ss; }
		std::string getMessage() { return m_ss.str(); }

		logLevel::Level getLevel() {return eventLevel;}
		std::shared_ptr<logger> getLogger() { return m_loggre; }

		pid_t getId() { return syscall(__NR_gettid); }

		//设置event信息
		void setThreadId() { thread_id = getId(); }
		void setTime() { elaps_time = time(NULL); }
		void setLocalTime() {	local_time = time(NULL);}
		void setMss(char* fmt,...) {
			char *buff;
			va_list   arg_ptr;   //定义可变参数指针 
			va_start(arg_ptr, fmt);   // i为最后一个固定参数
			vasprintf(&buff, fmt, arg_ptr);
			va_end(arg_ptr);        //  清空参数指针
			m_ss << buff;
			free(buff);
		}
		void setMss(std::string msg) {
			m_ss << msg;
		}
		void setLogLevel(logLevel::Level& l) { eventLevel = l; }
	private:
		pid_t				thread_id;		//event 船舰线程
		std::string			thread_name;	//线程名
		std::string			m_file;			//文件名
		uint32_t			m_line;			//行号

		time_t				elaps_time;		//event 运行时间
		time_t				local_time;     //event 时间戳

		std::ostringstream  m_ss;			//event 打印信息E
		logLevel::Level		eventLevel;		//事件等级

		std::shared_ptr<logger> m_loggre;	//日志控制器
	};
}