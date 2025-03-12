#ifndef KD_TREE_NODE_H
#define KD_TREE_NODE_H

#include "axis_aligned_bounding_box.h"
#include "object.h"
#include <vector>

class KdTreeNode {
public:
    enum DimSplit { X, Y, Z };
    KdTreeNode();
    ~KdTreeNode();
    void setBackNode(KdTreeNode* backNode);
    void setFrontNode(KdTreeNode* frontNode);
    void setDimSplit(KdTreeNode::DimSplit dimSplit);
    void setSplitDistance(float splitDistance);
    void setIsLeaf(bool isLeaf);
    void setObjectList(std::vector<AxisAlignedBoundingBox*> objects);
    void setAxisAlignedBoundingBox(AxisAlignedBoundingBox* axisAlignedBoundingBox);
    int getSize();
    KdTreeNode* getBackNode();
    KdTreeNode* getFrontNode();
    KdTreeNode::DimSplit getDimSplit();
    float getSplitDistance();
    bool getIsLeaf();
    std::vector<AxisAlignedBoundingBox*> getObjectList();
    KdTreeNode::DimSplit getNextDim();
    AxisAlignedBoundingBox* getAxisAlignedBoundingBox();
    void addObject(AxisAlignedBoundingBox* object);
    void printTreeNode(const std::string& prefix, KdTreeNode* node, bool isLeft);
    void printTree();

private:
    KdTreeNode *m_backNode, *m_frontNode;
    KdTreeNode::DimSplit m_dimSplit; // splitting in x, y, or z dimension
    float m_splitDistance; // from origin along split axis
    bool m_isLeaf;
    AxisAlignedBoundingBox* m_axisAlignedBoundingBox;
    std::vector<AxisAlignedBoundingBox*> m_ObjectList; // only for leaves
};

#endif