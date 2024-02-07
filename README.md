# Dynamic Functions
Library with class capable of storing any type of function given any return type and parameters and invoke it (using c++ 17 and template meta programming)

Function pointers, functor's and lamdas are stored internally in a std::function and through use of templates and std::variant, these function references are stored in a type-erased manner. This allows one to create a homogenus container of functions that can be invoked with a set of parameters and allows the return value to bbe captured via a call-back.

## TypeList Functionality
A typeList represents a list of compile time types which you can perform compile time operations on them such as add, delete, union, interseciton, difference, symmetric difference, length, and more! Typelists are useful in generic programming and have multiple usecases. The most useful case I have found is manipulating types at compile time in order to use them with an std::variant (esentially a structured union, similar to an enum in rust), this allows one to branch differently depending on the type stored in the union and access the specific data corresponding to the stored variant.

This library is equipped with a full suite of TypeList operations.
