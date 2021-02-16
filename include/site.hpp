#pragma once
#include<vector>
#include<string>
class site
{
private:
    
public:
    double position[3];
    int areaid[3];//临域id
    int siteid;
    std::vector<int> neighbors;
    bool isusing;
    //double allrate;//总速率
    int type;//该site的类型
    int state;//该位点的状态
    std::vector<int> site_events;//该site的所有event id
    std::vector<std::vector<int>> embed_list;
    std::vector<int> embed_framework_id;
    double *data;//用于存储附加浮点数据
    int data_length;//附加数据的长度
    int *idata;
    int idata_length;
    site(/* args */);
    int add_data(int length);//添加附加数据支持
    int add_idata(int length);//添加附加整数
    ~site();
};

site::site(/* args */)
{
    isusing = false;
    state = 0;
}

site::~site()
{
    if(data_length >0){
        delete [] data;
    }
    if(idata_length>0){
        delete [] idata;
    }
}

int site::add_data(int length){
    if (data_length ==0)
    {
        data_length=length;
        data=new double[length];
        return 1;
    }
    return 0;
}

int site::add_idata(int length){
    if (data_length ==0)
    {
        idata_length=length;
        idata=new int[length];
        return 1;
    }
    return 0;
}

class event
{
private:
    /* data */
public:
    int type;//事件类型
    std::string name;
    int id;//事件id
    int event_site;//save the site of this event
    int embed_index;//save the index of embed in this site
    int frame_e_index;//save the index in framework of this event
    int frame_index;//the index of framework
    double rate;
    double primary_rate;
    event(/* args */);
    ~event();
};

event::event(/* args */)
{
}

event::~event()
{
}
