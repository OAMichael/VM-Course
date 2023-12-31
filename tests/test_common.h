#ifndef TEST_COMMON_H
#define TEST_COMMON_H

#include <iostream>
#include <sstream>

namespace VM_Tests {

class CoutRedefiner {
private:
    std::ostringstream my;
    std::streambuf *cout_buff;

public:
    void changeOnMy() {
        cout_buff = std::cout.rdbuf();
        std::cout.rdbuf(my.rdbuf());
    }

    std::stringstream returnBack() {
        std::cout.rdbuf(cout_buff);
        std::stringstream ss(my.str());
        return ss;
    }
};

class CinRedefiner {
private:
    std::stringstream my;
    std::streambuf *cin_buff;

public:
    void changeOnMy() {
        cin_buff = std::cin.rdbuf();
        std::cin.rdbuf(my.rdbuf());
    }

    std::stringstream& get() {
        return my;
    }

    std::stringstream returnBack() {
        std::cin.rdbuf(cin_buff);
        std::stringstream ss(my.str());
        return ss;
    }
};

}

#endif