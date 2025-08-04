## 1. 项目介绍

WLWL是一款实验性的、以函数式为核心设计原则的编程语言，现已扩展支持面向对象编程特性。它的目标是构建一种语法结构清晰，可以无缝编译为Web标准（HTML/CSS/JavaScript）的语言。

### 核心设计原则
* **函数式优先**：所有操作（包括算术、逻辑和声明）都使用前缀函数调用形式，例如 `+(1, 2)`。
* **面向对象增强**：在保持函数式核心的基础上，提供类、继承、封装等OOP特性。
* **一切皆表达式**：所有语句都有返回值，包括类定义。
* **AI 可读性**：清晰、无歧义的语法，为AI辅助编程和代码生成而优化。
* **Web 友好**：原生支持HTML、CSS和DOM操作的语法，以及组件化开发。
* **语法严格性**：每个语句必须以分号 `;` 结尾，且不依赖缩进区分代码块。
* **统一性保证**：所有面向对象特性都以函数调用形式实现，保持语法一致性。

### 编程范式支持
- **函数式编程**：不可变数据、纯函数、高阶函数
- **面向对象编程**：类、继承、封装、多态
- **组件化编程**：Web组件、事件处理、响应式编程
- **混合范式**：可以在同一项目中灵活组合使用

本项目是该语言的一个早期C语言实现，包含了一个交互式解释器（REPL），用于执行基本的WLWL代码。

## 2. 编译与测试

目前需要用cygwin安装gcc-core执行编译。

### 编译步骤

1.  **编译**：
    * 打开命令行，进入项目根目录。
    * 执行以下命令：
        ```bash
        build.bat
        ```
    * 该脚本会创建一个 `build` 目录，并将编译后的可执行文件 `wlwl.exe` 放置其中。

2.  **调试模式编译**：
    * 如果需要编译为调试模式（会输出更多过程信息），请使用以下命令：
        ```bash
        build.bat debug
        ```

### 运行与测试

编译成功后，可以通过以下方式运行WLWL程序：

#### 1. 交互式REPL模式
启动 **REPL (Read-Eval-Print-Loop)** 环境：

```bash
build/wlwl.exe
```

启动后，您可以在 `>>` 提示符后输入WLWL代码并立即看到结果。

#### 2. 源文件执行模式
执行 `.wl` 源文件：

```bash
build/wlwl.exe 文件名.wl
```

**注意**：文件必须具有 `.wl` 扩展名。

### 基础功能测试示例

#### REPL中的基础测试
在REPL中输入以下代码（每行以回车结束）：
```wlwl
>> +(1, 2, 3);
Result: 6
>> -(10, 4);
Result: 6
>> *(3, 4);
Result: 12
>> /(10, 2);
Result: 5
>> CONCAT("Hello, ", "World!");
Result: "Hello, World!"
>> CONCAT("Number: ", 42);
Result: "Number: 42"
>> PRINT("Hello from WLWL!");
"Hello from WLWL!"
Result: NULL
>> LET(x, 42);
Result: 42
>> VAR(y, 10);
Result: 10
>> x;
Result: 42
>> SET(y, 20);
Result: 20
>> y;
Result: 20
>> =(5, 5);
Result: TRUE
>> >(10, 3);
Result: TRUE
```

#### 创建和运行示例文件
创建一个名为 `test.wl` 的文件，内容如下：

```wlwl
// 基础计算演示
PRINT("=== WLWL Language Demo ===");

// 变量声明
LET(pi, 3.14159);
VAR(counter, 1);

// 基础算术
LET(area, *(pi, *(5, 5)));
PRINT(CONCAT("Circle area (r=5): ", area));

// 字符串处理和类型自动转换
PRINT(CONCAT("Counter value: ", counter));

// 嵌套函数调用
LET(complex_calc, +(*(3, 4), /(10, 2)));
PRINT(CONCAT("3*4 + 10/2 = ", complex_calc));

// 比较操作
LET(is_equal, =(5, 5));
LET(is_greater, >(10, 3));
PRINT(CONCAT("5 equals 5: ", is_equal));
PRINT(CONCAT("10 > 3: ", is_greater));

// 可变变量修改
SET(counter, +(counter, 1));
PRINT(CONCAT("Counter after increment: ", counter));

PRINT("=== Demo completed ===");
```

然后执行：
```bash
build/wlwl.exe test.wl
```

#### Hello World示例运行结果

根据提供的 `helloworld.wl` 文件，正常的运行输出应该是：

```
"Hello, WLWL World!"
"Welcome to WLWL language"
"This is execution number: 1"
"Math result: 3*4 + 10/2 = 17"
"Hello, from, WLWL!"
"5 equals 5: TRUE"
"10 greater than 3: TRUE"
"Counter after increment: 2"
"Program execution completed successfully!"
```

### 面向对象功能预览
```wlwl
// 注意：以下功能在当前版本中尚未实现，仅展示语法设计
>> CLASS(Person, NULL, ["name": NULL, "greet": FUN((self), CONCAT("Hello, ", GET_PROP(self, "name")))]);
>> LET(person, NEW(Person));
>> SET_PROP(person, "name", "Alice");
>> greet(person);
Result: "Hello, Alice"
```

## 3. 功能实现对照表

下表详细列出了 `WLWL设计规范.md` 中定义的各项功能及其在当前C语言实现中的完成状态。

| 模块 | 功能点 | 规范定义 | 当前实现状态 | OOP扩展状态 |
| :--- | :--- | :--- | :--- | :--- |
| **核心语法** | **语句终结符** | 每个语句必须以分号 `;` 结尾。 | ✅ **已实现** | ✅ **兼容** |
| | **前缀表达式** | 所有操作均为函数调用 `FUNC(...)`。 | ✅ **已实现** | ✅ **保持一致** |
| | **注释支持** | 单行 `//` 和多行 `/* */` 注释 | ✅ **已实现** | ✅ **兼容** |
| **数据类型** | **基础类型** | Number, String, Boolean, Null。 | ✅ **已实现** | ✅ **兼容** |
| | **复合类型** | Array, Object, Function。 | ❌ **未实现** | 🔄 **规划中** |
| | **类类型** | Class, Instance | ❌ **未实现** | 📋 **新增规划** |
| **变量系统** | **不可变变量 `LET`** | `LET(name, value);` | ✅ **已实现** | ✅ **兼容** |
| | **可变变量 `VAR`** | `VAR(name, value);` | ✅ **已实现** | ✅ **兼容** |
| | **重新赋值 `SET`** | `SET(name, new_value);` | ✅ **已实现** | ✅ **兼容** |
| | **作用域管理** | 嵌套作用域和变量查找 | ✅ **已实现** | ✅ **兼容** |
| **运算符** | **算术运算符 `+ - * /`** | `+(a, b, ...)` 等形式。 | ✅ **已实现** | ✅ **兼容** |
| | **取模 `%` 和幂 `^`** | `%(a, b)` 和 `^(a, b)` | ❌ **未实现** | 🔄 **规划中** |
| | **比较运算符 `= >`** | `=(a, b)` 和 `>(a, b)` | ✅ **已实现** | ✅ **兼容** |
| | **比较运算符 `!= < >= <=`** | `!=(a,b)` 等 | ❌ **未实现** | 🔄 **规划中** |
| | **逻辑运算符 `AND OR NOT`** | `AND(a, b)` 等 | ❌ **未实现** | 🔄 **规划中** |
| | **字符串运算符 `CONCAT`** | `CONCAT(s1, s2, ...)` 支持自动类型转换 | ✅ **已实现** | ✅ **兼容** |
| | **字符串运算符 `SLICE`, `LENGTH`** | `SLICE(...)`, `LENGTH(...)` | ❌ **未实现** | 🔄 **规划中** |
| **控制流** | **条件语句 `IF`, `COND`** | `IF(cond, true_branch, ...)` | ❌ **未实现** | 🔄 **规划中** |
| | **循环语句 `WHILE`, `FOR`** | `WHILE(cond, body)` 等 | ❌ **未实现** | 🔄 **规划中** |
| **函数** | **函数定义 `FUN`** | `FUN(name(params...), (body))` | ❌ **未实现** | 🔄 **规划中** |
| | **函数调用** | `func_name(args...)` | ✅ **已实现** (仅限内置函数) | ✅ **兼容** |
| | **匿名函数 `LAMBDA`** | `LAMBDA((params...), (body))` | ❌ **未实现** | 🔄 **规划中** |
| **面向对象** | **类定义 `CLASS`** | `CLASS(name, parent, body)` | ❌ **未实现** | 📋 **新功能** |
| | **实例创建 `NEW`** | `NEW(class, args...)` | ❌ **未实现** | 📋 **新功能** |
| | **属性操作** | `GET_PROP()`, `SET_PROP()` | ❌ **未实现** | 📋 **新功能** |
| | **方法调用** | `CALL_METHOD()`, `method_name()` | ❌ **未实现** | 📋 **新功能** |
| | **继承系统** | `SUPER_CALL()`, `IS_INSTANCE()` | ❌ **未实现** | 📋 **新功能** |
| | **封装特性** | 私有属性/方法，访问控制 | ❌ **未实现** | 📋 **新功能** |
| **Web API** | **HTML/CSS/DOM 系统** | `HTML(...)`, `CSS(...)`, `DOM.QUERY(...)` 等 | ❌ **未实现** | 🔄 **规划中** |
| | **组件系统** | 基于类的UI组件 | ❌ **未实现** | 📋 **新功能** |
| | **事件处理** | 对象化事件系统 | ❌ **未实现** | 📋 **新功能** |
| **标准库** | **输入输出 `PRINT`** | `PRINT(value)` | ✅ **已实现** | ✅ **兼容** |
| | **输入输出 `INPUT`** | `INPUT(prompt)` | ❌ **未实现** | 🔄 **规划中** |
| | **集合类库** | `List`, `Dict`, `Set` 类 | ❌ **未实现** | 📋 **新功能** |
| | **其他标准库 `MATH`, `ARRAY`** | `MATH.ABS(x)` 等 | ❌ **未实现** | 🔄 **规划中** |

**图例说明：**
- ✅ **已实现**：功能已完成并可使用
- ❌ **未实现**：功能尚未开发
- 🔄 **规划中**：原有规划功能，将继续开发
- 📋 **新功能**：面向对象扩展新增功能

## 4. 项目结构概览

```
.
├── build.bat                    # Windows 构建脚本
├── LICENSE                      # 项目许可证 (GPL-3.0)
├── README.md                    # (本文档)
├── helloworld.wl.txt            # Hello World 示例程序
├── src
│   ├── core                     # 核心模块
│   │   ├── common.h            # 公共头文件
│   │   ├── dyn_array.c/h       # 动态数组实现
│   │   └── logger.c/h          # 日志系统
│   ├── lexer                    # 词法分析器
│   │   ├── token.c/h           # Token 定义和操作
│   │   └── lexer.c/h           # 词法分析器实现
│   ├── parser                   # 语法分析器
│   │   ├── ast.c/h             # 抽象语法树
│   │   └── parser.c/h          # 语法分析器实现
│   ├── runtime                  # 运行时系统
│   │   ├── object.c/h          # 对象系统
│   │   ├── environment.c/h     # 环境和作用域
│   │   ├── builtins.c/h        # 内置函数
│   │   └── evaluator.c/h       # 求值器（待整合）
│   ├── main.c                   # 主入口，实现 REPL
│   └── wl.h                     # 项目主头文件
├── WLWL设计规范.md              # 语言的官方设计和宪法文档
└── docs                         # 文档目录（规划中）
    ├── oop-specification.md    # 面向对象编程规范扩展
    └── examples/               # 示例代码
```

## 5. 常见问题与解决方案

### Q: 运行时出现 "argument to 'CONCAT' must be STRING" 错误
**A**: 这个问题已在最新版本中修复。`CONCAT` 函数现在支持自动类型转换，可以直接连接数字、布尔值等类型。请重新编译项目：
```bash
build.bat
```

### Q: 如何正确运行示例文件？
**A**: 确保文件扩展名为 `.wl`，然后使用以下命令：
```bash
# 将 helloworld.wl.txt 重命名为 helloworld.wl
cp helloworld.wl.txt helloworld.wl
build/wlwl.exe helloworld.wl
```

### Q: REPL中如何输入多行代码？
**A**: 当前版本的REPL只支持单行输入。对于复杂的代码，建议创建 `.wl` 文件并使用文件执行模式。

### Q: 为什么某些语法功能还未实现？
**A**: WLWL是一个正在开发中的实验性语言。当前实现了核心的函数式编程特性，面向对象和Web相关特性正在规划开发中。详见功能实现对照表。

## 6. 许可证

本项目采用 GPL-3.0 许可证。详见 [LICENSE](LICENSE) 文件。

---

## 附录：WLWL语言示例

### 函数式编程示例
```wlwl
// 基础计算
LET(result, +(*(3, 4), /(10, 2)));  // result = 17

// 字符串处理（支持自动类型转换）
LET(greeting, CONCAT("Hello, ", "WLWL", "!"));
LET(message, CONCAT("Result is: ", result));
PRINT(greeting);  // 输出: "Hello, WLWL!"
PRINT(message);   // 输出: "Result is: 17"

// 变量操作
VAR(counter, 0);
SET(counter, +(counter, 1));
PRINT(CONCAT("Counter: ", counter));

// 比较操作
LET(is_equal, =(5, 5));           // TRUE
LET(is_greater, >(10, 3));        // TRUE
PRINT(CONCAT("Equal: ", is_equal));
PRINT(CONCAT("Greater: ", is_greater));
```

### 面向对象编程示例（规划中）
```wlwl
// 类定义
CLASS(Calculator, NULL, [
    "result": 0,
    "init": FUN((self, initial), (
        SET_PROP(self, "result", initial || 0);
        RETURN(self);
    )),
    "add": FUN((self, value), (
        LET(current, GET_PROP(self, "result"));
        SET_PROP(self, "result", +(current, value));
        RETURN(self);
    )),
    "get_result": FUN((self), (
        RETURN(GET_PROP(self, "result"));
    ))
]);

// 使用类
LET(calc, NEW(Calculator, 10));
add(calc, 5);
add(calc, 3);
PRINT(CONCAT("Result: ", get_result(calc)));  // 输出: Result: 18
```

### Web开发示例（规划中）
```wlwl
// 组件定义
CLASS(TodoApp, Component, [
    "init": FUN((self), (
        SUPER_INIT(self, []);
        SET_PROP(self, "todos", NEW(List));
        RETURN(self);
    )),
    "add_todo": FUN((self, text), (
        LET(todos, GET_PROP(self, "todos"));
        add(todos, ["text": text, "done": FALSE]);
        CALL_METHOD(self, "render");
        RETURN(self);
    )),
    "render": FUN((self), (
        LET(todos, GET_PROP(self, "todos"));
        RETURN(DIV(["class": "todo-app"], [
            H1([], "Todo List"),
            UL([], ARRAY.MAP(GET_PROP(todos, "_items"), FUN((todo), (
                LI([], GET_PROP(todo, "text"))
            ))))
        ]));
    ))
]);

// 应用初始化
LET(app, NEW(TodoApp));
mount(app, DOM.QUERY(ID("root")));
add_todo(app, "Learn WLWL");
add_todo(app, "Build awesome apps");
```

这些示例展示了WLWL语言在不同编程范式下的表达能力，体现了其"函数式优先，面向对象增强"的设计理念。

## 7. 开发路线图

### 当前版本 (v0.1)
- ✅ 核心词法分析和语法分析
- ✅ 基础数据类型 (Number, String, Boolean, Null)
- ✅ 变量系统 (LET, VAR, SET)
- ✅ 算术运算符 (+, -, *, /)
- ✅ 比较运算符 (=, >)
- ✅ 字符串连接 (CONCAT) 支持自动类型转换
- ✅ 基础I/O (PRINT)
- ✅ 文件执行模式
- ✅ 注释支持

### 下一版本计划 (v0.2)
- 🔄 条件语句 (IF, COND)
- 🔄 循环语句 (WHILE, FOR)
- 🔄 更多比较运算符 (!=, <, >=, <=)
- 🔄 逻辑运算符 (AND, OR, NOT)
- 🔄 用户自定义函数 (FUN)
- 🔄 数组和对象字面量

### 中期目标 (v0.3-0.5)
- 📋 面向对象系统 (CLASS, NEW, 继承)
- 📋 标准库扩展 (MATH, ARRAY, STRING)
- 📋 模块系统 (IMPORT, EXPORT)
- 📋 错误处理 (TRY, CATCH)

### 长期目标 (v1.0+)
- 📋 Web API集成 (HTML, CSS, DOM)
- 📋 组件系统和响应式编程
- 📋 编译器自举
- 📋 性能优化和JIT编译