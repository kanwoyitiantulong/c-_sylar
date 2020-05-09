#include "config.h"
namespace sylar {

	config::config(std::string fileName):file(fileName){
		Init();
	}
	void config::Init() {
		loadYaml();
		for (auto temp : varMp) {
			std::cout << "key:\t" << temp.first << "\tvalue:\t" << temp.second->printToString() << std::endl;
		}
	}



	void config::prase_yaml(std::string prefix, YAML::Node node, std::list<std::pair<std::string, const YAML::Node>>& output) {
		output.push_back(std::make_pair(prefix, node));
		if (node.IsMap()) {
			for (auto it = node.begin();it != node.end();it++) {
				prase_yaml(prefix.empty() ? it->first.Scalar() : prefix + "." + it->first.Scalar(), it->second, output);
			}
		}
	}

	void config::loadYaml() {
		std::list<std::pair<std::string, const YAML::Node>>allnode;
		YAML::Node root = YAML::LoadFile(file);
		prase_yaml("", root, allnode);
		std::cout <<"size:	"<< allnode.size() << std::endl;
		for (auto temp : allnode) {
			std::string key = temp.first;
			if (key.empty())continue;
			configbase::ptr var = lookupBase(key);
			if (var) {
				std::ostringstream ss;
				ss << temp.second;
				var->getFromString(ss.str());//更新
			}
			else {
				std::ostringstream ss;
				ss << temp.second;
				lookup(key, ss.str(), std::string(""));//创建
				std::cout << "creat new" << std::endl;
			}
		}
	}

	configbase::ptr config::lookupBase(std::string str) {
		auto it = varMp.find(str);
		if (it != varMp.end())return it->second;
		return nullptr;
	}




}