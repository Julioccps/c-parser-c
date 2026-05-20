void conversor() {
    auto kelvin = [x](int y) {
        x = 273 + y;        
        return x;
    };
    int resultado = kelvin(10);
}

int main() {
    return 0;
}