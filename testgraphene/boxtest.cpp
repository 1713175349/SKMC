#include"box.hpp"
#include"stdio.h"
#include"stdlib.h"
int main(){
    box a("myboxlattice.dat");
    //a.updatewithfile("myboxlattice.dat");
    std::cout<<a.out_to_xyz();
    return 0;
}