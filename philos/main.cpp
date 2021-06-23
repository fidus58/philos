//
//  main.cpp
//  philos
//
//  Created by Klaus Ahrens on 09.07.14.
//  Copyright (c) 2014 Klaus Ahrens. All rights reserved.
//

#include <thread>
#include <vector>
#include <chrono>
#include <iostream>

std::mutex forks[5];

void philo(int i) {
    for (int n=0; n<10;++n) {
        forks[   i   ].lock();
        std::cout<<"fork "<<i<<" locked\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        forks[(i+1)%5].lock();
        std::cout<<"fork "<<(i+1)%5<<" locked\n";

        std::cout<<"philosopher "<<i<<" is eating "
                 <<"["<<n<<"]\n";
        forks[(i+1)%5].unlock();
        forks[   i   ].unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
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
