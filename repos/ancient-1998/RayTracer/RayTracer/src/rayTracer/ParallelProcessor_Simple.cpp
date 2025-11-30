#include "ParallelProcessor_Simple.hpp"

#if RayTracerConfig_ParallelProcessor_Simple	

namespace rayTracer {

int toNativeThreadPriotity(ParallelProcessor::ThreadPriority prio) {

	switch (prio) {

		case ParallelProcessor::ThreadPriority_Lower: return THREAD_PRIORITY_BELOW_NORMAL;
		case ParallelProcessor::ThreadPriority_Higher: return THREAD_PRIORITY_ABOVE_NORMAL;
	}

	return THREAD_PRIORITY_NORMAL;
}

ParallelProcessor::ParallelProcessor() 
	: mThreadPriority(ThreadPriority_Normal), mIsCreated(false), mIsCreatedCriticalSection(false), mIsEnding(false), mIsExiting(false), mExecutingTaskCount(0) {
}

ParallelProcessor::~ParallelProcessor() {

	if (mIsCreated) {

		destroy();

		assert(!hasTopQueuedTask());
	}

	if (mIsCreatedCriticalSection)
		DeleteCriticalSection(&mCriticalSection);
}

bool ParallelProcessor::create(ThreadPriority threadPriority, int threadCount, bool doStart) {

	if (!mIsCreated) {

		if (InitializeCriticalSectionAndSpinCount(&mCriticalSection, 4000)) {

			mIsCreatedCriticalSection = true;

			setThreadPriority(threadPriority);
			
			if (reserve(threadCount)) {

				mIsCreated = true;

				if (doStart)
					start();

				return true;
			}
		}

		return false;
	}

	return false;
}

void ParallelProcessor::setThreadPriority(ThreadPriority threadPriority) {

	mThreadPriority = threadPriority;
}

bool ParallelProcessor::reserve(int threadCount) {

	for (int i = 0; i < threadCount; ++i) {

		SoftPtr<Thread> thread = mThreads.addNewOne();

		if (!thread->isCreated()) {

			if (!thread->create(dref(this))) {

				assert(false);
				return false;

			} 
		}
	}

	return true;
}

void ParallelProcessor::start() {

	mIsEnding = false;

	for (int i = 0; i < mThreads.count; ++i) {

		mThreads[i]->resume();
	}
}

void ParallelProcessor::end() {

	mIsEnding = true;
	waitUntilAllTasksExecuted();
}

void ParallelProcessor::destroy() {

	start();

	mIsExiting = true;
	waitUntilAllThreadsExited();
}

bool ParallelProcessor::execute(Task* pTask) {

	if (!mIsCreated)
		return false;

	pTask->AddRef();

	EnterCriticalSection(&mCriticalSection);
	{
		mQueuedTasks.addOne(pTask);
	}
	LeaveCriticalSection(&mCriticalSection);

	return false;
}

bool ParallelProcessor::hasExecutingTasks() {

	bool ret;

	EnterCriticalSection(&mCriticalSection);
	{
		ret = mExecutingTaskCount > 0;
	}
	LeaveCriticalSection(&mCriticalSection);

	return ret;
}

void ParallelProcessor::signalExecutedTask() {

	EnterCriticalSection(&mCriticalSection);
	{
		--mExecutingTaskCount;
	}
	LeaveCriticalSection(&mCriticalSection);
}

bool ParallelProcessor::hasTopQueuedTask() {

	bool ret;

	EnterCriticalSection(&mCriticalSection);
	{
		ret = mQueuedTasks.count > 0;
	}
	LeaveCriticalSection(&mCriticalSection);

	return ret;
}

ParallelProcessor::Task* ParallelProcessor::consumeTopQueuedTask() {

	SoftPtr<ParallelProcessor::Task> ret;

	EnterCriticalSection(&mCriticalSection);
	{
		if (mQueuedTasks.count) {

			ret = mQueuedTasks[mQueuedTasks.count - 1];
			--mQueuedTasks.count;
			++mExecutingTaskCount;
		} 
	}
	LeaveCriticalSection(&mCriticalSection);

	return ret;
}

void ParallelProcessor::waitUntilAllTasksExecuted() {

	waitUntilAllTasksExecuting();

	while (hasExecutingTasks()) {

		SwitchToThread();
	}
}

void ParallelProcessor::waitUntilAllTasksExecuting() {

	while (hasTopQueuedTask()) {

		SwitchToThread();
	}
}

void ParallelProcessor::waitUntilAllThreadsExited() {

	for (int i = 0; i < mThreads.count; ++i) {

		while (!mThreads[i]->didExit()) {

			SwitchToThread();
		}
	}
}

ParallelProcessor::Thread::Thread() 
	: mHandle(NULL), mThreadID(-1), mHasTask(false) {
}

ParallelProcessor::Thread::~Thread() {

	if (isCreated()) {

		assert(!hasTask());

		CloseHandle(mHandle);
		mHandle = NULL;
	}
}

bool ParallelProcessor::Thread::create(ParallelProcessor& manager) {

	if (mHandle) {

		return false;
	}

	mManager = &manager;
	mHandle = CreateThread(NULL, 0, threadProcProxy, this, CREATE_SUSPENDED, &mThreadID);
	SetThreadPriority(mHandle, toNativeThreadPriotity(ThreadPriority_Lower));

	return (mHandle != NULL);
}

bool ParallelProcessor::Thread::didExit() {

	DWORD exitCode;

	if (GetExitCodeThread(mHandle, &exitCode)) {

		return exitCode != STILL_ACTIVE;
	}

	return true;
}

void ParallelProcessor::Thread::resume() {

	while(ResumeThread(mHandle) > 1) {
	}
}

int ParallelProcessor::Thread::threadProc() {

	while (true) {

		SoftRef<Task> task = mManager->consumeTopQueuedTask();

		if (task.isValid()) {

			SetThreadPriority(mHandle, toNativeThreadPriotity(mManager->mThreadPriority));

			mHasTask = true;
			task->mTaskThread = this;
			task->execute();
			task->mTaskThread = NULL;
			task.destroy();
			mManager->signalExecutedTask();
			mHasTask = false;

			SetThreadPriority(mHandle, toNativeThreadPriotity(ThreadPriority_Normal));

		} else {

			SetThreadPriority(mHandle, toNativeThreadPriotity(ThreadPriority_Lower));
		}

		if (mManager->isExiting()) {

			return !(STILL_ACTIVE);
		}

		if (mManager->isEnding()) {

			SuspendThread(mHandle);
		}

		SwitchToThread();
	}
}

DWORD WINAPI ParallelProcessor::Thread::threadProcProxy(LPVOID lpParameter) {

	ParallelProcessor::Thread& self = dref((ParallelProcessor::Thread*) lpParameter);

	return self.threadProc();	
}


}

#endif