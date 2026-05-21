void energia() {
    auto cinetica = [] (int m, int v) {
        return (1/2) * m * v * v;
    };
    int resultado = cinetica(5, v);
}

int main(){
    return 0;
}