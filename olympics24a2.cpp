#include "olympics24a2.h"

#include <exception>

olympics_t::olympics_t()
{
	// TODO: Your code goes here
	teams_table = new TeamTable;
	tournament_tree = new TournamentTree;
}

olympics_t::~olympics_t()
{
	// TODO: Your code goes here
	delete teams_table;
	delete tournament_tree;
}


StatusType olympics_t::add_team(int teamId)
{
	// TODO: Your code goes here
	if(teamId <= 0)
		return StatusType::INVALID_INPUT;
	if(teams_table->find(teamId))
		return StatusType::FAILURE;
	try{
		teams_table->insert(teamId);
	}
	catch(std::exception& object){
		return StatusType::ALLOCATION_ERROR;
	}
	return StatusType::SUCCESS;
}

StatusType olympics_t::remove_team(int teamId)
{
	// TODO: Your code goes here
	if(teamId <= 0)
		return StatusType::INVALID_INPUT;
	Team* team = teams_table->find(teamId);
	if(!team)
		return StatusType::FAILURE;
	try{
		TournamentTeam tournament_team(team->power, team->id);
		tournament_tree->remove(tournament_team);
		teams_table->remove(teamId);
	}
	catch(std::exception& object){
		return StatusType::ALLOCATION_ERROR;
	}
	return StatusType::SUCCESS;
}

StatusType olympics_t::add_player(int teamId, int playerStrength)
{
	// TODO: Your code goes here
	if(teamId <= 0 || playerStrength <= 0)
		return StatusType::INVALID_INPUT;
	Team* team = teams_table->find(teamId);
	if(!team)
		return StatusType::FAILURE;
	try{
		int old_power = team->power;
		team->insert_player(playerStrength);
		TournamentTeam team_to_update = TournamentTeam(old_power, team->id, team->medals);
		TournamentTeam newTeam = TournamentTeam(team->power, team->id, team->medals);
		if(tournament_tree->find(team_to_update)){
			tournament_tree->updatePower(newTeam, old_power);
		}
		else{
			tournament_tree->insert(newTeam);
		}
	}
	catch(std::exception& object){
		return StatusType::ALLOCATION_ERROR;
	}
	return StatusType::SUCCESS;
}

StatusType olympics_t::remove_newest_player(int teamId)
{
	// TODO: Your code goes here
	if(teamId <= 0)
		return StatusType::INVALID_INPUT;
	Team* team = teams_table->find(teamId);
	if(!team || team->players->get_size() == 0)
		return StatusType::FAILURE;
	try{
		int old_power = team->power;
		team->remove_newest_player();
		TournamentTeam team_to_update = TournamentTeam(old_power, team->id, team->medals);
		TournamentTeam new_team = TournamentTeam(team->power, team->id, team->medals);
		if(team->players->get_size() == 0){
			team->medals += tournament_tree->countWins(team_to_update);
			tournament_tree->remove(team_to_update);
		}
		else{
			tournament_tree->updatePower(new_team, old_power);
		}
	}
	catch(std::exception& object){
		return StatusType::ALLOCATION_ERROR;
	}
	return StatusType::SUCCESS;
}

output_t<int> olympics_t::play_match(int teamId1, int teamId2)
{
    // TODO: Your code goes here
	if(teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2)
		return StatusType::INVALID_INPUT;
	Team* team1 = teams_table->find(teamId1);
	Team* team2 = teams_table->find(teamId2);
	if(!team1 || !team2 || team1->players->get_size() == 0 || team2->players->get_size() == 0)
		return StatusType::FAILURE;
	int winning_team = -1;
	try{
		if(team1->power > team2->power){
			winning_team = team1->id;
		}
		else if(team2->power > team1->power){
			winning_team = team2->id;
		}
		else{
			winning_team = (team1->id < team2->id)?team1->id:team2->id;
		}
		if(winning_team == team1->id){
			team1->medals++;
			TournamentTeam team_update_rank(team1->power, team1->id, team1->medals);
			tournament_tree->updatePower(team_update_rank, team_update_rank.power);
		}
		else{
			team2->medals++;
			TournamentTeam team_update_rank(team2->power, team2->id, team2->medals);
			tournament_tree->updatePower(team_update_rank, team_update_rank.power);
		}

	}
	catch(std::exception& object){
		return StatusType::ALLOCATION_ERROR;
	}
    return winning_team;
}

output_t<int> olympics_t::num_wins_for_team(int teamId)
{
    // TODO: Your code goes here
	if(teamId <= 0)
		return StatusType::INVALID_INPUT;
	Team* team = teams_table->find(teamId);
	if(!team)
		return StatusType::FAILURE;
	int num_wins = -1;
	try{
		TournamentTeam tournament_team(team->power, team->id);
		num_wins = team->medals;
		if(team->players->get_size() > 0){
			num_wins += tournament_tree->countWins(tournament_team);
		}
	}
	catch(std::exception& object){
		return StatusType::ALLOCATION_ERROR;
	}
	return num_wins;
}

output_t<int> olympics_t::get_highest_ranked_team()
{
	// TODO: Your code goes here
	if(teams_table->get_size() == 0){
		return -1;
	}
    return tournament_tree->getMaxRank();
}

StatusType olympics_t::unite_teams(int teamId1, int teamId2)
{
	// TODO: Your code goes here
	if(teamId1 <= 0 || teamId2 <= 0 || teamId1 == teamId2)
		return StatusType::INVALID_INPUT;
	Team* team1 = teams_table->find(teamId1);
	Team* team2 = teams_table->find(teamId2);
	if(!team1 || !team2)
		return StatusType::FAILURE;
	try{
		int old_power = team1->power;
		team1->unite_players(team2);
		TournamentTeam new_team = TournamentTeam(team1->power, team1->id, team1->medals);
		TournamentTeam tournament_team(old_power, team1->id);
		int size = team1->players->get_size();
		remove_team(team2->id);
		if(tournament_tree->find(tournament_team)){
			tournament_tree->updatePower(new_team, old_power);
		}
		else if(size > 0){
			tournament_tree->insert(new_team);
		}
	}
	catch(std::exception& object){
		return StatusType::ALLOCATION_ERROR;
	}
    return StatusType::SUCCESS;
}

static bool isPowerOfTwo(int n) {
    // Check if n is non-negative and has exactly one bit set - o(1) complexity
    return (n > 0) && ((n & (n - 1)) == 0);
}

output_t<int> olympics_t::play_tournament(int lowPower, int highPower)
{
    // TODO: Your code goes here
	if(lowPower <= 0 || highPower <= 0 || highPower <= lowPower)
		return StatusType::INVALID_INPUT;

	if(!tournament_tree->getroot() || !isPowerOfTwo(tournament_tree->numberOfTeamsInRange(lowPower, highPower))){
		return StatusType::FAILURE;
	}
	int result = -1;
	try{
		result = tournament_tree->playTournament(lowPower, highPower);
	}
	catch(std::exception& object){
		return StatusType::ALLOCATION_ERROR;
	}

	return result;
}
