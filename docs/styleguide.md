# Style guide
## File settings
We use 4 space tabs in all of our code, except for Makefiles (Tabs). Line ending is LF. Encoding has to be UTF-8.

| File type  | Suffix |
|------------|--------|
| C++ Header | `.hpp` |
| C++ Source | `.cpp` |
| C Header   | `.h`   |
| C Source   | `.c`   |
| ASM files  | `.S`   |
## Headers
We use self-contained headers with the suffix `.h` (C) or `.hpp` (C++). That means that a header file begins with a `#pragma once` and `#include`s all its dependencies.

All classes or structs have to have at least some documentation.

Forward declarations are done inside the header files or in the same file directly after the includes (No separation between the last `#include` and the first forward declaration)

Between includes and code has to be an empty line.

## Namespaces
The contents of the main namespace is not indented. If you have global variables for a single source file, put them in an anonymous namespace.

Don't use a `use namespace`, as this pollutes the namespace.

## Functions

Functions are declared like:

    auto functionName(int someVar, const SomeType2 &someVar) -> SomeReturnType&;

Primitives are passed by-value, Objects are passed by-reference and Pointers are passed by pointer. The `auto -> ReturnType` is used to be consistent with lambdas and to be able to leave off the namespace.

Dummy functions (with `{}` as function body) may have a semicolon at the end.

The order is: in, out, optional (flags)

## Variables
Initialize all variables on the same line you declare them.

## Classes
### Implicit Conversions
To quote the Zen of Python,
> Explicit is better than implicit.

Which means you should do explicit conversions. Implicit `unsigned char` -> `unsigned short`, `short` -> `int`, `unsigned short` -> `unsigned int`, `int` -> `long long`, or `unsigned int` -> `unsigned long long` is allowed, however.

Convert pointers to `uintptr_t` and to `uintptr_t` exclusively to do maths with them.
### Access control
Use `private` for data members, `protected` for private functions and `public` for everything else. Create getters and setters for things other functions might want to get/change.

#### Order
`public`, `protected`, `private`. Omit empty sections.
### Special C++ features (Exceptions, RTTI)
As of now, the kernel has neither Exception nor RTTI support, so things like `throw` and `typeid()` are not usable.
