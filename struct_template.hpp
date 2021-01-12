#include<vector>
#include<fstream>
#include<iostream>
#include<sstream>
struct vertex
{
    double position[3];
    int id;
    int neighNumber;
    std::vector<int> neighboors;
};

class struct_template
{
private:
    /* data */
public:
    vertex *nodes;
    int noden;
    int eventn;
    int **start_state;//开始时对应各个node的状态，-1，表示任意状态
    int **end_state;
    struct_template(int n,int m);
    ~struct_template();
};

struct_template::struct_template(int n,int m)
{
    noden=n;
    eventn=m;
    nodes = new vertex[n];
    for (int i = 0; i < n; i++)
    {
        nodes[i].id = i;
    }
    start_state = new int*[m];
    end_state = new int*[m];
    for (int i = 0; i < m; i++)
    {
        start_state[i] = new int[n];
        start_state[i] = new int[n];
    }
    
    
}

struct_template::~struct_template()
{
    delete [] nodes;
    for (int i = 0; i < eventn; i++)
    {
        delete [] start_state[i] ;
        delete [] start_state[i] ;
    }
    delete [] start_state;
    delete [] end_state;
}

struct_template * read_file_to_temp(const char *filename){
    using namespace std;
    fstream fp;
    istringstream sbuf;
    int bufint;
    vertex *nvertex;
    int Nmax=1000;
    char buffer[Nmax];
    cout<<"ready read";
    fp.open(filename,ios::in);
    cout<<"read";
    fp.getline(buffer,Nmax);//掐头
    fp.getline(buffer,Nmax);
    int nodesNumber,EventNumber;
    sscanf(buffer,"%d  %d",&nodesNumber,&EventNumber);
    struct_template *stemp=new struct_template(nodesNumber,EventNumber);
    for (int i = 0; i < nodesNumber; i++)
    {
        fp.getline(buffer,Nmax);
        sbuf.clear();
        sbuf.str(buffer);
        sbuf.seekg(ios_base::beg);
        nvertex = stemp->nodes+i;
        sbuf >> nvertex->id;
        sbuf >> nvertex->position[0];
        sbuf >> nvertex->position[1];
        sbuf >> nvertex->position[2];
        sbuf >> nvertex->neighNumber;
        for (int j = 0; j < nvertex->neighNumber; j++)
        {   
            sbuf >> bufint;
            nvertex->neighboors.insert(nvertex->neighboors.end(),bufint);
        }     
    }
    fp.getline(buffer,Nmax);
    for (int i = 0; i < EventNumber; i++)
    {
        fp.getline(buffer,Nmax);
        sbuf.clear();
        sbuf.str(buffer);
        sbuf.seekg(ios_base::beg);
        for (int j = 0; j < nodesNumber; j++)
        {
            sbuf >> stemp->start_state[i][j];
        }
        sbuf>>buffer;
        for (int j = 0; j < nodesNumber; j++)
        {
            sbuf >> stemp->end_state[i][j];
        }
        
    }
    return stemp;
}