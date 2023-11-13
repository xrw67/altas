#include "cppboot/adv/plugin.h"

#include <map>
#include <set>
#include <string>
#include <memory>
#include <mutex>

#include "cppboot/base/fmt.h"
#include "cppboot/base/str_util.h"

namespace cppboot {

namespace {

Status VerifyPluginHeader(const PCPPBOOT_PLUGIN_HEADER hdr) {
  if (!hdr) return InvalidArgumentError("no plugin");
  if (!hdr->name || !*hdr->name) return InvalidArgumentError("no name");
  if (!hdr->version || !*hdr->version)
    return InvalidArgumentError("no version");
  if (!hdr->init) return InvalidArgumentError("no init func");
  if (!hdr->exit) return InvalidArgumentError("no exit func");
  return OkStatus();
}

};  // namespace

struct Plugin;
typedef std::shared_ptr<Plugin> PluginPtr;

struct Plugin {
  const PCPPBOOT_PLUGIN_HEADER hdr;
  std::string param;
  std::set<PluginPtr> sources;  // What plugins depend on me?
  std::set<PluginPtr> targets;  // What plugins do I depend on?

  explicit Plugin(const PCPPBOOT_PLUGIN_HEADER hdr) : hdr(hdr) {}
};

class PluginManagerImpl : public PluginManager {
 public:
  PluginManagerImpl(PluginLoader* loader) : loader_(loader) {}

 private:
  PluginLoader* loader_;

  std::mutex mutex_;
  std::map<std::string, PluginPtr> mods_;

  Status Load(const char* name, const char* param) {
    if (!name || !*name) return InvalidArgumentError("no plugin name");
    {
      std::lock_guard<std::mutex> lock(mutex_);
      if (mods_.find(name) != mods_.end()) return AlreadyExistsError(name);
    }

    PCPPBOOT_PLUGIN_HEADER hdr;
    Status st = loader_->Load(name, &hdr);
    if (!st.ok()) return st;

    PluginPtr me(new Plugin(hdr));

    if (!param) param = "";
    st = InitPulgin(me, hdr, param);
    if (!st.ok()) {
      loader_->Unload(name);
      return st;
    }

    std::lock_guard<std::mutex> lock(mutex_);
    me->param = param;
    mods_[hdr->name] = me;
    return OkStatus();
  }

  Status Unload(const char* name) {
    if (!name || !*name) return InvalidArgumentError("empty plugin name");

    PluginPtr me;

    {
      std::lock_guard<std::mutex> lock(mutex_);
      auto it = mods_.find(name);
      if (it == mods_.end()) {
        return NotFoundError(name);
      }

      me = it->second;

      // 判断依赖关系
      if (!me->sources.empty())
        return CancelledError("has depend on by others");

      for (auto target : me->targets) {
        target->sources.erase(me);
      }
      mods_.erase(it);
    }

    me->hdr->exit();
    loader_->Unload(me->hdr->name);
    return OkStatus();
  }

  Status InitPulgin(PluginPtr& plugin, const PCPPBOOT_PLUGIN_HEADER hdr,
                    const char* param) {
    Status st = VerifyPluginHeader(hdr);
    if (st.ok()) {
      st = VerifyDependence(plugin);
      if (st.ok()) {
        int err = hdr->init(param);
        if (err) return InvalidArgumentError("init retrn error");
      }
    }
    return st;
  }

  Status VerifyDependence(PluginPtr& plugin) {
    if (!plugin->hdr->requires) return OkStatus();

    auto require_list = StrSplit(plugin->hdr->requires, ',');
    for (auto& i : require_list) {
      auto name = StrTrim(i, " ");
      if (name.empty()) continue;
      auto it = mods_.find(to_string(name));
      if (it == mods_.end())
        return NotFoundError(format("depend on {}", name.data()));
      else
        AddpluginUsage(plugin, it->second);
    }
    return OkStatus();
  }

  // Plugin a uses b
  void AddpluginUsage(PluginPtr& a, PluginPtr& b) {
    b->sources.insert(a);
    a->targets.insert(b);
  }
};

PluginManager* PluginManager::New(PluginLoader* loader) {
  return new PluginManagerImpl(loader);
}
void PluginManager::Release(PluginManager* mgr) { delete mgr; }

}  // namespace cppboot