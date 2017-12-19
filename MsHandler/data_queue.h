#ifndef DATA_QUEUE_H
#define DATA_QUEUE_H

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

template<typename T>
class data_queue
{
public:
    data_queue() {_b_completed = false;}

public:
    bool push(T elem)
    {
        std::unique_lock<std::mutex> lck(_mtx);
        _que.push(elem);
        _cond.notify_one();
        return true;
    }

    T pop(bool block = false)
    {
        std::unique_lock<std::mutex> lck(_mtx);

        T elem;
        while(true){
            if(!_que.empty()){
                elem = _que.front();
                _que.pop();
                break;
            }
            else if(!_b_completed && block){
                _cond.wait_for(lck, std::chrono::milliseconds(50));
            }
            else{
                elem = nullptr;
                break;
            }
        }

        return elem;
    }

public:
    void set_completed_flag()
    {
        _b_completed = true;
    }

private:
    std::mutex _mtx;
    std::condition_variable _cond;
    std::queue<T> _que;
    bool _b_completed;
};

#endif // DATA_QUEUE_H
