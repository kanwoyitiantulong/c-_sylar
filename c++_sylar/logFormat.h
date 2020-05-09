#pragma once

/*

std::string			eventName;		//event 名称
std::thread::id		thread_id;		//event 船舰线程
time_t				creat_time;		//event 创建时间
time_t				local_time;     //event 时间戳
std::ostringstream  m_ss;			//event 打印信息E

logLevel::Level		eventLevel;		//事件等级

	该类用来定义需要输出的打印信息前缀


*/
#include <map>
#include <functional>
#include <sstream>
#include <vector>
#include <iostream>
#include <mutex>
#include <memory>
#include "log.h"
#include "logEvent.h"
#include "logger.h"


namespace sylar {
	class logFormat
	{
	public:
		typedef std::shared_ptr<logFormat> ptr;

		logFormat(std::shared_ptr<logEvent> event=nullptr,std::string fmt= "%T%d{%Y-%m-%d %H:%M:%S}%T%t%T%N%T%F%T[%p]%T[%c]%T%f:%l%T%m%n");
		~logFormat() {};
		void fraseFmt();
		void setFmt(std::string m_fmt);
		std::string getFmt();


		//void printMsg() { std::cout << m_os.str() << std::endl; }
		std::string getMsg(std::shared_ptr<logger>m_logger, logLevel::Level le, std::shared_ptr<logEvent> event);

		class formatItem {
		public:
			formatItem() {}
			formatItem(std::string s) {}
			typedef std::shared_ptr<formatItem> ptr;
		public:
			virtual ~formatItem() {};
			virtual void log(std::ostringstream &m_os,std::shared_ptr<logger>m_logger,logLevel::Level le, std::shared_ptr<logEvent> event)=0;
		};

		static std::map<std::string, std::function<logFormat::formatItem::ptr(std::string s)>> m_mp;
	private:
		//void log(std::ostringstream &os);

		std::ostringstream  m_ostringstream;
		std::shared_ptr<logEvent> m_event;
		std::string fmt;
		std::vector<std::pair<std::string, std::string>> vec;
		std::mutex  vec_mutex;
	};
	class stringItem :public logFormat::formatItem {		 //m
	public:
		stringItem(std::string &s):msg(s) {}
		void log(std::ostringstream &m_os, std::shared_ptr<logger>m_logger, logLevel::Level le, std::shared_ptr<logEvent> event)override {
			m_os << msg;
		}
	private:
		std::string msg;
	};
	class messageItem :public logFormat::formatItem {		 //m
	public:
		messageItem(std::string s) {}
		void log(std::ostringstream &m_os, std::shared_ptr<logger>m_logger, logLevel::Level le, std::shared_ptr<logEvent> event)override {
			m_os << event->getMessage();
		}
	};
	class evemtLevelItem :public logFormat::formatItem {		 //m
	public:
		evemtLevelItem(std::string s) {}
		void log(std::ostringstream &m_os, std::shared_ptr<logger>m_logger, logLevel::Level le, std::shared_ptr<logEvent> event)override {
			m_os << sylar::logLevel::levetToString(event->getLevel());
		}
	};

	 class elapseTimeItem :public logFormat::formatItem {	//n
	 public:
		 elapseTimeItem(std::string s) {}
		 void log(std::ostringstream &m_os, std::shared_ptr<logger>m_logger, logLevel::Level le, std::shared_ptr<logEvent> event)override {
			 m_os << event->getTime();
		 }
	 };
	 class loggerNameItem :public logFormat::formatItem {	//n
	 public:
		 loggerNameItem(std::string s) {}
		 void log(std::ostringstream &m_os, std::shared_ptr<logger>m_logger, logLevel::Level le, std::shared_ptr<logEvent> event)override {
			 m_os << event->getLogger()->getName();
		 }
	 };
	 class threadIdItem :public logFormat::formatItem {		//tid
	 public:
		 threadIdItem(std::string s) {}
		 void log(std::ostringstream &m_os, std::shared_ptr<logger>m_logger, logLevel::Level le, std::shared_ptr<logEvent> event)override {
			 m_os << event->getThreadId();
		 }
	 };

	 class nowTimeItem :public logFormat::formatItem {		//tm
	 public:
		 nowTimeItem(std::string s) {
			// if (s.size()>1)fmt = s;
			// std::cout << s << std::endl;
		 }
		 void log(std::ostringstream &m_os, std::shared_ptr<logger>m_logger, logLevel::Level le, std::shared_ptr<logEvent> event)override {
			 char buff[128] = {};
			 time_t m_time;
			 struct tm *local_time;
			 //m_time=event->getLocalTime();
			 m_time = time(NULL);
			 local_time = localtime(&m_time);
			 strftime(buff, sizeof(buff), fmt.c_str(), local_time);
			 std::cout << "\t"<<buff << std::endl;
			 m_os << std::string(buff);
		 }
	 private:
		 std::string fmt = "%Y-%m-%d  %I:%M:%S";
	 };
	 class enterItem :public logFormat::formatItem {		//endl
	 public:
		 enterItem(std::string s) {}
		 void log(std::ostringstream &m_os, std::shared_ptr<logger>m_logger, logLevel::Level le, std::shared_ptr<logEvent> event)override {
			 m_os << std::endl;
		 }
	 };

	 class fileNameItem :public logFormat::formatItem {		//ctm
	 public:
		 fileNameItem(std::string s) {}
		 void log(std::ostringstream &m_os, std::shared_ptr<logger>m_logger, logLevel::Level le, std::shared_ptr<logEvent> event)override {
			 m_os << event->getFileName();
		 }
	 };
	 class lineNumItem :public logFormat::formatItem {		//l
	 public:
		 lineNumItem(std::string s) {}
		 void log(std::ostringstream &m_os, std::shared_ptr<logger>m_logger, logLevel::Level le, std::shared_ptr<logEvent> event)override {
			 m_os << event->getFileLine();
		 }
	 };
	 class tabItem :public logFormat::formatItem {		//t
	 public:
		 tabItem(std::string s) {}
		 void log(std::ostringstream &m_os, std::shared_ptr<logger>m_logger, logLevel::Level le, std::shared_ptr<logEvent> event)override {
			 m_os << "\t";
		 }
	 };

	 class corThreadId :public logFormat::formatItem {		//t
	 public:
		 corThreadId(std::string s) {}
		 void log(std::ostringstream &m_os, std::shared_ptr<logger>m_logger, logLevel::Level le, std::shared_ptr<logEvent> event)override {
			// os << "\t";
		 }
	 };
	 class threadNameItem :public logFormat::formatItem {		//t
	 public:
		 threadNameItem(std::string s) {}
		 void log(std::ostringstream &m_os, std::shared_ptr<logger>m_logger, logLevel::Level le, std::shared_ptr<logEvent> event)override {
			 m_os << event->getThreadName();
		 }
	 };


}