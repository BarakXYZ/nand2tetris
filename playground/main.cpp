#include <iostream>

int mult(int r0, int r1){
    int mult{0};
    for(size_t i{0}; i < r0; ++i){
        mult += r1;
    }
    return mult;
}

int main(){
    int r0{6}, r1{7};
    std::cout << r0 << " * " << r1 << " = " << mult(r0, r1) << std::endl;
    return 0;
}
