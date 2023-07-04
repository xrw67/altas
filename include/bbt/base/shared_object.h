#ifndef BBT_BASE_SHARED_OBJECT_H_
#define BBT_BASE_SHARED_OBJECT_H_

#include <atomic>

namespace bbt {

//
// SharedObject
// https://github.com/apache/incubator-brpc/blob/master/src/brpc/shared_object.h
//
// Inherit this class to be intrusively shared. Comparing to shared_ptr,
// intrusive_ptr saves one malloc (for shared_count) and gets better cache
// locality when the ref/deref are frequent, in the cost of lack of weak_ptr
// and worse interface.
class SharedObject {
  friend void intrusive_ptr_add_ref(SharedObject*);
  friend void intrusive_ptr_release(SharedObject*);

 public:
  SharedObject() : nref_(0) {}
  long ref_count() const { return nref_.load(); }
  void AddRefManually() { ++nref_; }
  void RemoveRefManually() {
    if (--nref_ == 0) delete this;
  }

 protected:
  virtual ~SharedObject() {}

 private:
  std::atomic_long nref_;
};

inline void intrusive_ptr_add_ref(SharedObject* obj) { obj->AddRefManually(); }
inline void intrusive_ptr_release(SharedObject* obj) {
  obj->RemoveRefManually();
}

}  // namespace bbt

#endif  // BBT_BASE_SHARED_OBJECT_H_
