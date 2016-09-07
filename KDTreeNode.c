#include "KDTreeNode.h"
#include "SPKDArray.h"

// The node must contain the following information:
// 1- Dim = The splitting dimension
// 2- Val = The median value of the splitting dimension
// 3- Left = Pointer to the left subtree
// 4- Right = Pointer to the right subtree
// 5- Data = Pointer to a point (only if the current node is a leaf) otherwise this field value is NULL
