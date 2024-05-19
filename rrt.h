#ifndef RRT_H
#define RRT_H

#include "obstacles.h"
#include <cstdlib>
#include <vector>
#include <cmath>

using namespace std;
using namespace Eigen;

struct Node {
    vector<Node *> children;
    Node *parent;
    Vector2f position;
};

class RRT
{
public:
    RRT();
    void initialize();
    static Node* getRandomNode();
    Node* nearest(Vector2f point);
    static int distance(Vector2f &p, Vector2f &q);
    Vector2f newConfig(Node *q, Node *qNearest) const;
    void add(Node *qNearest, Node *qNew);
    bool reached();
    void setStepSize(int step);
    void setMaxIterations(int iter);
    void deleteNodes(Node *root);
    Obstacles *obstacles;
    vector<Node *> nodes;
    vector<Node *> path;
    Node *root, *lastNode;
    Vector2f start_pos, end_pos;

    void setStartPos(const Vector2f &startPos);

    void setEndPos(const Vector2f &endPos);

    int max_iter;
    int step_size;
};

#endif // RRT_H
