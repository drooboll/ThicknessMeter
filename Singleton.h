#pragma once

template <typename T>
class Singleton {
public:
    static T & getInstance(){
        static T instance ;
        return instance ;
    }
    Singleton() = delete ;
    Singleton(const Singleton<T> &) = delete ;
    const Singleton<T> & operator=(const Singleton<T> &) = delete ;
};
