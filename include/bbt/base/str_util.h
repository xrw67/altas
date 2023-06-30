#ifndef BBT_BASE_STR_UTIL_H_
#define BBT_BASE_STR_UTIL_H_

#include <vector>
#include <string>

namespace bbt {

std::vector<std::string> StrSplit(const std::string& text, char delimiter);

/// @brief 移除左侧的空格、换行符和制表符
std::string StrTrimLeft(const std::string& s, const std::string& cutset);

/// @brief 移除右侧的空格、换行符和制表符
std::string StrTrimRight(const std::string& s, const std::string& cutset);

/// @brief 移除左右两侧的空格、换行符和制表符
std::string StrTrim(const std::string& s, const std::string& cutset);

}  // namespace bbt

#endif  // BBT_BASE_STR_UTIL_H_
