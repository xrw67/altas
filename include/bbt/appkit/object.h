#ifndef BBT_APPKIT_OBJECT_H_
#define BBT_APPKIT_OBJECT_H_

#include <atomic>

namespace bbt {

class Object {
 public:
  Object();

  /// @brief 服务名
  virtual const char* service_name() const = 0;

  long ref_count() const;
  void AddRefManually();
  void Release();

 protected:
  virtual ~Object() {}

 private:
  std::atomic_long nref_;

  friend void intrusive_ptr_add_ref(Object*);
  friend void intrusive_ptr_release(Object*);
};

inline void intrusive_ptr_add_ref(Object* obj) { obj->AddRefManually(); }
inline void intrusive_ptr_release(Object* obj) { obj->Release(); }

inline Object::Object() : nref_(0) {}
inline long Object::ref_count() const { return nref_.load(); }
inline void Object::AddRefManually() { ++nref_; }
inline void Object::Release() {
  if (--nref_ == 0) delete this;
}

}  // namespace bbt

#endif  // BBT_APPKIT_OBJECT_H_
