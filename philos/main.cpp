#include <vector>
#include <thread>
#include <mutex>
#include <iostream>
#include <chrono>
#include <algorithm>
#include <string_view>

using namespace std::literals; // all literals

std::mutex forks[5], io;

void log(int i, std::string_view const& msg){
    std::lock_guard<std::mutex> l{io};
        std::cout<<"philo nr."<<i<<msg;
}

void philo(int i) {
    for (int n=0; n<100;++n) {
        std::scoped_lock sl(forks[i], forks[(i+1)%5]); // C++17! atomic !
        log(i, " got both forks\n"s);
        std::this_thread::sleep_for(1s);
        log(i, " eats\n"s);
    }
}
std::vector<std::thread> philos;

int main(int argc, const char * argv[]) {
    for (int i=0; i<5; ++i) {
        std::thread th(philo, i);
        philos.push_back(std::move(th));
    }
    std::for_each(begin(philos), end(philos),[](std::thread& t) {t.join();});
    return 0;
}
