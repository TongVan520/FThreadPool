//
// Created by FireFlower on 2024/12/11.
//

#include "FThreadPool.h"

#include <memory>

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;
using std::unique_lock, std::shared_lock, std::hash;

namespace fireflower {
	void mainForThread(Ref<FThreadPool> ownerThreadPool) {
		auto thread_id = hash<thread::id>{}(std::this_thread::get_id());
		if (ownerThreadPool.is_null()) {
			UtilityFunctions::push_warning(L"线程", thread_id, L"所有者无效...");
		}
		
		while (ownerThreadPool->stateEnum != FThreadPool::FThreadPoolStateEnum::Waiting) {
			// 处理暂停
			if (ownerThreadPool->stateEnum == FThreadPool::FThreadPoolStateEnum::Paused) {
				ownerThreadPool->pauseThrdsus.suspend();
				continue;
			}
			
			// 处理任务
			Ref<FThreadTask> task;
			// 取任务
			if (
				auto write_lock = unique_lock(ownerThreadPool->task_listShrdmtx);
				not ownerThreadPool->taskList.empty()
				) {
				task = ownerThreadPool->taskList.front();
				ownerThreadPool->taskList.pop_front();
			}
			else if (ownerThreadPool->stateEnum != FThreadPool::FThreadPoolStateEnum::Waiting) {
				// 挂起线程前释放锁
				write_lock.unlock();
				ownerThreadPool->taskThrdsus.suspend();
				continue;
			}
			// 执行任务
			if (task.is_valid()) {
				task->execute();
			}
		}
		
		UtilityFunctions::print(L"线程", thread_id, L"退出");
	}
	
	list<Ref<FThreadTask>>::const_iterator FThreadPool::findTask(Ref<FThreadTask> task) const {
		// Ref 已实现 operator==
		return std::find(this->taskList.begin(), this->taskList.end(), task);
	}
	
	void FThreadPool::adjustThread() {
		while (this->threadVec.size() < this->max_thread_count) {
			this->threadVec.push_back(thread(mainForThread, Ref(this)));
		}
		
		while (this->threadVec.size() > this->max_thread_count) {
			auto thrd = std::move(this->threadVec.back());
			if (thrd.joinable()) {
				thrd.join();
			}
			this->threadVec.pop_back();
		}
	}
	
	Ref<FSignal0> FThreadPool::addTask(Ref<FThreadTask> new_task) {
		if (new_task.is_null()) {
			return {};
		}
		
		auto write_lock = unique_lock(this->task_listShrdmtx);
		
		this->taskList.push_back(new_task);
		this->taskThrdsus.resume();
		
		return new_task->completed;
	}
	
	void FThreadPool::removeTask(Ref<FThreadTask> task) {
		if (task.is_null()) {
			return;
		}
		
		auto write_lock = unique_lock(this->task_listShrdmtx);
		
		auto iter = this->findTask(task);
		this->taskList.erase(iter);
	}
	
	bool FThreadPool::hasTask(Ref<FThreadTask> task) const {
		auto read_lock = shared_lock(this->task_listShrdmtx);
		return this->findTask(task) != this->taskList.end();
	}
	
	void FThreadPool::start() {
		if (this->stateEnum != FThreadPoolStateEnum::Stopped) {
			return;
		}
		
		this->stateEnum = FThreadPoolStateEnum::Started;
		this->adjustThread();
	}
	
	void FThreadPool::stop() {
		if (
			this->stateEnum != FThreadPoolStateEnum::Started and
			this->stateEnum != FThreadPoolStateEnum::Paused
			) {
			return;
		}
		
		this->stateEnum = FThreadPoolStateEnum::Waiting;
		this->pauseThrdsus.resume(this->pauseThrdsus.getSuspendedThreadCount());
		this->taskThrdsus.resume(this->taskThrdsus.getSuspendedThreadCount());
		// 阻塞等待所有线程退出
		for (auto& thrd: this->threadVec) {
			if (thrd.joinable()) {
				thrd.join();
			}
		}
		this->threadVec.clear();
		
		this->stateEnum = FThreadPoolStateEnum::Stopped;
	}
	
	void FThreadPool::pause() {
		if (this->stateEnum != FThreadPoolStateEnum::Started) {
			return;
		}
		
		this->stateEnum = FThreadPoolStateEnum::Paused;
	}
	
	void FThreadPool::resume() {
		if (this->stateEnum != FThreadPoolStateEnum::Paused) {
			return;
		}
		
		this->stateEnum = FThreadPoolStateEnum::Started;
		this->pauseThrdsus.resume(this->pauseThrdsus.getSuspendedThreadCount());
	}
	
	size_t FThreadPool::getTaskCount() const {
		auto read_lock = shared_lock(this->task_listShrdmtx);
		return this->taskList.size();
	}
	
	size_t FThreadPool::getThreadCount() const {
		return this->threadVec.size();
	}
	
	void FThreadPool::setMaxThreadCount(size_t new_count) {
		this->max_thread_count = new_count;
	}
	
	size_t FThreadPool::getMaxThreadCount() const {
		return this->max_thread_count;
	}
	
	Ref<FSignal1> FThreadPool::getMaxThreadCountChangedSignal() const {
		return this->maxThreadCountChanged;
	}
	
	void FThreadPool::_bind_methods() {
		// max_thread_count
		{
			// 属性
			ClassDB::bind_method(D_METHOD("setMaxThreadCount", "new_count"), &FThreadPool::setMaxThreadCount);
			ClassDB::bind_method(D_METHOD("getMaxThreadCount"), &FThreadPool::getMaxThreadCount);
			ClassDB::add_property(
				FThreadPool::get_class_static(),
				PropertyInfo(Variant::INT, "max_thread_count"),
				"setMaxThreadCount", "getMaxThreadCount"
			);
			
			// 信号
			ClassDB::bind_method(
				D_METHOD("getMaxThreadCountChangedSignal"),
				&FThreadPool::getMaxThreadCountChangedSignal
			);
			ClassDB::add_property(
				FThreadPool::get_class_static(),
				PropertyInfo(
					Variant::OBJECT, "maxThreadCountChanged",
					PROPERTY_HINT_NONE, "",
					PROPERTY_USAGE_SCRIPT_VARIABLE
				),
				"", "getMaxThreadCountChangedSignal"
			);
		}
		
		// addTask
		ClassDB::bind_method(D_METHOD("addTask", "new_task"), &FThreadPool::addTask);
		
		// removeTask
		ClassDB::bind_method(D_METHOD("removeTask", "task"), &FThreadPool::removeTask);
		
		// hasTask
		ClassDB::bind_method(D_METHOD("hasTask", "task"), &FThreadPool::hasTask);
		
		// start
		ClassDB::bind_method(D_METHOD("start"), &FThreadPool::start);
		
		// stop
		ClassDB::bind_method(D_METHOD("stop"), &FThreadPool::stop);
		
		// pause
		ClassDB::bind_method(D_METHOD("pause"), &FThreadPool::pause);
		
		// resume
		ClassDB::bind_method(D_METHOD("resume"), &FThreadPool::resume);
		
		// getTaskCount
		ClassDB::bind_method(D_METHOD("getTaskCount"), &FThreadPool::getTaskCount);
		
		// getThreadCount
		ClassDB::bind_method(D_METHOD("getThreadCount"), &FThreadPool::getThreadCount);
	}
} // fireflower