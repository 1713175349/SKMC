#include<boost/multi_array.hpp>

int main(){
    int nabc[3]={2,2,3};
    int site_cell_n=4;
    boost::multi_array<int,4> cells(boost::extents[nabc[0]][nabc[1]][nabc[2]][site_cell_n]);
    return 0;

}