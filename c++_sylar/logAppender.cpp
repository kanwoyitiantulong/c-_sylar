#include "logAppender.h"
#include <memory>

namespace sylar {
	fileAppender::fileAppender(std::shared_ptr<logFormat> format, std::string name, logLevel::Level l) :logAppender(format, l) {
		appenderName = name;
		try {
			Init();
		}
		catch (std::exception e) {
			std::cout << e.what() << std::endl;
		}
		catch (...) {
			std::cout << "fileAppender init catch error" << std::endl;
		}
	}

	void fileAppender::Init() {
		if (wfile) {
			wfile.close();
		}
		wfile.open(appenderName, std::ios::out | std::ofstream::app);
		if (!wfile.is_open()) {
			wfile.clear();
			std::cout << "open file :" << appenderName << "error" << std::endl;
			throw std::exception();
		}
	}


	void fileAppender::log(std::shared_ptr<logger> m_logger, logLevel::Level le, std::shared_ptr<logEvent> m_event) {
		std::cout << "enter fileappender log" << std::endl;
		wfile << m_format->getMsg(m_logger, le, m_event);
	}

	void stdoutAppender::log(std::shared_ptr<logger> m_logger, logLevel::Level le, std::shared_ptr<logEvent> m_event) {
		{
			//std::lock_guard<std::mutex> lock(m_mutex);
			if (m_format == nullptr)std::cout << "m_format error" << std::endl;
				std::cout << m_format->getMsg(m_logger, le, m_event);
		}
	}

}