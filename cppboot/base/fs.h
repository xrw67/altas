#ifndef CPPBOOT_BASE_FS_H_
#define CPPBOOT_BASE_FS_H_

#include <string>
#include <vector>

#include "cppboot/base/status.h"
#include "cppboot/base/string_view.h"

namespace cppboot {

std::string PathJoin(string_view a, string_view b);
std::string PathJoin(string_view a, string_view b, string_view c);
std::string PathJoin(string_view a, string_view b, string_view c,
                     string_view d);
std::string PathJoin(string_view a, string_view b, string_view c, string_view d,
                     string_view e);

std::vector<std::string> PathSplit(string_view path);

//  返回临时目录下的文件名

/**
 * @brief 返回临时目录下的文件名
 *
 * @param name 新的文件名
 * @return std::string 完整路径
 *
 * @example "/tmp/1.txt"
 */
std::string GetTempPath(const std::string& name);

std::string RealPath(string_view path);

/**
 * @brief 返回链接文件指向的真实文件路径
 *
 * @param linkpath 链接文件路径
 * @return std::string 真实路径
 */
std::string Readlink(const std::string& linkpath);

string_view Dir(string_view path);
string_view Basename(string_view path);

bool IsFileExist(const std::string& filename);
bool IsDir(const std::string& path);

size_t FileSize(const std::string& path);

/**
 * @brief 读文件内容
 *
 * @param filepath 文件路径
 * @return std::string 文件内容
 */
std::string ReadFile(string_view filename);

Status WriteFile(const std::string& filename, const std::string& content);
bool AppendFile(const std::string& filename, const std::string& data);

std::vector<std::string> GetDirectoryChildren(const std::string& path);

Status Remove(const std::string& path);
Status RemoveAll(const std::string& path);

Status Mkdir(const std::string& path, mode_t mode = 0755);
Status MkdirAll(const std::string& path, mode_t mode = 0755);

Status Hardlink(const std::string& src, const std::string& dst);
Status Symlink(const std::string& src, const std::string& dst);

Status CopyFile(const std::string& src, const std::string& dst);

}  // namespace cppboot
#endif  // CPPBOOT_BASE_FS_H_
