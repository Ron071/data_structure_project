#ifndef TEAM_H_
#define TEAM_H_

#include "AVL_Players.h"

struct Team{
    int id;//never changes
    int power;
    int medals;
    int hight;
    AVL_Players* players;
    Team* left;
    Team* right;

    Team(int id, int power=0, int medals=0);
    Team();
    ~Team();
    void insert_player(int force);
    void update_power();//I think I understood what that ment
    void update_Team_rankes(); // in this case only update the hight!
    bool remove_newest_player();
    int get_BF() const;
    void copy(Team* other);
    void unite_players(Team* other);

};

class Team_AVL{

    int size;
    int count; // dont understand why we need this but i implemented anyway
    Team* root;

    public:
    Team_AVL();//did
    ~Team_AVL();//did

    Team* get_root() const;//did
    int get_size();
    bool insert(Team* team);//did
    Team* find(int teamId);// the output might be NULL if the team doesnt exist
    bool remove(int teamId);//also deletes the players in the team!
    void copy_to_array(Team* array) const;//did
};

#endif // TEAM_H_