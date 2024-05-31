# Unit Testing

Always test new or changed code as small units/components, down to each method. Tests will be checking for both acceptance and rejection of input parameters and check for output correctness.

I use [__GoogleTest__][1] (or just __Gtest__) to write unit tests in C++.

My unit tests can be found at [./main/tests][3] directory, using the pattern __\<basename>_\<unittest>.cpp__ for each unit test being added.


[1]: https://github.com/google/googletest
[2]: https://developer.ibm.com/articles/au-googletestingframework/
[3]: ./main/tests/
