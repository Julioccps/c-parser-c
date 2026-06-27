#ifndef COMPAT_HPP
#define COMPAT_HPP
typedef const char* string;
inline void print(int x) {
    std::cout << x << std::endl;
}

inline void print(string s) {
    std::cout << s << std::endl;
}
#endif