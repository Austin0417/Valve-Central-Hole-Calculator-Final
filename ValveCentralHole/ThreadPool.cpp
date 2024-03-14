#include "ThreadPool.h"


ThreadPool::ThreadPool(int num_threads)
{
	setNumThreads(num_threads);
}

void ThreadPool::setNumThreads(int num_threads)
{
	num_threads_ = num_threads;
	// Creating the specified number of worker threads, and place them in the vector
	for (int i = 0; i < num_threads_; i++)
	{
		threads_.emplace_back([this]()
			{
				while (true)
				{
					std::function<void()> task;

					{
						std::unique_lock<std::mutex> lock(mutex_);

						// If the queue is non-empty, or the stop signal is indicated, release the lock
						// Until then, the thread remains in an idle state
						cv_.wait(lock, [this]()
							{
								return !tasks_.empty() || stop_;
							});

						// All tasks are completed and stop is indicated, terminate this worker thread
						if (stop_ && tasks_.empty())
						{
							return;
						}

						// If we are here, then that means there is an available task in the queue, this thread should execute it
						task = tasks_.front();
						tasks_.pop();
					}
					task();
				}
			});
	}
}

int ThreadPool::getNumThreads() const
{
	return num_threads_;
}



void ThreadPool::enqueue(const std::function<void()>& task)
{
	{
		std::unique_lock<std::mutex> lock(mutex_);
		tasks_.push(task);
	}
	cv_.notify_one();
}

const std::queue<std::function<void()>>& ThreadPool::getTaskQueue() const
{
	return tasks_;
}

ThreadPool::~ThreadPool()
{
	{
		std::unique_lock<std::mutex> lock(mutex_);
		stop_ = true;
	}

	cv_.notify_all();

	// Main thread waits for worker threads to finish any remaining work
	for (auto& thread : threads_)
	{
		thread.join();
	}
}


