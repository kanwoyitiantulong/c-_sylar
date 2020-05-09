#pragma once
/*
	2020年4月27日
	功能：类似log4j
	结构：
		log：		对外使用接口
		logEvent:	事件，存放需要打印的信息和打印的内容
		logFormat:	日志信息输出格式
		logger:		执行日志输出
		logAppender:日志输出位置

*/
#include <string>
namespace sylar {

	class logLevel
	{
	public:
		logLevel() {};
		~logLevel() {};
		enum Level
		{
			None	=	0,
			Debug	=	1,
			Info	=	2,
			Warn	=	3,
			Error	=	4,
			Fatel	=	5
		};
		static std::string levetToString(Level l) {
			switch (l)
			{
#define XX(name) \
				case sylar::logLevel::name: return #name
				XX(None);
				XX(Debug);
				XX(Info);
				XX(Warn);
				XX(Error);
				XX(Fatel);
#undef XX
			default:
				return "unknown";
			}
		}
		static logLevel::Level stringToLevel(std::string s) {

#define XX(level,str) if(#str==s)	return Level::level
			XX(None,None);
			XX(Debug,Debug);
			XX(Info,Info);
			XX(Warn,Warn);
			XX(Error,Error);
			XX(Fatel,Fatel);

			XX(None, none);
			XX(Debug, debug);
			XX(Info, info);
			XX(Warn, warn);
			XX(Error, error);
			XX(Fatel, fatel);
#undef XX
			return logLevel::None;
		}
	private:

	};
}