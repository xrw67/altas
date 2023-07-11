/**
 * OSAL
 */
#ifndef BBT_BASE_SYS_H_
#define BBT_BASE_SYS_H_

#include <string>

#include "bbt/base/config.h"

namespace bbt {
///
/// @brief 获取当前线程ID
///
/// @return uint32_t 线程ID
///
BBT_EXPORT uint64_t GetTid();

///
/// @brief 获取当前进程ID
///
/// @return uint32_t 进程ID
///
BBT_EXPORT uint64_t GetPid();

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

}  // namespace bbt

#endif  // BBT_BASE_SYS_H_
