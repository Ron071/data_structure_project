// 
// 234218 Data Structures 1.
// Semester: 2024A (winter).
// Wet Exercise #2.
// 
// Recommended TAB size to view this file: 8.
// 
// The following header file contains all methods we expect you to implement.
// You MAY add private methods and fields of your own.
// DO NOT erase or modify the signatures of the public methods.
// DO NOT modify the preprocessors in this file.
// DO NOT use the preprocessors in your other code files.
// 

#ifndef OLYMPICSA2_H_
#define OLYMPICSA2_H_
#include "wet2util.h"
#include "tournamentTree.h"
#include "teamsTable.h"

class olympics_t {
private:
	//
	// Here you may add anything you want
	//
    TeamTable *teams_table;
    TournamentTree *tournament_tree;

public:
	// <DO-NOT-MODIFY> {

    olympics_t();

	virtual ~olympics_t();

    StatusType add_team(int teamId);

    StatusType remove_team(int teamId);

    StatusType add_player(int teamId, int playerStrength);

    StatusType remove_newest_player(int teamId);

    output_t<int> play_match(int teamId1, int teamId2);

    output_t<int> num_wins_for_team(int teamId);

    output_t<int> get_highest_ranked_team();

    StatusType unite_teams(int teamId1, int teamId2);

    output_t<int> play_tournament(int lowPower, int highPower);
	
	// } </DO-NOT-MODIFY>
};

#endif // OLYMPICSA2_H_
