#ifndef BBT_APPKIT_SERVICE_H_
#define BBT_APPKIT_SERVICE_H_

#include <string>
#include <vector>

#include "bbt/base/config.h"
#include "bbt/base/status.h"
#include "bbt/base/shared_object.h"

namespace bbt {

/**
 * 所有Service的基类
 */
class Service : public SharedObject {
 public:
  /// @brief 服务名
  virtual const char* service_name() const = 0;

 protected:
  virtual ~Service() {}
};

class ServiceManager {
 public:
  static ServiceManager* New();
  static void Release(ServiceManager* mgr);

  virtual Status Register(Service* svc) = 0;
  virtual Status Unregister(Service* svc) = 0;
  virtual Status Get(const char* name, Service** psvc) = 0;
  virtual void Put(Service* svc) = 0;
  virtual std::vector<std::string> List() = 0;

 protected:
  virtual ~ServiceManager() {}
};

}  // namespace bbt

#endif  // BBT_APPKIT_SERVICE_H_
