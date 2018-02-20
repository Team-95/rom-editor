import { Component, OnInit, Input, OnChanges } from '@angular/core';
import { Player } from 'app/model/player';

@Component({
  selector: 'player',
  templateUrl: './player.component.html',
  styleUrls: ['./player.component.scss']
})
export class PlayerComponent implements OnInit {

  @Input() player: Player;

  constructor() { 
   
  }

  ngOnInit() {
  }

}
