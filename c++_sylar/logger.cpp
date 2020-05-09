#include "logger.h"
#include "logAppender.h"

namespace sylar {
	/*logger::logger() {
		if (!m_logEvent) {
			m_logEvent=std::shared_ptr<logEvent>(new logEvent());
		}
		if (!m_logFormat) {
			m_logFormat=std::shared_ptr<logFormat>(new logFormat(m_logEvent));
		}
		std::shared_ptr<stdoutAppender> temp=std::shared_ptr<stdoutAppender>(new stdoutAppender(m_logFormat));
		m_map[temp->appenderName] = temp;
	}*/
	logger::logger(std::string loggername, logLevel::Level l)
	:loggerName(loggername)
	, m_level(l)
	{}

	logger::~logger() {}

	std::string logger::getName() { return loggerName; }

	void logger::log(logger::ptr m_logger, logLevel::Level le, std::shared_ptr<logEvent> event) {
		if (!m_map.empty()) {
			std::cout << "m_map is not empty!!!" << std::endl;
			for (auto i : m_map) {
				i.second->log(m_logger, le, event);
			}
		}
		else if (m_root) {
			m_root->log(m_logger, le, event);
		}
	}

	bool logger::addAppender(std::shared_ptr<logAppender> appender) {
		std::cout << "adding appender:	" <<appender->getName()<< std::endl;
		if (appender != nullptr) {
			//首字母需要时字母、数字、下划线
			if (!isalpha(appender->getName()[0]) && !isalnum(appender->getName()[0]) && appender->getName()[0] != '_'&& appender->getName()[0] != '.')return false;
			//限定30个字节长
			appender->setName(appender->getName().substr(0, 30));
			std::cout << "appender name:" << appender->getName() << std::endl;
			auto it = m_map.find(appender->getName());
			if (it == m_map.end()) {
				m_map[appender->getName()] = appender;
				return true;
			}
			else {
				std::cout << "appender name exit!!! plase change appender name or delappender" << std::endl;
				return false;
			}
		}
		return true;
	}
	bool logger::delAppender(std::string name) {
		if (!isalpha(name[0]) && !isalnum(name[0]) && name[0] != '_'&& name[0] != '.')return false;
		//限定30个字节长
		name = name.substr(0, 30);
		auto it = m_map.find(name);
		if (it != m_map.end()) {
			m_map.erase(it);
			return true;
		}
		return true;
	}	
	void logger::listAppender() {
		if (!m_map.empty()) {
			for (auto temp : m_map) {
				std::cout << temp.first << " : " << temp.second << std::endl;;
			}
		}
	}
	std::shared_ptr<logAppender> logger::getAppender(std::string name) {
		if (!m_map.empty()) {
			std::unordered_map<std::string, logAppender::ptr>::iterator it;
			if ((it = m_map.find(name)) != m_map.end()) {
				return it->second;
			}
		}
	}

	std::string logger::getFormat() { return fmt; }
	void logger::setFormat(std::string s) { fmt = s; }
}