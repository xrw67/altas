#include "cppboot/adv/module.h"

#include <string>
#include <map>

#include <dlfcn.h>

#include "cppboot/base/log.h"
#include "cppboot/base/fs.h"
#include "cppboot/base/fmt.h"
#include "cppboot/base/str_util.h"

namespace cppboot {
//
// DLL Module Loader
//

class DllLoader : public ModuleLoader {
 public:
  DllLoader(const char* dir) : dir_(dir) {}
  ~DllLoader() {
    for (auto h : handles_) {
      if (::dlclose(h.second)) {
        CPPBOOT_RAW_LOG(ERROR, "unload module {} failed: {}", h.first, dlerror());
      }
    }
  }

 private:
  Status Load(const char* name, PCPPBOOT_MODULE_HEADER* result) {
    if (!name || !*name) {
      return InvalidArgumentError("no name");
    }
    if (!result) {
      return InvalidArgumentError("invalid result");
    }

    auto path = GetModulePath(name);
    void* handle = dlopen(path.c_str(), RTLD_NOW);
    if (!handle) {
      return CancelledError(
          format("load module {} failed: {}", name, dlerror()));
    }

    PCPPBOOT_MODULE_HEADER hdr =
        (PCPPBOOT_MODULE_HEADER)::dlsym(handle, "cppboot_module_header");
    if (!hdr) {
      ::dlclose(handle);
      return CancelledError(
          format("load module {} failed: no symbol cppboot_module_header", name));
    }

    handles_[name] = handle;
    *result = hdr;
    return OkStatus();
  }

  virtual Status Unload(const char* name) {
    if (!name || !*name) return InvalidArgumentError("no name");

    auto it = handles_.find(name);
    if (it != handles_.end()) {
      if (::dlclose(it->second)) {
        return UnknownError(::dlerror());
      }
      handles_.erase(it);
      return OkStatus();
    }
    return NotFoundError(name);
  }

  std::string GetModulePath(const char* name) {
    return PathJoin(dir_, StrCat("libmod_", name, ".so"));
  }

 private:
  std::string dir_;
  std::map<std::string, void*> handles_;
};

ModuleLoader* ModuleLoader::New(ModuleLoader::Type type, const char* dir) {
  switch (type) {
    case kDll:
      return new DllLoader(dir ? dir : "");
    default:
      CPPBOOT_ASSERT(true);
      return NULL;
  }
}

void ModuleLoader::Release(ModuleLoader* loader) { delete loader; }
}  // namespace cppboot