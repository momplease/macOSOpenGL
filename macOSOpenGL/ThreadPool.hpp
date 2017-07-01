//
//  ThreadPool.hpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/24/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#ifndef ThreadPool_hpp
#define ThreadPool_hpp

#include <thread>
#include <future>
#include <vector>
#include <queue>

class ThreadPool final {
public:
    ThreadPool();
    ThreadPool(unsigned int);
    ~ThreadPool();
    
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;
public:
    
    //template<typename T>
    //std::future<T> enqueue()
    
    
private:
    std::vector<std::thread> workingThreads;
    //std::queue<std::function<<#class _Fp#>>>
};

#endif /* ThreadPool_hpp */
