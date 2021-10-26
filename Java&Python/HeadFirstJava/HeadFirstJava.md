# Head First Java

## Chap 01 进入Java的世界
java将带你进入全新的领域, 同之前的语言不同, 它在设计时候就有下面的特性:
* 友好的语法
* 面向对象
* 内存管理
* 跨平台可移植

Java 程序的执行过程:<br>
![avator](figure/1.1.png)<br>
* source file: 拓展名为*.java,用来表示程序的一个组件, 类的内容在声明之后的花括号里
* 类: 带有一个或者多个方法, 方法必须在类的内部声明
* 方法: 花括号中编写的应该执行的指令, 可以理解为是一个函数或者过程(C++中的成员函数)
    - 不管程序多大(含有多少个类),都一定会有个 _Main_ 函数作为程序的起点

在main或者其他方法中, 我们可以
* do something: 声明\设定\调用方法等简单语句
* loop
  * while loop
  * do-while loop
  * for loop
    ```Java
    while(boolean expression){
        ... // do something
    }
    for(    ;   ;   ){
        ... // do some thing
    }
    ```
* branching : 在适当的条件下做某些事情
    ```Java
    if (x == 10) {
        System.out.print("x must be 10");
    } else {
        System.out.print("x isn’t 10");
    }
    if ((x < 3) & (name.equals("Dirk"))) {
        System.out.println("Gently");
    }
    ```

需要注意的是， ==Java 是面向对象的语言， **所有东西都必须包含在类中**==， 并且在条件判断中不支持别的类型到 boolean 的隐式类型转换， 可行的替代做法包括 三元运算法 `? :`/ 位运算/ if-else / 第三方库实现的静态方法。   

## Chap 02: 拜访对象村
<div align=center><img src="https://i.imgur.com/65Se4bz.png"/></div>

在继承中， subclass 会自动继承 superclass 的方法， 并且可以适用自定义的同名方法**覆盖（override）** 父类的方法。 在设计类时， 需要记住对象是靠类模型塑造出来的， 类不是对象， 却是对象的蓝图。我们适用圆点（`.`）来使用对象的变量与方法：
* 实例变量： 对象本身已知的事物
* 方法： 对象可以执行的动作

一般而言 mian 有两个用途， ==真正的 Java 程序会让对象和对象之间交互==：
* 测试真正的类
* 启动 Java 应用程序

在 Java 语言中不存在全局变量的概念， 我们适用 `public/static/final` 等修饰符就可以实现类似的 全局和常量等需求。 

## Chap 03: 认识变量
Java 语言中变量有两种： ==**primitive** 和 **reference**==.并且 Java 是静态类型语言， 注重变量的类型。 
* primitive 类型有八种， 四种整型两种浮点型还有 boolean 和 char, 需要注意的是它们都有指定的范围要避免溢出 <div align=center><img src="https://i.imgur.com/eBi4j59.png"/></div>
* 没有对象变量这样的东西， 有的只是==引用对象的变量==， 保存的是存取对象的方法，对象是放在可以回收的堆上， 而引用可以理解为实际对象的遥控器 <div align=center><img src="https://i.imgur.com/1Ffid7e.png"/></div>

实际上引用的大小取决于虚拟机的实现者， 只能保证任意一个虚拟机中所有的引用大小都一样。  
数组就像是一个杯架， 其中的每个元素是变量（可以位基础类型或者引用）， 数组本身也是一个对象， 一旦被声明出来只能装载所声明的类型的元素。 <div align=center><img src="https://i.imgur.com/aiIqLQv.png"/></div>

## Chap 04: 对象的行为
对象的状态影响行为，同时行为也会影响状态。例如我们可以给在函数调用的参数中使用实例变量
```Java
class Song{
    String title;
    String artist;
    void play(){
        soundPlayer.playSound(title);
    }
}
class Dog{
    int size;
    String name;
    void bark(){
        if(size >= 60){
            //...
        }
        else if{
            //...
        }
    }
}
```
* 方法中可能会运用形参， 调用时有时候需要传入实参， 实参传入方法后就成了形参， 跟局部变量是一样的。 
* 方法何以有返回值
* 可以向方法中传入多个参数， 但是个数和类型以及顺序要和声明的一致
* Java 是通过值传递， 就是拷贝传递

在处理数据的时候需要注意封装， 就是将实例变量设置为 private, 然后提供 public 的 getter 和 setter 提供对它的访问和修改。   
在 java 中注意局部变量和实例变量的区别：
* 实例变量声明在类内
* 局部变量声明在方法中
* ==局部变量没有默认值， 适用之前必须初始化==
* 实例变量的默认值， 根据类型不同可能为 flase/ 0/ null

## Chap 5: 编写程序 
我们记得以面向对象的方式来思考： ==专注于程序中出现的事物而不是过程==。我们在开发类的过程中遵循下面的流程：
* 找出类应该做的事情
* 列出实例变量和方法
* 编写方法的伪码
* 编写方法的测试用程序
* 实现类
* 测试方法
* 除错或者重新设计

其中的伪码大致包括三个部分：实例变量的声明， 方法的声明和方法的逻辑。 此外， 我们会在所有东西都可以测试之前写出测试用的部分。

## Chap 6: 使用 Java 函数库
java 内置有数百个类， 熟悉其中的功能在很大程度上可以避免重新发明轮子。 
### ArrayList
ArrayList 和普通数组的比较
* 一般数组在创建的时候就必须确定大小
* 在将对象存放到普通数组中时候需要制定存放位置 
* 一般数组用的时候有特殊语法，如`commonArray[i]`， 而 ArrayList 使用时候就是对象调用方法`someArrayList.get(5)`
* Java 5.0 之后的 ArrayList 是参数化的

在使用到 Java 代码库中的类时， 我们有两种方式明确适用的是哪一个
* import, 在源文件最前面 `import java.util.ArrayList;`
* 使用全名， `java.util.ArrayList<Dog> dogs = new java.util.ArrayList<Dog>()`

需要注意的是 import 只是帮助我们减少了类前面的名字， 程序并不会因此变大或者变慢。 而 `java.lang` 是一个预先被引用的包， 可以不必指定名称。  
至于其他更多的 API, 可以在参考书或者[网页](https://docs.oracle.com/javase/8/docs/api/)上查询。

## Chap 7: 继承与多态
规划程序的时候需要考虑到未来。 在设计继承的时候， 我们把共同的程序代码放在某个类中， 然后告诉其他类这个类是它们的父类， 这些子类就继承了父类的 public成员（包括实例变量和方法， 在 C++ 中被称为数据成员和成员函数）。   
在我们设计继承时候**需要考虑防止子类出现重复的程序代码**，还需要寻找使用共同的子类来找出更多抽象化的机会， 在层次结构中， 通过对象的引用调用方法时， 在继承层次树最下方的版本会胜利。  
继承应该是 is-a 关系， 而不是 has-a(Liskov 替换原则)， 并且这种替换关系可以沿着继承链传递。 继承关系不能反过来， 如父类对象想要调用子类的方法， 但是 `super` 关键字可以让子类调用父类的方法。一般而言我们在下面两种情况下使用继承
* 子类比其父类更具有特定的意义时
* 行为程序应该被多个相同基本类型的类所共享时

在继承中， 如果共同的部分需要修改， 新父类如果没有破坏到子类，它编译后， 子类不需要重新编译就能运用带新版本的父类。 
* 避免了重复的程序代码
* 定义出统统的协议

### 多态
* 引用与对象可以是不同的类型， 引用类型可以是实际对象类型的父类
* 参数和返回值类型也可以多态

我们可以通过下列方法阻止一个类被继承
* 存取控制， Classes in a different package won’t be able to subclass (or even use, for that matter) the non-public class.
* 使用 final 修饰符
* 让类只拥有 private 的构造程序

方法就是合约的标志， 在继承体系中覆盖父类的方法遵循下面的规则
* 参数必须相同并且返回类型需要兼容
* 不能降低方法的存取权限

和 overriding 名称类似的是 overloading, 但是实际含义完全不同， 它相当于是定义了名字相同的不同函数
* overloading 可以有不同的返回类型
* 但是不能只改变返回类型
* 可以更改存取权限

## Chap 8: 接口与抽象类
接口是 100% 的抽象类， 抽象类就是无法初始化的类。 适用的时候只需要在类的面前加入关键字 abstract:
```Java
abstract class Canine extend Animal{
    public void roam(){}
}
```
我们无法创建抽象类的实例， 但是可以使用抽象类来声明引用类型在多态中使用。 **抽象类除了被继承外， 是没有用途， 没有值， 没有目的**（抽象类可以有 static 成员）。 