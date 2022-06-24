// src\app\services\websocket.service.ts
import { Injectable } from "@angular/core";
import { Observable, Observer } from 'rxjs';
import { AnonymousSubject } from 'rxjs/internal/Subject';
import { Subject } from 'rxjs';
import { map } from 'rxjs/operators';

const websockurl = 'ws://' + window.location.hostname + ":" + window.location.port + "/softscope";

export enum webSockSrc {
  softscope = 1,
  logview = 2,
}
export interface Message {
    source: webSockSrc;
    content: ArrayBuffer;
}

@Injectable()
export class WebSocketService {
    private subject: AnonymousSubject<MessageEvent>;
    public messages: Subject<Message>;

    constructor() {
        this.subject = this.create(websockurl);
        console.log("Successfully connected: " + websockurl);

        this.messages = <Subject<Message>>this.subject.pipe(
            map(
                (response: MessageEvent): Message => {
                   if(response.data instanceof ArrayBuffer) {
                      console.log("Received " + response.data.byteLength.toString() + " bytes");
                      const src = new DataView(response.data, 0, 4);
                      const enc = new TextDecoder("utf-8");
                      const data = enc.decode(response.data.slice(4));
                      console.log(data);                         
                   } else {
                     console.log(response.data);
                   }
                return response.data;
                }
            )
        );
    }

    private create(url: string): AnonymousSubject<MessageEvent> {
        let ws = new WebSocket(url);
        ws.binaryType = "arraybuffer";
        let observable = new Observable((obs: Observer<MessageEvent>) => {
            ws.onmessage = obs.next.bind(obs);
            ws.onerror = obs.error.bind(obs);
            ws.onclose = obs.complete.bind(obs);
            return ws.close.bind(ws);
        });
        let observer: Observer<MessageEvent> = {
            error: (err: any) => null,
            complete: () => null,
            next: (data: Object) => {
                console.log('Message sent to websocket: ', data);
                if (ws.readyState === WebSocket.OPEN) {
                    ws.send(JSON.stringify(data));
                }
            }
        };
        return new AnonymousSubject<MessageEvent>(observer, observable);
    }
}
