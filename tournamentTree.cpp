#include <iostream>
#include "tournamentTree.h"
using namespace std;




Node::Node(TournamentTeam& team, int size, int height, Node* l ,Node* r, int accumulated_medals):
    team(team), size(size), height(height), left(l), right(r), accumulated_medals(accumulated_medals), rankMaxTeam(accumulated_medals + team.rank){}

bool operator<(const TournamentTeam& team_1, const TournamentTeam& team_2){
    if(team_1.power < team_2.power)return true;
    if(team_1.power == team_2.power)return team_1.id > team_2.id;
    return false;
}
bool operator==(const TournamentTeam& team_1, const TournamentTeam& team_2){
    return team_1.power == team_2.power && team_1.id == team_2.id;
}
ostream& operator<<(std::ostream& os, Node* team){
    os << "power: " << team->team.power <<" id: " << team->team.id << " accumulated_medals: " << team->accumulated_medals << " maxRank: " << team->rankMaxTeam<< endl;
    return os;
}

TournamentTree::TournamentTree():root(nullptr), num_of_elements(0){}

void destroy_tree(Node* current){
    if(current == nullptr)return;
    destroy_tree(current->left);
    destroy_tree(current->right);
    delete current;
}

TournamentTree::~TournamentTree(){
    destroy_tree(root);
    root = nullptr;
}

Node* TournamentTree::getroot(){
    return root;
}

void inorder(Node* curr){
    if(curr == nullptr)return;
    inorder(curr->left);
    cout << curr << endl;
    inorder(curr->right);
}

int getHeight(Node* n){
    if(n == nullptr)return 0;
    return n->height;
}
int size(Node* n){
    if(n == nullptr)return 0;
    return n->size;
}
int getMaxRank(Node* n){
    if(n == nullptr)return 0;
    return n->rankMaxTeam;
}
int BF(Node* n){
    if(n == nullptr)return 0;
    return getHeight(n->left) - getHeight(n->right);
}
void fixMax(Node* n){
    if(!n)return;
    if(n->left && n->right)n->rankMaxTeam = max(max(getMaxRank(n->left), getMaxRank(n->right)), n->team.rank)+n->accumulated_medals;
    else if(n->left)n->rankMaxTeam = max(getMaxRank(n->left), n->team.rank) +n->accumulated_medals;
    else if(n->right)n->rankMaxTeam = max(getMaxRank(n->right), n->team.rank) +n->accumulated_medals;
    else n->rankMaxTeam = n->team.rank+n->accumulated_medals;
}
void fixAll(Node* n){
    if(!n)return;
    fixMax(n);
    n->height = max(getHeight(n->left), getHeight(n->right))+1;
    n->size = 1+size(n->left) + size(n->right);
}
void setExtra(Node* node, TournamentTeam& team, int* addExtra){
    if(node == nullptr)return;
    *addExtra -= node->accumulated_medals;
    fixMax(node);
}
int getIndex(Node* root, int find){//find index starts from 1! to call use getIndex()--
    if(root == nullptr)return 0;
    else if(find < root->team.power)return getIndex(root->left, find);
    else return 1+ size(root->left)+getIndex(root->right, find);
}
int getIndexOfTeam(Node* root, TournamentTeam& find){//find index starts from 1! to call use getIndex()--
    if(root == nullptr)return 0;
    else if(find < root->team)return getIndexOfTeam(root->left, find);
    else return 1+ size(root->left)+getIndexOfTeam(root->right, find);
}
Node* minNode(Node* root, int* add){
    Node* temp = root;
    *add += temp->accumulated_medals;
    while(temp->left != nullptr){
        temp = temp->left;
        *add += temp->accumulated_medals;
    }
    return temp;
}
Node* maxNode(Node* root, int* add){
    Node* temp = root;
    *add += temp->accumulated_medals;
    while(temp->right != nullptr){
        temp = temp->right;
        *add += temp->accumulated_medals;
    }
    return temp;
}

int TournamentTree::findLowIndex(int power){
    if(!root){
        return -1;
    }
    int i = 0;
    if(maxNode(root, &i)->team.power < power)return -1;
    return getIndex(root, power-1);
}
        
int TournamentTree::findHightIndex(int power){
    if(!root){
        return -1;
    }
    int i = 0;
    if(minNode(root, &i)->team.power > power)return -1;
    return getIndex(root, power);
}


int TournamentTree::numberOfTeamsInRange(int lowPower, int highPower){
    int low_index = findLowIndex(lowPower);
    int high_index = findHightIndex(highPower);
    if(low_index == -1 || high_index == -1){
        return -1;
    }
    return high_index - low_index;
}

void countExtra(Node* root, TournamentTeam& team, int* result){
    if(root == nullptr)return;
    *result += root->accumulated_medals;
    if(root->team == team)return;
    else if(team < root->team)countExtra(root->left, team, result);
    else countExtra(root->right, team, result);
}

void addUntilIndex(Node* node, int index, int add, bool* went_right){
    if(node == nullptr)return;
    int leftSize = size(node->left);
    if(index == leftSize){
        if(!(*went_right)){
            node->accumulated_medals += add;
            *went_right = true;
        }
        if(node->right){
            node->right->accumulated_medals -= add;
            fixMax(node->right);
        }
    }
    else if(index < leftSize){
        if(*went_right){
            node->accumulated_medals -= add;
            *went_right = false;
        }
        addUntilIndex(node->left, index, add, went_right);
    }
    else{
        if(!(*went_right)){
            node->accumulated_medals += add;
            *went_right = true;
        }
        addUntilIndex(node->right, index-leftSize-1, add, went_right);
    }
    fixMax(node);
}
void TournamentTree::addOneWinFromItoJ(int i, int j){
    bool temp = false;
    if(j > -1)addUntilIndex(this->root, j, 1, &temp);
    temp = false;
    if(i-1 > -1)addUntilIndex(this->root, i-1, -1, &temp);
}

Node* LeftRoll(Node* n){
    Node* one = n->right;
    int extraL = 0;
    if(one){
        extraL = one->accumulated_medals;
        one->accumulated_medals += n->accumulated_medals;
    }
    if(n)n->accumulated_medals = -extraL;
    Node* two = one->left;
    if(two)two->accumulated_medals += extraL;
    one->left = n;
    n->right = two;
    fixAll(two);
    fixAll(n);
    fixAll(one);
    return one;
}
Node* RightRoll(Node* n){
    Node* one = n->left;
    int extraR = 0;
    if(one){
        extraR = n->left->accumulated_medals;
        one->accumulated_medals += n->accumulated_medals;
    }
    if(n)n->accumulated_medals = -extraR;
    Node* two = one->right;
    if(two)two->accumulated_medals += extraR;
    one->right = n;
    n->left = two;
    fixAll(two);
    fixAll(n);
    fixAll(one);
    return one;
}

Node* balance(Node* root){
    if(root == nullptr)return nullptr;
    fixAll(root);
    int balanceFactor = BF(root);
    if(balanceFactor > 1 && BF(root->left) >= 0)return RightRoll(root); //ll
    if(balanceFactor < -1 && BF(root->right) <= 0)return LeftRoll(root);//rr
    if(balanceFactor > 1 && BF(root->left) == -1){
        root->left = LeftRoll(root->left);
        return RightRoll(root);//lr
    }
    if(balanceFactor < -1 && BF(root->right) == 1){
        root->right = RightRoll(root->right);
        return LeftRoll(root);//rl
    }
    return root;
}

Node* insert_tree(Node* root, TournamentTeam& new_team, bool* success, int* addExtra){
    if(root == nullptr){
        return new Node(new_team, 1, 1, nullptr, nullptr, (*addExtra));
    }
    if(new_team == root->team){
        *success = false;
        return root;
    }
    else if(new_team < root->team){
        root->left = insert_tree(root->left,new_team, success, addExtra);
    }
    else {
        root->right = insert_tree(root->right, new_team, success,addExtra);
    }
    return balance(root);
}
bool TournamentTree::insert(TournamentTeam& team){
    
    bool success = true;
    int addExtra = 0;
    countExtra(root, team, &addExtra);
    addExtra*=-1;
    root = insert_tree(root, team, &success, &addExtra);
    if(success)num_of_elements++;
    return success;
}

bool find_node(Node* root, TournamentTeam& team){
    if(root == nullptr)
        return false;
    if(root->team == team){
        return true;
    }
    if(root->team < team){
        return find_node(root->right, team);
    }
    else{
        return find_node(root->left, team);
    }
}

bool TournamentTree::find(TournamentTeam& t){
    return find_node(root, t);
}

void remove_node(Node*& root){
    Node* temp;
    if(root->left != nullptr)temp = root->left;
    else temp = root->right;
    if(temp == nullptr){
        temp = root;
        root = nullptr;
    }
    else {
        int tempExtra = root->accumulated_medals + temp->accumulated_medals;
        *root = *temp;
        root->accumulated_medals = tempExtra;
    }
    delete temp;
}

void swapForRemove(Node* root, Node* temp, int addExtraToRoot){
    int tempExtra = root->accumulated_medals;
    root->team = temp->team;
    root->accumulated_medals = addExtraToRoot;
    root->left->accumulated_medals = (tempExtra + root->left->accumulated_medals) - addExtraToRoot;
    root->right->accumulated_medals = (tempExtra + root->right->accumulated_medals) - addExtraToRoot;
    fixMax(root->left);
    fixMax(root->right);
    fixMax(root);
}


Node* find_delete(Node* root, TournamentTeam& team, bool* success){
    if(root == nullptr)return root;
    if(team < root->team)root->left = find_delete(root->left, team, success);
    else if(team == root->team){
        *success = true;
        if(root->left == NULL || root->right == NULL){
            remove_node(root);
        }
        else{
            int addExtraToRoot = root->accumulated_medals;
            Node* temp = minNode(root->right, &addExtraToRoot);
            swapForRemove(root, temp, addExtraToRoot);
            root->right = find_delete(root->right, temp->team, success);
        }
    }
    else root->right = find_delete(root->right, team, success);
    return balance(root);
}
bool TournamentTree::remove(TournamentTeam& team){
    bool success = false;
    root = find_delete(this->root, team, &success);
    if(success)num_of_elements--;
    return success;
}

int TournamentTree::countWins(TournamentTeam& team){
    int res = 0;
    countExtra(this->root, team, &res);
    return res;
}

int TournamentTree::getMaxRank(){
    if(!root)return 0;
    return root->rankMaxTeam;
}

void find_index_for_id(Node* root, int index, int* Id){
    if(root == nullptr)return;
    int leftSize = size(root->left);
    if(index == leftSize){
        *Id = root->team.id;
        return;
    }
    else if(index < leftSize)find_index_for_id(root->left, index, Id);
    else find_index_for_id(root->right, index - leftSize - 1, Id);
}

int TournamentTree::playTournament(int lowPower, int highPower){
    int low_index = findLowIndex(lowPower);
	int high_index = findHightIndex(highPower) - 1;
    int middle_index = (high_index + low_index) / 2 + 1;
    if(low_index == high_index){
        return IdOfTeamAtIndex(high_index);
    }
	while(middle_index <= high_index){
        addOneWinFromItoJ(middle_index, high_index);
        middle_index = (high_index + middle_index) / 2 + 1;
    } 
    return IdOfTeamAtIndex(high_index);
}

int TournamentTree::IdOfTeamAtIndex(int index){
    int result = -1;
    find_index_for_id(root, index, &result);
    return result;
}

void Extrabefore(Node* root, TournamentTeam& team, int* result){
    if(root == nullptr)return;
    if(root->team == team)return;
    *result += root->accumulated_medals;
    if(team < root->team)Extrabefore(root->left, team, result);
    if(root->team < team)Extrabefore(root->right, team, result);
}


void TournamentTree::updatePower(TournamentTeam& team, int oldPower){//TODO fix
    TournamentTeam old(oldPower,team.id);
    int wins = countWins(old);
    remove(old);
    insert(team);
    bool success = false;
    int index = getIndexOfTeam(root, team)-1;
    addUntilIndex(root, index, wins, &success);
    success = false;
    addUntilIndex(root, index-1, -wins, &success);
}

