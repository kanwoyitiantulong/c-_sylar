#include "syncSignal.h"
#include <iostream>
namespace sylar {
	semaphore::semaphore(unsigned int count) {
		if (sem_init(&m_sem, 0, count)) {
			std::cout << "init sem error" << std::endl;
		}
	}
	semaphore::~semaphore() {
		sem_destroy(&m_sem);
	}

	void semaphore::wait() {
		if (sem_wait(&m_sem)) {
			std::cout << "sem_wait error" << std::endl;
		}
	}
	void semaphore::notify() {
		if (sem_post(&m_sem)) {
			std::cout << "sem_post error" << std::endl;
		}
	}

	Mutex::Mutex() {
		pthread_mutex_init(&m_mutex, nullptr);
		isLock = false;
	}
	Mutex::~Mutex() {
		pthread_mutex_destroy(&m_mutex);
	}
	void Mutex::lock() {
		pthread_mutex_lock(&m_mutex);
	}
	void Mutex::unlock() {
		pthread_mutex_unlock(&m_mutex);

	}

	RWLock::RWLock() {
		pthread_rwlock_init(&m_rwLock, nullptr);
	}
	RWLock::~RWLock() {
		pthread_rwlock_destroy(&m_rwLock);
	}
	void RWLock::readLock() {
		pthread_rwlock_rdlock(&m_rwLock);
	}
	void RWLock::writeLock() {
		pthread_rwlock_wrlock(&m_rwLock);
	}
	void RWLock::unlock() {
		pthread_rwlock_unlock(&m_rwLock);
	}


	cond::cond() {
		pthread_cond_init(&m_cond,nullptr);
	}
	cond::~cond() {
		pthread_cond_destroy(&m_cond);
	}
	void cond::wait() {
		pthread_cond_wait(&m_cond,NULL);
	}


	SpinLock::SpinLock() {
		pthread_spin_init(&m_spinlock,0);
	}
	SpinLock::~SpinLock() {
		pthread_spin_destroy(&m_spinlock);
	}
	void SpinLock::lock() {
		pthread_spin_lock(&m_spinlock);
	}
	void SpinLock::unlock() {
		pthread_spin_unlock(&m_spinlock);
	}
}