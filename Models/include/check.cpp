#include <iostream>
#include "workspace.hpp"

int main()
{
    Workspace<size_t> w;

    std::cout << w.getWidth();
}