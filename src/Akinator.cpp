#include <stdlib.h>
#include <string.h>
#include "Akinator.hpp"
#include "Tree.hpp"
#include "Utils.hpp"
#include "Stack.hpp"

static const int COMMAND_STUFF_LENGTH = 59;
static const int MAX_COMPARE_STRING_LENGTH = 34;

enum TreePath
{
    LEFT,
    RIGHT,
};

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
    char _toSay[2 * MAX_STRING_LENGTH + MAX_COMPARE_STRING_LENGTH] = "";            \
    sprintf(_toSay, __VA_ARGS__);                                                   \
    fputs(_toSay, stdout);                                                          \
    char _command[sizeof(_toSay) + COMMAND_STUFF_LENGTH] = "";                      \
    sprintf(_command, "echo '(SayText \"%s\")' "                                    \
    "| festival --pipe", _toSay);                                                   \
    /*system(_command);*/                                                           \
} while (0)

#define FOLLOW_PATH(direction, caseLeft, caseRight)                                 \
switch (direction)                                                                  \
{                                                                                   \
case LEFT:                                                                          \
    caseLeft;                                                                       \
    break;                                                                          \
case RIGHT:                                                                         \
    caseRight;                                                                      \
    break;                                                                          \
default:                                                                            \
    return ERROR_BAD_VALUE;                                                         \
}

#define SWITCH_Y_N(value, yesCode, noCode, qCode, defaultCode)                      \
switch (value)                                                                      \
{                                                                                   \
case 'y':                                                                           \
case 'Y':                                                                           \
    yesCode;                                                                        \
    break;                                                                          \
case 'n':                                                                           \
case 'N':                                                                           \
    noCode;                                                                         \
    break;                                                                          \
case 'q':                                                                           \
case 'Q':                                                                           \
    qCode;                                                                          \
    break;                                                                          \
case EOF:                                                                           \
    return EXIT;                                                                    \
default:                                                                            \
    defaultCode;                                                                    \
    break;                                                                          \
}

ErrorCode _guess(Tree* dataTree);

ErrorCode _recGuess(TreeNode* node);

ErrorCode _addNewObject(TreeNode* node);

ErrorCode _compare(Tree* dataTree);

ErrorCode _describe(Tree* dataTree);

ErrorCode _describeObject(TreeNode* node, Stack* stack, const char* obj,
                          StackElementResult prevPop);

TreeNodeResult _recFindNode(TreeNode* node, Stack* stack, const char* obj);

ErrorCode _pushPath(Stack* stack, TreeNode* node);

ErrorCode _exitMenu(Tree* dataTree);

char _retryInput(const char* options);


ErrorCode Play(Tree* dataTree)
{
    PRINT_SAY("Hello, bag of flash! I am the smartest intelligence in the Universe!\n");
    PRINT_SAY("Due to my generousity I am willing to show you how smart I am.\n");
    PRINT_SAY("Tell me what to show you:\n");
    PRINT_SAY("Type g if you want me to guess whats on your mind.\n");
    PRINT_SAY("Type c if you want me to compare two objects.\n");
    PRINT_SAY("Type d if you want me to describe an object.\n");
    PRINT_SAY("Type q if you want to stay dumb.\n");

    char userAnswer = '\0';
    if (scanf("%c", &userAnswer) != 1)
        return EXIT;
    ClearBuffer(stdin);

    while (true)
    {
        switch (userAnswer)
        {
        case 'g':
        case 'G':
            RETURN_ERROR(_guess(dataTree));
            break;
        case 'c':
        case 'C':
            RETURN_ERROR(_compare(dataTree));
            break;
        case 'd':
        case 'D':
            RETURN_ERROR(_describe(dataTree));
            break;
        case 'q':
        case 'Q':
        case EOF:
            return _exitMenu(dataTree);
        default:
            userAnswer = _retryInput("g, c, d, q");
            break;
        }
        PRINT_SAY("What do you want to do next? g, c, d, q.\n");
        if (scanf("%c", &userAnswer) != 1)
            return EXIT;
        ClearBuffer(stdin);
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
        PRINT_SAY("Is it %s?\n", node->value);
        char userAnswer = '\0';
        if (scanf("%c", &userAnswer) != 1)
        {
            PRINT_SAY("You left me, traitor!\n");
            return EXIT;
        }
        ClearBuffer(stdin);
        while (true)
        {
            SWITCH_Y_N(userAnswer,
            return _recGuess(node->left),
            return _recGuess(node->right),
            return EXIT,
            userAnswer = _retryInput("y, n, q")
            );
        }
    }

    PRINT_SAY("Let me guess... ");
    PRINT_SAY("Its %s, innit?\n", node->value);

    char userAnswer = '\0';
    if (scanf("%c", &userAnswer) != 1)
        return EXIT;
    ClearBuffer(stdin);

    while (true)
        SWITCH_Y_N(
        userAnswer,

        PRINT_SAY("As I said!\n");
        putchar('\n');
        return EVERYTHING_FINE,

        return _addNewObject(node),

        return EXIT,

        userAnswer = _retryInput("y, n, q")
        );
}

ErrorCode _addNewObject(TreeNode* node)
{
    MyAssertSoft(node, ERROR_NULLPTR);

    PRINT_SAY("Tell me who it was.\n");
    char* object = (char*)calloc(MAX_STRING_LENGTH + 1, 1);
    fgets(object, MAX_STRING_LENGTH, stdin);

    object[strlen(object) - 1] = '\0';

    PRINT_SAY("How are %s and %s different?\n", node->value, object);
    PRINT_SAY("%s is ", object);

    char* difference = (char*)calloc(MAX_STRING_LENGTH + 1, 1);
    fgets(difference, MAX_STRING_LENGTH, stdin);

    difference[strlen(difference) - 1] = '\0';

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
    MyAssertSoft(dataTree, ERROR_NULLPTR);
    ERR_DUMP_RET(dataTree);

    PRINT_SAY("What objects do you want to compare?\n");
    char obj1[MAX_STRING_LENGTH] = "";
    fgets(obj1, MAX_STRING_LENGTH, stdin);
    char obj2[MAX_STRING_LENGTH] = "";
    fgets(obj2, MAX_STRING_LENGTH, stdin);

    putchar('\n');

    // Deleting \n in the end.
    obj1[strlen(obj1) - 1] = '\0';
    obj2[strlen(obj2) - 1] = '\0';

    StackResult stackRes1 = StackInit();
    RETURN_ERROR(stackRes1.error);
    Stack* stack1 = stackRes1.value;
    
    TreeNodeResult nodeRes1 = _recFindNode(dataTree->root, stack1, obj1);
    RETURN_ERROR(nodeRes1.error);

    StackResult stackRes2 = StackInit();
    RETURN_ERROR(stackRes2.error);
    Stack* stack2 = stackRes2.value;

    TreeNodeResult nodeRes2 = _recFindNode(dataTree->root, stack2, obj2);
    RETURN_ERROR(nodeRes2.error);

    StackElementResult obj1Pop = Pop(stack1);
    StackElementResult obj2Pop = Pop(stack2);

    RETURN_ERROR(obj1Pop.error);
    RETURN_ERROR(obj2Pop.error);

    TreeNode* curNode = dataTree->root;
    if (obj1Pop.value == obj2Pop.value)
    {
        PRINT_SAY("%s and %s are similar because they\n", obj1, obj2);

        while (!obj1Pop.error && !obj2Pop.error && obj1Pop.value == obj2Pop.value)
        {
            FOLLOW_PATH(obj1Pop.value, 
            {
                PRINT_SAY("are %s\n", curNode->value);
                curNode = curNode->left;
            },
            {
                PRINT_SAY("are not %s\n", curNode->value);
                curNode = curNode->right;
            });

            obj1Pop = Pop(stack1);
            obj2Pop = Pop(stack2);
        }
    }

    if (!obj1Pop.error)
    {
        TreeNode* curNode1 = curNode;

        PRINT_SAY("Unlike %s\n", obj2);

        RETURN_ERROR(_describeObject(curNode1, stack1, obj1, obj1Pop));
    }

    if (!obj2Pop.error)
    {
        TreeNode* curNode2 = curNode;

        PRINT_SAY("Unlike %s\n", obj1);

        RETURN_ERROR(_describeObject(curNode2, stack2, obj2, obj2Pop));
    }

    RETURN_ERROR(StackDestructor(stack1));
    RETURN_ERROR(StackDestructor(stack2));

    return EVERYTHING_FINE;
}

ErrorCode _describe(Tree* dataTree)
{
    MyAssertSoft(dataTree, ERROR_NULLPTR);
    ERR_DUMP_RET(dataTree);

    PRINT_SAY("What object do you want me to describe?\n");
    char obj[MAX_STRING_LENGTH] = "";
    fgets(obj, MAX_STRING_LENGTH, stdin);

    putchar('\n');

    obj[strlen(obj) - 1] = '\0';

    StackResult stackRes = StackInit();
    RETURN_ERROR(stackRes.error);
    Stack* stack = stackRes.value;

    RETURN_ERROR(_recFindNode(dataTree->root, stack, obj).error);

    RETURN_ERROR(_describeObject(dataTree->root, stack, obj, Pop(stack)));

    return StackDestructor(stack);
}

ErrorCode _describeObject(TreeNode* node, Stack* stack, const char* obj,
                          StackElementResult prevPop)
{
    MyAssertSoft(node, ERROR_NULLPTR);
    MyAssertSoft(stack, ERROR_NULLPTR);
    MyAssertSoft(obj, ERROR_NULLPTR);

    TreeNode* curNode = node;
    StackElementResult objPop = prevPop;

    if (!objPop.error)
    {
        PRINT_SAY("%s\n", obj);

        while (!objPop.error)
        {
            FOLLOW_PATH(objPop.value,
            {
                PRINT_SAY("is %s\n", curNode->value); 
                curNode = curNode->left;
            },
            {
                PRINT_SAY("is not %s\n", curNode->value);
                curNode = curNode->right;
            });
            objPop = Pop(stack);
        }
        putchar('\n');
    }
    if (objPop.error != ERROR_INDEX_OUT_OF_BOUNDS)
        return objPop.error;

    return EVERYTHING_FINE;
}

TreeNodeResult _recFindNode(TreeNode* node, Stack* stack, const char* obj)
{
    MyAssertSoftResult(node, nullptr, ERROR_NULLPTR);
    MyAssertSoftResult(obj, nullptr, ERROR_NULLPTR);

    if (node->id == BAD_ID)
        return { nullptr, ERROR_TREE_LOOP };
    
    if (!node->left && !node->right && strcasecmp(node->value, obj) == 0)
    {
        ErrorCode pathPushError = _pushPath(stack, node);
        if (pathPushError)
            return { nullptr, pathPushError };

        return { node, EVERYTHING_FINE };
    }

    size_t oldId = node->id;
    node->id = BAD_ID;

    if (node->left)
    {
        if (node->left->parent != node)
        {
            node->id = oldId;
            return { nullptr, ERROR_BAD_TREE };
        }

        TreeNodeResult findRes = _recFindNode(node->left, stack, obj);

        if (!findRes.error)
        {
            node->id = oldId;

            ErrorCode pathPushError = _pushPath(stack, node);
            if (pathPushError)
                return { nullptr, pathPushError };
                
            return findRes;
        }
        else if (findRes.error != ERROR_NOT_FOUND)
        {
            node->id = oldId;
            return findRes;
        }
    }

    if (node->right)
    {
        if (node->right->parent != node)
        {
            node->id = oldId;
            return { nullptr, ERROR_BAD_TREE };
        }

        TreeNodeResult findRes = _recFindNode(node->right, stack, obj);

        if (!findRes.error)
        {
            node->id = oldId;

            ErrorCode pathPushError = _pushPath(stack, node);
            if (pathPushError)
                return { nullptr, pathPushError };

            return findRes;
        }
        else if (findRes.error != ERROR_NOT_FOUND)
        {
            node->id = oldId;
            return findRes;
        }
    }

    node->id = oldId;
    
    return { nullptr, ERROR_NOT_FOUND };
}

ErrorCode _pushPath(Stack* stack, TreeNode* node)
{
    MyAssertSoft(stack, ERROR_NULLPTR);
    MyAssertSoft(node, ERROR_NULLPTR);

    if (node->parent)
    {
        if (node->parent->left == node)
            return Push(stack, LEFT);
        if (node->parent->right == node)
            return Push(stack, RIGHT);
        return ERROR_BAD_TREE;
    }

    return EVERYTHING_FINE;
}

ErrorCode _exitMenu(Tree* dataTree)
{
    PRINT_SAY("Do you want to save data?\n");
    char userAnswer = '\0';
    if (scanf("%c", &userAnswer) != 1)
        return EXIT;
    ClearBuffer(stdin);

    if (userAnswer == 'y' || userAnswer == 'Y')
        return dataTree->Print(AKINATOR_DATA);
    return EXIT;
}

char _retryInput(const char* options)
{
    PRINT_SAY("Incorrect option. Availible options are %s. Try again.\n", options);
    char userAnswer = '\0';
    if (scanf("%c", &userAnswer) != 1)
        return EXIT;
    ClearBuffer(stdin);
    return userAnswer;
}
