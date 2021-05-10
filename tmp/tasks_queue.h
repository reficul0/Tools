#ifndef TOOLS_TASKS_QUEUE_H
#define TOOLS_TASKS_QUEUE_H

#if defined _MSC_VER && _MSC_VER >= 1020u
#pragma once
#endif

#include <list>
#include <boost/function.hpp>

namespace tools {

	class tasks_queue
	{
	public:
		using task_t = boost::function<void()>;

		tasks_queue();
		~tasks_queue();

		void push(task_t task);
		task_t pop();

		void wait_for_any_task() const;
		bool empty() const;

		void flush() const;
	private:
		class Impl;
		std::unique_ptr<Impl> p_impl_;
	};

}

#endif