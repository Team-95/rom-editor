import { Component, OnInit } from '@angular/core';
import { RomService } from '../../providers/rom.service';
import { UIMessage, UIMessageType } from '../../model/uiMessage';
import { Team } from 'app/model/team';

@Component({
  selector: 'stats-page',
  templateUrl: './stats.component.html',
  styleUrls: ['./stats.component.scss']
})
export class StatsComponent implements OnInit {

  noROMMessage: UIMessage = new UIMessage('No ROM loaded. Please load a ROM.', UIMessageType.warn);
  selectedTeam: Team;

  constructor(private romService: RomService) {
    
  }

  ngOnInit() {

  }

  selectTeam(team: Team) {
    this.selectedTeam = team;
  }

}
