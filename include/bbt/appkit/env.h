#ifndef BBT_APPKIT_ENV_H_
#define BBT_APPKIT_ENV_H_

#include <string>

#include "bbt/base/config.h"
#include "bbt/base/status.h"

namespace bbt {

class Object;

class GlobalEnv {
 public:
  static GlobalEnv* New();
  static void Release(GlobalEnv* env);

  virtual Status InsertObject(Object* obj) = 0;
  virtual Status RemoveObject(Object* obj) = 0;
  virtual Status GetObject(const char* name, Object** pobj) = 0;

  virtual void InsertString(const char* key, const char* value) = 0;
  virtual void RemoveString(const char* key) = 0;
  virtual std::string GetString(const char* key) = 0;

 protected:
  virtual ~GlobalEnv() {}
};

}  // namespace bbt

#endif  // BBT_APPKIT_ENV_H_
