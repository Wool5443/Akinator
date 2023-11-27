#include <string.h>
#include "Akinator.hpp"

int main()
{
    Tree akinator = {};

    akinator.Read(AKINATOR_DATA);

    ErrorCode guessError = EVERYTHING_FINE;

    while (!guessError)
    {
        guessError = Guess(&akinator);
        if (guessError && guessError != EXIT)
        {
            SetConsoleColor(stderr, COLOR_RED);
            fprintf(stderr, "%s in Guess Game!\n", ERROR_CODE_NAMES[guessError]);
            SetConsoleColor(stderr, COLOR_WHITE);
        }
    }

    akinator.Dump();

    akinator.Print(AKINATOR_DATA);

    akinator.Destructor();

    return 0;
}