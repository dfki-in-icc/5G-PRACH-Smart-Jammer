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
  scopetime = '';
  scopestatus = 'stopped';
  startstop = 'start';
  constructor(private wsService: WebSocketService) {
    wsService.messages.subscribe((msg: ArrayBuffer) => {
      this.ProcessScopeMsg(this.wsService.DeserializeMessage(msg));
    });
  }
 
  DecodScopeBinmsg(message: ArrayBuffer) {
     const enc = new TextDecoder("utf-8");
     return enc.decode(message);
  }
  
  ProcessScopeMsg (message: Message) {
      this.scopestatus='started';
      this.startstop='stop';
      this.scopetime=this.DecodScopeBinmsg(message.content);
  }
  
  sendMsg(strmessage : string) {
	const byteArray = new TextEncoder().encode(strmessage);
    let message = {
      source: webSockSrc.softscope,
      content: byteArray.buffer
    };
    this.wsService.messages.next(this.wsService.SerializeMessage(message));
  }

  startorstop() {
    if (this.scopestatus === 'stopped') {
        this.sendMsg('start');
    } else {
        this.sendMsg('stop');
    }
  }
}
