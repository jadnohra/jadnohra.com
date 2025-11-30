#include "RayTracerConfig.hpp"

#if RayTracerConfig_ParallelProcessor_Simple	

#ifndef _RayTracer_ParallelProcessor_Simple_hpp
#define _RayTracer_ParallelProcessor_Simple_hpp

#include "RayTracerTypes.hpp"
#include "WE3/pool/WEReusableObjectPool.h"
#include "WE3/WERef.h"
#include <map>

namespace rayTracer {

	class ParallelProcessor {
	protected:

		class Thread;

	public:

		class Task : public Ref {
		public:

			virtual void execute() = 0 {};

		protected:
		friend class Thread;

			inline Thread* taskThread() { return mTaskThread; }

			SoftPtr<Thread> mTaskThread;
		};

		enum ThreadPriority {

			ThreadPriority_None = -1, 
			ThreadPriority_Lower, 
			ThreadPriority_Normal, 
			ThreadPriority_Higher, 
		};

	public:

		ParallelProcessor();
		~ParallelProcessor();

		bool create(ThreadPriority threadPriority, int threadCount, bool doStart);
		
		void start();
		bool execute(Task* pTask);
		void end();

		void waitUntilAllTasksExecuting();
		void waitUntilAllTasksExecuted();
		
		void destroy();

	protected:

		class Thread;

		bool reserve(int threadCount);
		void setThreadPriority(ThreadPriority threadPriority);

		void waitUntilAllThreadsExited();

		inline bool isEnding() { return mIsEnding; }
		inline bool isExiting() { return mIsExiting; }

		bool hasExecutingTasks(); 
		bool hasTopQueuedTask(); 
		Task* consumeTopQueuedTask(); //release when done
		void signalExecutedTask(); 

	protected:

		class Thread {
		public:
		
			Thread();
			~Thread();

			bool isCreated() { return mHandle != NULL; }
			bool create(ParallelProcessor& manager);

			inline unsigned int getID() { return mThreadID; }
			inline bool hasTask() { return mHasTask; }

			bool didExit();

			void resume();

		protected:

			int threadProc();
			static DWORD WINAPI threadProcProxy(LPVOID lpParameter);

		protected:

			HANDLE mHandle;
			DWORD mThreadID;
			bool mHasTask;

			SoftPtr<ParallelProcessor> mManager;
		};

		typedef WETL::CountedArray<Task*, false, int> Tasks;
		typedef WETL::CountedPtrArrayEx<Thread, int> ThreadPtrs;

		bool mIsCreated;
		bool mIsEnding;
		bool mIsExiting;

		int mExecutingTaskCount;

		bool mIsCreatedCriticalSection;
		CRITICAL_SECTION mCriticalSection;

		Tasks mQueuedTasks;
		ThreadPtrs mThreads;

		ThreadPriority mThreadPriority;

	};
}

#endif

#endif