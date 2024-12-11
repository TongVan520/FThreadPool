//
// Created by FireFlower on 2024/12/11.
//

#ifndef FTHREADPOOL_FTHREADTASK_H
#define FTHREADPOOL_FTHREADTASK_H


#include <godot_cpp/classes/ref_counted.hpp>

#include <FSignal/FSignal.h>

using godot::RefCounted;

namespace fireflower {
	/// @类名 线程任务
	class GDE_EXPORT FThreadTask : public RefCounted {
	GDCLASS(FThreadTask, RefCounted);
	
	private:
		/// @名称 回调函数
		/// @描述 以槽为实现
		Ref<FSlot0> callbackSlot0;
	
	public:
		/// @名称 任务开始信号
		/// @描述 任务开始执行时发出
		Ref<FSignal0> started = memnew(FSignal0);
		
		/// @名称 任务完成信号
		/// @描述 任务执行完成时发出
		Ref<FSignal0> completed = memnew(FSignal0);
	
	public:
		/// @名称 从槽函数构造
		/// @参数名称 <code>init_callbackSlot0</code>
		/// @参数描述 初始化回调函数
		/// @返回值 该类的新实例
		static Ref<FThreadTask> makeFromSlot(Ref<FSlot0> init_callbackSlot0);
		
		/// @名称 执行任务
		void execute();
	};
} // fireflower

#endif //FTHREADPOOL_FTHREADTASK_H
