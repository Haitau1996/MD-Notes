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

每个单元测试和其他单元测试都必须是独立的, 如果单元测试之间是以特定的顺序执行, 拿将是致命的. 当离开一个单元测试的时候, 不应该改变测试单元的状态. 

#### 一个测试一个断言
一个测试中多个断言带来的问题是显而易见的: