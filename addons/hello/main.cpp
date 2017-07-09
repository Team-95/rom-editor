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

int Apply(const char* inPath, const char* teamStatsPath, const char* playerStatsPath, const char* outPath)
{
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
			curTeam->SetPlayer(playerIndex, new Player(romRead, romWrite, playerOffset));

			int cellNumber = 0;
			while (getline(lineStream, cell, ','))
			{
				Player* curPlayer = curTeam->GetPlayer(playerIndex);
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

void ApplyFilesToRom(const FunctionCallbackInfo<Value>& args) 
{
  Isolate* isolate = args.GetIsolate();

  if(args.Length() < 4)
  {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }

  if(!args[0]->IsString() || !args[1]->IsString() || !args[2]->IsString() || !args[3]->IsString())
  {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong arguments")));
    return;
  }

  Apply(ToCString(args[0]->ToString()), ToCString(args[1]->ToString()), ToCString(args[2]->ToString()), ToCString(args[3]->ToString()));
}

void init(Local<Object> exports) 
{
  NODE_SET_METHOD(exports, "ApplyFilesToRom", ApplyFilesToRom);
}

NODE_MODULE(addon, init)
