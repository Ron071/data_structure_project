#ifndef TEAMSTABLE_H
#define TEAMSTABLE_H
#include "team.h"

class TeamTable{
    Team_AVL** team_avl_arr;
    int m; // hashtable's m from the lectures
    int size;
    public:
    TeamTable(int m=11);
    ~TeamTable();
    int hash(int teamId);
    Team* copy_to_array();
    void insert(int teamId);
    void insert(Team* team);
    void remove(int teamId);
    void resize(int new_m);
    Team* find(int teamId);
    int get_m(){
        return m;
    }
    int get_size(){
        return size;
    }
};

#endif