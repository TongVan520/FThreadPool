//
// Created by FireFlower on 2024/12/11.
//

#ifndef FTHREADPOOL_FTHREADSUSPENDER_H
#define FTHREADPOOL_FTHREADSUSPENDER_H

#include <semaphore>
#include <atomic>

using std::counting_semaphore, std::atomic;

namespace fireflower {
	/// @类名 线程挂起器
	/// @描述 用于挂起线程的工具类
	class FThreadSuspender final {
	private:
		/// @名称 挂起状态
		atomic<size_t> suspended_thread_count = 0;
		
		/// @名称 信号量
		counting_semaphore<> semaphore{0};
	
	public:
		FThreadSuspender() = default;
		~FThreadSuspender() = default;
		FThreadSuspender(const FThreadSuspender&) = delete;
		FThreadSuspender(FThreadSuspender&&) = delete;
		FThreadSuspender& operator=(const FThreadSuspender&) = delete;
		FThreadSuspender& operator=(FThreadSuspender&&) = delete;
		
		/// @名称 挂起
		/// @描述 挂起调用该函数所在的线程
		void suspend();
		
		/// @名称 恢复
		/// @描述 恢复一定数量的正在由该对象挂起的线程\n
		/// 默认恢复一个线程
		void resume(size_t count = 1);
		
		/// @名称 获取已挂起的线程数量
		/// @描述 获取正在由该对象挂起的线程数量
		/// @返回值 已挂起的线程数量
		size_t getSuspendedThreadCount() const;
		
	};
} // fireflower

#endif //FTHREADPOOL_FTHREADSUSPENDER_H
