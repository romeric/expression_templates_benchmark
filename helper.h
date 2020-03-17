#include <iostream>
#include <chrono>

namespace bench_views {

template<typename T>
void print(const T &u) {
    std::cout << u << '\n';
}
template<typename T>
void print(T &&u) {
    std::cout << u << '\n';
}
template<typename T, typename ... Ts>
void print(const T &u, const Ts &...us) {
    print(u);
    print(us...);
}

template<typename T>
void println(const T &u) {
    std::cout << u << ' ';
}
template<typename T>
void println(T &&u) {
    std::cout << u << ' ';
}
template<typename T, typename ... Ts>
void println(const T &u, const Ts &...us) {
    println(u);
    println(us...);
}


template<typename T=double>
struct timer
{
    inline void tic() {t0 = std::chrono::high_resolution_clock::now();}

    inline T toc(const std::string &msg="") {
        using namespace std::chrono;
        elapsed = high_resolution_clock::now() - t0;
        T elapsed_seconds = duration<T,seconds::period>(elapsed).count();
        if (msg.empty()) std::cout << "Elapsed time is: " <<
            elapsed_seconds << " seconds \n";
        return elapsed_seconds;
    }

    std::chrono::high_resolution_clock::time_point t0;
    std::chrono::high_resolution_clock::duration elapsed;
};

}

