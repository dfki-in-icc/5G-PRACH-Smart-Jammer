
import { Injectable } from '@angular/core';
import { webSocket } from 'rxjs/webSocket';
import { environment } from "src/environments/environment";

const subject$ = webSocket<ArrayBuffer>({
    url: 'ws://' + environment.backend,
    openObserver: { next: () => { console.log('connection established') } },
    closeObserver: { next: () => { console.log('socket closed') } },
    // serializer: (msg: Uint8Array) => {
    //     const offset = msg.byteOffset;
    //     const length = msg.byteLength;
    //     return msg.buffer.slice(offset, offset + length)
    // },
    // deserializer: msg => new Uint8Array(msg.data as ArrayBuffer),
    binaryType: 'arraybuffer'
});

export enum WebSockSrc {
    softscope = "s".charCodeAt(0),
    logview = "l".charCodeAt(0),
}

export interface MyMessage {
    source: WebSockSrc; // header 1 Byte
    content: ArrayBuffer;
}

export const arraybuf_data_offset = 1; //Bytes

@Injectable()
export class RxWsService {

    constructor() { }

    public register() {
        return subject$.subscribe(
            msg => console.log('message received: ' + msg), // next
            err => console.log(err), // error
            () => console.log('complete') // complete
        );
    }

    public unregister() {
        subject$.unsubscribe(); // the connection stays open if other subscriptions
    }

    public close() {
        subject$.complete(); // Closes the connection.
    }

    public send(msg: any) {
        subject$.next(msg); // This will send a message to the server once a connection is made. Remember value is serialized with JSON.stringify by default!
    }

    public error(code: string, reason: string) {
        subject$.error({ code: code, reason: reason }); // This will send an error message to the server + closes connection
    }

    static DeserializeMessage(msg: ArrayBuffer): MyMessage {
        const src = new DataView(msg, 0, arraybuf_data_offset);
        return {
            source: src.getUint8(0),
            content: msg.slice(arraybuf_data_offset)
        };
    }

    static SerializeMessage(msg: MyMessage): ArrayBuffer {
        let buff = new ArrayBuffer(msg.content.byteLength + arraybuf_data_offset);   // 1 Byte header 
        let buffview = new DataView(buff);
        buffview.setUint8(0, msg.source);
        return buffview.buffer;
    }
}

