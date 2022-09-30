
import { Injectable } from '@angular/core';
import { webSocket, WebSocketSubject } from 'rxjs/webSocket';
import { environment } from "src/environments/environment";

export enum webSockSrc {
  softscope = "s".charCodeAt(0),
  logview = "l".charCodeAt(0),
}

export interface Message {
  source: webSockSrc;
  content: ArrayBuffer;
}

const deserialize = (msg: ArrayBuffer): Message => {
  const src = new DataView(msg, 0, 1); // 1 Byte header
  return {
    source: src.getUint8(0),
    content: msg.slice(1)
  };
}

const serialize = (msg: Message): ArrayBuffer => {
  let buff = new ArrayBuffer(msg.content.byteLength + 1);   // 1 Byte header
  let buffview = new DataView(buff);
  buffview.setUint8(0, msg.source);
  return buffview.buffer;
}

@Injectable()
export class WebSocketService2 {

  subject$: WebSocketSubject<Message>;

  constructor() {
    this.subject$ = webSocket<Message>({
      url: 'ws://' + environment.backend,
      openObserver: { next: () => { console.log('connection established') } },
      closeObserver: { next: () => { console.log('socket closed') } },
      serializer: msg => serialize(msg),
      deserializer: msg => deserialize(msg.data),
      binaryType: 'arraybuffer'
    });
  }

  public registerScopeSocket() {
    return this.subject$.multiplex(
      () => { console.log('scope connection established') },
      () => { console.log('scope connection closed') },
      msg => msg.source === webSockSrc.softscope, // filter
    );
  }

  public registerLogSocket() {
    return this.subject$.multiplex(
      () => { console.log('log connection established') },
      () => { console.log('log connection closed') },
      msg => msg.source === webSockSrc.logview, // filter
    );
  }

  public send(msg: Message) {
    this.subject$.next(msg); // This will send a message to the server once a connection is made. Remember value is serialized with JSON.stringify by default!
  }

  public close() {
    this.subject$.complete(); // Closes the connection.
  }

  public error(err: any) {
    this.subject$.error(err); // This will send an error message to the server + closes connection
  }
}

