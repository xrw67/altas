#ifndef BBT_SYS_UTIL_H_
#define BBT_SYS_UTIL_H_

#include <string>

#include "bbt/base/config.h"

namespace bbt {
///
/// @brief 获取当前线程ID
///
/// @return uint32_t 线程ID
///
BBT_EXPORT uint32_t GetTid();

///
/// @brief 获取当前进程ID
///
/// @return uint32_t 进程ID
///
BBT_EXPORT uint32_t GetPid();

///
/// @brief 获取当前进程路径
///
/// @return std::string
///
BBT_EXPORT std::string GetAppPath();

///
/// @brief 获得当前工作目录
///
/// @return std::string 工作目录路径
///
BBT_EXPORT std::string GetCurrentDir();

///
/// @brief 休眠当前线程(单位：秒)
///
/// @param msec
///
BBT_EXPORT void SleepMsec(uint32_t msec);

}  // namespace bbt

#endif  // BBT_SYS_UTIL_H_
