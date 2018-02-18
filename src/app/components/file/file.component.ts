import { Component, OnInit } from '@angular/core';
import { UIMessage, UIMessageType } from '../../model/uiMessage';
import { RomService } from 'app/providers/rom.service';
import addon from '../../addon';
const { exec } = require('child_process');

@Component({
  selector: 'file-page',
  templateUrl: './file.component.html',
  styleUrls: ['./file.component.scss']
})
export class FileComponent implements OnInit {

  inputPath: String;
  outputPath: String;
  playerStatsPath: String;
  teamStatsPath: String;

  loadMessage: UIMessage;

  lastSuccessfulOutput: String;

  constructor(private romService: RomService) { }

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

  load(){

    this.loadMessage = null;

    setTimeout(() => {
      try{
        this.romService.LoadROM(this.inputPath);
        this.loadMessage = new UIMessage('ROM loaded successfully', UIMessageType.success);
      }
      catch(e){
        this.loadMessage = new UIMessage('ROM could not be loaded', UIMessageType.error);
        console.log((<Error>e).message);
      }
    }, 500);

  }

  submit(){
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
