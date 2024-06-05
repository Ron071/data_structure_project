#include "team.h"
#include <assert.h>

/////////////////// MATH - FUNCTIONS /////////////////////////////////////////
int max2(int a, int b){
    if(a>b){
        return a;
    }
    else 
        return b;
}



//////////////// TEAM - FUNCTIONS ////////////////////////////////////////////
Team::Team(int idGiven, int powerGiven, int medalsGiven)
: id(idGiven), power(powerGiven), medals(medalsGiven), hight(0), left(nullptr), right(nullptr){
    this->players = new AVL_Players;
}

Team::Team()
: id(-1), power(0), medals(0), hight(0), left(nullptr), right(nullptr){
    this->players = new AVL_Players;
} 

Team::~Team(){
    delete players;
    delete left;
    delete right;
}

void Team::insert_player(int force){
    this->players->insert(force,this->players->get_count());
    this->update_power();
}

bool Team::remove_newest_player(){
    bool result = this->players->remove_newest();
    this->update_power();
    return result;
}

void Team::update_Team_rankes(){
    int leftHight = -1;
    int rightHight =-1;
    if(this->left){
        leftHight = this->left->hight;
    }
    if(this->right){
        rightHight = this->right->hight;
    }
    this->hight = max2(leftHight,rightHight) + 1;
}

void Team::update_power(){
    this->power = (this->players->get_mid_force()) * (this->players->get_size());
}

int Team::get_BF() const{
    int leftHight = -1;
    int rightHight = -1;
    if(this->left){
        leftHight = this->left->hight;
    }
    if(this->right){
        rightHight = this->right->hight;
    }
    return leftHight - rightHight;
}

void Team::copy(Team* other){
    this->id = other->id;
    this->power = other->power;
    this->medals = other->medals;
    // hight doesnt changed!
    if(this->players != nullptr){
        delete this->players; // deleting the first team players
    }
    this->players = other->players;
    other->players = nullptr;
    // the left anf right doent change!
}

void Team::unite_players(Team* other){
    AVL_Players* temp = unite(this->players,other->players);
    other->players = nullptr;
    this->players = temp;
    this->update_power();
}
//////////////////////// AVL - FUNCTIONS ///////////////////////////////////////

Team_AVL::Team_AVL()
: size(0), count(0), root(nullptr){}

Team_AVL::~Team_AVL(){
    delete root;
    root = nullptr;
}

Team* Team_AVL::get_root() const{
    return this->root;
}

int Team_AVL::get_size(){
    return this->size;
} 

Team* RightRotation(Team* b){
    Team* a = b->left;
    Team* d = a->right;
    a->right = b;
    b->left = d;
    b->update_Team_rankes();
    a->update_Team_rankes();
    return a;
}

Team* LeftRotation(Team* a){
    Team* b = a->right;
    Team* d = b->left;
    b->left = a;
    a->right = d;
    a->update_Team_rankes();
    b->update_Team_rankes();
    return b;
}

Team* balance_subtree_Teams(Team* node){
    if(node == nullptr){
        return nullptr;
    }
    node->update_Team_rankes();
    int bf = node->get_BF();
    if(bf > 1 && node->left->get_BF() >= 0){
        return RightRotation(node);
    }
    if(bf < -1 && node->right->get_BF() <= 0){
        return LeftRotation(node);
    }
    if(bf > 1 && node->left->get_BF() == -1){
        node->left = LeftRotation(node->left);
        return RightRotation(node);//lr
    }
    if(bf < -1 && node->right->get_BF() == 1){
        node->right = RightRotation(node->right);
        return LeftRotation(node);//rl
    }
    return node;
}


Team* insert_to_Team_tree(Team* root, Team* team, bool& succeeded){
    if(root == nullptr){
        return team;
    }
    if(root->id > team->id){
        root->left = insert_to_Team_tree(root->left, team, succeeded);
    }
    else if(root->id < team->id){
        root->right = insert_to_Team_tree(root->right, team, succeeded);
    }
    else{//root->id == team->id
        // assert(root->id == team->id);
        succeeded = false;
    }
    return balance_subtree_Teams(root);
}

bool Team_AVL::insert(Team* team){
    bool succeeded = true;
    this->root = insert_to_Team_tree(this->root, team, succeeded);
    if(succeeded){
        this->count += 1;
        this->size += 1;
    }
    return succeeded;
}

Team* delete_node_Team(Team* node){
    Team* temp;
    if(node->left != nullptr){
        temp = node->left;
    }
    else{
        temp = node->right;
    } 
    if(temp == nullptr){
        delete node->players;
        temp = node;
        node = nullptr;
    }
    else {
        delete node->players;
        *node = *temp;
    }
    temp->players = nullptr;
    temp->left = nullptr;
    temp->right = nullptr;
    delete temp;
    return node;
}

Team* find_and_remove(Team* node, int teamID, bool& succeeded){
    if(node == nullptr){
        succeeded = false;
        return node;
    }
    if(node->id == teamID){
        if(node->left == nullptr || node->right == nullptr){// the node have 1 or 0 sons
            return balance_subtree_Teams(delete_node_Team(node));
        }
        else{// the node has 2 sons
            Team* toReplace;
            toReplace = node->right;
            while(toReplace->left != nullptr){
                toReplace = toReplace->left;
            }
            node->copy(toReplace);
            int IDToDelete = toReplace->id;
            node->right = find_and_remove(node->right, IDToDelete, succeeded); 
        }
    }
    else if(node->id > teamID){
        node->left = find_and_remove(node->left, teamID,succeeded);
    }
    else if(node->id < teamID){
        node->right = find_and_remove(node->right, teamID, succeeded);
    }
    return balance_subtree_Teams(node);
}

bool Team_AVL::remove(int teamID){
    bool succeeded = true;
    this->root = find_and_remove(this->root, teamID, succeeded);
    if(succeeded){
        this->size -= 1;
    }
    return succeeded;
}

Team* find_Team(Team* node, int teamID){
    if(node == nullptr){
        return node;
    }
    else if(node->id == teamID){
        return node;
    }
    else if(node->id > teamID){
        return find_Team(node->left, teamID); 
    }
    else{// node->id < teamID
        return find_Team(node->right, teamID);
    }
}

Team* Team_AVL::find(int teamID){
    Team* result = find_Team(this->root, teamID);
    return result;// the result may be NULL if the team doesnt exist
}

void tree_to_array_Teams(Team* array, Team* root, int& index){
    if(root == nullptr){
        return;
    }
    tree_to_array_Teams(array, root->left, index);
    array[index].copy(root);
    index += 1;
    tree_to_array_Teams(array,root->right,index);
}

void Team_AVL::copy_to_array(Team* array) const{
    int index = 0;
    tree_to_array_Teams(array,this->root, index);
}

