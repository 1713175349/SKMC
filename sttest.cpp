#include "struct_template.hpp"

int main(int argc, char *argv[]){
    std::cout<<"ds1";
    struct_template *a=new struct_template(10,10);
    std::cout<<"ds";
    a = read_file_to_temp("template_example.dat");
    return 0;
}