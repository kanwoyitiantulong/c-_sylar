#include "fiber.h"
#include <iostream>
#include <atomic>
#include <cstring>
#include<assert.h>
namespace sylar {
	static std::atomic<int>	fiberId{0};
	static std::atomic<int>	fiber_count{ 0 };
	static thread_local fiber*  main_fiber=nullptr;
	static thread_local std::shared_ptr<fiber>  current_fiber=nullptr;
	//thread_local int		 fiber_count;

	fiber::fiber() {
		m_state = EXEC;
		getcontext(&m_ucp);
		//setThisContext();
		main_fiber = this;
	}
	fiber::fiber(cbFunc cb, int s_size) {
		getcontext(&m_ucp);
		m_func = cb;
		stack_size = s_size;
		m_stack = malloc(s_size);
		m_ucp.uc_stack.ss_sp = m_stack;
		m_ucp.uc_stack.ss_size = s_size;
		makecontext(&m_ucp, (void(*)(void))run,1, this);
		m_state = INIT;
		//setThisContext(shared_from_this());
	}
	fiber::~fiber() {
		if (m_stack) {
			assert(m_state == INIT || m_state==READY||m_state == TERM);
			free(m_stack);
			m_stack = nullptr;
		}
		else {
			std::cout << "free mian fiber" << std::endl;
		}
		std::cout << "enter ~fiber" << std::endl;
	}
	
	ucontext_t fiber::getContext() {
		return m_ucp;
	}

	void fiber::setFunc(cbFunc f) {
		m_func = f;
	}
	void fiber::setThisContext(std::shared_ptr<fiber> ptr) {
		current_fiber = ptr;
	}
	int fiber::swapContext() {
		return swapcontext(&main_fiber->m_ucp, &current_fiber->m_ucp);
	}

	void fiber::reset(ptr fib,cbFunc f) {
		getcontext(&fib->m_ucp);
		fib->m_func = f;
		memset(fib->m_stack, 0, fib->stack_size);
		fib->m_ucp.uc_stack.ss_sp = fib->m_stack;
		fib->m_ucp.uc_stack.ss_size = fib->stack_size;
		makecontext(&fib->m_ucp, (void(*)(void))run, 1, fib->shared_from_this());
		fib->m_state = INIT;
	}

	void fiber::run(void *args) {
		fiber *l_fiber = (fiber*)args;
		l_fiber->m_func();
		l_fiber->m_state = TERM;
		l_fiber->swapout();
	}


	void fiber::swapin() {
		setThisContext(shared_from_this());
		swapcontext(&main_fiber->m_ucp, &m_ucp);
	}
	void fiber::swapout() {
		setThisContext(main_fiber->shared_from_this());
		swapcontext(&m_ucp, &main_fiber->m_ucp);
	}

	std::shared_ptr<fiber> fiber::getThis() {
		if (current_fiber)return current_fiber;
		sylar::fiber::ptr l_fiber(new fiber());
		main_fiber=l_fiber.get();
		current_fiber = l_fiber;
		return current_fiber;
	}
	int fiber::getFiberCount() {
		return fiber_count;
	}
	//void fiber::YalIn() {}
	void fiber::YaloutReady() {
		current_fiber->m_state = sylar::fiber::READY;
		current_fiber->swapout();
	}
	void fiber::YaloutHold() {
		current_fiber->m_state = sylar::fiber::HOLD;
		current_fiber->swapout();
	}
	void fiber::YalOut() {
		current_fiber->swapout();
	}




}