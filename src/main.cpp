#include <string.h>
#include "Akinator.hpp"

int main()
{
    Tree akinator = {};

    akinator.Read(AKINATOR_DATA);

    akinator.Dump();

    ErrorCode playError = Play(&akinator);
    if (playError != EVERYTHING_FINE && playError != EXIT)
        return playError;

    akinator.Dump();

    // akinator.Print(AKINATOR_DATA);

    akinator.Destructor();

    return 0;
}