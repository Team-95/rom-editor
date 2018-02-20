#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "player.h"
#include "team.h"
#include "helpers.h"
#include "constantsPlayer.h"
#include "constantsTeam.h"
#include "constants.h"
#include "v8_helpers.h"

#include <node.h>

using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Object;
using v8::String;
using v8::Value;
using v8::Array;
using v8::Number;

bool VerifyParameters(const FunctionCallbackInfo<Value>& args, int count) {
	Isolate* isolate = args.GetIsolate();

  	if(args.Length() != count)
  	{
    	isolate->ThrowException(Exception::TypeError(
        	String::NewFromUtf8(isolate, "Wrong number of arguments")));
    	return false;
  	}

  	for(int i = 0; i < count; i++)
  	{
		if(!args[i]->IsString())
		{
			isolate->ThrowException(Exception::TypeError(
        		String::NewFromUtf8(isolate, "Wrong arguments")));
    		return false;
		}
  	}


	return true;
}

int Apply(const char* inPath, const char* teamStatsPath, const char* playerStatsPath, const char* outPath) {
	std::ofstream logFile("logfile.txt");
	logFile << "set method called" << endl;
	FILE *tempRead;
	FILE* romRead;
	FILE* romWrite;

	tempRead = fopen(inPath, "rb");

	romWrite = fopen(outPath, "wb");
	romRead = fopen(outPath, "rb");

	ifstream playerStats(playerStatsPath);
	ifstream teamStats(teamStatsPath);

	if (!tempRead || !romWrite || !romRead || !playerStats || !teamStats)
	{
		logFile << "File could not be opened" << endl;
		return 1;
	}

	CopyFile(tempRead, romWrite);
	fclose(tempRead);

	string line;

	string curTeamAb = "";
	Team* curTeam;
	int playerIndex = 0;

	while (getline(playerStats, line))
	{
    	logFile << line << endl;
		stringstream lineStream(line);
		string cell;

		getline(lineStream, cell, ',');

		if (cell == "Tm") 
		{
			continue;
		}

		if (cell != curTeamAb && cell != "NOP")
		{
			curTeamAb = cell;

			unsigned int tOff;
			unsigned int mOff;

			TeamOffsetsFromAbbreviation(cell, tOff, mOff);

			curTeam = new Team(romRead, romWrite, tOff, mOff);
			
			string teamLine;
			while (getline(teamStats, teamLine))
			{
				playerIndex = 0;

				stringstream teamLineStream(teamLine);
				string teamCell;
				getline(teamLineStream, teamCell, ',');

				if (teamCell == curTeamAb)
				{
					int cellNumber = 0;
					while (getline(teamLineStream, teamCell, ','))
					{
						unsigned char value = StringToChar(teamCell);
						
						// For God knows what reason, these teams cannot have values above 27
						if (value > 27 &&  ( curTeamAb == "CHO" || curTeamAb == "NYK" || curTeamAb == "PHI" ))
						{
							value = 27;
						}

						switch (cellNumber)
						{
						case 0:
							curTeam->SetAttribute(TEAM_OVERALL, value);
							break;
						case 2:
							curTeam->SetAttribute(TEAM_SCORING, value);
							break;
						case 3:
							curTeam->SetAttribute(TEAM_DEFENSE, value);
							break;
						case 4:
							curTeam->SetAttribute(TEAM_REBOUNDS, value);
							break;
						case 5:
							curTeam->SetAttribute(TEAM_BALLCONTROL, value);
							break;
						}
						cellNumber++;
					}

					teamStats.seekg(0, ios::beg);
					break;
				}
			}
		}

		if (playerIndex < 12)
		{
			unsigned int playerOffset;
			unsigned char *bytes = ReadRom(romRead, curTeam->GetTeamAddress() + (4 * playerIndex), 4);
			playerOffset = BytesToInt(bytes);
			Player* curPlayer = new Player(romRead, romWrite, playerOffset);

			int cellNumber = 0;
			while (getline(lineStream, cell, ','))
			{
				switch (cellNumber)
				{
				case 1:
					curPlayer->SetName(cell.c_str());
					break;
				case 2: 
					unsigned char val;
					if (cell == "C") { val = 0; }
					else if (cell == "PF") { val = 1; }
					else if (cell == "SF") { val = 2; }
					else if (cell == "PG") { val = 3; }
					else if (cell == "SG") { val = 4; }

					curPlayer->SetAttribute(PLAYER_POSITION, val);
					break;
				case 3:
					curPlayer->SetAttribute(PLAYER_NUMBER, StringToChar(cell)); 
					break;
				case 4:
					curPlayer->SetAttribute(PLAYER_HEIGHT, StringToChar(cell));
					break;
				case 5:
					curPlayer->SetAttribute(PLAYER_WEIGHT, StringToChar(cell), true);
					break;
				//case 6:// College, need a helper function
				//	   //curPlayer->SetAttribute(PLAYER_NUMBER, StringToChar(cell));
				//	break;
				case 9:
					curPlayer->SetAttribute(PLAYER_S_GAMES, StringToShort(cell));
					break;
				case 11:
					curPlayer->SetAttribute(PLAYER_S_MINUTES, StringToShort(cell)); 
					break;
				case 12:
					curPlayer->SetAttribute(PLAYER_S_MADEGOALS, StringToShort(cell));
					break;
				case 13:
					curPlayer->SetAttribute(PLAYER_S_ATTEMPTEDGOALS, StringToShort(cell));
					break;
				case 15:
					curPlayer->SetAttribute(PLAYER_S_MADETHREES, StringToShort(cell));
					break;
				case 16:
					curPlayer->SetAttribute(PLAYER_S_ATTEMPTEDTHREES, StringToShort(cell));
					break;
				case 22:
					curPlayer->SetAttribute(PLAYER_S_MADEFREETHROWS, StringToShort(cell));
					break;
				case 23:
					curPlayer->SetAttribute(PLAYER_S_ATTEMPTEDFREETHROWS, StringToShort(cell));
					break;
				case 25:
					curPlayer->SetAttribute(PLAYER_S_OFFREBOUNDS, StringToShort(cell));
					break;
				case 27:
					curPlayer->SetAttribute(PLAYER_S_REBOUNDS, StringToShort(cell));
					break;
				case 28:
					curPlayer->SetAttribute(PLAYER_S_ASSISTS, StringToShort(cell));
					break;
				case 29:
					curPlayer->SetAttribute(PLAYER_S_STEALS, StringToShort(cell));
					break;
				case 30:
					curPlayer->SetAttribute(PLAYER_S_BLOCKS, StringToShort(cell));
					break;
				case 31:
					curPlayer->SetAttribute(PLAYER_S_TURNOVERS, StringToShort(cell));
					break;
				case 32:
					curPlayer->SetAttribute(PLAYER_S_FOULS, StringToShort(cell));
					break;
				case 33:
					curPlayer->SetAttribute(PLAYER_S_POINTS, StringToShort(cell));
					break;
				case 36:
					curPlayer->SetAttribute(PLAYER_R_GOALS, StringToChar(cell));
					break;
				case 37:
					curPlayer->SetAttribute(PLAYER_R_THREES, StringToChar(cell));
					break;
				case 38:
					curPlayer->SetAttribute(PLAYER_R_FREETHROW, StringToChar(cell));
					break;
				case 39:
					curPlayer->SetAttribute(PLAYER_R_DUNKING, StringToChar(cell));
					break;
				case 40:
					curPlayer->SetAttribute(PLAYER_R_STEALING, StringToChar(cell));
					break;
				case 41:
					curPlayer->SetAttribute(PLAYER_R_BLOCKS, StringToChar(cell));
					break;
				case 42:
					curPlayer->SetAttribute(PLAYER_R_OFFREBOUNDING, StringToChar(cell));
					break;
				case 43:
					curPlayer->SetAttribute(PLAYER_R_DEFREBOUNDING, StringToChar(cell));
					break;
				case 44:
					curPlayer->SetAttribute(PLAYER_R_PASSING, StringToChar(cell));
					break;
				case 45:
					curPlayer->SetAttribute(PLAYER_R_OFFAWARENESS, StringToChar(cell));
					break;
				case 46:
					curPlayer->SetAttribute(PLAYER_R_DEFAWARENESS, StringToChar(cell));
					break;
				case 47:
					curPlayer->SetAttribute(PLAYER_R_SPEED, StringToChar(cell));
					break;
				case 48:
					curPlayer->SetAttribute(PLAYER_R_QUICKNESS, StringToChar(cell));
					break;
				case 49:
					curPlayer->SetAttribute(PLAYER_R_JUMPING, StringToChar(cell));
					break;
				case 50:
					curPlayer->SetAttribute(PLAYER_R_DRIBLING, StringToChar(cell));
					break;
				case 51:
					curPlayer->SetAttribute(PLAYER_R_STRENGTH, StringToChar(cell));
					break;
				case 52:
					curPlayer->SetAttribute(PLAYER_HAIR, StringHexToChar(cell));
				case 53:
					curPlayer->SetAttribute(PLAYER_SKINCOLOR, StringHexToChar(cell));
				default:
					break;
				}

				cellNumber++;
			}
		}

		playerIndex++;
	}

	// Disable checksum
	WriteRom(romWrite, CHECKSUM_OVERRIDE_ADDRESS, CHECKSUM_OVERRIDE, 4);

	//Remove Jerseys
	WriteRom(romWrite, JERSEY_ENABLE_ADDRESS, JERSEY_ENABLE, 4);

	fclose(romRead);
	fclose(romWrite);
	playerStats.close();
	teamStats.close();
}

int GetTeams(Team teams[], const char* inPath) {
	FILE* romRead;
	romRead = fopen(inPath, "rb");

	if (!romRead) {
		return -1;
	}

	TeamAddressSet* addressSets = TeamAddressSet::GetAddresses();

	for(int i = 0; i < 30; i++) {
		teams[i] = Team(romRead, NULL, addressSets[i].DataAddress, addressSets[i].MenuAddress);
	}

	return 0;
}

void ApplyFilesToRomConverter(const FunctionCallbackInfo<Value>& args) {
	if(!VerifyParameters(args, 4)) {
		return;
	}

  	Apply(ToCString(args[0]->ToString()), ToCString(args[1]->ToString()), ToCString(args[2]->ToString()), ToCString(args[3]->ToString()));
}

void GetTeamsConverter(const FunctionCallbackInfo<Value>& args) {

	if(!VerifyParameters(args, 1)) {
		return;
	}

	Isolate* isolate = args.GetIsolate();
	
	Team teams[30];
	GetTeams(teams, ToCString(args[0]->ToString()));

	Local<Array> myArray = Array::New(isolate);
	for (int i = 0; i < 30; i++) {
		Local<Object> team = Object::New(isolate);
		team->Set(String::NewFromUtf8(isolate, "scoring"), v8::Number::New(isolate, teams[i].GetAttribute(TEAM_SCORING)));
		team->Set(String::NewFromUtf8(isolate, "rebounds"), v8::Number::New(isolate, teams[i].GetAttribute(TEAM_REBOUNDS)));
		team->Set(String::NewFromUtf8(isolate, "ballControl"), v8::Number::New(isolate, teams[i].GetAttribute(TEAM_BALLCONTROL)));
		team->Set(String::NewFromUtf8(isolate, "defense"), v8::Number::New(isolate, teams[i].GetAttribute(TEAM_DEFENSE)));
		team->Set(String::NewFromUtf8(isolate, "overall"), v8::Number::New(isolate, teams[i].GetAttribute(TEAM_OVERALL)));
		team->Set(String::NewFromUtf8(isolate, "teamSelectBackgroundColor"), v8::Number::New(isolate, teams[i].GetAttribute(TEAM_TS_BACKGROUND_COLOR)));
		team->Set(String::NewFromUtf8(isolate, "teamSelectBannerColor"), v8::Number::New(isolate, teams[i].GetAttribute(TEAM_TS_BANNER_COLOR)));
		team->Set(String::NewFromUtf8(isolate, "teamSelectTextColor"), v8::Number::New(isolate, teams[i].GetAttribute(TEAM_TS_TEXT_COLOR)));
		team->Set(String::NewFromUtf8(isolate, "dataAddress"), v8::Number::New(isolate, teams[i].GetTeamAddress()));
		team->Set(String::NewFromUtf8(isolate, "menuAddress"), v8::Number::New(isolate, teams[i].GetMenuAddress()));
		//team->Set(String::NewFromUtf8(isolate, "courtName"), String::NewFromUtf8(isolate, ((char*)teams[i].GetCourtName())));
		//team->Set(String::NewFromUtf8(isolate, "location"), String::NewFromUtf8(isolate, ((char*)teams[i].GetLocation())));
		team->Set(String::NewFromUtf8(isolate, "teamName"), String::NewFromUtf8(isolate, ((char*)teams[i].GetTeamName())));
		team->Set(String::NewFromUtf8(isolate, "teamNameSize"), v8::Number::New(isolate, teams[i].GetTeamNameSize()));
		team->Set(String::NewFromUtf8(isolate, "location"), String::NewFromUtf8(isolate, ((char*)teams[i].GetLocation())));
		team->Set(String::NewFromUtf8(isolate, "locationSize"), v8::Number::New(isolate, teams[i].GetLocationSize()));
		team->Set(String::NewFromUtf8(isolate, "courtLocation"), String::NewFromUtf8(isolate, ((char*)teams[i].GetCourtLocation())));
		team->Set(String::NewFromUtf8(isolate, "courtLocationSize"), v8::Number::New(isolate, teams[i].GetCourtLocationSize()));
		team->Set(String::NewFromUtf8(isolate, "initials"), String::NewFromUtf8(isolate, ((char*)teams[i].GetInitials())));
		
		Local<Array> localPlayers = Array::New(isolate);
		Player** players = teams[i].GetPlayers();

		for (int j = 0; j < 12; j++) {
			Local<Object> player = Object::New(isolate);
			player->Set(String::NewFromUtf8(isolate, "number"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_NUMBER)));
			player->Set(String::NewFromUtf8(isolate, "position"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_POSITION)));
			player->Set(String::NewFromUtf8(isolate, "experience"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_EXP)));
			player->Set(String::NewFromUtf8(isolate, "height"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_HEIGHT)));
			player->Set(String::NewFromUtf8(isolate, "weight"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_WEIGHT)));
			player->Set(String::NewFromUtf8(isolate, "university"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_UNIVERSITY)));
			player->Set(String::NewFromUtf8(isolate, "skinColor"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_SKINCOLOR)));
			player->Set(String::NewFromUtf8(isolate, "hair"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_HAIR)));
			player->Set(String::NewFromUtf8(isolate, "sGames"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_S_GAMES)));
			player->Set(String::NewFromUtf8(isolate, "sMinutes"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_S_MINUTES)));
			player->Set(String::NewFromUtf8(isolate, "sMadeGoals"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_S_MADEGOALS)));
			player->Set(String::NewFromUtf8(isolate, "sAttemptedGoals"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_S_ATTEMPTEDGOALS)));
			player->Set(String::NewFromUtf8(isolate, "sMadeThrees"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_S_MADETHREES)));
			player->Set(String::NewFromUtf8(isolate, "sAttemptedThrees"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_S_ATTEMPTEDTHREES)));
			player->Set(String::NewFromUtf8(isolate, "sMadeFreeThrows"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_S_MADEFREETHROWS)));
			player->Set(String::NewFromUtf8(isolate, "sAttemptedFreeThrows"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_S_ATTEMPTEDFREETHROWS)));
			player->Set(String::NewFromUtf8(isolate, "sOffRebounds"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_S_OFFREBOUNDS)));
			player->Set(String::NewFromUtf8(isolate, "sBlocks"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_S_BLOCKS)));
			player->Set(String::NewFromUtf8(isolate, "sRebounds"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_S_REBOUNDS)));
			player->Set(String::NewFromUtf8(isolate, "sAssists"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_S_ASSISTS)));
			player->Set(String::NewFromUtf8(isolate, "sSteals"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_S_STEALS)));
			player->Set(String::NewFromUtf8(isolate, "sTurnovers"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_S_TURNOVERS)));
			player->Set(String::NewFromUtf8(isolate, "sPoints"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_S_POINTS)));
			player->Set(String::NewFromUtf8(isolate, "sFouledOut"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_S_FOULEDOUT)));
			player->Set(String::NewFromUtf8(isolate, "sFouls"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_S_FOULS)));
			player->Set(String::NewFromUtf8(isolate, "rGoals"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_R_GOALS)));
			player->Set(String::NewFromUtf8(isolate, "rThrees"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_R_THREES)));
			player->Set(String::NewFromUtf8(isolate, "rFreeThrows"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_R_FREETHROW)));
			player->Set(String::NewFromUtf8(isolate, "rDunking"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_R_DUNKING)));
			player->Set(String::NewFromUtf8(isolate, "rStealing"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_R_STEALING)));
			player->Set(String::NewFromUtf8(isolate, "rBlocks"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_R_BLOCKS)));
			player->Set(String::NewFromUtf8(isolate, "rOffRebounding"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_R_OFFREBOUNDING)));
			player->Set(String::NewFromUtf8(isolate, "rDefRebounding"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_R_DEFREBOUNDING)));
			player->Set(String::NewFromUtf8(isolate, "rPassing"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_R_PASSING)));
			player->Set(String::NewFromUtf8(isolate, "rOffAwareness"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_R_OFFAWARENESS)));
			player->Set(String::NewFromUtf8(isolate, "rDefAwareness"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_R_DEFAWARENESS)));
			player->Set(String::NewFromUtf8(isolate, "rSpeed"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_R_SPEED)));
			player->Set(String::NewFromUtf8(isolate, "rQuickness"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_R_QUICKNESS)));
			player->Set(String::NewFromUtf8(isolate, "rJumping"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_R_JUMPING)));
			player->Set(String::NewFromUtf8(isolate, "rDribling"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_R_DRIBLING)));
			player->Set(String::NewFromUtf8(isolate, "rStrength"), v8::Number::New(isolate, players[j]->GetAttribute(PLAYER_R_STRENGTH)));
			player->Set(String::NewFromUtf8(isolate, "playerOffset"), v8::Number::New(isolate, players[j]->GetOffset()));
			player->Set(String::NewFromUtf8(isolate, "firstName"), String::NewFromUtf8(isolate, ((char*)players[j]->GetFirstName())));
			player->Set(String::NewFromUtf8(isolate, "lastName"), String::NewFromUtf8(isolate, ((char*)players[j]->GetLastName())));
			player->Set(String::NewFromUtf8(isolate, "nameSize"), v8::Number::New(isolate, players[j]->GetNameSize()));

			localPlayers->Set(j, player);
		}

		team->Set(String::NewFromUtf8(isolate, "players"), localPlayers);

		myArray->Set(i, team);
	}

	args.GetReturnValue().Set(myArray);
}

void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "ApplyFilesToRom", ApplyFilesToRomConverter);
  NODE_SET_METHOD(exports, "GetTeams", GetTeamsConverter);
}

NODE_MODULE(addon, init)
