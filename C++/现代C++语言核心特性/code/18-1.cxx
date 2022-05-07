#include <mutex>
std::mutex mx;
bool shared_flag = true;
int main()
{
    if (std::lock_guard<std::mutex> lock(mx); shared_flag) { 
        shared_flag = false;
    }
}

#include <condition_variable>
#include <chrono>
using namespace std::chrono_literals;
std::condition_variable cv;
std::mutex cv_m;
int main()
{
    switch (std::unique_lock<std::mutex> lk(cv_m); cv.wait_for(lk, 100ms))
    {
    case std::cv_status::timeout:
        break;
    case std::cv_status::no_timeout:
        break;
    }
}