#ifndef TOURNAMENT_H
#define TOURNAMENT_H

struct TournamentTeam{
    int power;
    int id;
    int rank;
    TournamentTeam(int power, int id, int medals = 0):power(power), id(id), rank(power+medals){}
};


struct Node{
    TournamentTeam team;
    int size;
    int height;
    Node* left;
    Node* right;
    int accumulated_medals;
    int rankMaxTeam;
    Node(TournamentTeam& t, int size, int height, Node* l ,Node* r, int accumulated_medals);
};

class TournamentTree{
    Node* root;
    int num_of_elements;
    public:
        TournamentTree();
        ~TournamentTree();
        Node* getroot();
        bool insert(TournamentTeam& t);
        bool remove(TournamentTeam& t);
        void addOneWinFromItoJ(int i, int j);
        int playTournament(int lowPower, int highPower);
        int countWins(TournamentTeam& t);
        int getMaxRank();
        int findLowIndex(int power);
        int findHightIndex(int power);
        int IdOfTeamAtIndex(int index);
        bool find(TournamentTeam& t);
        int numberOfTeamsInRange(int lowPower, int highPower);
        void updatePower(TournamentTeam& t, int oldPower);
};




#endif