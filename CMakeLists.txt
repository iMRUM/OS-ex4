cmake_minimum_required(VERSION 3.29)
project(OS_ex4)

set(CMAKE_CXX_STANDARD 14)

add_executable(OS_ex4
    isEulerian.cpp
        ../part2/ASingletonGUARD.h
        ../part2/ASingletonDCLO.h
        ../part2/ASingleton_GUARD.cpp
        ../part2/ASingleton_DCLO.cpp
        ../part2/Scope_Mutex.cpp
        ../part2/ScopeMutex.h)
