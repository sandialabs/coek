#include <coek/coek.hpp>
#include <iostream>
#include <string>
#include <vector>

#define UNUSED(x) (void)(x)

int main(int argc, char* argv[])
{
    if (argc < 3) {
        std::cout << "coek_micro <name> <size>" << std::endl;
        return 1;
    }

    std::string name = argv[1];
    size_t size = std::strtoul(argv[2], NULL, 0);

if (name == "forloop") {
    double value=0;
    for (size_t i=0; i<size; ++i) {
        UNUSED(i);
        value += 1.2;
        }
    std::cout << "Value: " << value << " Size: " << size << std::endl;
    }
else if (name == "range") {
    double value=0;
    for (size_t i : coek::range<size_t>(size)) {
        UNUSED(i);
        value += 1.2;
        }
    std::cout << "Value: " << value << " Size: " << size << std::endl;
    }
else if (name == "sequence") {
    double value=0;
    for (size_t i : coek::sequence<size_t>(size)) {
        UNUSED(i);
        value += 1.2;
        }
    std::cout << "Value: " << value << " Size: " << size << std::endl;
    }

else if (name == "nested-forloop") {
    double value=0;
    for (size_t i=0; i<size; ++i) {
        UNUSED(i);
        for (size_t j=0; j<size; ++j) {
            UNUSED(j);
            value += 1.2;
            }
        }
    std::cout << "Value: " << value << " Size: " << size << std::endl;
    }
else if (name == "nested-range") {
    double value=0;
    for (size_t i : coek::range<size_t>(size)) {
        UNUSED(i);
        for (size_t j : coek::range<size_t>(size)) {
            UNUSED(j);
            value += 1.2;
            }
        }
    std::cout << "Value: " << value << " Size: " << size << std::endl;
    }
else if (name == "nested-sequence") {
    double value=0;
    for (size_t i : coek::sequence<size_t>(size)) {
        UNUSED(i);
        for (size_t j : coek::sequence<size_t>(size)) {
            UNUSED(j);
            value += 1.2;
            }
        }
    std::cout << "Value: " << value << " Size: " << size << std::endl;
    }

    else {
        std::cout << "UNKNOWN TEST: " << name << std::endl;
        return 1;
    }

    return 0;
}
