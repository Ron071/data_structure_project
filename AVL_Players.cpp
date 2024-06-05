#include "AVL_Players.h"
#include <assert.h>
///////////// MATH - FUNCTIONS //////////
int max1(int a, int b){
    if(a>b){
        return a;
    }
    else 
        return b;
}

int log_min(int num){
    int a = 0;
    while(num != 1){
        num /= 2;
        a += 1;
    }
    return a;
}

int power(int a, int b){// a^b
    int temp = 1;
    while(b != 0){
        temp = temp*a;
        b -= 1;
    }
    return temp;
}

int mid(int a){
    return (a/2);
}

////////////////////// AVL - FUNCTIONS //////////////////////////////////////
AVL_Players::AVL_Players()
: root(nullptr), size(0), count(0), mid_force(0){}

AVL_Players::~AVL_Players(){
    delete root; // same as: root->~NodePlayer();
    root = nullptr;
}

NodePlayer* AVL_Players::get_root() const{
    return this->root;
}

int AVL_Players::get_size() const{
    return this->size;
}

int AVL_Players::get_count() const{
    return this->count;
}

int AVL_Players::get_mid_force(){
    return this->mid_force;
}
void AVL_Players::set_size(int size){
    this->size = size;
}

void AVL_Players::set_count(int count){
    this->count = count;
}

void AVL_Players::set_root(NodePlayer* root){
    this->root = root;
}

void AVL_Players::set_mid_force(int new_mid_force){
    this->mid_force = new_mid_force;
}

NodePlayer* LeftRotation(NodePlayer* a){
    NodePlayer* b = a->right;
    NodePlayer* d = b->left;
    b->left = a;
    a->right = d;
    a->update_node_rankes();
    b->update_node_rankes();
    return b;
}

NodePlayer* RightRotation(NodePlayer* b){
    NodePlayer* a = b->left;
    NodePlayer* d = a->right;
    a->right = b;
    b->left = d;
    b->update_node_rankes();
    a->update_node_rankes();
    return a;
}

NodePlayer* balance_subtree(NodePlayer* node){
    if(node == nullptr){
        return nullptr;
    }
    node->update_node_rankes();
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

NodePlayer* insert_to_tree(NodePlayer* node, int force, int ID, bool* succeeded){
    if(node == nullptr){
        return new NodePlayer(force,ID);
    }
    if(node->force > force){
        node->left = insert_to_tree(node->left,force,ID,succeeded);
    }
    else if(node->force < force){
        node->right = insert_to_tree(node->right,force,ID,succeeded);
    }
    else{// node->force == force
        if(node->ID > ID){
            node->left = insert_to_tree(node->left,force,ID,succeeded);
        }
        else{
            node->right = insert_to_tree(node->right,force,ID,succeeded);
        }
    }
    return balance_subtree(node);
    
}

bool AVL_Players::insert(int player_force, int player_ID){
    bool succeeded = true;
    this->root = insert_to_tree(root, player_force, player_ID, &succeeded);
    if(succeeded){
        this->size = root->size;
        this->count += 1;
        update_mid_force();
    }
    return succeeded;
}




NodePlayer* delete_node(NodePlayer* node){
    NodePlayer* temp;
    if(node->left != nullptr){
        temp = node->left;
    }
    else{
        temp = node->right;
    } 
    if(temp == nullptr){
        temp = node;
        node = nullptr;
    }
    else {
        *node = *temp;
    }
    temp->left = nullptr;
    temp->right = nullptr;
    delete temp;
    return node;
}

NodePlayer* find_and_delete_by_IDForce(NodePlayer* node, int force, int ID){
    if(node->force > force){
        node->left = find_and_delete_by_IDForce(node->left, force, ID);
    }
    else if(node->force < force){
        node->right = find_and_delete_by_IDForce(node->right, force, ID);
    }
    else{// node->force == force
        if(node->ID > ID){
            node->left = find_and_delete_by_IDForce(node->left, force, ID);
        }
        else if(node->ID < ID){
            node->right = find_and_delete_by_IDForce(node->right, force, ID);
        }
        else{// node->force == force && node->ID == ID
            if(node->left == nullptr || node->right == nullptr){// the node have 1 or 0 sons
                return balance_subtree(delete_node(node));
            }
            else{
                // delete_node_with_sons;
                NodePlayer* toReplace;
                toReplace = node->right;
                while(toReplace->left != nullptr){
                    toReplace = toReplace->left;
                }
                node->copy(toReplace);
                int forceToDelete = toReplace->force;
                int IDToDelete = toReplace->ID;
                node->right = find_and_delete_by_IDForce(node->right, forceToDelete, IDToDelete); 
            }
        }
    }
    return balance_subtree(node);
}

NodePlayer* find_and_delete(NodePlayer* node){
    if(node == nullptr){
        return node;
    }
    if(node->ID == node->max_ID){//we are at the node we need to delete
        if(node->left == nullptr || node->right == nullptr){// the node have 1 or 0 sons
            return balance_subtree(delete_node(node));
        }
        else{// the node has 2 sons
            // delete_node_with_sons(node);
            NodePlayer* toReplace;
            toReplace = node->right;
            while(toReplace->left != nullptr){
                toReplace = toReplace->left;
            }
            node->copy(toReplace);
            int forceToDelete = toReplace->force;
            int IDToDelete = toReplace->ID;
            node->right = find_and_delete_by_IDForce(node->right, forceToDelete, IDToDelete); 
        }
    }
    else if(node->left && node->right){
        if(node->left->max_ID > node->right->max_ID){
            node->left = find_and_delete(node->left);
        }
        else{//node->left->max_ID < node->right->max_ID
            node->right = find_and_delete(node->right);
        }
    }
    else if(node->left == nullptr && node->right){
        node->right = find_and_delete(node->right);
    }
    else if(node->right == nullptr && node->left){
        node->left = find_and_delete(node->left);
    }
    
    // not posibolle any more optione because not posiibole that we are at a node with no sons that is not the max_ID
    return balance_subtree(node);
}
bool AVL_Players::remove_newest(){
    if(this->size == 0){
        return false;
    }
    this->root = find_and_delete(this->root);
    this->size -= 1;
    update_mid_force();
    return true;
}

NodePlayer* find(int index, NodePlayer* root){//find index starts from 0! 
    if(root == nullptr)return nullptr;
    if(root->left == nullptr){
        if(index==0){
            return root;
        }
        else{
            return find(index-1, root->right);
        }
    }
    else if(index < root->left->size){
        return find(index, root->left);
    }
    else if(index == root->left->size){
        return root;
    }    
    else{
        return find(index- root->left->size -1, root->right);
    }    
}

// NodePlayer* find(int& index , NodePlayer* root){//the index is defined as - if you put the nodes inorder what index will the node get
//     if(root->size == 1 && index == 1){
//         return root;
//     }
//     if(root->left == nullptr && index == 0){
//         return root;
//     }
//     if(root->left == nullptr && index > 0){
//         int newIndex = index - 1;
//         return find(newIndex,root->right);
//     }
//     if(root->left->size == index){
//         return root;
//     }
//     if(root->left->size > index){
//         return find(index,root->left);
//     }
//     else{//root->left->size < index so that means that its in the right subtree
//         int newIndex = index - root->left->size-1;
//         return find(newIndex,root->right);
//     }
// }

void  AVL_Players::update_mid_force(){
    int index_of_mid_force = mid(this->get_size());
    NodePlayer* mid_force_node = find(index_of_mid_force, this->get_root());
    int new_force = (mid_force_node)?mid_force_node->force:0;
    this->set_mid_force(new_force); 
}

void tree_to_array(NodePlayer* array, NodePlayer* root, int& index){
    if(root == nullptr){
        return;
    }
    tree_to_array(array, root->left, index);
    array[index].copy(root);
    index += 1;
    tree_to_array(array,root->right,index);
}

NodePlayer* merge(NodePlayer* array_first, NodePlayer* array_second,int size_first_array, int size_second_array){//works - checked
    NodePlayer* merged_array = new NodePlayer[size_first_array+size_second_array];
    int i = 0;
    int j = 0;
    int k = 0;
    while(i < size_first_array && j < size_second_array){
        if(array_first[i].isSmaller(&array_second[j])){
            merged_array[k].copy(&array_first[i]);
            i += 1;
        }
        else{
            merged_array[k].copy(&array_second[j]);
            j += 1;
        }
        k += 1;
    }

    while(i < size_first_array){
        merged_array[k].copy(&array_first[i]);
        i += 1;
        k += 1;
    }
    while(j < size_second_array){
        merged_array[k].copy(&array_second[j]);
        j += 1;
        k += 1;
    }
    return merged_array;
}

NodePlayer* built_AVL(NodePlayer* node, int& hight){ // works - checked
    if(hight == 0){
        if(node){
            return node;
        }
        else{
            return new NodePlayer;
        }
    }

    if(node == nullptr){
        NodePlayer* new_node = new NodePlayer;
        int a = hight - 1;
        int b = hight - 1;
        new_node->left = built_AVL(new_node->left,a);
        new_node->right = built_AVL(new_node->right,b);
        return new_node;
    }

    else{// node != null so it is the first one
        hight -= 1;
        node->left = built_AVL(node->left, hight);
        node->right = built_AVL(node->right, hight);
        return node;
    }
}

NodePlayer* delete_leafs(NodePlayer* root, int& leafs_to_delete){// works - checked
    if(root == nullptr){
        return nullptr;
    }
    root->right = delete_leafs(root->right,leafs_to_delete);
    if(root->left == nullptr && root->right == nullptr && leafs_to_delete > 0){
        leafs_to_delete -= 1;
        delete root;
        return nullptr;
    }
    root->left = delete_leafs(root->left,leafs_to_delete);
    return root;
}

void update_tree_ranks(NodePlayer* root){
    if(root == nullptr){
        return;
    }
    update_tree_ranks(root->left);
    update_tree_ranks(root->right);
    root->update_node_rankes();
    
}

void insert_array_to_tree(NodePlayer* root, NodePlayer* array, int& index){// works - checked
    if(root == nullptr){
        return;
    }
    insert_array_to_tree(root->left,array,index);
    root->copy(&array[index]);
    index += 1;
    insert_array_to_tree(root->right,array,index);
}

int find_max_ID(NodePlayer* array, int array_size){
    int result = -1;
    for(int i=0; i<array_size; i++){
        result = max1(result,array[i].ID);
    }
    return result;
}

AVL_Players* built_tree(NodePlayer* array, int array_size){// works - checked
    int max_ID = find_max_ID(array,array_size);
    int hight_of_tree = log_min(array_size);
    int num_of_laefs_to_delete = (power(2,hight_of_tree + 1)- 1) - array_size; // num of nodes in complete tree - num of actual nodes 
    AVL_Players* result = new AVL_Players;
    NodePlayer* node = new NodePlayer;
    NodePlayer* root = built_AVL(node, hight_of_tree);

    delete_leafs(root, num_of_laefs_to_delete);
    int index = 0;
    insert_array_to_tree(root,array,index);
    update_tree_ranks(root);
    result->set_root(root);
    result->set_size(root->size);
    result->set_count(max_ID + 1);
    result->update_mid_force();
    return result;
}

AVL_Players* unite(AVL_Players* first, AVL_Players* second){
    if(first->get_size() > 0 && second->get_size() > 0){
        NodePlayer* array_first = new NodePlayer[first->get_size()];
        NodePlayer* array_second = new NodePlayer[second->get_size()];
        int index = 0;
        tree_to_array(array_first,first->get_root(),index);
        index = 0;
        tree_to_array(array_second,second->get_root(),index);
        int first_max_ID = first->get_root()->max_ID;
        int i = 0;
        while(i<second->get_size()){
            array_second[i].ID += (first_max_ID+1);
            i += 1;
        }
        NodePlayer* merged_array = merge(array_first, array_second, first->get_size(), second->get_size());
        int size_of_merged_array = first->get_size() + second->get_size();
        delete[] array_first;
        delete[] array_second;
        delete first;
        delete second;
        AVL_Players* merged_tree = built_tree(merged_array,size_of_merged_array);
        delete[] merged_array;
        return merged_tree;
    }
    else if(first->get_size() > 0){
        delete second;
        return first;
    }
    else{
        delete first;
        return second;
    }
}




//MERGE - TEST
 // NodePlayer* array_first = new NodePlayer[10];
    // NodePlayer* array_second = new NodePlayer[20];
    // for(int i = 0; i<10 ; i++){
    //     array_first[i].ID = i;
    //     array_first[i].force = i;
    // }
    // for(int i = 0; i<20 ; i++){
    //     array_second[i].ID = 20-i;
    //     array_second[i].force = i;
    // }
    // NodePlayer* merged_array = merge(array_first, array_second, 10, 20);
    // for(int i = 0 ; i<30 ; i++){
    //     std::cout<<"Force: "<<merged_array[i].force<<" ,ID: "<<merged_array[i].ID<<std::endl;
    // }


// TEST FIND()
// AVL_Players tree;
//     for(int i=0 ; i<99 ; i++){
//         tree.insert(i,i);
//     }
//     for(int i = 0 ; i<99 ; i++){
//         int index = i;
//         int a = find(index , tree.get_root())->force;
//         std::cout<<a<<std::endl;
//         assert(a==i);

//     }