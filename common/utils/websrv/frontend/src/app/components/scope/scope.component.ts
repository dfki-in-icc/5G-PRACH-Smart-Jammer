import { Component, Output, EventEmitter} from "@angular/core";
import { UntypedFormGroup,UntypedFormControl } from "@angular/forms";
import { Message, WebSocketService, webSockSrc } from "src/app/services/websocket.service";
import { NgxSliderModule, Options} from '@angular-slider/ngx-slider';
import { Observable } from 'rxjs';
import { ChartConfiguration, ScatterDataPoint } from 'chart.js';

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
  @Output() ScopeEnabled = new EventEmitter<boolean>();
  sliderForm: UntypedFormGroup = new UntypedFormGroup({
  sliderControl: new UntypedFormControl()
  });
  options: Options = {
    floor: 0.1,
    ceil: 3,
    step: 0.1
  };
  
  public IQDatasets: ChartConfiguration<'scatter'>['data']['datasets'] = [
    {
      data: [
        { x: 1, y: 1 },
        { x: 2, y: 3 },
        { x: 3, y: -2 },
        { x: 4, y: 4 },
        { x: 5, y: -3},
      ],
      label: 'Series A',
      pointRadius: 1,
    },
  ];

  public IQOptions: ChartConfiguration<'scatter'>['options'] = {
    responsive: false,
  };
  
  constructor(private wsService: WebSocketService) {
    wsService.messages.subscribe((msg: ArrayBuffer) => {
      this.ProcessScopeMsg(this.wsService.DeserializeMessage(msg));
    });
  }
  
  ngOnInit() {
  } 
 
  DecodScopeBinmsg(message: ArrayBuffer) {
     const enc = new TextDecoder("utf-8");
     return enc.decode(message);
  }
  
  
  ProcessScopeMsg (message: Message) {
	  let msgcontent = this.DecodScopeBinmsg(message.content);
	  console.log("Scope received msg type " + message.msgtype.toString() + " " + message.content);
	  switch ( message.msgtype ) {		  
		  case SCOPEMSG_TYPE_STATUSUPD:
            if (msgcontent === 'stopped') {
              this.scopestatus='stopped';
              this.startstop='start';
            } else if (msgcontent === 'disabled') {
		      this.ScopeEnabled.emit(false);
		      this.sendMsg(SCOPEMSG_TYPE_STATUSUPD,'disabled'); //Ack disabled message
			} else if (msgcontent === 'enabled') {
		      this.ScopeEnabled.emit(true);
		      this.sendMsg(SCOPEMSG_TYPE_STATUSUPD,'init');
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
    console.log("Scope sent msg type " + type.toString() + " " + strmessage);
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
    console.log("Scope sent msg type " + type.toString() + " (binary content)");
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
