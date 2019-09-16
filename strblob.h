#ifndef STRBLOB_H_INCLUDED
#define STRBLOB_H_INCLUDED

#include"strvec.h"
class StrBlobPtr;


class StrBlob{
public:
    typedef StrVec::size_type size_type;
    friend class StrBlobPtr;
private:
    shared_ptr<StrVec> data;
    void check(size_type i,const string & msg)const;

public:
    StrBlob():data(make_shared<StrVec>()){}
    StrBlob(initializer_list<string> li):data(make_shared<StrVec>(li)){}
    StrBlob(const StrBlob &sb):data(sb.data){};
    StrBlob(shared_ptr<StrVec> sp):data(sp){}
    size_type size()const{return data->size();}
    bool empty()const{return data->size()==0;}
    shared_ptr<StrVec> get_data()const {return data;};
    void pop_back();
    void push_back(const string &s){data->push_back(s);}
    string& front();
    const string& front()const;
    const string& back()const;
    StrBlobPtr begin();
    StrBlobPtr end();
    const StrBlobPtr cbegin()const;
    const StrBlobPtr cend()const;

    friend ostream& operator<<(ostream& os,const StrBlob& sb){
        for(auto i:*(sb.data))
            os<<i;
        return os;
    }
};

class StrBlobPtr{
private:
    weak_ptr<StrVec> wptr;
    size_t curr;
    shared_ptr<StrVec> check(size_t i,const string &msg)const{//为了方便写成内联  实际不该这么写

        auto ret=wptr.lock();
        if(ret==nullptr){
            throw runtime_error("unbound StrBlolPtr");
        }else if(i>=ret->size()){
            throw out_of_range(msg);
        }else{
            return ret;
        }

    }
public:
    StrBlobPtr():curr(0){}
    StrBlobPtr(StrBlob &a,size_t sz=0):wptr(a.data),curr(sz){}
    StrBlobPtr(const StrBlob &a,size_t sz=0):wptr(a.data),curr(sz){}
    bool operator!=(const StrBlobPtr& sbp){
//        cout<<sbp.curr<<" "<<curr;
//        cout<<"!=\n";
        return sbp.curr!=curr;
    }//实际上还需要判断两个ptr 是不是指向的同一个vector 这里进行简略处理
    string& deref()const{
        auto p=check(curr, "deref pass end");
        return (*p)[curr];
    }//同不该内联
    StrBlobPtr& incr(){//不该内联
        check(curr,"incr pass end");
        curr++;
        return *this;
    }

};

StrBlobPtr StrBlob::begin(){
    cout<<"begin\n";
    return StrBlobPtr(*this);
}

StrBlobPtr StrBlob::end(){
    cout<<"end\n";
    return StrBlobPtr(*this,data->size());
}

const StrBlobPtr StrBlob::cbegin()const{
    cout<<"begin\n";
    return StrBlobPtr(*this);
}

const StrBlobPtr StrBlob::cend()const{
    cout<<"end\n";
    return StrBlobPtr(*this,data->size());
}

void StrBlob::check(size_type i, const string& msg)const{
    if(data->size()<i)
        throw out_of_range(msg);
}

void StrBlob::pop_back(){
    check(0,"empty blob");
    data->pop_back();
}

string& StrBlob::front(){
    check(0,"empty blob");
    return data->front();
}
const string& StrBlob::front()const{
    check(0,"empty blob");
    return data->front();
}

const string& StrBlob::back()const{
    check(0,"empty blob");
    return data->back();
}

class TextResult;

class TextQuery{
public:
    using line_no = StrVec::size_type;
    friend class TextResult;
private:
    //StrBlob text; 类版本
    shared_ptr<StrVec> text;
    map<string,shared_ptr<set<TextQuery::line_no>>> word_map;

public:

    TextQuery(){};
    TextQuery(istream & is);
    TextResult Query(const string &key);
    //类版本
    //StrBlob get_text_ptr()const{return text;}
    shared_ptr<StrVec> get_text_ptr()const{return text;}
    map<string,shared_ptr<set<TextQuery::line_no>>> get_word_map_prt()const{return word_map;}//无用
};

class TextResult{
public:

    friend ostream& print(ostream& os,const TextResult &tr);

    TextResult(TextQuery &tq):text(tq.get_text_ptr()){}
    //类版本
    //TextResult(const string& s,const StrBlob &sb,shared_ptr<set<TextQuery::line_no>> lp=nullptr):text(sb.get_data()),line_set(lp),key(s){};
    TextResult(const string& s,shared_ptr<StrVec> tp=nullptr,shared_ptr<set<TextQuery::line_no>> lp=nullptr):text(tp),line_set(lp),key(s){};

    set<TextQuery::line_no>::iterator begin()const{return line_set->begin();}
    set<TextQuery::line_no>::iterator end()const{return line_set->end();}
    shared_ptr<StrVec> get_text_ptr()const {return text;}

private:
    //StrBlob text;  类版本
    shared_ptr<StrVec> text;
    shared_ptr<set<TextQuery::line_no>> line_set;
    string key;
};
//类版本
//TextQuery::TextQuery(istream &is):text(),word_map(map<string,shared_ptr<set<TextQuery::line_no>>>()){
//    string line;
//    line_no lines=0;
//    clog<<"start construct TextQuery\n";
//    while(getline(is,line)){
//        clog<<line<<"\n";
//
//        text.push_back(line);
//
//        istringstream iss(line);
//        string word;
//        while(iss>>word){
//            cout<<"test";
//            auto &sp=word_map[word];
//            if(sp==nullptr){//没有有这个词
//                sp.reset(new set<line_no>);
//            }
//            sp->insert(lines);
//            cout<<word<<"\t"<<sp->size()<<endl;
//        }
//        lines++;
//    }
//}

TextQuery::TextQuery(istream &is):text(make_shared<StrVec>()),word_map(map<string,shared_ptr<set<TextQuery::line_no>>>()){
    string line;
    line_no lines=0;
    clog<<"start construct TextQuery\n";
    while(getline(is,line)){
        clog<<line<<"\n";
        text->push_back(line);
        istringstream iss(line);
        string word;
        while(iss>>word){
            //cout<<"test";
            auto &sp=word_map[word];
            if(sp==nullptr){//没有有这个词
                sp.reset(new set<line_no>);
            }
            sp->insert(lines);
            //cout<<word<<"\t"<<sp->size()<<endl;
        }
        lines++;
    }
}

TextResult TextQuery::Query(const string &key){
    auto p=word_map.find(key);
    if(p!=word_map.end()){
        return TextResult(key,text,(*p).second);
    }else{//没找到
        return TextResult(key);
        //类版本
//        return TextResult(key,StrBlob());
    }
}

ostream& print(ostream& os,const TextResult &tr){
    if(tr.line_set==nullptr){
        os<<"key: "<<tr.key<<" number lines: 0"<<endl;
        return os;
    }
    os<<"key: "<<tr.key<<" number lines: "<<tr.line_set->size()<<endl;

    for(auto &i:*tr.line_set){
        os<<(*tr.text)[i]<<endl;
        //类版本
        //os<<(*tr.text.get_data())[i]<<endl;
    }

    return os;
}



void RunQueries(ifstream& fin){
    TextQuery tq(fin);
    string s;

    do{
        cout<<"enter word to look for(q to quit):";
        cin>>s;
        if(s!="q")print(cout,tq.Query(s))<<endl;
    }while(cin&&s!="q");
}


#endif // STRBLOB_H_INCLUDED
