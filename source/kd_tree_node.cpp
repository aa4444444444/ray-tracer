#include "../header/kd_tree_node.h"

KdTreeNode::KdTreeNode()
{
    m_backNode = nullptr;
    m_frontNode = nullptr;
    m_axisAlignedBoundingBox = nullptr;
    m_isLeaf = true;
}

KdTreeNode::~KdTreeNode()
{
    if (m_backNode != nullptr) {
        delete m_backNode;
    }

    if (m_frontNode != nullptr) {
        delete m_frontNode;
    }

    if (m_axisAlignedBoundingBox != nullptr) {
        delete m_axisAlignedBoundingBox;
    }
}

KdTreeNode::DimSplit KdTreeNode::getNextDim()
{
    // Round-Robin fashion
    if (m_dimSplit == DimSplit::X) {
        return DimSplit::Y;
    } else if (m_dimSplit == DimSplit::Y) {
        return DimSplit::Z;
    } else {
        return DimSplit::X;
    }
}

void KdTreeNode::setBackNode(KdTreeNode* backNode)
{
    m_backNode = backNode;
    m_isLeaf = false;
}
void KdTreeNode::setFrontNode(KdTreeNode* frontNode)
{
    m_frontNode = frontNode;
    m_isLeaf = false;
}
void KdTreeNode::setDimSplit(KdTreeNode::DimSplit dimSplit) { m_dimSplit = dimSplit; }
void KdTreeNode::setSplitDistance(float splitDistance) { m_splitDistance = splitDistance; }
void KdTreeNode::setIsLeaf(bool isLeaf) { m_isLeaf = isLeaf; }
void KdTreeNode::setObjectList(std::vector<AxisAlignedBoundingBox*> objects) { m_ObjectList = objects; }
void KdTreeNode::setAxisAlignedBoundingBox(AxisAlignedBoundingBox* axisAlignedBoundingBox)
{
    m_axisAlignedBoundingBox = axisAlignedBoundingBox;
}

int KdTreeNode::getSize()
{
    if (m_isLeaf) {
        return 0;
    }

    int left = 0;
    int right = 0;

    if (m_frontNode != nullptr) {
        left = m_frontNode->getSize();
    }

    if (m_backNode != nullptr) {
        right = m_backNode->getSize();
    }

    return left + right + 1;
}
KdTreeNode* KdTreeNode::getBackNode() { return m_backNode; }
KdTreeNode* KdTreeNode::getFrontNode() { return m_frontNode; }
KdTreeNode::DimSplit KdTreeNode::getDimSplit() { return m_dimSplit; }
float KdTreeNode::getSplitDistance() { return m_splitDistance; }
bool KdTreeNode::getIsLeaf() { return m_isLeaf; }
std::vector<AxisAlignedBoundingBox*> KdTreeNode::getObjectList() { return m_ObjectList; }
AxisAlignedBoundingBox* KdTreeNode::getAxisAlignedBoundingBox() { return m_axisAlignedBoundingBox; }

void KdTreeNode::addObject(AxisAlignedBoundingBox* object) { m_ObjectList.push_back(object); }

void KdTreeNode::printTreeNode(const std::string& prefix, KdTreeNode* node, bool isLeft)
{
    if (node != nullptr) {
        std::cout << prefix;

        std::cout << (isLeft ? "|--" : "L--");

        // print the value of the node
        if (node->getAxisAlignedBoundingBox() != nullptr) {
            node->getAxisAlignedBoundingBox()->printAABB();
        }

        // enter the next tree level - left and right branch
        printTreeNode(prefix + (isLeft ? "|   " : "    "), node->getBackNode(), true);
        printTreeNode(prefix + (isLeft ? "|   " : "    "), node->getFrontNode(), false);
    }
}
void KdTreeNode::printTree() { printTreeNode("", this, false); }