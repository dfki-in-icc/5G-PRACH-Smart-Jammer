import { Component, OnInit } from '@angular/core';
import { Subscription } from 'rxjs';
import { Message, WebSocketService } from 'src/app/services/websocket.service';

@Component({
  selector: 'app-logger',
  templateUrl: './logger.component.html',
  styleUrls: ['./logger.component.css']
})
export class LoggerComponent implements OnInit {

  JSON = JSON

  // websocket service object and related subscription for message reception 
  wsSubscription?: Subscription;

  messages: Message[] = [];

  constructor(
    private wsService: WebSocketService
  ) { }

  ngOnInit(): void {
    this.wsSubscription = this.wsService.subject$.subscribe(
      (msg: Message) => this.messages.push(msg)
    )
  }

}
