#include "context.h"
#include <sys/event.h>
#include <mutex>

namespace cppboot {
namespace io {

static const struct timespec kTimeout = {10, 0};

Context::Context() : quit_(false), kq_(kqueue()), events_(kInitEventListSize) {}
Context::~Context() {}

void Context::Stop() {
  quit_.exchange(true);
  WeakUpPoll();
}

void Context::RunOnce() {
  PollOnce();
  DoPendingFunctors();
};

void Context::Post(Functor cb) { pending_functors_.push_back(std::move(cb)); }

void Context::PollOnce() {
  kevent(kq_, nullptr, 0, events_.data(), static_cast<int>(events_.size()),
         &kTimeout);
}

void Context::DoPendingFunctors() {
  std::vector<Functor> functors;
  {
    std::lock_guard<std::mutex> lock(mutex_);
    functors.swap(pending_functors_);
  }

  for (const Functor& fn : functors) {
    fn();
  }
}

void Context::WeakUpPoll() {
  // TODO:
}

}  // namespace io
}  // namespace cppboot