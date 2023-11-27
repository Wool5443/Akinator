#include <string.h>
#include "Akinator.hpp"

int main()
{
    Tree akinator = {};

    akinator.Read(AKINATOR_DATA);

    RETURN_ERROR(Play(&akinator));

    akinator.Dump();

    akinator.Print(AKINATOR_DATA);

    akinator.Destructor();

    return 0;
}