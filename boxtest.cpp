#include"box.hpp"
#include"stdio.h"
#include"stdlib.h"
int main(){
    std::cout<<"1"<<std::endl;
    box a("myboxlattice.dat");
    //a.updatewithfile("myboxlattice.dat");
    std::cout<<a.out_to_xyz();
    return 0;
}