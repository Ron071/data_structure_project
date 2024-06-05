#ifndef AVL_Players_H_
#define AVL_Players_H_

#include "NodePlayer.h"

class AVL_Players{
    NodePlayer* root;
    int size;
    int count;
    int mid_force;

    public:
    AVL_Players();
    ~AVL_Players();

    void update_mid_force();

    int get_size() const;
    int get_count() const;
    NodePlayer* get_root() const;
    int get_mid_force();
    void set_size(int newSize);
    void set_count(int newCount);
    void set_root(NodePlayer* newRoot);
    void set_mid_force(int new_mid_force); // indexes are from 0!


    bool insert(int force, int ID);
    bool remove_newest();
    
    friend AVL_Players* unite(AVL_Players* first, AVL_Players* second);



};





#endif // AVL_Players_H_