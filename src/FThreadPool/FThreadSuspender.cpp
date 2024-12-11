//
// Created by FireFlower on 2024/12/11.
//

#include "FThreadSuspender.h"

using namespace std;

namespace fireflower {
	void FThreadSuspender::suspend() {
		this->suspended_thread_count++;
		this->semaphore.acquire();
	}
	
	void FThreadSuspender::resume(size_t count) {
		count = min(count, this->suspended_thread_count.load());
		this->suspended_thread_count -= count;
		this->semaphore.release(static_cast<int64_t>(count));
	}
	
	size_t FThreadSuspender::getSuspendedThreadCount() const {
		return this->suspended_thread_count;
	}
} // fireflower