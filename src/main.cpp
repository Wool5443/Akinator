#include <string.h>
#include "Akinator.hpp"

int main()
{
    Tree akinator = {};

    akinator.Read("data.txt");

    TreeNodeResult rootRes = TreeNode::New("Unknown", nullptr, nullptr);
    MyAssertSoft(!rootRes.error, rootRes.error);

    akinator.Init(rootRes.value);

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

    // akinator.Print("data.txt");

    akinator.Destructor();

    return 0;
}