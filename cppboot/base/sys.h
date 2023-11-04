/**
 * OSAL
 */
#ifndef CPPBOOT_BASE_SYS_H_
#define CPPBOOT_BASE_SYS_H_

#include <string>

#include "cppboot/base/config.h"

namespace cppboot {
///
/// @brief 获取当前线程ID
///
/// @return uint32_t 线程ID
///
CPPBOOT_EXPORT uint64_t GetTid();

///
/// @brief 获取当前进程ID
///
/// @return uint32_t 进程ID
///
CPPBOOT_EXPORT uint64_t GetPid();

///
/// @brief 获取当前进程路径
///
/// @return std::string
///
CPPBOOT_EXPORT std::string GetAppPath();

///
/// @brief 获得当前工作目录
///
/// @return std::string 工作目录路径
///
CPPBOOT_EXPORT std::string GetCurrentDir();


CPPBOOT_EXPORT int64_t GetNanoClock();

}  // namespace cppboot

#endif  // CPPBOOT_BASE_SYS_H_
