#pragma once

// stl
#include <stdint.h>
#include <cstddef>
#include <cstdio>  //std::vsnprintf
#include <functional>
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

// todo  补充mt/st async/sync的说明 以及相应的benchmark到头文件中

//
#include "zlogger-defs.h"
//
#include "zlogger-interface.h"
//
#include "zlogger-factory.h"

/**
 * @brief    提供一些宏来做编译时日志埋点裁包
 *
 */
#include "zlogger-macro.h"
