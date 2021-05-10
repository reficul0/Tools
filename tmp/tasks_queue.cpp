#include "pch.h"
#include "tasks_queue.h"

namespace tools {

class tasks_queue::Impl 
{
public:
	~Impl()
	{
		flush();
	}

	void push(task_t task)
	{
		{ 
			boost::unique_lock<decltype(tasks_mutex_)> _(tasks_mutex_);
			tasks_.push(task);
		}
		cond_.notify_one();
	}
	task_t pop()
	{
		task_t next_task;
		{
			boost::unique_lock<decltype(tasks_mutex_)> _(tasks_mutex_);

			BOOST_ASSERT(tasks_.empty());

			next_task = std::move(tasks_.front());
			tasks_.pop();
		}
		return std::move(next_task);
	}

	void wait_for_any_task() const
	{
		boost::unique_lock<decltype(tasks_mutex_)> tasks_lock(tasks_mutex_);
		cond_.wait(tasks_lock, [this]() { return !tasks_.empty(); });
	}
	bool empty() const
	{
		boost::shared_lock<decltype(tasks_mutex_)> _(tasks_mutex_);
		return tasks_.empty();
	}

	void flush() const
	{
		boost::unique_lock<decltype(tasks_mutex_)> tasks_lock(tasks_mutex_);
		cond_.notify_all();
	}
private:
	std::queue<task_t> tasks_;
	mutable boost::shared_mutex tasks_mutex_;
	mutable boost::condition_variable_any cond_;
};

tasks_queue::tasks_queue()
	: p_impl_(std::make_unique<Impl>())
{

}
tasks_queue::~tasks_queue() = default;

void tasks_queue::push(tasks_queue::task_t task)
{
	p_impl_->push(std::move(task));
}
tasks_queue::task_t tasks_queue::pop()
{
	return p_impl_->pop();
}

void tasks_queue::wait_for_any_task() const
{
	p_impl_->wait_for_any_task();
}
bool tasks_queue::empty() const
{
	return p_impl_->empty();

}

void tasks_queue::flush() const
{
	p_impl_->flush();

}
}