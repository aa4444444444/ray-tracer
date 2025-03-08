#include "../header/kd_tree_node.h"

KdTreeNode::KdTreeNode()
{
    m_backNode = nullptr;
    m_frontNode = nullptr;
    m_isLeaf = true;
}

void KdTreeNode::setBackNode(KdTreeNode* backNode) { m_backNode = backNode; }
void KdTreeNode::setFrontNode(KdTreeNode* frontNode) { m_frontNode = frontNode; }
void KdTreeNode::setDimSplit(KdTreeNode::DimSplit dimSplit) { m_dimSplit = dimSplit; }
void KdTreeNode::setSplitDistance(float splitDistance) { m_splitDistance = splitDistance; }
void KdTreeNode::setIsLeaf(bool isLeaf) { m_isLeaf = isLeaf; }
KdTreeNode* KdTreeNode::getBackNode() { return m_backNode; }
KdTreeNode* KdTreeNode::getFrontNode() { return m_frontNode; }
KdTreeNode::DimSplit KdTreeNode::getDimSplit() { return m_dimSplit; }
float KdTreeNode::getSplitDistance() { return m_splitDistance; }
bool KdTreeNode::getIsLeaf() { return m_isLeaf; }
std::vector<Object*> KdTreeNode::getObjectList() { return m_ObjectList; }

void KdTreeNode::addObject(Object* object) { m_ObjectList.push_back(object); }