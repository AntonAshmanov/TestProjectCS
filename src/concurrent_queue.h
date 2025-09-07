#ifndef CONCURRENT_QUEUE_H
#define CONCURRENT_QUEUE_H

#include <queue>
#include <memory>
#include <condition_variable>

template<typename T>
class threadsafe_queue {
    private:
        mutable std::mutex mut;
        std::queue<std::shared_ptr<T>> data_queue;
        std::condition_variable data_cond;
    public:
        threadsafe_queue() {}
        void push(T new_value) {
            std::shared_ptr<T> data(std::make_shared<T>(std::move(new_value)));
            std::lock_guard<std::mutex> lk(mut);
            data_queue.push(data);
            data_cond.notify_one();
        }

        bool empty() const {
            std::lock_guard<std::mutex> lk(mut);
            return data_queue.empty();
        }

        std::shared_ptr<T> wait_and_pop() {
            std::unique_lock<std::mutex> lk(mut);
            data_cond.wait(lk, [this]{return !data_queue.empty();});
            std::shared_ptr<T> res = data_queue.front();
            data_queue.pop();
            return res;
        }
};

#endif