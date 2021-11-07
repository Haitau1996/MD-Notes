class Grand{

}
class ParentsA:virtual public Grand{

}
class ParentsB:virtual public Grand{

}
class Object:public ParentsA, public ParentsB{
    
}