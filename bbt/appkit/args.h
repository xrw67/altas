#ifndef BBT_APPKIT_ARGS_H_
#define BBT_APPKIT_ARGS_H_

#include "bbt/base/status.h"

namespace bbt {

class Args {
 public:
  Args();
  ~Args();

  void AddBool(char short_name, const char* long_name, const char* help);
  void AddString(char short_name, const char* long_name,
                 const char* default_value, const char* help);
  void AddLong(char short_name, const char* long_name, long long default_value,
               const char* help);

  bool GetBool(const char* long_name);
  std::string GetString(const char* long_name);
  long long GetLong(const char* long_name);

  std::string Help();

  Status Parse(const char* arg_line);
  Status Parse(int argc, const char* const argv[]);

 private:
  struct Impl;
  Impl* impl_;
};

}  // namespace bbt

#endif  // BBT_APPKIT_ARGS_H_