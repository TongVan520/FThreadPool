//
// Created by FireFlower on 2024/12/11.
//

#ifndef FTHREADPOOL_FTHREADPOOL_H
#define FTHREADPOOL_FTHREADPOOL_H

#include <shared_mutex>
#include <mutex>
#include <list>
#include <thread>
#include <vector>
#include <semaphore>

#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/ref_counted.hpp>

#include "FThreadTask.h"
#include <FSignal/FSignal.h>

using std::shared_mutex, std::list, std::thread, std::vector, std::mutex, std::binary_semaphore;
using godot::Ref, godot::RefCounted;

namespace fireflower {
	/// @类名 线程池
	/// @描述 可以自动将任务分配到各线程\n
	/// 没有任务时自动挂起线程
	class FThreadPool : public RefCounted {
	GDCLASS(FThreadPool, RefCounted);
		
		/// @名称 任务线程主函数
		/// @描述 仅用于线程回调
		friend void mainForThread(Ref<FThreadPool> ownerThreadPool);
	
	public:
		/// @类名 线程池状态枚举
		enum class FThreadPoolStateEnum {
			/// @名称 已停止
			Stopped = 0,
			
			/// @名称 已启动
			Started,
			
			/// @名称 已暂停
			Paused,
			
			/// @名称 等待
			/// @描述 正在等待线程退出
			Waiting,
		};
	
	private:
		/// @名称 任务列表
		list<Ref<FThreadTask>> taskList;
		
		/// @名称 任务队列的读写锁
		/// @描述 确保任务队列线程安全
		mutable shared_mutex task_listShrdmtx;
		
		/// @名称 线程集合
		vector <thread> threadVec;
		
		/// @名称 线程锁
		/// @描述 用于挂起线程的锁
		mutable mutex threadMtx;
		
		/// @名称 暂停条件变量
		/// @描述 用于挂起线程的条件变量
		condition_variable pauseCdtvar;
		
		// condition_variable
		
		/// @名称 状态
		/// @描述 <b>注意：确保该成员在其他线程中只读</b>
		FThreadPoolStateEnum stateEnum = FThreadPoolStateEnum::Stopped;
		
		/// @名称 最大线程数量
		/// @描述 自动分配的线程数量不会超过这个值\n
		/// 注意：修改该值只会从<b>下一次</b>分配线程时开始生效
		size_t max_thread_count = thread::hardware_concurrency() - 1;
	
	public:
		/// @名称 最大线程数量改变信号
		/// @描述 在最大线程数量改变时发出
		Ref<FSignal1> maxThreadCountChanged = memnew(FSignal1);
	
	private:
		/// @名称 查找任务
		/// @描述 从任务列表中查找任务
		/// @参数名称 <code>task</code>
		/// @参数描述 任务
		/// @返回值 该任务在任务列表中的迭代器
		list<Ref<FThreadTask>>::const_iterator findTask(Ref<FThreadTask> task) const;
		
		/// @名称 分配线程
		/// @描述 调整线程数量
		void adjustThread();
	
	public:
		/// @名称 添加任务
		/// @描述 <b>线程安全</b>，<b>原子操作</b>
		/// @参数名称 <code>new_task</code>
		/// @参数描述 新任务
		/// @返回值 任务完成时发出的信号
		Ref<FSignal0> addTask(Ref<FThreadTask> new_task);
		
		/// @名称 移除任务
		/// @描述 <b>线程安全</b>，<b>原子操作</b>
		/// @参数名称 <code>task</code>
		/// @参数描述 任务
		void removeTask(Ref<FThreadTask> task);
		
		/// @名称 查找任务
		/// @描述 <b>线程安全</b>，<b>原子操作</b>\n
		/// <b>注意：仅能查找尚未被执行的任务</b>
		/// @参数名称 <code>task</code>
		/// @参数描述 任务
		/// @返回值 是否有任务
		bool hasTask(Ref<FThreadTask> task) const;
		
		/// @名称 启动线程池
		/// @描述 <b>线程安全</b>，<b>原子操作</b>
		void start();
		
		/// @名称 停止线程池
		/// @描述 <b>线程安全</b>，<b>阻塞函数</b>\n
		/// 阻塞调用线程并等待其它线程退出
		void stop();
		
		/// @名称 暂停线程池
		/// @描述 <b>线程安全</b>，<b>原子操作</b>
		void pause();
		
		/// @名称 恢复线程池
		/// @描述 <b>线程安全</b>，<b>原子操作</b>
		void resume();
		
		/// @名称 获取任务数量
		/// @描述 <b>线程安全</b>，<b>原子操作</b>
		/// @返回值 当前剩余未被执行的任务数量
		size_t getTaskCount() const;
		
		/// @名称 获取线程数量
		/// @描述 <b>线程安全</b>，<b>原子操作</b>
		/// @返回值 当前线程数量
		size_t getThreadCount() const;
		
		void setMaxThreadCount(size_t new_count);
		size_t getMaxThreadCount() const;
		Ref<FSignal1> getMaxThreadCountChangedSignal() const;
	};
} // fireflower

#endif //FTHREADPOOL_FTHREADPOOL_H
