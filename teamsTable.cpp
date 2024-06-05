#include "teamsTable.h"



TeamTable::TeamTable(int m): m(m), size(0){
    team_avl_arr = new Team_AVL*[m];
    for(int i = 0; i < m; ++i){
        team_avl_arr[i] = new Team_AVL;
    }
}

TeamTable::~TeamTable(){
    for(int i = 0; i < m; ++i){
        delete team_avl_arr[i];
    }
    delete[] team_avl_arr;
}

int TeamTable::hash(int teamId){
    return teamId % m;
}

Team* TeamTable::find(int teamId){
    return team_avl_arr[hash(teamId)]->find(teamId);
}

void TeamTable::insert(Team* team){
    team_avl_arr[hash(team->id)]->insert(team);
    ++size;
    if(size > m / 2){
        resize(m * 2);
    }
}

void TeamTable::insert(int teamId){
    Team* team = new Team(teamId);
    insert(team);
}

void TeamTable::remove(int teamId){
   if(team_avl_arr[hash(teamId)]->remove(teamId))
        --size;
    if(size <= m / 8 && m > 11){
        resize(m / 2);
    }
}


Team* TeamTable::copy_to_array(){
    Team* new_array = new Team[size];
    int array_index = 0;
    for(int i = 0; i < m; ++i){
        if(team_avl_arr[i]->get_size() > 0){
            team_avl_arr[i]->copy_to_array(new_array + array_index);
            array_index += team_avl_arr[i]->get_size();
        }
    }

    return new_array;
}

void TeamTable::resize(int new_m){
    Team* teams_array = copy_to_array();
    Team_AVL** to_remove = team_avl_arr;
    team_avl_arr = new Team_AVL*[new_m];
    int old_m = m;
    m = new_m;
    int old_size = size;
    size = 0;
    for(int i = 0; i < new_m; ++i){
        team_avl_arr[i] = new Team_AVL;
    }
    for(int i = 0; i < old_size; ++i){
        Team* team_to_insert = new Team;
        team_to_insert->copy(&teams_array[i]);
        teams_array[i].players = nullptr;
        insert(team_to_insert);
        
    }
    for(int i = 0; i < old_m; ++i){
        delete to_remove[i];
    }
    delete[] teams_array;
    delete[] to_remove;
}