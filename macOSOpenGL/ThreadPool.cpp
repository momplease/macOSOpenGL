//
//  ThreadPool.cpp
//  macOSOpenGL
//
//  Created by Andrii Zaitsev on 6/24/17.
//  Copyright © 2017 Andrii Zaitsev. All rights reserved.
//

#include "ThreadPool.hpp"

ThreadPool::ThreadPool() : ThreadPool(std::thread::hardware_concurrency()) {
}

ThreadPool::ThreadPool(unsigned int threads) {
}

ThreadPool::~ThreadPool() {
}
