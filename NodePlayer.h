#ifndef NodePlayer_H_
#define NodePlayer_H_
#include <iostream>


class NodePlayer{
    public:
    int force;
    int ID;
    int max_ID;
    int size;
    int hight;
    NodePlayer* left;
    NodePlayer* right;
    
    NodePlayer(int forceGiven, int IDGiven);
    NodePlayer();
    ~NodePlayer();
    NodePlayer(const NodePlayer& other) = default;
    NodePlayer& operator=(const NodePlayer& other) = default;

    void update_hight();
    void update_max_ID();
    void update_size();

    void update_node_rankes();
    int get_BF() const;
    void copy(const NodePlayer* other);
    bool isSmaller(const NodePlayer* other);



};




#endif // NodePlayer_H_