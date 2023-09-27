#ifndef BBT_IO_CONTEXT_H_
#define BBT_IO_CONTEXT_H_

#include <atomic>
#include <functional>
#include <memory>
#include <mutex>
#include <vector>

#include <sys/event.h>

namespace bbt {
namespace io {

class Context {
 public:
  typedef std::function<void()> Functor;

  Context();
  ~Context();

  void Stop();

  void RunOnce();

  // run queue

  /**
   * @brief 如果在当前线程立即执行，否则加入到队列
   *
   * @param cb
   */
  void Dispatch(Functor cb);

  /**
   * @brief enqueue, run at next weakup
   *
   * @param cb
   */
  void Post(Functor cb);
  size_t QueueSize() const { return pending_functors_.size(); }

 private:
  void PollOnce();
  void DoPendingFunctors();

  void WeakUpPoll();

  typedef std::vector<struct kevent> EventList;
  typedef std::vector<Functor> FunctorList;

  static const int kInitEventListSize = 16;

  std::atomic_bool quit_;

  int kq_;
  EventList events_;

  std::mutex mutex_;
  FunctorList pending_functors_;  // GUARDED_BY(mutex_)
};

}  // namespace io
}  // namespace bbt

#endif  // BBT_IO_CONTEXT_H_
