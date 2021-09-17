
class Memento
{
    string state;
    //状态表示和 Originator 对应, 但不需要一样, 如可以经过某些编码
public:
    Memento(const string & s) : state(s) {}
    string getState() const { return state; }
    void setState(const string & s) { state = s; }
};



class Originator
{
    string state;
    //....
public:
    Originator() {}
    Memento createMomento() {
        Memento m(state); // 实际实现可能相当复杂, 这里相当于是拍一个快照
        return m;
    }
    void setMomento(const Memento & m) {
        state = m.getState();
    }
};



int main()
{
    Originator orginator;
    
    //捕获对象状态，存储到备忘录
    Memento memento = orginator.createMomento();
    
    //... 改变orginator状态
    
    //从备忘录中恢复
    orginator.setMomento(memento);
 
}