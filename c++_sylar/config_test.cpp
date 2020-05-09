#include "config.h"
#include <iostream>
#include <string>
# include<cstdlib>
#include <vector>
#include <sstream>
#include "boost/lexical_cast.hpp" // 需要包含的头文件
#include "yaml-cpp/yaml.h" 
#include "config.h"

void listYaml(const YAML::Node &node,int level) {
	if (node.IsScalar()) {
		std::cout << std::string(level*4,'-')<<node.as<std::string>()<<":"<<node.Type()<<":"<<level <<std::endl;
	}
	else if (node.IsSequence()) {
		std::ostringstream ss;
		ss << node;
		std::cout << "node sequeuence: " << ss.str() << std::endl;
	}
	else if (node.IsMap()) {
		for (auto it = node.begin();it != node.end();it++) {
			//std::cout << "map key: " << it->first << "	map second: " << it->second<<"	map second type: "<<it->second.Type()<<std::endl;
			listYaml(it->second,level+1);
		}
	}
}


void print_yaml(const YAML::Node& node, int level) {
	if (node.IsScalar()) {
		std::cout<< std::string(level * 4, ' ')
			<< node.Scalar() << " - " << node.Type() << " - " << level<<std::endl;
	}
	else if (node.IsNull()) {
		std::cout<< std::string(level * 4, ' ')
			<< "NULL - " << node.Type() << " - " << level;
	}
	else if (node.IsMap()) {
		for (auto it = node.begin();
			it != node.end(); ++it) {
			//std::cout << std::string(level * 4, ' ')
			//	<< it->first << " - " << it->second.Type() << " - " << level;
			print_yaml(it->second, level + 1);
		}
	}
	else if (node.IsSequence()) {
		for (size_t i = 0; i < node.size(); ++i) {
			//std::cout<< std::string(level * 4, ' ')
				//<< i << " - " << node[i].Type() << " - " << level;
			print_yaml(node[i], level + 1);
		}
	}
}


YAML::Node ymal_test() {
	YAML::Node config = YAML::LoadFile("/home/fang/projects/sylar/config.yaml");
	return config;
}
class test {
public:
	typedef std::shared_ptr<test> ptr;
	template<typename T>
	T te(T var) {
		std::cout << var + var << std::endl;
		return var + var;
	}
};
int main() {
	test::ptr t(new test);
	t->te(std::string("haha"));

	sylar::configVar<int> var("test", 1,"");
	sylar::configVar<int>::ptr tvar(new sylar::configVar<int>("test", 1, ""));
	std::cout <<"test:	" <<var.printToString() << std::endl;

//	listYaml(ymal_test(),0);
//	print_yaml(ymal_test(),0);

	sylar::config::ptr conf(new sylar::config("/home/fang/projects/sylar/config.yaml"));
	conf->test(1);
	sylar::configVar<int>::ptr a=conf->lookup("test", 1,"");
	std::cout << "a:	" << a->printToString() << std::endl;
	auto temp=conf->lookupBase(std::string("vec2"));
	temp->printToString();
	std::cout <<"printToString:\t" <<temp->printToString() << std::endl;
	YAML::Node node = YAML::Load(temp->printToString());
	if (node.IsSequence()) {
		for (int i = 0;i < node.size();i++) {
			std::cout << node[i].as<std::string>() << std::endl;
		}
	}

	return 0;
}