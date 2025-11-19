//
//  ZegoQueueRunner.hpp
//  ZegoConnection
//
//  Created by kebo on 2019/11/19.
//

#ifndef zego_queue_runner_hpp
#define zego_queue_runner_hpp

#include <functional>
#include <memory>
#include <mutex>

#include <zegoasyncall.h>
#include <zegolock.h>
#include <zegostd.h>
#include <zegotask.h>

#undef __MODULE__
#define __MODULE__ "QueueRunner"

#if __cplusplus >= 201402L
#define ZEGO_MOVE(value_to_be_move)                                                                \
    value_to_be_move { std::move(value_to_be_move) }
#else
#define ZEGO_MOVE(value_to_be_move) value_to_be_move
#endif

namespace ZEGO {
namespace BASE {

using ZegoRunnable = std::function<void()>;

class CZegoQueueRunner {
  public:
    task_id AsyncRun(ZegoRunnable &&job, CZEGOTaskDefault *pTask,
                     task_priority priority = task_priority::task_priority_normal);

    task_id DelayRun(ZegoRunnable &&job, CZEGOTaskDefault *pTask, int64 delayInMiliseconds);

    bool SyncRun(ZegoRunnable &&job, CZEGOTaskDefault *pTask, int64 waitTimeInMiliseconds = -1);

    /**
         execute job immediately if call in pTask thread
         
         @param job job to execute
         @param pTask thread on which job execute
         @return > 0 task id; 0 job done;
         */
    task_id Dispatch(ZegoRunnable &&job, CZEGOTaskDefault *pTask);

  private:
    struct zego_functor_task : public zego_task_call_base {
        std::shared_ptr<void> _event;
        ZegoRunnable _job;
    };

    task_id add_job(ZegoRunnable &&job, CZEGOTaskDefault *pTask, int64 delayInMiliseconds,
                    std::shared_ptr<void> event,
                    task_priority priority = task_priority::task_priority_normal);

  private:
    using T_ = CZegoQueueRunner;
    CScopeSource<T_, false, CRefCountSafe> init_normal_source;
    friend class CScopeSource<T_, false, CRefCountSafe>;
    static inline int32 offset_of_source(void) {
        return ((size_t) & (((T_ *)1)->init_normal_source) - 1);
    }

  public:
    CRefCount *SafeInstance(void);
    int32 invoke(CCallTaskArg *arg);
};

} // namespace BASE
} // namespace ZEGO

#endif /* zego_queue_runner_hpp */
