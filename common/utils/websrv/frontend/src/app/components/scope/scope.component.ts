import { Component } from "@angular/core";
import { FormControl } from "@angular/forms";
import { Message, WebSocketService, webSockSrc } from "src/app/services/websocket.service";

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
	let strcontent = this.messageFC.value;
	const byteArray = new TextEncoder().encode(strcontent);
    let message = {
      source: webSockSrc.softscope,
      content: byteArray.buffer
    };
    

    this.sent.push(message);
    this.wsService.messages.next(message);
  }

}
