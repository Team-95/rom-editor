import { Player } from './player';

export class Team {
    
    team: Player [] = [];

    mOffset: Number;
	tOffset: Number;

	scoring: Number;
	rebounds: Number;
	ballControl: Number;
	defense: Number;
	overall: Number;

	tsBackgroundColor: Number;
	tsBannerColor: Number;
	tsTextColor: Number;

	courtName: String;
	location: String;
	teamName: String;

    constructor() {

    }

}