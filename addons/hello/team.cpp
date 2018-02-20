#include "team.h"

Team::Team(){}

Team::Team(FILE* romR, FILE* romW, unsigned int teamAddress, unsigned int menuAddress) {
	romRead = romR;
	romWrite = romW;
	tOffset = teamAddress;
	mOffset = menuAddress;

	scoring = BytesToChar(ReadRom(romRead, tOffset + TEAM_SCORING));
	rebounds = BytesToChar(ReadRom(romRead, tOffset + TEAM_REBOUNDS));
	ballControl = BytesToChar(ReadRom(romRead, tOffset + TEAM_BALLCONTROL));
	defense = BytesToChar(ReadRom(romRead, tOffset + TEAM_DEFENSE));
	overall = BytesToChar(ReadRom(romRead, tOffset + TEAM_OVERALL));

	tsBackgroundColor = BytesToChar(ReadRom(romRead, tOffset + TEAM_TS_BACKGROUND_COLOR));
	tsBannerColor = BytesToChar(ReadRom(romRead, tOffset + TEAM_TS_BANNER_COLOR));
	tsTextColor = BytesToChar(ReadRom(romRead, tOffset + TEAM_TS_TEXT_COLOR));

	teamNameSize = 0;
	locationSize = 0;
	courtLocationSize = 0;

	// Name
	unsigned char * teamNameAddressArray = ReadRom(romRead, tOffset + TEAM_TEAMNAME_ADDRESS, 4);
	unsigned int teamNameAddress = BytesToInt(teamNameAddressArray);
	teamNameSize = GetText(teamNameAddress, romRead, teamName);

	// Location
	unsigned char * locationAddressArray = ReadRom(romRead, tOffset + TEAM_TEAMLOCATION_ADDRESS, 4);
	unsigned int locationAddress = BytesToInt(locationAddressArray);
	locationSize = GetText(locationAddress, romRead, location);

	// Court location
	unsigned char * courtLocationAddressArray = ReadRom(romRead, tOffset + TEAM_COURTLOCATION_ADDRESS, 4);
	unsigned int courtLocationAddress = BytesToInt(courtLocationAddressArray);
	courtLocationSize = GetText(courtLocationAddress, romRead, courtLocation);

	// Initials
	GetText(tOffset + TEAM_INITIALS_ADDRESS, romRead, initials, 4);

	for (int i = 0; i < 12; i++) {
		unsigned char * playerAddressArray = ReadRom(romRead, tOffset + TEAM_PLAYER1 + (i * 4), 4);
		unsigned int playerAddress = BytesToInt(playerAddressArray);
		//delete[] playerAddressArray;
		players[i] = new Player(romRead, NULL, playerAddress);
	}
}

//Team::Team(FILE* romR, FILE* romW, unsigned int &teamAddress, unsigned int menuAddress){}

unsigned int Team::GetMenuAddress(){ return mOffset; }
unsigned int Team::GetTeamAddress(){ return tOffset; }

unsigned short Team::GetAttribute(unsigned int attribute) {
	switch (attribute){
	case TEAM_SCORING:
		return scoring;
	case TEAM_REBOUNDS:
		return rebounds;
	case TEAM_BALLCONTROL:
		return ballControl;
	case TEAM_DEFENSE:
		return defense;
	case TEAM_OVERALL:
		return overall;
	case TEAM_TS_BACKGROUND_COLOR:
		return tsBackgroundColor;
	case TEAM_TS_BANNER_COLOR:
		return tsBannerColor;
	case TEAM_TS_TEXT_COLOR:
		return tsTextColor;
	}
	return 0;
}

void Team::SetAttribute(unsigned int attribute, unsigned char value) {
	switch (attribute){
	case TEAM_SCORING:
		scoring = value;
		break;
	case TEAM_REBOUNDS:
		rebounds = value;
		break;
	case TEAM_BALLCONTROL:
		ballControl = value;
		break;
	case TEAM_DEFENSE:
		defense = value;
		break;
	case TEAM_OVERALL:
		overall = value;
		break;
	case TEAM_TS_BACKGROUND_COLOR:
		tsBackgroundColor = value;
		break;
	case TEAM_TS_BANNER_COLOR:
		tsBannerColor = value;
		break;
	case TEAM_TS_TEXT_COLOR:
		tsTextColor = value;
		break;
	}
	WriteRom(romWrite, tOffset + attribute, &value);
}

unsigned char* Team::GetLocation() {
	return location;
}

int Team::GetLocationSize() {
	return locationSize;
}

unsigned char* Team::GetTeamName() {
	return teamName;
}

int Team::GetTeamNameSize() { return teamNameSize; }

unsigned char* Team::GetCourtLocation() {
	return courtLocation;
}

int Team::GetCourtLocationSize() {
	return courtLocationSize;
}

unsigned char* Team::GetInitials() {
	return initials;
}

Player* Team::GetPlayer(int index) {
	return players[index];
}

Player** Team::GetPlayers() {
	return players;
}
 
//void Team::SetMenuAddress(unsigned int off){
//	mOffset = off;
//
//	// Write the team address to the menu location
//	WriteRom(romWrite, mOffset, IntToBytes(tOffset), 4);
//}
//void Team::SetTeamAddress(unsigned int off){
//	tOffset = off;
//
//	// Write the team address to the menu location
//	WriteRom(romWrite, mOffset, IntToBytes(tOffset), 4);
//}