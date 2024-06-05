#include "NodePlayer.h"

int max(int a, int b){
    if(a>b){
        return a;
    }
    else 
        return b;
}

int max(int a, int b, int c){
    int d = max(a,b);
    return max(d,c);
}

NodePlayer::NodePlayer(int forceGiven, int IDGiven)
: force(forceGiven), ID(IDGiven), max_ID(IDGiven), size(1),
hight(0), left(nullptr), right(nullptr){}

NodePlayer::NodePlayer()
: force(-1),ID(-1),max_ID(-1),size(1),hight(1),left(nullptr),right(nullptr){} 

NodePlayer::~NodePlayer(){
    delete left;
    delete right;
}

void NodePlayer::update_hight(){
    int leftHight = -1;
    int rightHight =-1;
    if(this->left){
        leftHight = this->left->hight;
    }
    if(this->right){
        rightHight = this->right->hight;
    }
    this->hight = max(leftHight,rightHight) +1;
}

void NodePlayer::update_max_ID(){
    int leftmax = -1;
    int rightmax = -1;
    if(this->left){
        leftmax = this->left->max_ID;
    }
    if(this->right){
        rightmax = this->right->max_ID;
    }
    this->max_ID = max(leftmax,rightmax,this->ID);
}

void NodePlayer::update_size(){
    int leftSize = 0;
    int rightSize = 0;
    if(this->left){
        leftSize = this->left->size;
    }
    if(this->right){
        rightSize = this->right->size;
    }
    this->size = leftSize + rightSize + 1;
}

void NodePlayer::update_node_rankes(){
    this->update_hight();
    this->update_max_ID();
    this->update_size();
}

int NodePlayer::get_BF() const{
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

void NodePlayer::copy(const NodePlayer* other){
    this->force = other->force;
    this->ID = other->ID;
}

bool NodePlayer::isSmaller(const NodePlayer* other){
    if(this->force > other->force){
        return false;
    }
    if(this->force < other->force){
        return true;
    }
    else{//this->force == other->force;
        if(this->ID > other->ID){
            return false;
        }
        else{
            return true;
        }
    }
}