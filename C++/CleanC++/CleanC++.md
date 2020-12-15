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

如果还没有开始使用 C++11 的更高版本, 请立即开始使用.

### 良好的命名
源代码文件,命名空间, 类, 模板, 函数, 参数,变量和常量, 都应该有具体并且富有表现力的名称, 我们看一个反面的例子:
```C++
SbxInfo* ProcessWrapper::GetInfo( short nIdx ){
    Methods* p = &pMethods[ nIdx ];
    // Wenn mal eine Hilfedatei zur Verfuegung steht:
    // SbxInfo* pResultInfo = new SbxInfo( Hilfedateiname, p->nHelpId );
    SbxInfo* pResultInfo = new SbxInfo;
    short nPar = p->nArgs & _ARGSMASK;
    for( short i = 0; i < nPar; i++ ){
        p++;
        String aMethodName( p->pName, RTL_TEXTENCODING_ASCII_US );
        sal_uInt16 nInfoFlags = ( p->nArgs >> 8 ) & 0x03;
        if( p->nArgs & _OPT )
        nInfoFlags |= SBX_OPTIONAL;
        pResultInfo->AddParam( aMethodName, p->eType, nInfoFlags );
    }
    return pResultInfo;
}
```
这段代码一个主要的问题就是命名不佳, 如函数名 GetInfo() 非常抽象, 命名空间 `ProcessWrapper` 也不是很有帮助, 如果这个函数提取正在运行的进程信息, `RetrieveProcessInformation()` 就是个很好的名字, 此外函数还用 new 运算符创建新的东西, 为了强调这个事实, `CreateProcessInformation()` 也是一个更好的选择. 此外, 这个函数的参数和返回值都相对较为抽象.<br>
#### 名称应该自注释
所谓自注释代码, 就是不需要注释和解释其用途的代码, 这种自注释代码要求为它的命名空间/类/变量/常量和函数提供自我解释性质的命名. 
```C++
// 没有意义的命名例子
unsigned int num;
bool flag;
std::vector<Customer> list;
Product data;
// 对他们的一些改进
unsigned int numberOfArticles;
bool isChanged;
std::vector<Customer> customers;
Product orderedProduct;
// 太冗长的命名也是不恰当的
totalNumberOfCustomerEntriesWithMangledAddressInformation =
        amountOfCustomerEntriesWithIncompleteOrMissingZipCode +
        amountOfCustomerEntriesWithoutCityInformation +
        amountOfCustomerEntriesWithoutStreetInformation;
```

#### 使用域中的名称
领域驱动设计(Domain-Driven Design) 试图将业务领域中的事务和概念映射到代码中, 使你的软件称为一个真实系统的模型. 这可以促进开发人员与其他利益相关人员之间的沟通和交流. 如一个汽车租赁使用情况的类可以这样设计:
```C++
class ReserveCarUseCaseController {
public:
    Customer identifyCustomer(const UniqueIdentifier& customerId);
    CarList getListOfAvailableCars(const Station& atStation, 
                                   const RentalPeriod&  desiredRentalPeriod) const;
    ConfirmationOfReservation reserveCar(const UniqueIdentifier& carId, 
                                         const RentalPeriod& rentalPeriod) const;
private:
    Customer& inquiringCustomer;
};
```
**选择适当抽象层次的名称**:
为了控制软件系统的复杂性, 这些系统通常是分层的, 将整个问题分解为子任务, 直到软件开发人员确信他们能够处理这些较小部分, 每当我们在结构层次深入一层的时候, 元素的名称就会变得更加具体.<br>
**避免冗余的名称**:
例如将类型的名称放到成员变量名, 就是多余的, 如:
```C++
class Movie {
private:
    std::string movieTitle;// 一个类的成员, 没有必要加上类名
    std::string stringTitle;// 没有必要名字加上 String ,这相当于是小的自我重复
    // ...
};
```
**避免晦涩难懂的缩写**:
这会明显降低代码的可读性, 变量也应该容易发音. 如:
```C++
std::size_t idx; // Bad!
std::size_t index; // Good; might be sufficient in some cases
std::size_t customerIndex; // To be preferred, especially in situations where several objects are indexed
Car ctw; // Bad!
Car carToWash; // Good
```
**避免匈牙利命名和命名前缀**:
使用匈牙利命名法时, 变量的类型被用作该变量命名的前缀. 这种做法其实会带来一些问题, 例如:
* 我们可能在开发期间改变了变量的类型, 但是并没有将变量的前缀修正过来
* 在支持多态的面向对象程序设计语言中, 前缀不能轻易指定, 或者前缀会令人费解

**避免相同的名称用于不同的目的**

### 注释
比较专业的代码必须有适当的注释, 其他开发人员或者新团队成员可以轻松地理解代码的意图. <br>
**代码应该像是讲故事一样**:能够自我解释, 且尽可能避免注释. <br>
**不要为易懂的代码写注释**: 无用的代码注释本身就是代码的一种重复, 还有可能带来的问题就是代码已经被修改了, 但是相应的注释并没有被完全修改. 例如:
```C++
CustomerAccount* account = customer->getAccount(); // Retrieve the customer's account(这是很显然的)
account->setLoyaltyDiscountInPercent(discount); // Grant a 10% discount(这里代码和注释就不同了)
```
**不要通过注释禁用代码**:这除了增加代码的混乱程度外没有带来实际意义的好处. **除了快速进行测试之外, 不要通过注释禁用代码, 同时还需要有一个版本控制系统**.<br>
**不要写块注释**: 这种注释纯粹是引入混乱, 应该立即删除. 我们无需添加版权注释或者进行其他操作即可对自己的作品拥有版权, 如果要添加, 可以将它们放到独立的文件中. 此外, 不要使用注释代替版本控制, `git log --[filename]` 可以轻松查看到修改的历史记录.<br>
**特殊情况的注释是有用的**: 如果一段代码具有高度的内在复杂性, 以至于没有深入专业知识的人无法轻易理解, 那么注释就是合理的. 这时候可能故意偏离了良好的设计原则, 如违背了DRY. 下面是针对不可避免添加的注释的建议:
* 确保你的注释为代码增加了价值
* 应该解释为什么这样做, 而不是怎样做的
* 尽量做到注释尽可能短而且富有表现力

#### 从源代码生成文档
Doxygen 可以从文档生成提取文档, 为了获得有意义的文档, 代码必须以特定的注释格式进行注释. 如果不得不用这种风格注释, 建议**只关注公共API的部分**, 对于其他部分(如内部使用的模块或者私有功能), 不建议配备 Doxygen 注释.
```C++
//! Each customer must have an account, so bookings can be made. The account
//! is also necessary for the creation of monthly invoices.
//! @ingroup entities
//! @ingroup accounting
class CustomerAccount {
    // ...
    //! Regular customers occasionally receive a regular discount on their
    //! purchases.
    void grantDiscount(const PercentageValue& discount);
    // ...
};
```
这时候, 我们就可以用 `const PercentageValue` 替代 `unsigned short`,让参数可以自我解释.

### 函数
函数代表代码之上的第一个组织单位, 编写良好的函数可以显著提高程序的可读性和可维护性. 我们在设计优秀的软件时应该考虑下面的因素:<br>
**只做一件事情**: 一个函数, 必须定义一个非常清晰的任务或者功能,它应该用它的函数签名来表示. 下面是一些判断函数做了太多事情的标志:
* 函数体量比较大
* 如果试图为函数找到一个有意义的词语描述函数的功能的时候, 名字中无法避免使用连词(_and/or_)
* 圈复杂度比较高(包含太多的 if-else 或者 switch)
* 函数的输入参数比较多

**让函数尽可能小**: 函数应该尽可能小, 理想情况下是 5-6 行, 最多 12~15 行. 大函数使用起来, 通常有比较高的复杂性, 难以看出函数的作用, 此外也容易拥有比较多的职责. 当然, 如果一个包含单个 switch 语句的函数极其整洁也容易理解, 这也是可以接受的. 现代的 C++ 编译器已经非常擅长优化, 我们应该将时间花在真正地性能问题上(通常源于糟糕的结构和设计), 不用过于担心函数调用的开销.<br>
**函数命名**: 函数定义了程序的行为, 其名称都有动词, 一般都用动词开头. 如果用于提供信息, 通常用 is/has 开头, 下面是一些良好命名函数的例子:
```C++
void CustomerAccount::grantDiscount(DiscountValue discount);
void Subject::attachObserver(const Observer& observer);
void Subject::notifyAllObservers() const;
int Bottling::getTotalAmountOfFilledBottles() const;
bool AutomaticDoor::isOpen() const;
```