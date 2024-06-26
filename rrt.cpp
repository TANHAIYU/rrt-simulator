#include "rrt.h"

RRT::RRT()
{
    obstacles = new Obstacles;
    start_pos.x() = START_POS_X;
    start_pos.y() = START_POS_Y;
    end_pos.x() = END_POS_X;
    end_pos.y() = END_POS_Y;
    root = new Node;
    root->parent = nullptr;
    root->position = start_pos;
    lastNode = root;
    nodes.push_back(root);
    step_size = 3;
    max_iter = 3000;
}

/**
 * @brief Initialize root node of RRT.
 */
void RRT::initialize()
{
    root = new Node;
    root->parent = nullptr;
    root->position = start_pos;
    lastNode = root;
    nodes.push_back(root);
}

/**
 * @brief Generate a random node in the field.
 * @return
 */
Node* RRT::getRandomNode()
{
    Node* ret;
    Vector2f point(drand48() * WORLD_WIDTH, drand48() * WORLD_HEIGHT);
    if (point.x() >= 0 && point.x() <= WORLD_WIDTH && point.y() >= 0 && point.y() <= WORLD_HEIGHT) {
        ret = new Node;
        ret->position = point;
        return ret;
    }
    return nullptr;
}

/**
 * @brief Helper method to find distance between two positions.
 * @param p
 * @param q
 * @return
 */
int RRT::distance(Vector2f &p, Vector2f &q)
{
    Vector2f v = p - q;
    return sqrt(powf(v.x(), 2) + powf(v.y(), 2));
}

/**
 * @brief Get nearest node from a given configuration/position.
 * @param point
 * @return
 */
Node* RRT::nearest(Vector2f point)
{
    float minDist = 1e9;
    Node *closest = nullptr;
    for(auto & node : nodes) {
        float dist = distance(point, node->position);
        if (dist < minDist) {
            minDist = dist;
            closest = node;
        }
    }
    return closest;
}

/**
 * @brief Find a configuration at a distance step_size from nearest node to random node.
 * @param q
 * @param qNearest
 * @return
 */
Vector2f RRT::newConfig(Node *q, Node *qNearest) const {
    Vector2f to = q->position;
    Vector2f from = qNearest->position;
    Vector2f intermediate = to - from;
    intermediate = intermediate / intermediate.norm();
    Vector2f ret = from + step_size * intermediate;
    return ret;
}

/**
 * @brief Add a node to the tree.
 * @param qNearest
 * @param qNew
 */
void RRT::add(Node *qNearest, Node *qNew)
{
    qNew->parent = qNearest;
    qNearest->children.push_back(qNew);
    nodes.push_back(qNew);
    lastNode = qNew;
}

/**
 * @brief Check if the last node is close to the end position.
 * @return
 */
bool RRT::reached()
{
    if (distance(lastNode->position, end_pos) < END_DIST_THRESHOLD)
        return true;
    return false;
}

void RRT::setStepSize(int step)
{
    step_size = step;
}

void RRT::setMaxIterations(int iter)
{
    max_iter = iter;
}

/**
 * @brief Delete all nodes using DFS technique.
 * @param root
 */
void RRT::deleteNodes(Node *root)
{
    for(auto & i : root->children) {
        deleteNodes(i);
    }
    delete root;
}

void RRT::setStartPos(const Vector2f &startPos) {
    start_pos = startPos;
}

void RRT::setEndPos(const Vector2f &endPos) {
    end_pos = endPos;
}
