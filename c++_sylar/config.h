#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <list>
#include "boost/lexical_cast.hpp" // 需要包含的头文件
#include "yaml-cpp/yaml.h" 
namespace sylar {
	/*
		config选项
			功能：
				变量转换成字符串
				字符串转换到变量类型
				管理所有配置选项（增删改）
	*/

	class configbase
	{
	public:
		typedef std::shared_ptr<configbase> ptr;
		configbase(std::string name, std::string describtion) :m_name(name), describtion(describtion) {}

		std::string& getName() { return m_name; }
		std::string& getDes() { return describtion; }

		void setName(std::string &name) { m_name = name; }
		void setDes(std::string &des) { describtion = des; }

		virtual std::string printToString() = 0;
		virtual void getFromString(std::string str) = 0;
	private:
		std::string m_name;
		std::string describtion;
	};

	template<typename F, typename T>
	class m_lexical_cast{
	public:
		T operator()(const F &v) {
			return boost::lexical_cast<T>(v);
		}
	};
	//string to vector
	template<typename T>
	class m_lexical_cast<std::string,std::vector<T>> {
	public:
		T operator()(const std::string &v) {
			YAML::Node node = YAML::Load(v);
			std::stringstream ss;
			typename std::vector<T> vec;
			for (int i = 0;i < node.size();i++) {
				ss << node[i];
				vec.push_back(m_lexical_cast<std::string,T>(ss.str()));
			}
			return vec;
		}
	};
	//vector to string
	template<typename T>
	class m_lexical_cast<std::vector<T>,std::string> {
	public:
		T operator()(const std::vector<T> &v) {
			YAML::Node node;
			for (int i = 0;i < v.size();i++) {
				node.push_back(m_lexical_cast<T, std::string>(v[i]));
			}
			std::ostringstream ss;
			ss << node;
			return ss.str();
		}
	};


	template<typename T>
	class configVar:public configbase
	{
	public:
		typedef std::shared_ptr<configVar> ptr;
		configVar(std::string name, T v,std::string describtion) :configbase(name, describtion), var(v) {}
		//
		std::string printToString()override { return m_lexical_cast<T,std::string>()(var); }
		void getFromString(std::string str)override { var= m_lexical_cast<std::string,T>()(str); }
		//void getFromString(std::string str)override {  }
	private:
		T var;
	};

	class config {
	public:
		typedef std::shared_ptr<config> ptr;
		config(std::string fileName);

		void prase_yaml(std::string prefix, YAML::Node node, std::list<std::pair<std::string, const YAML::Node>>& output);
		void loadYaml();

		void Init();//解析配置文件

		configbase::ptr lookupBase(std::string str);

		template<typename T>
		typename configVar<T>::ptr lookup(std::string name, T var, const std::string& des) {
			auto it = varMp.find(name);
			if (it != varMp.end()) {
			auto temp = std::dynamic_pointer_cast<configVar<T>>(it->second);
			if (temp) {
			return temp;
			}
			else {
			//std::cout << "dynamic cast failed " << "mp->second type" << typeid(it->second).name() << "temp type:" << typeid(T).name() << std::end;
			return nullptr;
			}
			}
			typename configVar<T>::ptr temp(new configVar<T>(name, var, des));
			varMp[name] = temp;
			return temp;
		}
		template<typename T>
		typename configVar<T>::ptr lookup(const std::string &name) {
			auto it = varMp.find(name);
			if (it != varMp.end())return std::dynamic_pointer_cast<configVar<T>>(it->second);
			return nullptr;
		}

		template<typename T>
		typename configVar<T>::ptr test(T var) {
			return typename configVar<T>::ptr(new configVar<T>("test", var, ""));
		}

	private:
		std::string file;
		std::unordered_map<std::string, configbase::ptr> varMp;
	};
}
