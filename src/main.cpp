#include <string.h>
#include "Akinator.hpp"

int main()
{
    Tree::StartHtmlLogging();

    Tree akinator = {};

    akinator.Read(AKINATOR_DATA);
    akinator.Dump();

    ErrorCode playError = Play(&akinator);
    if (playError != EVERYTHING_FINE && playError != EXIT)
        return playError;

    akinator.Dump();

    akinator.Destructor();

    Tree::EndHtmlLogging();

    return 0;
}