#include "bbt/appkit/service.h"

#include <map>
#include <mutex>

namespace bbt {

class ServiceManagerImpl : public ServiceManager {
 public:
  ServiceManagerImpl() {}
  ~ServiceManagerImpl() {}

 private:
  std::mutex mutex_;
  std::map<std::string, Service*> services_;

  Status Register(Service* svc) {
    if (!svc) return InvalidArgumentError("no service");
    const char* name = svc->service_name();
    if (!name || !*name) return InvalidArgumentError("no name");

    std::lock_guard<std::mutex> lock(mutex_);

    if (services_.find(name) != services_.end())
      return AlreadyExistsError(name);

    svc->AddRefManually();
    services_[name] = svc;
    return OkStatus();
  }

  Status Unregister(Service* svc) {
    if (!svc) return InvalidArgumentError("no service");
    const char* name = svc->service_name();
    if (!name || !*name) return InvalidArgumentError("no name");

    std::lock_guard<std::mutex> lock(mutex_);

    auto it = services_.find(name);
    if (it == services_.end()) {
      return NotFoundError(name);
    }
    if (it->second != svc) {
      return UnknownError("same name, but service object different");
    }

    services_.erase(it);
    svc->RemoveRefManually();
    return OkStatus();
  }

  Status Get(const char* name, Service** psvc) {
    if (!name || !*name) return InvalidArgumentError("no name");
    if (!psvc) return InvalidArgumentError("no receiver");

    auto it = services_.find(name);
    if (it == services_.end()) {
      return NotFoundError(name);
    }

    Service* svc = it->second;
    svc->AddRefManually();
    *psvc = svc;
    return OkStatus();
  }

  void Put(Service* svc) {
    if (svc) svc->RemoveRefManually();
  }

  std::vector<std::string> List() {
    std::vector<std::string> result;

    std::lock_guard<std::mutex> lock(mutex_);

    for (auto i : services_) {
      result.push_back(i.first);
    }
    return result;
  }
};

ServiceManager* ServiceManager::New() { return new ServiceManagerImpl(); }
void ServiceManager::Release(ServiceManager* mgr) { delete mgr; }

}  // namespace bbt