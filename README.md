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
```

## 示例

创建 `test.wl`:
```wlwl
LET(pi, 3.14159);
LET(area, *(pi, *(5, 5)));
PRINT(CONCAT("Circle area: ", area));
```

执行：
```bash
build/wlwl.exe test.wl
```

## 功能状态

✅ 已实现：
- 基础数据类型 (Number, String, Boolean, Null)
- 变量系统 (LET, VAR, SET)
- 算术运算 (+, -, *, /)
- 比较运算 (=, >)
- 字符串连接 (CONCAT)
- 基础I/O (PRINT)

🔄 开发中：
- 控制流 (IF, WHILE, FOR)
- 用户函数定义
- 面向对象特性

## 构建要求

- GCC (通过Cygwin或MinGW)
- Windows 环境

## 许可证

GPL-3.0