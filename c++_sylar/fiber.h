#pragma once
#include <functional>
#include <memory>
#include <ucontext.h>
namespace sylar {
	/**
	uc_link主要指向的是当前上下文返回之后指向的上下文
	uc_mcontext主要是保存一些寄存器
	uc_sigmask主要保存阻塞信号量集（apue中有介绍）
	uc_stack主要是保存堆栈信息

	int getcontext(ucontext_t *ucp);
	void makecontext(ucontext_t *ucp, void (*func)(), int argc, ...)
	setcontext(const ucontext_t *ucp)
	int swapcontext(ucontext_t *oucp, const ucontext_t *ucp);
	
	**/
	class fiber:public std::enable_shared_from_this<fiber> {
	public:
		typedef std::function<void()> cbFunc;
		typedef std::shared_ptr<fiber> ptr;
		enum  state
		{
			UNKOWN=0,
			INIT,
			HOLD,
			EXEC,
			TERM,
			READY
		};

		fiber();
		fiber(cbFunc cb,int s_size);
		~fiber();
		void setFunc(cbFunc f);
		void setThisContext(std::shared_ptr<fiber> ptr);
		ucontext_t getContext();
		void swapin();
		void swapout();
		static void reset(ptr fib,cbFunc f);

		static std::shared_ptr<fiber> getThis();
		static int getFiberCount();
//		static void YalIn();
		static void YaloutReady();
		static void YaloutHold();
		static void YalOut();

		static int swapContext();
		static void run(void *args);
		
	private:
		ucontext_t	m_ucp;
		state		m_state;
		int			f_id;
		void		*m_stack;
		int			stack_size;
		cbFunc		m_func;
	};

}
