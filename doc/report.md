# Compiler Project `minijavac`

卢力韬 15307130084

## 简介

`minijavac` 是 Minijava 的编译器前端。

### Dependencies
Bison 3.2
Flex 2.6.4
C++ 14
CMake 3.13.1

### 编译
```bash
mkdir build && cd build
cmake ..
make
```

### 运行

*输出AST*
```bash
cd build/src
./parser_exe ../../test/factorial.java | ../../utils/vi.py
xdg-open ast.png
```


*检查程序错误*
```bash
./parser_exe ../../test/manyerror.java > /dev/null
```

`test/` 目录下有多种测试样例




## 功能

• 正确的词法/语法分析，给出对应的文法文件: `parser.yy`,
• 输出正确的抽象语法树 见 `printer.hpp`
    • 错误处理功能 见 test/
        词法错误 test/syntaxerror.java
        语法错误 test/syntaxerror.java
        语义错误 test/*.java `typechecker.hpp`
           - type check
           - scope check
           -
    - 错误恢复
        - bison error production
            在下一statement恢复
            在下一method恢复
            在下一class恢复


额外功能:
    user-friendly error reporting
        可以显示错误的token位置
    function overloading
        不同参数类型，同样的id
    dynamic dispatch check
        子类可以override base class同样的method
    optional "else"
    var decl anywhere
        不必定义在method头部
    shadowing variable
        子scope可以定义同样的id，shadowing外部scope变量
    array for any type. array's type rules
        A[] arr;
        int[][] arr;
    main class do not have be first.
    error supress
    check syclic depenence.

后端[tbd]
x86_64代码生成
