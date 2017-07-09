import { Component, OnInit } from '@angular/core';
import addon from '../../addon';

const { exec } = require('child_process');

@Component({
  selector: 'app-home',
  templateUrl: './home.component.html',
  styleUrls: ['./home.component.scss']
})
export class HomeComponent implements OnInit {
  
  inputPath: String;
  outputPath: String;
  playerStatsPath: String;
  teamStatsPath: String;

  lastSuccessfulOutput: String;

  constructor() { }

  ngOnInit() {
  }

  handleInputFileChange(e){
    this.inputPath = '';
    if(document.getElementById('inputRomFile')['files'].length > 0){
      this.inputPath = document.getElementById('inputRomFile')['files'][0].path;
    }
  }

  handleOutputFileChange(e){
    this.outputPath = '';
    if(document.getElementById('outputRomFile')['files'].length > 0){
      this.outputPath = document.getElementById('outputRomFile')['files'][0].path;
    }
  }

  handleTeamStatsFileChange(e){
    this.teamStatsPath = '';
    if(document.getElementById('teamStatsFile')['files'].length > 0){
      this.teamStatsPath = document.getElementById('teamStatsFile')['files'][0].path;
    }
  }

  handlePlayerStatsFileChange(e){
    this.playerStatsPath = '';
    if(document.getElementById('playerStatsFile')['files'].length > 0){
      this.playerStatsPath = document.getElementById('playerStatsFile')['files'][0].path;
    }
  }

  submit(f){
    try{
      addon.ApplyFilesToRom(this.inputPath, this.teamStatsPath, this.playerStatsPath, this.outputPath);
      this.lastSuccessfulOutput = this.outputPath;
    }
    catch(e){
      console.log((<Error>e).message);
      this.lastSuccessfulOutput = '';
    }
  }

  launch(){
    exec("D:\\Programs\\Fusion364\\Fusion.exe " + this.lastSuccessfulOutput, (error, stdout, stderr) => {
      if (error) {
        console.error(`exec error: ${error.message}`);
        return;
      }
      console.log(`stdout: ${stdout}`);
      console.log(`stderr: ${stderr}`);
    });
  }

}
