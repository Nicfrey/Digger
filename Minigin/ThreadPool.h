#pragma once
#include <functional>
#include <mutex>
#include <queue>
#include <thread>

class ThreadPool final
{
public:
	ThreadPool(size_t nbThread);
	~ThreadPool();
	ThreadPool(const ThreadPool& other) = delete;
	ThreadPool(ThreadPool&& other) noexcept = delete;
	ThreadPool& operator=(const ThreadPool& other) = delete;
	ThreadPool& operator=(ThreadPool&& other) noexcept = delete;

	void EnqueueTask(std::function<void()> function);
private:
	std::vector<std::jthread> m_Workers;
	std::queue<std::function<void()>> m_Tasks;
	std::mutex m_Mutex;
	std::condition_variable m_Condition;
	std::atomic<bool> m_IsStopping;

	void WorkerThread(std::stop_token stopToken);
};

