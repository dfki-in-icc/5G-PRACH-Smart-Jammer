import { Component } from "@angular/core";
import { FormControl } from "@angular/forms";
import { Message, WebSocketService } from "src/app/services/websocket.service";

@Component({
  selector: 'app-scope',
  templateUrl: './scope.component.html',
  styleUrls: ['./scope.component.css'],
})
export class ScopeComponent {

  title = 'socketrv';
  messageFC = new FormControl('');
  received: Message[] = [];
  sent: Message[] = [];

  constructor(private wsService: WebSocketService) {
    wsService.messages.subscribe((msg: Message) => {
      this.received.push(msg);
      console.log("Response from websocket: " + msg);
    });
  }

  sendMsg() {
    let message = {
      source: '',
      content: ''
    };
    message.source = 'localhost';
    message.content = this.messageFC.value;

    this.sent.push(message);
    this.wsService.messages.next(message);
  }

}
