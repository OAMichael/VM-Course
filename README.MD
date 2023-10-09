# Virtual Machine

## Build run and test
### Build
```
$ git clone --recurse-submodules https://github.com/OAMichael/VM-Course.git
$ cd VM-Course
$ cmake -B build -DCMAKE_BUILD_TYPE=Release && cd build
$ cmake --build .
```
### Run main program
```
$ cmake --build . --target run
```
### Run all tests
```
$ cmake --build . --target run_all_tests
```