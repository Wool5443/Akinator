//! @file

#ifndef TREE_HPP
#define TREE_HPP

#include "Utils.hpp"
#include "TreeSettings.ini"

/**
 * @brief Log folders
 */
static const char* DOT_FOLDER = "log/dot";
static const char* IMG_FOLDER = "log/img";
static const char* HTML_FILE_PATH = "log.html";

static const size_t BAD_ID = 0;

struct TreeNodeResult;
/** @struct TreeNode
 * @brief A binary tree node containing value and ptrs to children
 * 
 * @var TreeNode::value - TreeElemen_t value
 * @var TreeNode::left - TreeNode* left
 * @var TreeNode::right - TreeNode* right
 * @var TreeNode::parent - TreeNode* parent
 * @var TreeNode::id - size_t id - unique id of a node, used for dumping
 * @var TreeNode::nodeCount - number of all nodes going from the current one
*/
struct TreeNode
{
    TreeElement_t value;
    TreeNode* left;
    TreeNode* right;
    TreeNode* parent;

    size_t id;

    #ifdef SIZE_VERIFICATION
    size_t nodeCount;
    #endif

    /**
     * @brief Returns a new node result
     * 
     * @param value - value
     * @param left - left child
     * @param right - right child
     * @return TreeNodeResult - new node
     */
    static TreeNodeResult New(TreeElement_t value, TreeNode* left, TreeNode* right);

    /**
     * @brief Deletes a node
     * 
     * @return Error
     */
    ErrorCode Delete();

    /**
     * @brief Copies the node and returns the copy
     * 
     * @return TreeNodeResult the copy
     */
    TreeNodeResult Copy();

    /**
     * @brief Sets the left node.
     * 
     * @param left - the left node.
     * @return Error
     */
    ErrorCode SetLeft(TreeNode* left);
    
    /**
     * @brief Sets the right node.
     * 
     * @param right - the right node.
     * @return Error
     */
    ErrorCode SetRight(TreeNode* right);
};
struct TreeNodeResult
{
    TreeNode* value;
    ErrorCode error;
};

/** @struct TreeNodeCountResult
 * @brief Used for counting nodes.
 * 
 * @var TreeNodeCountResult::value - how many nodes
 * @var TreeNodeCountResult::error
 */
struct TreeNodeCountResult
{
    size_t value;
    ErrorCode error;
};

/** @struct Tree
 * @brief Represents a binary tree
 * 
 * @var Tree::root - root of the tree
 * @var Tree::size - number of nodes in the tree
 */
struct Tree
{
    TreeNode* root;

    #ifdef SIZE_VERIFICATION
    size_t* size;
    #endif

    /**
     * @brief Initializes a tree with a root node
     * 
     * @param root
     * @return Error
     */
    ErrorCode Init(TreeNode* root);

    /**
     * @brief Destroys the tree
     * 
     * @return Error
     */
    ErrorCode Destructor();

    /**
     * @brief Checks the tree's integrity
     * 
     * @return Error
     */
    ErrorCode Verify();
    
    /**
     * @brief Counts nodes in the tree
     * 
     * @return Error
     */
    TreeNodeCountResult CountNodes();

    #ifdef SIZE_VERIFICATION
    /**
     * @brief Recalculates @ref TreeNode::nodeCount for every node in tree
     * 
     * @return Error
     */
    ErrorCode RecalculateNodes();
    #endif

    /**
     * @brief Draws a tree into @ref IMG_FOLDER using Graphviz
     * 
     * @return Error
     */
    ErrorCode Dump();

    static ErrorCode StartHtmlLogging();
    static ErrorCode EndHtmlLogging();

    /**
     * @brief Saves the tree in pre-order
     * 
     * @param outPath - where to save
     * @return Error
     */
    ErrorCode Print(const char* outPath);

    /**
     * @brief Read the tree in pre-order
     * 
     * @attention Make sure to delete the tree before reading into it
     * 
     * @param readPath - what to read
     * @return Error
     */
    ErrorCode Read(const char* readPath);
};

#endif
