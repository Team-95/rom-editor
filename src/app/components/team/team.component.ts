import { Component, OnInit, Input } from '@angular/core';
import { Team } from 'app/model/team';
import { UIMessage, UIMessageType } from 'app/model/uiMessage';
import { Player } from 'app/model/player';

@Component({
  selector: 'team',
  templateUrl: './team.component.html',
  styleUrls: ['./team.component.scss']
})
export class TeamComponent implements OnInit {

  @Input('team') team: Team;

  noTeamMessage: UIMessage = new UIMessage('Select a team to edit', UIMessageType.warn);

  selectedPlayer: Player;

  constructor() { }

  ngOnInit() {
  }

}
