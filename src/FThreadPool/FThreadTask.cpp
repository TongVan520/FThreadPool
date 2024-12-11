//
// Created by FireFlower on 2024/12/11.
//

#include "FThreadTask.h"

using namespace godot;

namespace fireflower {
	Ref<FThreadTask> FThreadTask::makeFromSlot(Ref<FSlot0> init_callbackSlot0) {
		auto resultTask = Ref(memnew(FThreadTask));
		
		resultTask->callbackSlot0 = init_callbackSlot0;
		
		return resultTask;
	}
	
	void FThreadTask::execute() {
		this->started->emit();
		this->callbackSlot0->invoke();
		this->completed->emit();
	}
	
	Ref<FSignal0> FThreadTask::getStartedSignal() const {
		return this->started;
	}
	
	Ref<FSignal0> FThreadTask::getCompletedSignal() const {
		return this->completed;
	}
	
	void FThreadTask::_bind_methods() {
		// started
		{
			ClassDB::bind_method(D_METHOD("getStartedSignal"), &FThreadTask::getStartedSignal);
			ClassDB::add_property(
				FThreadTask::get_class_static(),
				PropertyInfo(
					Variant::OBJECT, "started",
					PROPERTY_HINT_NONE, "",
					PROPERTY_USAGE_SCRIPT_VARIABLE
				),
				"", "getStartedSignal"
			);
		}
		
		// completed
		{
			ClassDB::bind_method(D_METHOD("getCompletedSignal"), &FThreadTask::getCompletedSignal);
			ClassDB::add_property(
				FThreadTask::get_class_static(),
				PropertyInfo(
					Variant::OBJECT, "completed",
					PROPERTY_HINT_NONE, "",
					PROPERTY_USAGE_SCRIPT_VARIABLE
				),
				"", "getCompletedSignal"
			);
		}
		
		// makeFromSlot
		ClassDB::bind_static_method(
			FThreadTask::get_class_static(),
			D_METHOD("makeFromSlot", "init_callbackSlot0"),
			&FThreadTask::makeFromSlot
		);
		
		// execute
		ClassDB::bind_method(D_METHOD("execute"), &FThreadTask::execute);
	}
} // fireflower