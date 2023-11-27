#include <stdlib.h>
#include "Akinator.hpp"
#include "StringFunctions.hpp"
#include "Tree.hpp"
#include "Utils.hpp"

#define ERR_DUMP_RET(tree)                                                          \
do                                                                                  \
{                                                                                   \
    ErrorCode _verifyError = tree->Verify();                                        \
    if (_verifyError)                                                               \
    {                                                                               \
        tree->Dump();                                                               \
        return _verifyError;                                                        \
    }                                                                               \
} while (0);

#define ERR_DUMP_RET_RESULT(tree, value)                                            \
do                                                                                  \
{                                                                                   \
    ErrorCode _verifyError = tree->Verify();                                        \
    if (_verifyError)                                                               \
    {                                                                               \
        tree->Dump();                                                               \
        return { nullptr, _verifyError };                                           \
    }                                                                               \
} while (0);

#define NEW_NODE(name, val, leftNode, rightNode)                                    \
TreeNode* name = nullptr;                                                           \
{                                                                                   \
    TreeNodeResult _res = TreeNode::New(val, leftNode, rightNode);                  \
    MyAssertSoft(!_res.error, _res.error);                                          \
    name = _res.value;                                                              \
}

#define COPY_NODE(name, copy)                                                       \
TreeNode* name = nullptr;                                                           \
{                                                                                   \
    TreeNodeResult _res = copy->Copy();                                             \
    MyAssertSoft(!_res.error, _res.error);                                          \
    name = _res.value;                                                              \
}

#define PRINT_SAY(...)                                                              \
do                                                                                  \
{                                                                                   \
    char _toSay[MAX_STRING_LENGTH] = "";                                            \
    sprintf(_toSay, __VA_ARGS__);                                                   \
    printf("%s\n", _toSay);                                                         \
    char _command[MAX_COMMAND_LENGTH] = "";                                         \
    sprintf(_command, "echo '(voice_cmu_us_ahw_cg) (SayText \"%s\")' "              \
    "| festival --pipe", _toSay);                                                   \
    system(_command);                                                               \
} while (0)

ErrorCode _guess(Tree* dataTree);

char _retryInput(const char options[]);

ErrorCode _recGuess(TreeNode* node);

ErrorCode _addNewObject(TreeNode* node);

ErrorCode _compare(Tree* dataTree);

ErrorCode Play(Tree* dataTree)
{
    PRINT_SAY("Hello, bag of flash! I am the smartest intelligence in the Universe!\n"
    "Due to my generousity I am willing to show you how smart I am.");
    PRINT_SAY("Tell me what to show you:\n"
    "Type g if you want me to guess whats on your mind.");
    PRINT_SAY("Type c if you want me to compare to objects.\n"
    "Type d if you want me to describe an object.");
    PRINT_SAY("Type q if you want to stay dumb.");

    char userAnswer = '\0';
    scanf("%c", &userAnswer);
    ClearBuffer(stdin);

    while (true)
    {
        switch (userAnswer)
        {
        case 'g':
        case 'G':
            RETURN_ERROR(_guess(dataTree));
            PRINT_SAY("What do you want to do next?");
            scanf("%c", &userAnswer);
            ClearBuffer(stdin);
            break;
        case 'q':
        case 'Q':
            return EXIT;
        default:
            userAnswer = _retryInput("g, q");
            break;
        }
    }
}

ErrorCode _guess(Tree* dataTree)
{
    ERR_DUMP_RET(dataTree);
    return _recGuess(dataTree->root);
}

ErrorCode _recGuess(TreeNode* node)
{
    MyAssertSoft(node, ERROR_NULLPTR);

    if (node->left)
    {
        PRINT_SAY("Is it %s?", node->value);
        char userAnswer = '\0';
        scanf("%c", &userAnswer);
        ClearBuffer(stdin);
        while (true)
        {
            switch (userAnswer)
            {
            case 'y':
            case 'Y':
                return _recGuess(node->left);
            case 'n':
            case 'N':
                return _recGuess(node->right);
            case 'q':
            case 'Q':
                return EXIT;
            default:
                userAnswer = _retryInput("y, n, q");
            }
        }
    }

    PRINT_SAY("Let me guess...");
    PRINT_SAY("Its %s, innit?", node->value);

    char userAnswer = '\0';
    scanf("%c", &userAnswer);
    ClearBuffer(stdin);

    while (true)
        switch (userAnswer)
        {
        case 'y':
        case 'Y':
            PRINT_SAY("As I said!");
            putchar('\n');
            return EVERYTHING_FINE;
        case 'n':
        case 'N':
            return _addNewObject(node);
        case 'q':
        case 'Q':
            return EXIT;
        default:
            userAnswer = _retryInput("y, n, q");
        }
}

ErrorCode _addNewObject(TreeNode* node)
{
    MyAssertSoft(node, ERROR_NULLPTR);

    PRINT_SAY("Tell me who it was.");
    char* object = (char*)calloc(MAX_STRING_LENGTH + 1, 1);
    scanf("%" MAX_STRING_LENGTH_DEFINE "s", object);
    ClearBuffer(stdin);

    PRINT_SAY("How are %s and %s different?", node->value, object);
    PRINT_SAY("Tell me in form: \"%s is doing something/is cool\".", object);

    char* difference = (char*)calloc(MAX_STRING_LENGTH + 1, 1);
    scanf("%" MAX_STRING_LENGTH_DEFINE "s", difference);
    ClearBuffer(stdin);

    TreeNodeResult res = TreeNode::New(object, nullptr, nullptr);
    RETURN_ERROR(res.error);
    TreeNode* objectNode = res.value;

    COPY_NODE(nodeCopy, node);

    node->value = difference;
    RETURN_ERROR(node->SetLeft(objectNode));
    RETURN_ERROR(node->SetRight(nodeCopy));

    return EVERYTHING_FINE;
}

ErrorCode _compare(Tree* dataTree)
{
    PRINT_SAY("What object do you want to compare?");
    char obj1[MAX_STRING_LENGTH] = "";
    fgets(obj1, MAX_STRING_LENGTH, stdin);
    char obj2[MAX_STRING_LENGTH] = "";
    fgets(obj2, MAX_STRING_LENGTH, stdin);

    TreeNodeResult obj1NodeRes = _findNode(dataTree, obj1);
}

TreeNodeResult _findNode(Tree* dataTree, const char* obj)
{
    MyAssertSoftResult(dataTree, nullptr, ERROR_NULLPTR);
    MyAssertSoftResult(obj, nullptr, ERROR_NULLPTR);

    ERR_DUMP_RET_RESULT(dataTree, nullptr);

    
}

TreeNodeResult _recFindNode(TreeNode* node, const char* obj)
{
    MyAssertSoftResult(node, nullptr, ERROR_NULLPTR);
}

char _retryInput(const char options[])
{
    PRINT_SAY("Incorrect option. Availible options are %s. Try again.", options);
    char userAnswer = '\0';
    scanf("%c", &userAnswer);
    ClearBuffer(stdin);
    return userAnswer;
}
