#include <compat.hpp>

int velocidade(int s, int t) {
    int v = 0;
    v = s / t;
    return v;
}

int main() {
    print(velocidade(10, 5));
    return 0;
}