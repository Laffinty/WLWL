# WLWL

一个实验性编程语言的早期实现，使用前缀函数调用语法。

## 快速开始

### 编译
```bash
build.bat
```

### 运行
```bash
# 交互模式
build/wlwl.exe

# 执行文件
build/wlwl.exe test.wl
```

## 基础语法

```wlwl
// 变量声明
LET(x, 42);        // 不可变
VAR(y, 10);        // 可变
SET(y, 20);        // 重赋值

// 函数调用
+(1, 2, 3);        // 结果: 6
*(3, 4);           // 结果: 12
=(5, 5);           // 结果: TRUE
>(10, 3);          // 结果: TRUE

// 字符串操作
CONCAT("Hello, ", "World!");
PRINT("Hello from WLWL!");

// 条件语句
IF(>(x, 0), PRINT("Positive"), PRINT("Not positive"));

// 多分支条件
COND([>(x, 0), "positive"], [<(x, 0), "negative"], [TRUE, "zero"]);

// 循环语句
WHILE(<(i, 5), (
    PRINT("i =", i);
    SET(i, +(i, 1));
));

FOR(VAR(j, 0), <(j, 3), SET(j, +(j, 1)), (
    PRINT("j =", j);
));

FOREACH(item, [1, 2, 3], (
    PRINT("item =", item);
));

// 循环控制
WHILE(<(k, 10), (
    IF(=(k, 5), BREAK);
    IF(=(%(k, 2), 0), CONTINUE);
    PRINT("k =", k);
    SET(k, +(k, 1));
));
```

## 示例

创建 `test.wl`:
```wlwl
LET(pi, 3.14159);
LET(area, *(pi, *(5, 5)));
PRINT(CONCAT("Circle area: ", area));

IF(>(area, 50), 
   PRINT("Large circle"), 
   PRINT("Small circle"));

// 循环示例
VAR(count, 0);
WHILE(<(count, 5), (
    PRINT("Count:", count);
    SET(count, +(count, 1));
));
```

执行：
```bash
build/wlwl.exe test.wl
```

## 开发进度

✅ 已完成：
- 词法分析 (Lexer)
- 语法分析 (Parser) 
- 基础数据类型 (Number, String, Boolean, Null)
- 变量系统 (LET, VAR, SET)
- 算术运算 (+, -, *, /, %, 支持多参数)
- 比较运算 (=, !=, >, <, >=, <=)
- 条件语句 (IF, COND)
- **循环结构 (WHILE, FOR, FOREACH)** ⭐️ 新增
- **循环控制 (BREAK, CONTINUE)** ⭐️ 新增
- **语句块 `(stmt1; stmt2; ...)`** ⭐️ 新增
- 字符串操作 (CONCAT)
- 基础I/O (PRINT)
- 运行时环境和求值器
- 内置函数系统

🔄 开发中：
- 用户函数定义 (FUN)
- 数组和对象类型
- 面向对象特性

📋 计划中：
- 类定义和继承 (CLASS, NEW, SUPER)
- Web集成功能 (HTML, CSS, DOM)
- 模块系统 (IMPORT, EXPORT)
- 异常处理 (TRY, CATCH)
- 标准库完善

## 构建要求

- GCC (通过Cygwin或MinGW)
- Windows 环境

## 许可证

GPL-3.0