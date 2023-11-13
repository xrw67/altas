#include "cppboot/adv/plugin_manager.h"

#include <string>
#include <map>

#include <dlfcn.h>

#include "cppboot/base/log.h"
#include "cppboot/base/fs.h"
#include "cppboot/base/fmt.h"
#include "cppboot/base/str_util.h"

namespace cppboot {
//
// DLL Plugin
//

class DllLoader : public PluginLoader {
 public:
  DllLoader(const char* dir) : dir_(dir) {}
  ~DllLoader() {
    for (auto h : handles_) {
      if (::dlclose(h.second)) {
        CPPBOOT_RAW_LOG(ERROR, "unload plugin {} failed: {}", h.first,
                        dlerror());
      }
    }
  }

 private:
  Status Load(const char* name, struct cppboot_plugin** result) {
    if (!name || !*name) {
      return InvalidArgumentError("no name");
    }
    if (!result) {
      return InvalidArgumentError("invalid result");
    }

    auto path = GetPluginPath(name);
    void* handle = dlopen(path.c_str(), RTLD_NOW);
    if (!handle) {
      return CancelledError(
          format("load plugin {} failed: {}", name, dlerror()));
    }

    struct cppboot_plugin* hdr =
        (struct cppboot_plugin*)::dlsym(handle, "cppboot_plugin_entry");
    if (!hdr) {
      ::dlclose(handle);
      return CancelledError(format(
          "load plugin {} failed: no symbol cppboot_plugin_entry", name));
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

  std::string GetPluginPath(const char* name) {
    return PathJoin(dir_, StrCat("libplugin_", name, ".so"));
  }

 private:
  std::string dir_;
  std::map<std::string, void*> handles_;
};

PluginLoader* PluginLoader::New(PluginLoader::Type type, const char* dir) {
  switch (type) {
    case kDll:
      return new DllLoader(dir ? dir : "");
    default:
      CPPBOOT_ASSERT(true);
      return NULL;
  }
}

void PluginLoader::Release(PluginLoader* loader) { delete loader; }
}  // namespace cppboot