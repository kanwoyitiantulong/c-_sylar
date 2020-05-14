#include <iostream>

#include <unistd.h>

#include "schedule.h"



void test() {
	std::cout << "test schedule" << sylar::thread::GetThreadId()<< std::endl;
	sleep(1);
}
void test1(int count) {
	std::cout << "test1:\t" << count << std::endl;
}

int main() {
	sylar::schedule::ptr  shcedule_t(new sylar::schedule(5));
	shcedule_t->start();
	sleep(10);
	shcedule_t->scheduler(test);
	shcedule_t->scheduler(std::bind(test1,1));

	std::cout << "***********" << std::endl;
	sleep(20);
//	sleep(100);
	shcedule_t->stop();
	return 0;
}