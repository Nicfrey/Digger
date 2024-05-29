#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t nbThread): m_IsStopping{false}
{
	for(size_t i{}; i < nbThread; ++i)
	{
		m_Workers.emplace_back([this](std::stop_token stop)
		{
				WorkerThread(std::move(stop));
		});
	}
}

ThreadPool::~ThreadPool()
{
	m_IsStopping.store(true);
	m_Condition.notify_all();
	for (auto& worker : m_Workers) {
		if (worker.joinable()) {
			worker.request_stop();
			worker.join();
		}
	}
}

void ThreadPool::EnqueueTask(std::function<void()> function)
{
	std::unique_lock<std::mutex> lock(m_Mutex);
	m_Tasks.emplace(std::move(function));
	m_Condition.notify_one();
}

void ThreadPool::WorkerThread(std::stop_token stopToken)
{
	while(!stopToken.stop_requested() && !m_IsStopping.load())
	{
		std::function<void()> task;
		std::unique_lock<std::mutex> lock(m_Mutex);
		m_Condition.wait(lock, [this, &stopToken]
			{
				return !m_Tasks.empty() || stopToken.stop_requested() || m_IsStopping.load();
			});
		if ((m_IsStopping.load() || stopToken.stop_requested()) && m_Tasks.empty())
		{
			return;
		}
		task = std::move(m_Tasks.front());
		m_Tasks.pop();
		task();
	}
}

