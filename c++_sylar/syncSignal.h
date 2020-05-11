#pragma once
#include <semaphore.h>
#include <atomic>
namespace sylar {
	/**
		int sem_init(sem_t *sem,int pshared,unsigned int value); 
		int sem_destroy(sem_t *sem);
		int sem_wait(sem_t *sem);  // 资源减少1
		int sem_trywait(sem_t *sem);
		int sem_post(sem_t *sem);  // 资源增加1
		int sem_getvalue(sem_t *sem);

	**/
	class semaphore {
	public:
		semaphore(unsigned int count);
		~semaphore();

		void wait();
		void notify();

	private:
		sem_t m_sem;
	};
	class Mutex {
		/***
		int pthread_mutex_init(pthread_mutex_t *restrict mutex, const pthread_mutexattr_t *restrict attr);
		int pthread_mutex_destroy(pthread_mutex_t *mutex);

		int pthread_mutex_lock(pthread_mutex_t *mutex);
		int pthread_mutex_trylock(pthread_mutex_t *mutex);
		int pthread_mutex_unlock(pthreadd_mutex_t *mutex);
		int pthread_mutex_timedlock(pthread_mutex_t *restrict mutex, const struct timesec *restrict tsptr);
		**/
	public:
		Mutex();
		~Mutex();
		void lock();
		void unlock();
	private:
		pthread_mutex_t m_mutex;
		bool isLock;
	};
	class RWLock {
	/**
		int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock, const pthread_rwlockattr_t *restrict attr);
		int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);

		int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
		int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);
		int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);

		int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
		int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
	**/
	public:
		RWLock();
		~RWLock();
		void readLock();
		void writeLock();
		void unlock();
	private:
		pthread_rwlock_t m_rwLock;
		std::atomic<bool> isLock;
	};
	class cond {
	/**
		int pthread_cond_init(pthread_cond_t *restrict cond, const pthread_condattr_t *restrict attr);
		int pthreead_cond_destroy(pthread_cond_t *cond);
		int pthread_cond_wait(pthread_cond_t *restrict cond, pthread_mutex_t *restrict mutex);
	**/
	public:
		cond();
		~cond();
		void wait();
	private:
		pthread_cond_t m_cond;
	};
	class SpinLock {
	/**
		PTHREAD_PROCESS_PRIVATE  //线程共享
		PTHREAD_PROCESS_SHARED	//进程共享
		int pthread_spin_init(pthread_spinlock_t *lock, int pshared);
		int pthread_spin_destroy(pthread_spinlock_t *lock);

		int pthread_spin_lock(pthread_spinlock_t *lock);
		int pthread_spin_trylock(pthread_spinlock_t *lock);
		int pthread_spin_unlock(pthread_spinlock_t *lock);
	**/
	public:
		SpinLock();
		~SpinLock();
		void lock();
		void unlock();
	private:
		pthread_spinlock_t m_spinlock;
	};

	/**
		lock_guards  mutex spinlock
	**/
	template<class T>
	class MutexGuard {
	public:
		MutexGuard(T& lock):m_mutex(lock) {
			m_mutex.lock();
		}
		~MutexGuard() {
			m_mutex.unlock();
		}
	private:
		T &m_mutex;
	};
	/**
		lock_guard rwlock
	**/
	template<class T>
	class readLock {
	public:
		readLock(T& lock):m_rLock(lock) {
			m_rLock.readLock();
		}
		~readLock() {
			m_rLock.unlock();
		}
	private:
		T &m_rLock;
	};
	template<class T>
	class writeLock {
	public:
		writeLock(T& lock):m_wLock(lock) {
			m_wLock.writeLock();
		}
		~writeLock() {
			m_wLock.unlock();
		}
	private:
		T &m_wLock;
	};








}