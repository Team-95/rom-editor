import { Player } from './player';

export class Team {
    
    players: Player [] = [];

    dataAddress: Number;
	menuAddress: Number;

	scoring: Number;
	rebounds: Number;
	ballControl: Number;
	defense: Number;
	overall: Number;

	tsBackgroundColor: Number;
	tsBannerColor: Number;
	tsTextColor: Number;

	teamName: String;
	location: String;
	courtLocation: String;
	initials: String;

	teamNameSize: Number;
	locationSize: Number;
	courtLocationSize: Number;

    constructor() {

    }

}