#ifndef TOOLS_TASK_WRAPPED_H
#define TOOLS_TASK_WRAPPED_H

#if defined _MSC_VER && _MSC_VER >= 1020u
#pragma once
#endif

#include <boost/thread/thread.hpp>

namespace tools
{
	namespace detail
	{
		/*
		Проблема:
		 Задача может
			получить прерывание, адресованное другой задаче.
		Решение:
		 Обёртка, сбрасывающая все предыдущие прерывания.
		*/
		template<class TaskT>
		class task_wrapped
		{
		public:
			explicit task_wrapped(TaskT wrap_me)
				: task_unwrapped_(std::move(wrap_me))
			{
			}
			void operator()() const {
				try
				{
					boost::this_thread::interruption_point();
				}
				catch (boost::thread_interrupted const&) {}

				task_unwrapped_();
			}
		private:
			TaskT task_unwrapped_;
		};

		template<class TaskT>
		inline task_wrapped<TaskT> make_task_wrapped(TaskT wrap_me)
		{
			return task_wrapped<TaskT>(wrap_me);
		}
	}
}

#endif
