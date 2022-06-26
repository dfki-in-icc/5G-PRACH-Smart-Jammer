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
    public messages: Subject<ArrayBuffer>;

    constructor() {
        this.subject = this.create(websockurl);
        console.log("Successfully connected: " + websockurl);

        this.messages = <Subject<ArrayBuffer>>this.subject.pipe(
            map(
                (response: MessageEvent): ArrayBuffer => {
                   if(response.data instanceof ArrayBuffer) {
                      console.log("Received binary message " + response.data.byteLength.toString() + " bytes");
                      return response.data;            
                   } else {
                     console.log(response.data);
                     return new ArrayBuffer(5);
                   }
                
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
            next: (data: any) => {
 //               console.log('Message sent to websocket: ', data);
                if (ws.readyState === WebSocket.OPEN) {
                    ws.send(data);
                }
            }
        };
        return new AnonymousSubject<MessageEvent>(observer, observable);
    }
    
    public SerializeMessage( msg: Message) : ArrayBuffer {
        let buff = new ArrayBuffer(msg.content.byteLength+4);
        let fullbuff = new Uint8Array(buff, 0, buff.byteLength).set(new Uint8Array(msg.content), 4);
        let buffview = new DataView(buff);
        buffview.setInt32(0,msg.source);
        
        return buffview.buffer;      
    }

    public DeserializeMessage( msg: ArrayBuffer) : Message {
        const src = new DataView(msg, 0, 4);

        
        
        return { source: src.getInt32(0), content: msg.slice(4)};      
    }
}
