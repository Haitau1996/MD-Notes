# Ｃ++代码整洁之道：C++17可持续软件开发者模式实践

## Chap 1: 简介

很多软件项目的开发形势十分严峻, 而起原因并不是单纯的技术以原因, 其代码可能 **外部质量** 很高, 能够通过质量保证部门的黑盒测试/用户测试以及验收测试, 真正地问题在于能够正常工作的这种代码 **内部质量** 很低, 可读性不高, 维护扩展困难. <br>
软件会随着时间的推移而变得混乱, 其中一个和语言相关的原因是 C++ 是一个中等层次的多范式编程语言, 包含了高级语言和低级语言的特点. 另一个原因是很多程序开发者并没有 IT 背景.<br>
整洁的代码是容易被任何团队的成员理解和维护的, 它是高效工作的基础. 

## Chap 2: 构建安全体系

在过去几年中, 测试已经成为衡量软件质量好坏的一个重要指标, 一个好的测试策略所带来的好处是巨大的, 软件测试是 QA 的一个组成部分.不同级别的质量保证通常用金字塔形式表述:<br>
![](figure/2.1.jpg)<br>
这意味着我们要做更多的低层次单元测试, 测试和维护的成本总是朝着金字塔顶端增长, 例如一个自动化的 UI 测试是很难编写的, 他们也难以检查系统中所有可能的执行情况,系统级别的测试失败了, 也难以找到错误的确切原因.<br>
QA是安全体系的第二道防线, 他们会检查以前的质量保证措施是否充分有效, 在此之前开发人员应该要做充分的测试.<br>
**单元测试**: 在特定上下文环境中,单元测试能执行产品的一部分代码, 在很短时间内展示代码是否达到了预期运行的结果.
* 单元测试阶段修复 bug 的代价相对低
* 得到关于整个代码库(有测试那部分) 的及时反馈
* 让开发人员有信心重构代码, **没有单元测试的重构是非常危险的**
* 高覆盖率的单元测试, 可以防止陷入耗时和让人手足无措的代码调试中
* 可以促进实现整洁且良好的接口

### 良好的单元测试原则
高质量要求产品代码的同时, 也要高质量要求单元测试的代码, 理论上产品代码和测试代码之间不应该有任何区别.
#### 单元测试代码的命名
测试失败后, 开发人员希望得到以下信息:
* 测试单元名称, 谁的单元测试失败了
* 单元测试测了什么, 环境是怎样的(测试场景)
* 预期的结果是什么, 时代的实际结果是什么

因此一个直观而且容易理解的名称应该包含三点:
* 测试的前置条件,执行单元测试前 被测试系统(SUT,System Under Test) 的状态
* 被单元测试的部分(通肠胃过程,函数或者方法的名称)
* 单元测试的结果
    ```C++
    // 建议的名称模板如下
    <PreconditionAndStateOfUnitUnderTest>_<TestedPartOfAPI>_<ExpectedBehavior>
    // 例子 
    void CustomerCacheTest::cacheIsEmpty_addElement_sizeIsOne();
    void CustomerCacheTest::cacheContainsOneElement_removeElement_sizeIsZero();
    void ComplexNumberCalculatorTest::givenTwoComplexNumbers_add_Works();
    void MoneyTest:: givenTwoMoneyObjectsWithDifferentBalance_theInequalityComparison_Works();
    void MoneyTest::createMoneyObjectWithParameter_getBalanceAsString_returnsCorrectString();
    void InvoiceTest::invoiceIsReadyForAccounting_getInvoiceDate_returnsToday();
    ```

另一种直观且容易理解的测试名称, 是直接面熟特定的需求,如:
```C++
void UserAccountTest::creatingNewAccountWithExistingEmailAddressThrowsException();
void ChessEngineTest::aPawnCanNotMoveBackwards();
void ChessEngineTest::aCastlingIsNotAllowedIfInvolvedKingHasBeenMovedBefore();
void ChessEngineTest::aCastlingIsNotAllowedIfInvolvedRookHasBeenMovedBefore();
void HeaterControlTest::ifWaterTemperatureIsGreaterThan92DegTurnHeaterOff();
void BookInventoryTest::aBookThatIsInTheInventoryCanBeBorrowedByAuthorizedPeople();
void BookInventoryTest::aBookThatIsAlreadyBorrowedCanNotBeBorrowedTwice();
```

#### 单元测试的独立性

每个单元测试和其他单元测试都必须是独立的, 如果单元测试之间是以特定的顺序执行, 拿将是致命的. 当离开一个单元测试的时候, 不应该改变测试单元的状态. 同时需要注意, 测试环境也是要独立初始化, 每个单元测试都必须是应用程序的一个独立可运行的实例: 完全自行设置和初始化所需要的环境, 执行单元测试后的清理工作.

#### 一个测试一个断言
一个测试中多个断言带来的问题是显而易见的:
* 开发人员难以快速找到错误的原因
* 我们应该用精确而且富有表现力的方式命名测试, 但是很难为多个测试的单元找到合适的名字
    ```C++
    // 一个测试用一个断言
    void MoneyTest::givenTwoMoneyObjectsWithDifferentBalance_theInequalityComparison_Works(){ 
        const Money m1(-4000.0); 
        const Money m2(2000.0); 
        ASSERT_TRUE(m1 != m2); 
    }
    // 一个测试里面多个断言, 不是好的范例
    void MoneyTest::givenTwoMoneyObjectsWithDifferentBalance_testAllComparisonOperators() {
        const Money m1(-4000.0);
        const Money m2(2000.0);
        ASSERT_TRUE(m1 != m2);
        ASSERT_FALSE(m1 == m2);
        ASSERT_TRUE(m1 < m2);
        ASSERT_FALSE(m1 > m2);
        // ...more assertions here...
    }
    ```
#### 一些提醒 

* **不到对 getters 和 setters 做单元测试**: 测试常见且简单的单元 _getters / setters_ 是没有必要的
* **不要对第三方代码做单元测试**: 我们预设第三方库都是有自己的单元测试, 在自己的项目中, 不要使用那些没有单元测试或者质量可疑的库或者框架
* 不要对外部系统做单元测试: 需要注意的是这不是你的责任, 测试自己的代码而不是别人的
* 处理数据库的访问: **能不使用数据库进行单元测试, 就不使用**, 单元测试为了不同的目的而共享数据库, 对独立性有影响,此外数据库的访问可能使得测试需要多花费几分钟的时间(解决的办法是模拟数据库, 也不要访问磁盘网络)
* **不要混淆测试代码和产品代码**,上面这个例子将用一个布尔值确认是否进入测试代码, 测试时用的替身(FackDAOForTest) 替换 DAO(Data Access Object), 带来两个问题:
    - 增加产品复杂度并且降低代码的可读性
    - 测试类也变成了系统的一部分, 被部署到了生产环境中
    ```C++
    #include <memory>
    #include "DataAccessObject.h"
    #include "CustomerDAO.h"
    #include "FakeDAOForTest.h"
    using DataAccessObjectPtr = std::unique_ptr<DataAccessObject>;
    class Customer {
    public:
        Customer() {}
        explicit Customer(bool testMode) : inTestMode(testMode) {}
        void save() {
        DataAccessObjectPtr dataAccessObject = getDataAccessObject();
        // ...use dataAccessObject to save this customer...
        };
    private:
        DataAccessObjectPtr getDataAccessObject() const {
            if (inTestMode) {
                return std::make_unique<FakeDAOForTest>();
            } else {
                return std::make_unique<CustomerDAO>();
            }
        }
        // ...more operations here...
        bool inTestMode{ false };
        // ...more attributes here...
    };
    ```
    解决办法有很多种, 如在 save 中注入特定的DAO作为一个参数:
    ```C++
    class DataAccessObject;
    class Customer {
    public:
        void save(DataAccessObject& dataAccessObject) {
        // ...use dataAccessObject to save this customer...
        }
    }
    ```
    可以在构造实例的时候完成, 保存一个 DAO 对象的引用, 然后用特定 DAO 构造对象, 同时禁止默认构造函数:
    ```C++
    class DataAccessObject;
    class Customer {
    public:
        Customer() = delete;
        Customer(DataAccessObject& dataAccessObject) : dataAccessObject(dataAccessObject) {}
        void save() {
        // ...use member dataAccessObject to save this customer...
        }
        // ...
    private:
        DataAccessObject& dataAccessObject;
        // ...
    };
    ```
    第三种方式是用一个 Customer 已知的工厂函数创建, 测试环境中可以外部配置工厂创建需要的DAO对象, 这三种做法的目的在于: **Customer 与特定的 DAO 没有依赖关系**.
* **测试必须快速执行** : 单元测试必须为开发者建立一套快速反馈机制. 

#### 测试替身
要测试的单元与其他模块或者外部系统的依赖性应该被**测试替身**(Test DOubles) 替换, 尽量达到被测试单元之间的松耦合, 例如我们写的程序用外部的 Web 服务进行货币转换, 单元测试期间, 外部服务不可得.就必须在代码中间引入一个可变点(通常使用一个接口达到目的, C++ 中形式就是一个仅包含 纯虚成员函数的抽象类), 得益于接口这种抽象, 客户代码在运行时对于不同的实现都很 transparent:
```C++
class CurrencyConverter {
public:
    virtual ~CurrencyConverter() { }
    virtual long double getConversionFactor() const = 0;
};
// Internet 访问被封装在此
class RealtimeCurrencyConversionService : public CurrencyConverter {
public:
    virtual long double getConversionFactor() const override;
    // ...more members here that are required to access the service...
};
// 使用一个测试替身
class CurrencyConversionServiceMock : public CurrencyConverter {
public:
    virtual long double getConversionFactor() const override {
        return conversionFactor;
    }
    void setConversionFactor(const long double value) {
        conversionFactor = value;
    }
private:
    long double conversionFactor{0.5};
};
```
![](figure/2.2.jpg)<br>

## Chap 3: 原则
原则是一种规则或者指引你的观点, 并不是不可改变的法律, 有时编程必须故意违背其中一些原则, 只要我们有充分的理由, 但是这么做的时候要小心.
* 保持简单和直接: 避免做一些没有必要的复杂性工作, 即使很多时候内在的复杂性不可避免.
* 不需要原则: 总是在真正需要的时候才去实现它们, 确定真正需要的时候再写代码, 那时候重构依旧来得及
* 避免复制原则: 复制和粘贴是一个设计错误, **在一个系统内部, 任何一个知识点都必须有一个单一的/明确的/权威的描述**(知识点包括文档,测试计划和系统配置信息)
* 信息隐藏原则: 一个代码调用了另一段代码, 那么调用者不应该知道被调用者的内部实现.(和封装有一点点区别, 信息隐藏是帮助开发人员找到好的设计模式的原则, 封装是依赖于编程语言的技术, 用于限制对模块内部的访问, 封装有助于但并不保证信息隐藏)
* 高内聚原则: 当模块实现定义确切的功能时, 应该具有高内聚的特性,例如下面的例子中, 想要单独使用功能A/B/C 的时候都需要依赖整个 MyModule 模块, 这会导致太多的依赖, 可维护性也会相应降低:<br>
    ![](figure/3.1.jpg)<br>
    这个为了提高内聚性, 功能 A/B/C 应该彼此分离:<br>
    ![](figure/3.2.jpg)<br>
* 松耦合原则: 我们构建的系统中, 每个模块应该很少使用或者不知道其他独立模块的定义. 如我们给一个灯设计开关, 如果开关依赖具体的类会是一个非常糟糕的设计:<br>
    ![](figure/3.3.jpg)<br>
    改进的办法也很简单, 关键是使用接口, 接口类似于一种契约, 声明类的公共行为, 而不涉及该类的具体实现.松耦合可以为系统的各个独立模块提供高的自洽性, 一般而言高内聚会促进松耦合.<br>
    ![](figure/3.4.jpg)<br>
* 最小优化原则: 只要没有明确的性能要求, 就避免优化. 必要的时候可以使用 Profiler 这类动态程序分析工具
* 最小惊讶原则: 不应该让用户对用户界面的意外响应而感到惊讶
* 童子军原则: 离开营地的时候, 应该让露营比你来的时候更干净. 一旦发现了容易引起混乱的东西, 应该立即处理.

## Chap 4: C++ 整洁代码的基本规范
