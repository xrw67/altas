#ifndef BBT_BASE_FS_H_
#define BBT_BASE_FS_H_

#include <string>

#include "bbt/base/status.h"

namespace bbt {

std::string PathJoin(const std::string& a, const std::string& b);
std::string PathJoin(const std::string& a, const std::string& b,
                     const std::string& c);
std::string PathJoin(const std::string& a, const std::string& b,
                     const std::string& c, const std::string& d);

//  返回临时目录下的文件名
std::string GetTempPath(const std::string& name);

/**
 * @brief 返回链接文件指向的真实文件路径
 *
 * @param linkpath 链接文件路径
 * @return std::string 真实路径
 */
std::string Readlink(const std::string& linkpath);


Status WriteFile(const std::string &filename, const std::string &content);

}  // namespace bbt
#endif  // BBT_BASE_FS_H_
