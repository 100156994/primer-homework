#include"strvec.h"

void StrVec::push_back(const string &s){
    chk_n_alloc();
    alloc.construct(first_free++,s);
}

void StrVec::reallocate(){
    //¿½±´string
//    size_t new_size=size()?size()*2:10;
//    auto new_data=alloc.allocate(new_size);
//    auto e=std::uninitialized_copy_n(elements,size(),new_data);
//    free();
//    first_free=e;
//    elements=new_data;
//    cap=new_data+new_size;

    //ÒÆ¶¯¿½±´string
    size_t new_size=size()?size()*2:1;
    alloc_n_move(new_size);
}


pair<string*,string*> StrVec::alloc_n_copy(const string*b ,const string *e){
    auto data=alloc.allocate(e-b);
    return {data,std::uninitialized_copy(b,e,data)};
}

void StrVec::free(){
    if(elements){
//        for(auto b=first_free;b!=elements;){
//            alloc.destroy(--b);
//        }
        for_each(elements,first_free,[](string &i){alloc.destroy(&i);});
        alloc.deallocate(elements,cap-elements);
    }
}
StrVec::StrVec( initializer_list<string> l){
    auto new_data=alloc_n_copy(l.begin(),l.end());
    elements=new_data.first;
    cap=first_free=new_data.second;
}

StrVec::StrVec(const StrVec &sv){
    auto new_data=alloc_n_copy(sv.begin(),sv.end());
    elements=new_data.first;
    cap=first_free=new_data.second;
}
StrVec& StrVec::operator=(const StrVec &sv){
    auto new_data=alloc_n_copy(sv.begin(),sv.end());
    free();
    elements=new_data.first;
    cap=first_free=new_data.second;
    return *this;
}
StrVec::~StrVec(){
    free();
}

void StrVec::alloc_n_move(size_t sz){
    auto new_data=alloc.allocate(sz);
    auto dest=new_data;
    auto ele=elements;
    for(size_t i=0;i<size();i++)
        alloc.construct(dest++,std::move(*ele++));
    free();
    elements=new_data;
    first_free=dest;
    cap=new_data+sz;
}

void StrVec::reserve(size_t sz){
    if(sz<=capacity()) return;
    alloc_n_move(sz);
}


void StrVec::resize(size_t sz,const string& s){
    if(sz<size()){
        for(auto i=sz;i!=0;--i)
            alloc.destroy(--first_free);
    }else{
        auto new_cap=capacity();
        while(sz>new_cap) new_cap*=2;
        reserve(new_cap);
        for(auto i=size();i!=sz;i++)
            alloc.construct(first_free++,s);
    }


}
