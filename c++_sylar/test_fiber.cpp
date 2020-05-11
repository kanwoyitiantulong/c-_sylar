#include "fiber.h"
#include "thread.h"
#include <unistd.h>
#include<vector>
#include<iostream>

void test() {
	sleep(2);
	std::cout << "hello fiber" << std::endl;
	//sylar::fiber::YalOut();
}
void testFiber() {
	std::cout << "start...." << std::endl;
	sylar::fiber::ptr m_fiber = sylar::fiber::getThis();
	sylar::fiber::ptr f_fiber = sylar::fiber::ptr(new sylar::fiber(test, 1024));
	f_fiber->swapin();
	std::cout << "end....." << std::endl;
}
int main() {
	//sylar::fiber m_fiber;
	//sylar::fiber::ptr l_fiber(new sylar::fiber(test, 1024));
	//ucontext_t ucp = l_fiber->getContext();
	//sylar::fiber::swapContext(&ucp);
	std::vector<sylar::thread::ptr> vec;
	for (int i = 0;i < 3;i++) {
		 sylar::thread::ptr t_th(new sylar::thread("namet_"+std::to_string(i),testFiber));
		 vec.push_back(t_th);
	}
	for (int i = 0;i < 3;i++) {
		vec[i]->join();
	}
	return 0;
}