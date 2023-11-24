#include <string.h>
#include "Tree.hpp"

int main()
{
    Tree akinator = {};

    akinator.Read("akinatorData.txt");

    akinator.Dump();

    akinator.Destructor();

    return 0;
}