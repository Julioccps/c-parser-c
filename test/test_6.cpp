#include <compat.hpp>

int peso(int massa, int gravidade) {
    int p = 0;
    p = massa * gravidade;
    return p;
}

int main() {
    print(peso(10, 10));
    return 0;
}