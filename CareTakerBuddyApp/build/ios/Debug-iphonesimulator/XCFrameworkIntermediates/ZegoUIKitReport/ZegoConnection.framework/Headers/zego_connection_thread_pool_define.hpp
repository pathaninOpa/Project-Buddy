#ifndef zego_connection_thread_pool_define_hpp
#define zego_connection_thread_pool_define_hpp

#include "zego_connection_define.hpp"

namespace ZEGO
{

namespace CONNECTION
{
    enum THREA_POOL_TASK_BASE_TYPE
    {
        THREA_POOL_TASK_BASE_NONE=0,
        THREA_POOL_TASK_BASE_NORMAL =1,  
        THREA_POOL_TASK_BASE_IO = 2, 
        THREA_POOL_TASK_BASE_ZEGONS = 3, // 用于 zegons 的 io 线程
    };

    enum JOB_RUNNABLE_TYPE
    {
        JOB_MAYBE_BLOCK = 0, 
        JOB_NO_BLOCK = 1, 
    };
    
}// CONNECTION
}// ZEGO


#endif /* zego_connection_thread_pool_define_hpp */