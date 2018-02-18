
import { Injectable } from '@angular/core';
import { ROM } from '../model/rom';
import { Team } from '../model/team';
import addon from '../addon';
import { Player } from 'app/model/player';

@Injectable()
export class RomService {

    rom: ROM;

    constructor() {
       
    }

    LoadROM(path: String): ROM {
        this.rom = new ROM();

        let teamsDto = addon.GetTeams(path);

        for (let teamDto of teamsDto) {
            let team = new Team();

            team.ballControl = teamDto.ballControl;
            team.dataAddress = teamDto.dataAddress;
            team.defense = teamDto.defense;
            team.menuAddress = teamDto.menuAddress;
            team.overall = teamDto.overall;
            team.rebounds = teamDto.rebounds;
            team.scoring = teamDto.scoring;
            team.tsBackgroundColor = teamDto.teamSelectBackgroundColor;
            team.tsBannerColor = teamDto.teamSelectBannerColor;
            team.tsTextColor = teamDto.teamSelectTextColor;

            // Missing team team, location, courtname

            for (let playerDto of teamDto.players) {
                let player = new Player();

                player.experience = playerDto.experience;
                player.firstName = playerDto.firstName;
                player.lastName = playerDto.lastName;
                player.hair = playerDto.hair;
                player.height = playerDto.height + 5; // ROM has it in inches, 5 less than actual
                player.lastName = playerDto.lastName;
                player.nameSize = playerDto.nameSize - 1; // Wo don't need to worry about the 0
                player.number = playerDto.number;
                player.playerOffset = playerDto.playerOffset;
                player.position = playerDto.position;

                player.rBlocks = playerDto.rBlocks;
                player.rDefAwareness = playerDto.rDefAwareness;
                player.rDefRebounding = playerDto.rDefRebounding;
                player.rDribling = playerDto.rDribling;
                player.rDunking = playerDto.rDunking;
                player.rFreeThrows = playerDto.rFreeThrows;
                player.rGoals = playerDto.rGoals;
                player.rJumping = playerDto.rJumping;
                player.rOffAwareness = playerDto.rOffAwareness;
                player.rOffRebounding = playerDto.rOffRebounding;
                player.rPassing = playerDto.rPassing;
                player.rQuickness = playerDto.rQuickness;
                player.rSpeed = playerDto.rSpeed;
                player.rStealing = playerDto.rStealing;
                player.rStrength = playerDto.rStrength;
                player.rPassing = playerDto.rPassing;
                player.rThrees = playerDto.rThrees;
                
                player.sAssists = playerDto.sAssists;
                player.sAttemptedFreeThrows = playerDto.sAttemptedFreeThrows;
                player.sAttemptedGoals = playerDto.sAttemptedGoals;
                player.sAttemptedThrees = playerDto.sAttemptedThrees;
                player.sBlocks = playerDto.sBlocks;
                player.sGames = playerDto.sGames;
                player.sMadeFreeThrows = playerDto.sMadeFreeThrows;
                player.sMadeGoals = playerDto.sMadeGoals;
                player.sMadeThrees = playerDto.sMadeThrees;
                player.sMinutes = playerDto.sMinutes;
                player.sOffRebounds = playerDto.sOffRebounds;
                player.sPoints = playerDto.sPoints;
                player.sRebounds = playerDto.sRebounds;
                player.sSteals = playerDto.sSteals;
                player.sTurnovers = playerDto.sTurnovers;

                team.players.push(player);
            }

            this.rom.teams.push(team);
        }

        console.log(teamsDto);
        console.log(this.rom);

        return this.rom;
    }

}