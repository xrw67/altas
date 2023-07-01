#ifndef BBT_BASE_FS_H_
#define BBT_BASE_FS_H_

#include <string>

namespace bbt {

std::string PathJoin(const std::string& a, const std::string& b);
std::string PathJoin(const std::string& a, const std::string& b,
                     const std::string& c);
std::string PathJoin(const std::string& a, const std::string& b,
                     const std::string& c, const std::string& d);

}  // namespace bbt
#endif  // BBT_BASE_FS_H_
