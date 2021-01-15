# FunctionAny
Class capable of storing any type of function given any return type and parameters and invoke it (using c++ 17 and template meta programming)

Function pointers, functor's and lamdas are stored internally in a std::function and through use of templates and std::variant, these function references are stored in a type-erased manner. This allows one to create a homogenus container of functions that can be invoked with a set of parameters and allows the return value to bbe captured via a call-back.
