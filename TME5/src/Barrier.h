#include <mutex>
#include <condition_variable>
#include <iostream>
namespace pr{
class Barrier {
private:
    std::mutex m;
    std::condition_variable cv;
    int counter = 0;   
    int N;              

public:
    Barrier(int expected): N(expected) {}

    
    void done() {
        std::unique_lock<std::mutex> lock(m);
        counter++;
        if (counter >= N) {
            cv.notify_all(); 
        }
    }

    
    void waitFor() {
        std::unique_lock<std::mutex> lock(m);
        cv.wait(lock, [this]{ return counter >= N; });
    }
};
}
