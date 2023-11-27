#ifndef AKINATOR_HPP
#define AKINATOR_HPP

#include "Utils.hpp"
#include "Tree.hpp"

static const int MAX_STRING_LENGTH = 128;
static const int MAX_COMMAND_LENGTH = 256;
static const char* AKINATOR_DATA = "Animals.txt";
#define MAX_STRING_LENGTH_DEFINE "128"

ErrorCode Guess(Tree* dataTree);



#endif
