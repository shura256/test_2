#ifndef THREAD_QUEUE_H_
#define THREAD_QUEUE_H_

#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>

template <typename T>
class ThreadsafeQueue {
 public:
  typedef std::shared_ptr<ThreadsafeQueue> Ptr;

  void push(T&& value) {
    std::lock_guard<std::mutex> lk(mut_);
    queue_.push(std::move(value));
    cond_.notify_one();
  }

  bool try_pop(T& value) {
    std::lock_guard<std::mutex> lk(mut_);
    if (queue_.empty()) return false;
    value = std::move(queue_.front());
    queue_.pop();
    return true;
  }

  T wait_and_pop() {
    std::unique_lock<std::mutex> lk(mut_);
    cond_.wait(lk, [this] { return !queue_.empty(); });
    T data = std::move(queue_.front());
    queue_.pop();
    return data;
  }

 private:
  std::mutex mut_;
  std::condition_variable cond_;
  std::queue<T> queue_;
};

#endif  // TREADSAFE_QUEUE_H_
