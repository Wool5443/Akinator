#include <string.h>
#include "Akinator.hpp"

int main()
{
    Tree::StartHtmlLogging();

    Tree akinator = {};

    akinator.Read(AKINATOR_DATA);

    ErrorCode playError = Play(&akinator);
    if (playError != EVERYTHING_FINE && playError != EXIT)
        return playError;

    akinator.Destructor();

    Tree::EndHtmlLogging();

    return 0;
}