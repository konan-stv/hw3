

template<class T, class A = std::allocator<T>> 
class kCont {
private:
/*  struct rec {
    T val;
    rec *next;
  };
  rec *root, last;*/
  using tData=std::list<int, A>;
  tData data;
  decltype(data.begin()) c;
public:
  kCont() {}
  ~kCont() {}
  void add(T v) {data.push_back(v);}
  void begin() {c = data.begin();}
  T next() {return *(c++);}
};

