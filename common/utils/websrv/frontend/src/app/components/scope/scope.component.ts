import { Component, Output, EventEmitter, ViewChild} from "@angular/core";
import { FormGroup,FormControl } from "@angular/forms";
import { Message, WebSocketService, webSockSrc } from "src/app/services/websocket.service";
import { Observable } from 'rxjs';
import { Chart, ChartConfiguration, ChartOptions, ChartEvent, ChartType, ScatterDataPoint } from 'chart.js';
import { BaseChartDirective } from 'ng2-charts';
import {IGraphDesc, IScopeDesc, IScopeCmd,  ScopeApi  } from 'src/app/api/scope.api';

const SCOPEMSG_TYPE_STATUSUPD=1;   
const SCOPEMSG_TYPE_REFRATE=2;
const SCOPEMSG_TYPE_TIME=3;
const SCOPEMSG_TYPE_DATA=10;


const SCOPEMSG_DATA_IQ=1;
@Component({
  selector: 'app-scope',
  templateUrl: './scope.component.html',
  styleUrls: ['./scope.component.css'],
})

export class ScopeComponent {

  scopetitle = '';
  scopetime = '';
  scopestatus = 'stopped';
  startstop = 'start';
  rfrate = 2;
  
  channel_list = [""];
  selected_channels = [""];
  
  @Output() ScopeEnabled = new EventEmitter<boolean>();

  @ViewChild(BaseChartDirective) chart?: BaseChartDirective;

  public IQDatasets: ChartConfiguration<'scatter'>['data']['datasets'] = [
    {
      data: [],
//      label: 'Series A',
      pointRadius: 0.5,
      showLine: false,
      fill:false,
      pointStyle: 'circle',
      pointBackgroundColor:'yellow',
      pointBorderColor:'yellow',
//      parsing: false,
    },
    {
      data: [],
//      label: 'Series A',
      pointRadius: 0.5,
      showLine: false,
      pointStyle: 'circle',
      pointBackgroundColor:'cyan',
      pointBorderColor:'cyan',
//      parsing: false,
    },
    {
      data: [],
//      label: 'Series A',
      pointRadius: 0.5,      
      showLine: false,
      pointStyle: 'circle',
      pointBackgroundColor:'red',
      pointBorderColor:'red',
//      parsing: false,
    }       
  ];

  public IQOptions: ChartConfiguration<'scatter'>['options'] = {
    responsive: false,
    aspectRatio: 1,
    plugins: { legend: { display: false, },  tooltip: { enabled: false, }, },
  };
  
  constructor(private wsService: WebSocketService,public scopeApi: ScopeApi ) {
    wsService.messages.subscribe((msg: ArrayBuffer) => {
      this.ProcessScopeMsg(this.wsService.DeserializeMessage(msg));
    });
  }
  
  ngOnInit() {
  } 
 
  DecodScopeBinmsgToString(message: ArrayBuffer) {
     const enc = new TextDecoder("utf-8");
     return enc.decode(message);
  }

  configScope(resp: IScopeDesc) {
	  this.scopetitle=resp.title;
	  for (let graphIndex = 0; graphIndex < resp.graphs.length; graphIndex++) {
		  this.channel_list[graphIndex]=resp.graphs[graphIndex].title;
	  }
  }
  
  ProcessScopeMsg (message: Message) {	  
	  switch ( message.msgtype ) {		  
		  case SCOPEMSG_TYPE_STATUSUPD:
		    let msgcontent = this.DecodScopeBinmsgToString(message.content);
            if (msgcontent === 'disabled') {
		      this.ScopeEnabled.emit(false);
		      this.sendMsg(SCOPEMSG_TYPE_STATUSUPD,'disabled'); //Ack disabled message
			} else if (msgcontent === 'enabled') {
		      this.ScopeEnabled.emit(true);
		      this.scopeApi.getScopeInfos$().subscribe(resp => {
				  this.configScope(resp);
		        });
			}  else {
			  console.log("Scope received  " + msgcontent + ", unknown status update");
			}
            break; 
          case SCOPEMSG_TYPE_TIME:
            this.scopetime=this.DecodScopeBinmsgToString(message.content);
            break;
          case  SCOPEMSG_TYPE_DATA:   
            const bufferview = new DataView(message.content);
            let bl=(bufferview.byteLength)/8;//8: 32 bits floats and two parts in buffer: one half for I one half for Q
			switch (message.chartid) {
			  case SCOPEMSG_DATA_IQ:
			    for ( let i=0;i<bl; i++) {
                  this.IQDatasets[message.dataid].data[i+(message.segnum * bl)]={ x: bufferview.getFloat32(i), y: bufferview.getFloat32(i+bl)};
                }
                if(message.update) {
				  console.log("Scope update chart " + message.chartid.toString() + ", dataset " + message.dataid.toString());
                  this.chart?.update();
			    }
              break;
                default:
                break;
		      }
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
      chartid: 0,
      dataid: 0,
      segnum: 0,
      update: false,
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
      chartid: 0,
      dataid: 0,
      segnum:0,
      update: false,
      content: buffview.buffer
    };
    this.wsService.messages.next(this.wsService.SerializeMessage(message));
    console.log("Scope sent msg type " + type.toString() + " (binary content)");
  }
  
  startorstop() {
    if (this.scopestatus === 'stopped') {
        this.scopeApi.setScopeParams$({name:"startstop",value:"start"}).subscribe(
          () => {
			 this.scopestatus='started';
             this.startstop='stop';
		  },
		  err => {
		  }
        );   
    } else {
        this.scopeApi.setScopeParams$({name:"startstop",value:"stop"}).subscribe(
          () => {
			 this.scopestatus='stopped';
             this.startstop='start';
		  },
		  err => {
		  }        
        );      
    }
  }
  
  OnRefrateChange() {
	 this.scopeApi.setScopeParams$({name:"refrate",value:(this.rfrate*10).toString()}).subscribe(); 
  }
  
  channelsChanged(value: string[]) {
//	if (event.isUserInput) {
  if (value.length > 0)
    console.log(value[0]);
 //   this.selected_channels = ;
    }  
}
