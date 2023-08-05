#ifndef BBT_BASE_FS_H_
#define BBT_BASE_FS_H_

#include <string>
#include <vector>

#include "bbt/base/status.h"
#include "bbt/base/string_view.h"

namespace bbt {

std::string PathJoin(string_view a, string_view b);
std::string PathJoin(string_view a, string_view b, string_view c);
std::string PathJoin(string_view a, string_view b, string_view c,
                     string_view d);
std::string PathJoin(string_view a, string_view b, string_view c, string_view d,
                     string_view e);

//  返回临时目录下的文件名
std::string GetTempPath(const std::string& name);

/**
 * @brief 返回链接文件指向的真实文件路径
 *
 * @param linkpath 链接文件路径
 * @return std::string 真实路径
 */
std::string Readlink(const std::string& linkpath);

Status WriteFile(const std::string& filename, const std::string& content);

string_view Dir(string_view path);
string_view Basename(string_view path);

bool IsFileExist(const std::string& filename);
bool IsDir(const std::string& path);

size_t FileSize(const std::string &path);

/**
 * @brief 读文件内容
 *
 * @param filepath 文件路径
 * @return std::string 文件内容
 */
std::string ReadFile(string_view filename);

bool AppendFile(const std::string& filename, const std::string& data);

std::vector<std::string> GetDirectoryChildren(const std::string& path);

}  // namespace bbt
#endif  // BBT_BASE_FS_H_
