#ifndef BBT_BASE_FS_H_
#define BBT_BASE_FS_H_

#include <string>

namespace bbt {

std::string PathJoin(const std::string& a, const std::string& b);
std::string PathJoin(const std::string& a, const std::string& b,
                     const std::string& c);
std::string PathJoin(const std::string& a, const std::string& b,
                     const std::string& c, const std::string& d);

/**
 * @brief 返回链接文件指向的真实文件路径
 *
 * @param linkpath 链接文件路径
 * @return std::string 真实路径
 */
std::string Readlink(const std::string& linkpath);

}  // namespace bbt
#endif  // BBT_BASE_FS_H_
