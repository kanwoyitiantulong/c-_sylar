#include <iostream>
#include<vector>
#include <unistd.h>
#include "thread.h"

long long count = 0;
sylar::Mutex l_mutex;
sylar::SpinLock spin_mutex;
sylar::RWLock rwlock;
void fun1() {
	std::cout << "test fun1" << std::endl;
	for (int i = 0;i < 10;i++)
	{
		//sylar::MutexGuard<sylar::SpinLock> s(spin_mutex);
		sylar::writeLock<sylar::RWLock> s(rwlock);
		//l_mutex.lock();
		count=count+1;
		///std::cout << count << std::endl;
		//l_mutex.unlock();
	}	
}

int main() {
	std::vector<sylar::thread::ptr> vec;
	for (int i = 0;i <20;i++) {
		sylar::thread::ptr lthread(new sylar::thread("first", &fun1));
		vec.push_back(lthread);
		//sleep(2);
	}
	
	for (int i = 0;i < vec.size();i++)
		vec[i]->join();
	std::cout << "count:\t" << count << std::endl;
	return 0;
}