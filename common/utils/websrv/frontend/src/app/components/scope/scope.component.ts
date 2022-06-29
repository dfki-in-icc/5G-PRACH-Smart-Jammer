import { Component, Output, EventEmitter} from "@angular/core";
import { FormGroup,FormControl } from "@angular/forms";
import { Message, WebSocketService, webSockSrc } from "src/app/services/websocket.service";
import { NgxSliderModule, Options} from '@angular-slider/ngx-slider';
import { Observable } from 'rxjs';

const SCOPEMSG_TYPE_STATUSUPD=1;   
const SCOPEMSG_TYPE_REFRATE=2;
const SCOPEMSG_TYPE_TIME=3;

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
  rfrate = 2;
  @Output() ScopeDisabled = new EventEmitter<boolean>();
  
  sliderForm: FormGroup = new FormGroup({
  sliderControl: new FormControl()
  });
  options: Options = {
    floor: 0.1,
    ceil: 3,
    step: 0.1
  };
  constructor(private wsService: WebSocketService) {
    wsService.messages.subscribe((msg: ArrayBuffer) => {
      this.ProcessScopeMsg(this.wsService.DeserializeMessage(msg));
    });
    this.sendMsg(SCOPEMSG_TYPE_STATUSUPD,'init');
  }
  
  ngOnInit() {
  } 
 
  DecodScopeBinmsg(message: ArrayBuffer) {
     const enc = new TextDecoder("utf-8");
     return enc.decode(message);
  }
  
  
  ProcessScopeMsg (message: Message) {
	  let msgcontent = this.DecodScopeBinmsg(message.content);
	  switch ( message.msgtype ) {		  
		  case SCOPEMSG_TYPE_STATUSUPD:
            if (msgcontent === 'stopped') {
              this.scopestatus='stopped';
              this.startstop='start';
            } else if (msgcontent === 'disabled') {
		      this.ScopeDisabled.emit(true);
			}
            break; 
          case SCOPEMSG_TYPE_TIME:
            this.scopestatus='started';
            this.startstop='stop';
            this.scopetime=msgcontent;
            break;
          default:
            break;
      }
  }
  
  sendMsg(type: number, strmessage : string) {
	const byteArray = new TextEncoder().encode(strmessage);
    let message = {
      source: webSockSrc.softscope,
      msgtype: type,
      content: byteArray.buffer
    };
    this.wsService.messages.next(this.wsService.SerializeMessage(message));
  }

  sendBinMsg(type: number, binmessage: number) {
    let buff = new ArrayBuffer(4);   
    let buffview = new DataView(buff);
    buffview.setUint32(0,binmessage);
    let message = {
      source: webSockSrc.softscope,
      msgtype: type,
      content: buffview.buffer
    };
    this.wsService.messages.next(this.wsService.SerializeMessage(message));
  }
  
  startorstop() {
    if (this.scopestatus === 'stopped') {
        this.sendMsg(SCOPEMSG_TYPE_STATUSUPD,'start');
    } else {
        this.sendMsg(SCOPEMSG_TYPE_STATUSUPD,'stop');
    }
  }
  
  OnRefrateChange() {
	 this.sendBinMsg(SCOPEMSG_TYPE_REFRATE,this.rfrate*10); 
  }
}
