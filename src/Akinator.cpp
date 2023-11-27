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

#define PRINT_NODE(node)                                                            \
fprintf(stderr, #node " addr: %p\n", (node));                                       \
fprintf(stderr, #node " val: " TREE_ELEMENT_SPECIFIER "\n", (node)->value);         \
fprintf(stderr, #node " parent: %p\n", (node)->parent);                             \
fprintf(stderr, #node " left: %p\n", (node)->left);                                 \
fprintf(stderr, #node " right: %p\n\n--------------------------\n", (node)->right);

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
    sprintf(_command, "echo %s | festival --tts", _toSay);                          \
    /*system(_command);*/                                                               \
} while (0)

ErrorCode _recGuess(TreeNode* node);

ErrorCode _addNewObject(TreeNode* node);

ErrorCode Guess(Tree* dataTree)
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
            return ERROR_SYNTAX;
        }
    }

    PRINT_SAY("Let me guess...");
    PRINT_SAY("It's %s, isn'it?", node->value);

    char userAnswer = '\0';
    scanf("%c", &userAnswer);
    ClearBuffer(stdin);

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
            return ERROR_SYNTAX;
            break;
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
