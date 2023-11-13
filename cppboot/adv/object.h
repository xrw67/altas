#ifndef CPPBOOT_ADV_OBJECT_H_
#define CPPBOOT_ADV_OBJECT_H_

#include <atomic>

namespace cppboot {

class Object {
 public:
  Object();

  virtual const char* object_id() const = 0;

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

}  // namespace cppboot

#endif  // CPPBOOT_ADV_OBJECT_H_
