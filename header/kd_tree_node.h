#ifndef KD_TREE_NODE_H
#define KD_TREE_NODE_H

#include "object.h"
#include <vector>

class KdTreeNode {
public:
    enum DimSplit { X, Y, Z };
    KdTreeNode();
    void setBackNode(KdTreeNode* backNode);
    void setFrontNode(KdTreeNode* frontNode);
    void setDimSplit(KdTreeNode::DimSplit dimSplit);
    void setSplitDistance(float splitDistance);
    void setIsLeaf(bool isLeaf);
    KdTreeNode* getBackNode();
    KdTreeNode* getFrontNode();
    KdTreeNode::DimSplit getDimSplit();
    float getSplitDistance();
    bool getIsLeaf();
    std::vector<Object*> getObjectList();

    void addObject(Object* object);

private:
    KdTreeNode *m_backNode, *m_frontNode;
    KdTreeNode::DimSplit m_dimSplit; // splitting in x, y, or z dimension
    float m_splitDistance; // from origin along split axis
    bool m_isLeaf;
    std::vector<Object*> m_ObjectList; // only for leaves
};

#endif