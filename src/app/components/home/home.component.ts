import { Component, OnInit } from '@angular/core';
import addon from '../../addon';
import { UIMessage, UIMessageType } from '../../model/uiMessage';

enum MenuItem {
  ROM = 1,
  Stats = 2,
  Emulator = 3
}

@Component({
  selector: 'app-home',
  templateUrl: './home.component.html',
  styleUrls: ['./home.component.scss']
})
export class HomeComponent implements OnInit {

  sectionEnum = MenuItem;
  selectedSection: MenuItem = MenuItem.ROM;

  constructor() { }

  ngOnInit() {
  }

  handleSectionClick(choice: MenuItem){
    this.selectedSection = choice;
  }

}
