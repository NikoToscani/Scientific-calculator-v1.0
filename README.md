# Scientific calculator v1.0

Implementation of Scientific calculator v1.0.

The russian version of the task can be found in the repository.

## Contents

1. [Chapter I](#chapter-i) \
   1.1. [Introduction](#introduction)
   1.2. [Information](#information)
    1.3. [Prerequisites](#Prerequisites)
2. [Chapter II](#chapter-ii) \
   2.1. [Technical specifications](#Task-for-implementation-of-scientific-calculator-v1.0) 


## Chapter I

## Introduction

TL;DR: to run the program, get to the src directory and execute:
```bash
make run
```

The main goal of writing this classic program is to apply object-oriented programming (OOP) principles and the SOLID principles in practice.

Due to the requirement to implement the program using the MVC (Model-View-Controller) pattern, the use of OOP and SOLID is mainly concentrated within the Model library (src/model/model.h, model.cc, and src/model/lib/functions.h).

Smart pointers were intentionally not used — for educational purposes.

The presented implementation is an attempt to model a real-world "calculator" device by mentally breaking it down into simpler components (RAM, ROM, ALU, etc.), simplifying those components into C++ classes, and organizing their interaction. This approach made it possible to reuse previously developed classes as the program evolved and became more complex (infix calculator → calculator with variable X → calculator with X and graphing support).

## Information

In this project an extended version of the standard calculator is implemented in C++. In addition to basic arithmetic operations such as add/subtract and multiply/divide, the calculator is able to calculate arithmetic expressions by following the order, as well as some mathematical functions (sine, cosine, logarithm, etc.). Besides calculating expressions, it also supports the use of the variable _x_ and the plot graph of the corresponding function. *Dijkstra's algorithm* used to translate expressions into *reverse Polish notation* to implement the calculator.

### MVC pattern

The Model-View-Controller (MVC) pattern is a scheme for separating application modules into three macro components: a model that contains the business logic, a view that is a UI form for interacting with the program, and a controller that modifies the model by user action.

![](misc/images/MVC-Process.png)


## Prerequisites

Prerequisites (compiler, libraries):
- compiler (gcc, clang) support C++20
- make
- cmake
- gtest (googletest)
- lcov
- Qt library (Qt version 5.15.3 and later should be fine)


## Chapter II

## Task for implementation of Scientific calculator v1.0

- The program must be developed in C++ language of C++20 standard;
- Classes must be implemented within the `scn` namespace;
- Prepare full coverage of expression calculation modules with unit-tests using the GTest library;
- The program must be built with Makefile which contains standard set of targets for GNU-programs: all, install, uninstall, clean, dvi, dist, tests. Installation directory could be arbitrary;
- GUI implementation, based on any GUI library with API for C++20;
- The program must be implemented using the MVC pattern, and also:
   - there should be no business logic code in the view code;
   - there should be no interface code in the controller and the model;
   - controllers must be thin;
- Both integers and real numbers with a dot can be input into the program. You should provide the input of numbers in exponential notation;
- The calculation must be done after you complete entering the calculating expression and press the `=` symbol;
- Calculating arbitrary bracketed arithmetic expressions in infix notation;
- Calculate arbitrary bracketed arithmetic expressions in infix notation with substitution of the value of the variable _x_ as a number;
- Plotting a graph of a function given by an expression in infix notation with the variable _x_;
- Bracketed arithmetic expressions in infix notation must support the following arithmetic operations and mathematical functions:
   - **Arithmetic operators**:
     | Operator name | Infix notation <br /> (Classic) | Prefix notation <br /> (Polish notation) |  Postfix notation <br /> (Reverse Polish notation) |
      | --------- | ------ | ------ | ------ |
      | Brackets | (a + b) | (+ a b) | a b + |
      | Addition | a + b | + a b | a b + |
      | Subtraction | a - b | - a b | a b - |
      | Multiplication | a * b | * a b | a b * |
      | Division | a / b | / a b | a b \ |
      | Power | a ^ b | ^ a b | a b ^ |
      | Modulus | a mod b | mod a b | a b mod |
      | Unary plus | +a | +a | a+ |
      | Unary minus | -a | -a | a- |
      >Note that the multiplication operator contains the obligatory sign `*`. Processing an expression with the omitted `*` sign is optional and is left to the developer's decision.
   - **Functions**:
      | Function description | Function |
      | ------ | ------ |
      | Computes cosine | cos(x) |
      | Computes sine | sin(x) |
      | Computes tangent | tan(x) |
      | Computes arc cosine | acos(x) |
      | Computes arc sine | asin(x) |
      | Computes arc tangent | atan(x) |
      | Computes square root | sqrt(x) |
      | Computes natural logarithm | ln(x) |
      | Computes common logarithm | log(x) |
