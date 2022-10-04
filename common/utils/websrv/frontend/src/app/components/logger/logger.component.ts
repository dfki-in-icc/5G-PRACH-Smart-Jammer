import { Component } from '@angular/core';
import { Subscription } from 'rxjs';
import { WebSocketService2 } from 'src/app/services/websocket2.service';
import { WebSocketService3 } from 'src/app/services/websocket3.service';

@Component({
  selector: 'app-logger',
  templateUrl: './logger.component.html',
  styleUrls: ['./logger.component.scss']
})
export class LogComponent {

  wsSubscription?: Subscription;

  constructor(
    private rxWs: WebSocketService2
  ) {
    // this.wsSubscription = this.rxWs.registerLogSocket()
  }
}
