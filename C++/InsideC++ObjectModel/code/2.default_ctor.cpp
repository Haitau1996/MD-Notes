#include<iostream>

using namespace std;
class Obj_with_ctor{
public:
    Obj_with_ctor(){
        cout << "good AHHHHHA\n"; 
    }
};
class Obj_withour_ctor{
private:
    int m_i;
    int m_j;
public:
    void funct(){
        cout<< "I'm func of BHHHHHH\n";
    }
};
int main(){
    Obj_withour_ctor some_obj;
    Obj_with_ctor other_obj;
}