#ifndef STRVEC_H_INCLUDED
#define STRVEC_H_INCLUDED

#include<iostream>
#include<string>
#include<memory>
#include<utility>
#include<functional>
#include<algorithm>
using std::string;
using std::pair;
using std::size_t;
using std::allocator;
using std::initializer_list;
class StrVec{
private:
    string *elements;
    string *first_free;
    string *cap;
    static allocator<string> alloc;
    //tool func
    pair<string*,string*> alloc_n_copy(const string* ,const string *);
    void alloc_n_move(size_t);
    void free();
    void reallocate();
    void chk_n_alloc(){if(size()==capacity()) reallocate();}

public:
    typedef size_t size_type;
    StrVec():elements(nullptr),first_free(nullptr),cap(nullptr){}
    StrVec(const StrVec &sv);
    StrVec& operator=(const StrVec &sv);
    StrVec( initializer_list< string > l);
    ~StrVec();
    size_t size()const{return first_free-elements;};
    size_t capacity()const{return cap-elements;}
    string* begin()const {return elements;}
    string* end()const{return first_free;}
    void push_back(const string &s);
    void pop_back(){alloc.destroy(first_free--);}
    string& front()const{return *elements;}
    string& back()const{return *(first_free-1);}
    void reserve(size_t);
    void resize(size_t sz,const string& s="");
    string& operator[](size_t index){return *(elements+index);}
};


class String{
private:
    static allocator<char> alloc;
    char *elements;
    char *first_free;
    pair<char *,char *> alloc_n_copy(const char*b ,const char *e){
        auto data=alloc.allocate(e-b);
        return {data,std::uninitialized_copy(b,e,data)};
    };
    void free(){
        if(elements){
        std::for_each(elements,first_free,[](char &i){alloc.destroy(&i);});
        alloc.deallocate(elements,first_free-elements);
    }}
public:
    String():elements(nullptr),first_free(nullptr){}
    String(const char *s){
        auto b=const_cast<char*>(s);
        while(*s!='\0')
            s++;
        auto d=alloc_n_copy(b,s);
        elements=d.first;
        first_free=d.second;
    };
    String(const String&)=delete;
    String& operator=(const String&)=delete;
    ~String(){free();}

};

//allocator<string> StrVec::alloc;

#endif // STRVEC_H_INCLUDED
