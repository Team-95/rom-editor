import { Component, OnInit } from '@angular/core';
import { UIMessage, UIMessageType } from '../../model/uiMessage';
import { Input } from '@angular/core';

enum MenuItem {
  ROM = 1,
  Stats = 2,
  Emulator = 3
}

@Component({
  selector: 'ui-message',
  templateUrl: './message.component.html',
  styleUrls: ['./message.component.scss']
})
export class MessageComponent implements OnInit {

  @Input('message') message: UIMessage;
  
  constructor() { 

  }

  ngOnInit() {

  }

  getColor(): String {
    switch (this.message.type) {
      case UIMessageType.info: return 'black';
      case UIMessageType.error: return 'red';
      case UIMessageType.success: return 'green';
      case UIMessageType.warn: return 'yellow';
      default: return 'black';
    }

  }

}
