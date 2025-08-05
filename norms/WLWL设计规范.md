# WLWL编程语言设计规范（第二版）

> **版本信息**：第二版 v2.0  
> **发布日期**：2025年8月  
> **状态**：正式规范  
> **兼容性**：完全向后兼容第一版规范

---

## 🔒 WLWL项目开发基本原则

**这是WLWL编程语言项目开发的基本原则，具有至高无上的权威性。任何对编译器、解析器、工具链、标准库的开发都必须严格遵守以下不可变原则。这些规则如宪法条文，具有最高优先级，任何技术实现、性能优化、功能扩展、版本迭代都不得违背。**

### 🚫 绝对禁止的修改行为

**无论任何理由、任何版本、任何技术进步，以下核心特性永远不允许改动：**

1. **语法结构神圣不可侵犯**
   - 前缀表达式 `FUNCTION(参数...)` 是唯一合法语法
   - 分号 `;` 作为语句终结符不可更改
   - 空白不敏感原则不可违背
   - 文件扩展名 `.wl` 不可更改

2. **关键字神圣性**
   - `LET`, `VAR`, `IF`, `WHILE`, `FOR`, `FUN` 等核心关键字不可修改
   - `HTML`, `CSS`, `DOM` 等Web元素关键字不可修改
   - 运算符形式 `+(a,b)`, `=(a,b)` 等不可改为中缀表达式

3. **类型系统不可动摇**
   - Number, String, Boolean, Null, Array, Object 基础类型不可修改
   - 变量不可变性原则（LET默认不可变，VAR可变）不可改变

4. **编译目标锁定**
   - 必须能编译为标准HTML/CSS/JavaScript
   - Web友好性不可妥协

### 📝 宪法誓词

**每个WLWL编译器实现都应遵守原则：**

> "我承诺严格遵守WLWL编程语言设计规范，维护语法统一性，保证向后兼容，拒绝任何违宪修改。我理解WLWL的设计哲学，并将忠实地实现每一个规范细节。"

---

## 第一条：核心设计原则

1. **函数式优先**：所有操作都是函数调用，使用前缀表达式
2. **面向对象增强**：在函数式基础上提供类、继承、封装等OOP特性
3. **类型安全**：运行时类型检查，编译时类型推断
4. **不可变性**：变量默认不可变，可变变量需显式声明
5. **表达式化**：一切皆表达式，所有语句都有返回值
6. **AI可读性**：语法结构清晰，易于大语言模型理解和生成
7. **语句终结符**：每个语句必须以分号";"结尾
8. **空白不敏感**：不依赖缩进和换行区分语句，仅作美化用途
9. **编译型设计**：可编译为HTML/CSS/JavaScript，Web友好
10. **统一性保证**：所有新特性都遵循函数调用形式，保持语法一致性

## 第二条：词法规则

```
# 文件扩展名
FILE_EXTENSION := .wl

# 语句终结符
STATEMENT_END := ;

# 标识符规则
IDENTIFIER := [a-zA-Z_][a-zA-Z0-9_]*
NUMBER := [0-9]+(\.[0-9]+)?
STRING := "([^"\\]|\\.)*"
BOOLEAN := TRUE | FALSE
NULL := NULL

# 保留关键字
KEYWORDS := LET | VAR | SET | IF | WHILE | FOR | FUN | RETURN | BREAK | CONTINUE | 
           HTML | HEAD | BODY | DIV | SPAN | P | H1-H6 | IMG | A | UL | OL | LI |
           CSS | STYLE | DOM | ID | CLASS | ATTR | APPEND | REMOVE | REPLACE |
           CLASS | NEW | SUPER | PRIVATE | PUBLIC | STATIC

# 空白字符（仅用于美化，不影响语法）
WHITESPACE := [ \t\n\r]+
```

## 第三条：数据类型系统

```
# 基础类型
Number    := 整数或浮点数
String    := 字符串
Boolean   := TRUE | FALSE
Null      := NULL

# 复合类型
Array     := [元素1, 元素2, ...]
Object    := ["键1":值1, "键2":值2, ...]
Function  := FUN(参数列表, 函数体)

# Web类型
HTMLNode  := HTML元素实例
CSSRule   := CSS样式规则

# 面向对象类型
Class     := 类定义
Instance  := 类实例
```

## 第四条：变量声明与赋值

```
# 不可变变量声明
LET(变量名, 初始值);

# 可变变量声明
VAR(变量名, 初始值);

# 变量重新赋值（仅对VAR变量有效）
SET(变量名, 新值);

# 示例
LET(PI, 3.14159);               // 不可变常量
VAR(counter, 0);                // 可变变量
SET(counter, +(counter, 1));    // 重新赋值
```

## 第五条：运算符规范

```
# 算术运算符
+(a, b, ...);        // 加法，支持多参数
-(a, b);             // 减法
*(a, b, ...);        // 乘法，支持多参数
/(a, b);             // 除法
%(a, b);             // 取模
^(a, b);             // 幂运算

# 比较运算符
=(a, b);             // 等于
!=(a, b);            // 不等于
>(a, b);             // 大于
<(a, b);             // 小于
>=(a, b);            // 大于等于
<=(a, b);            // 小于等于

# 逻辑运算符
AND(a, b, ...);      // 逻辑与，支持多参数
OR(a, b, ...);       // 逻辑或，支持多参数
NOT(a);              // 逻辑非

# 字符串运算符
CONCAT(s1, s2, ...); // 字符串连接
SLICE(str, start, end); // 字符串切片
LENGTH(str);         // 字符串长度
```

## 第六条：控制流结构

```
# 条件语句
IF(条件表达式, 真分支);
IF(条件表达式, 真分支, 假分支);

# 多分支条件
COND(
    (条件1, 分支1),
    (条件2, 分支2),
    (TRUE, 默认分支)
);

# 循环语句
WHILE(条件, 循环体);
FOR(初始化, 条件, 步进, 循环体);
FOREACH(元素变量, 集合, 循环体);

# 示例
IF(>(x, 0), PRINT("x是正数"), PRINT("x不是正数"));

WHILE(<(VAR(i, 0), 10), (
    PRINT(i);
    SET(i, +(i, 1));
));
```

## 第七条：函数定义与调用

```
# 函数定义
FUN(函数名(参数1, 参数2, ...), (
    函数体表达式
));

# 匿名函数
LAMBDA((参数1, 参数2, ...), (
    函数体表达式
));

# 函数调用
函数名(参数1, 参数2, ...);

# 示例
FUN(factorial(n), (
    IF(<=(n, 1), 
        1, 
        *(n, factorial(-(n, 1)))
    )
));

LET(result, factorial(5));
```

## 第八条：面向对象系统核心

### 类定义语法
```
# 类定义
CLASS(类名, 父类, 类体);

# 类体结构
CLASS(ClassName, ParentClass, [
    # 属性定义
    "property1": 默认值1,
    "property2": 默认值2,
    
    # 构造方法
    "init": FUN((self, param1, param2), (
        # 初始化逻辑
    )),
    
    # 实例方法
    "method1": FUN((self, param), (
        # 方法逻辑
    )),
    
    # 静态方法（类方法）
    "STATIC_method": FUN((param), (
        # 静态方法逻辑
    ))
]);
```

### 实例操作
```
# 实例创建
NEW(类名, 构造参数...);

# 属性操作
GET_PROP(对象, "属性名");         // 获取属性
SET_PROP(对象, "属性名", 新值);    // 设置属性
HAS_PROP(对象, "属性名");         // 检查属性
DEL_PROP(对象, "属性名");         // 删除属性

# 方法调用
CALL_METHOD(对象, "方法名", 参数...);
```

### 继承与多态
```
# 父类方法调用
SUPER_CALL(self, "方法名", 参数...);
SUPER_INIT(self, 参数...);

# 类型检查
IS_INSTANCE(对象, 类名);         // 检查继承关系
IS_TYPE(对象, 类名);             // 检查直接类型
IS_SUBCLASS(子类, 父类);         // 检查类关系
```

## 第九条：HTML元素系统

```
# HTML文档结构
HTML(
    HEAD(子元素...),
    BODY(子元素...)
)

# 常用HTML元素
DIV(属性对象, 子元素...)
SPAN(属性对象, 子元素...)
P(属性对象, 文本内容)
H1-H6(属性对象, 标题文本)
IMG(属性对象)
A(属性对象, 链接文本)
UL(子元素...)
OL(子元素...)
LI(文本内容)

# 属性对象格式
["id":"元素ID", "class":"CSS类名", "自定义属性":"属性值"]

# 示例
LET(page, HTML(
    HEAD(
        TITLE("我的网页")
    ),
    BODY(
        DIV(["id":"container", "class":"main"], (
            H1([], "欢迎"),
            P([], "这是一个段落"),
            IMG(["src":"image.jpg", "alt":"图片"])
        ))
    )
));
```

## 第十条：CSS样式系统

```
# CSS规则定义
CSS(选择器, 样式属性对象)

# 选择器类型
ELEMENT(元素名)          // 元素选择器
ID(id名)                // ID选择器  
CLASS(类名)             // 类选择器
ATTR(属性名, 属性值)     // 属性选择器

# 样式操作
STYLE.ADD(css规则数组)           // 添加样式
STYLE.REMOVE(选择器, 属性名)     // 移除样式
STYLE.UPDATE(css规则数组)        // 更新样式

# 示例
STYLE.ADD([
    CSS(ELEMENT("body"), [
        "font-family": "Arial, sans-serif",
        "margin": "0",
        "padding": "20px"
    ]),
    CSS(ID("container"), [
        "max-width": "800px",
        "margin": "0 auto"
    ]),
    CSS(CLASS("highlight"), [
        "background-color": "#ffff00",
        "padding": "5px"
    ])
]);
```

## 第十一条：DOM操作系统

```
# DOM查询
DOM.QUERY(选择器)              // 查询单个元素
DOM.QUERY_ALL(选择器)          // 查询所有匹配元素

# DOM操作
DOM.APPEND(父元素, 子元素)      // 添加子元素
DOM.PREPEND(父元素, 子元素)     // 前置添加子元素
DOM.REMOVE(元素)               // 移除元素
DOM.REPLACE(旧元素, 新元素)     // 替换元素

# 属性操作
DOM.SET_ATTR(元素, 属性名, 属性值)  // 设置属性
DOM.GET_ATTR(元素, 属性名)          // 获取属性
DOM.REMOVE_ATTR(元素, 属性名)       // 移除属性

# 内容操作
DOM.SET_TEXT(元素, 文本)        // 设置文本内容
DOM.GET_TEXT(元素)              // 获取文本内容
DOM.SET_HTML(元素, html内容)    // 设置HTML内容

# 示例
LET(container, DOM.QUERY(ID("container")));
DOM.APPEND(container, P([], "新段落"));
DOM.SET_ATTR(container, "data-loaded", "true");
```

## 第十二条：Web组件系统

```
# 组件基类
CLASS(Component, NULL, [
    "props": [],
    "state": [],
    "element": NULL,
    
    "init": FUN((self, props), (
        SET_PROP(self, "props", props || []);
        SET_PROP(self, "state", []);
        CALL_METHOD(self, "initialize");
        RETURN(self);
    )),
    
    "initialize": FUN((self), (
        # 子类重写此方法
        RETURN(NULL);
    )),
    
    "render": FUN((self), (
        # 子类必须重写此方法
        THROW("render method must be implemented");
    )),
    
    "mount": FUN((self, parent), (
        LET(rendered, CALL_METHOD(self, "render"));
        SET_PROP(self, "element", rendered);
        DOM.APPEND(parent, rendered);
        RETURN(self);
    ))
]);

# 事件处理
CLASS(EventHandler, NULL, [
    "handlers": [],
    
    "on": FUN((self, event, callback), (
        LET(handlers, GET_PROP(self, "handlers"));
        IF(NOT(HAS_PROP(handlers, event)), (
            SET_PROP(handlers, event, []);
        ));
        ARRAY.PUSH(GET_PROP(handlers, event), callback);
        RETURN(self);
    )),
    
    "emit": FUN((self, event, data), (
        LET(handlers, GET_PROP(self, "handlers"));
        IF(HAS_PROP(handlers, event), (
            LET(callbacks, GET_PROP(handlers, event));
            FOREACH(callback, callbacks, (
                callback(data);
            ));
        ));
        RETURN(self);
    ))
]);
```

## 第十三条：模块系统

```
# 模块导入
IMPORT(模块路径)
IMPORT(模块路径, [导入项1, 导入项2])

# 模块导出
EXPORT(导出项)
EXPORT_DEFAULT(默认导出项)

# 示例
// utils.wl
EXPORT(FUN(add(a, b), +(a, b)));
EXPORT(FUN(multiply(a, b), *(a, b)));

// main.wl
IMPORT("./utils.wl", [add, multiply]);
LET(result, add(multiply(3, 4), 2));
```

## 第十四条：错误处理

```
# 异常处理
TRY(
    可能出错的代码,
    CATCH(错误变量, (
        错误处理代码
    )),
    FINALLY((
        清理代码
    ))
)

# 抛出异常
THROW(错误消息)

# 示例
TRY(
    LET(result, /(10, 0)),
    CATCH(error, (
        PRINT(CONCAT("发生错误: ", error))
    ))
);
```

## 第十五条：类型注解（可选）

```
# 变量类型注解
LET(变量名: 类型, 初始值)

# 函数类型注解
FUN(函数名(参数1: 类型1, 参数2: 类型2): 返回类型, (
    函数体
))

# 类型定义
TYPE(类型名, 类型定义)

# 示例
LET(count: Number, 0);
FUN(greet(name: String): String, (
    CONCAT("Hello, ", name)
));

TYPE(Point, ["x": Number, "y": Number]);
```

## 第十六条：标准库规范

```
# 数学函数
MATH.ABS(x)         // 绝对值
MATH.SQRT(x)        // 平方根
MATH.SIN(x)         // 正弦
MATH.COS(x)         // 余弦
MATH.RANDOM()       // 随机数

# 数组操作
ARRAY.PUSH(arr, item)    // 添加元素
ARRAY.POP(arr)           // 移除最后元素
ARRAY.MAP(arr, fn)       // 映射
ARRAY.FILTER(arr, fn)    // 过滤
ARRAY.REDUCE(arr, fn, init) // 归约

# 字符串操作
STRING.SPLIT(str, delimiter)  // 分割
STRING.JOIN(arr, delimiter)   // 连接
STRING.REPLACE(str, old, new) // 替换
STRING.TRIM(str)              // 去除空白

# 集合类
CLASS(List, NULL, [...]);     // 列表类
CLASS(Dict, NULL, [...]);     // 字典类
CLASS(Set, NULL, [...]);      // 集合类

# 输入输出
PRINT(value)         // 打印到控制台
INPUT(prompt)        // 获取用户输入
```

## 第十七条：注释规范

```
# 单行注释
// 这是单行注释

# 多行注释
/*
这是多行注释
可以跨越多行
*/

# 文档注释
/**
 * 函数说明
 * @param name 参数说明
 * @return 返回值说明
 */
```

## 第十八条：编译器实现指导

1. **词法分析**：按照第二条规则进行词法分析，识别分号作为语句终结符
2. **语法分析**：构建抽象语法树(AST)，忽略空白字符
3. **语义分析**：类型检查和作用域分析
4. **目标代码生成**：
   - **HTML输出**：HTML元素直接转换为对应HTML标签
   - **CSS输出**：CSS规则转换为标准CSS语法
   - **JavaScript输出**：控制流和函数转换为JavaScript代码，类转换为ES6类
5. **优化**：死代码消除、常量折叠等优化

### 面向对象编译示例
```wl
// WLWL源代码
CLASS(Person, NULL, [
    "name": NULL,
    "greet": FUN((self), (
        RETURN(CONCAT("Hello, ", GET_PROP(self, "name")));
    ))
]);

LET(person, NEW(Person));
SET_PROP(person, "name", "Alice");
LET(greeting, CALL_METHOD(person, "greet"));
```

编译为JavaScript：
```javascript
class Person {
    constructor() {
        this.name = null;
    }
    
    greet() {
        return "Hello, " + this.name;
    }
}

const person = new Person();
person.name = "Alice";
const greeting = person.greet();
```

## 第十九条：AI协作规范

1. **代码生成**：AI应严格遵循此规范生成代码
2. **代码解释**：AI应能正确解释符合此规范的代码
3. **错误检查**：AI应能检测语法和语义错误
4. **重构建议**：AI可以提供符合规范的重构建议
5. **性能优化**：AI应考虑性能和可读性的平衡
6. **范式协调**：AI应帮助开发者在函数式和面向对象范式间做出合适选择

## 第二十条：版本兼容性

1. **向后兼容**：新版本应保持向后兼容
2. **弃用警告**：过时功能应给出弃用警告
3. **迁移指南**：提供版本升级迁移指南
4. **语义版本**：遵循语义版本规范

## 第二十一条：最佳实践指导

### 编程范式选择
1. **函数式优先**：对于数据转换、计算逻辑，优先使用函数式方法
2. **面向对象补充**：对于状态管理、UI组件，使用面向对象方法
3. **混合使用**：在同一项目中可以灵活组合两种范式

### 命名规范
```wl
# 类名：大驼峰命名法
CLASS(PersonManager, NULL, [...]);

# 方法名：小驼峰命名法
"getUserInfo": FUN((self, user_id), (...));

# 私有方法：下划线前缀
"_validateInput": FUN((self, input), (...));

# 常量：全大写
"MAX_RETRY_COUNT": 3;
```

### 设计原则
1. **组合优于继承**：优先使用组合而非深层继承
2. **接口分离**：保持类的职责单一
3. **依赖注入**：通过构造函数传入依赖
4. **不可变对象**：优先设计不可变对象

---

## 📋 完整示例：待办事项应用

```wl
// 数据模型（函数式）
FUN(createTodo(text), (
    ["id": MATH.RANDOM(), "text": text, "done": FALSE]
));

FUN(toggleTodo(todo), (
    SET_PROP(todo, "done", NOT(GET_PROP(todo, "done")));
    RETURN(todo);
));

// UI组件（面向对象）
CLASS(TodoApp, Component, [
    "init": FUN((self), (
        SUPER_INIT(self, []);
        SET_PROP(self, "todos", NEW(List));
        RETURN(self);
    )),
    
    "addTodo": FUN((self, text), (
        LET(todo, createTodo(text));
        LET(todos, GET_PROP(self, "todos"));
        add(todos, todo);
        CALL_METHOD(self, "render");
        RETURN(self);
    )),
    
    "render": FUN((self), (
        LET(todos, GET_PROP(self, "todos"));
        RETURN(DIV(["class": "todo-app"], [
            H1([], "Todo List"),
            DIV(["class": "input-section"], [
                INPUT(["type": "text", "id": "todo-input"]),
                BUTTON(["onclick": "addTodoHandler()"], "Add")
            ]),
            UL(["class": "todo-list"], 
                ARRAY.MAP(GET_PROP(todos, "_items"), FUN((todo), (
                    LI(["class": IF(GET_PROP(todo, "done"), "done", "")], [
                        SPAN([], GET_PROP(todo, "text")),
                        BUTTON(["onclick": "toggleTodoHandler()"], "Toggle")
                    ])
                )))
            )
        ]));
    ))
]);

// 应用初始化
LET(app, NEW(TodoApp));
mount(app, DOM.QUERY(ID("root")));

// 添加样式
STYLE.ADD([
    CSS(CLASS("todo-app"), [
        "max-width": "600px",
        "margin": "0 auto",
        "padding": "20px"
    ]),
    CSS(CLASS("done"), [
        "text-decoration": "line-through",
        "opacity": "0.6"
    ])
]);
```

---

## 🔐 数字签名与版本控制

```
WLWL_CONSTITUTION_VERSION: 2.0
CONSTITUTION_HASH: SHA256(WLWL_CORE_RULES_V2.0)
IMMUTABLE_CORE_LOCKED: TRUE
MODIFICATION_ALLOWED: FALSE
EXTENSION_ALLOWED: TRUE
BACKWARD_COMPATIBILITY: GUARANTEED
OOP_INTEGRATION: COMPLETE
```

---

## 实现路线图

### 阶段一：核心语言实现
- [x] 词法分析器
- [x] 语法分析器  
- [x] 基础运算符和控制流
- [x] 变量系统

### 阶段二：函数式特性完善
- [ ] 函数定义和调用
- [ ] 高阶函数
- [ ] 匿名函数
- [ ] 闭包支持

### 阶段三：面向对象系统
- [ ] 类定义和实例化
- [ ] 继承和多态
- [ ] 封装和访问控制
- [ ] 方法调用优化

### 阶段四：Web集成
- [ ] HTML元素系统
- [ ] CSS样式系统
- [ ] DOM操作API
- [ ] 组件系统

### 阶段五：高级特性
- [ ] 模块系统
- [ ] 错误处理
- [ ] 类型系统
- [ ] 响应式编程

### 阶段六：工具链完善
- [ ] 编译器自举
- [ ] 标准库完善
- [ ] 开发工具链
- [ ] AI集成优化

---

**这是WLWL编程语言的完整设计规范第二版。本规范在保持第一版核心宪法不变的基础上，系统性地整合了面向对象编程特性，形成了统一、一致、严谨的语言设计。所有实现都必须严格遵循本规范，确保WLWL语言的统一性和一致性。**