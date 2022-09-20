import { Component, Output, EventEmitter, QueryList, ViewChild, ViewChildren } from "@angular/core";
import { FormGroup,FormControl } from "@angular/forms";
import { Message, WebSocketService, webSockSrc } from "src/app/services/websocket.service";
import { Observable } from 'rxjs';
import { Chart, ChartConfiguration, ChartOptions, ChartEvent, ChartType, ScatterDataPoint } from 'chart.js';
import { BaseChartDirective } from 'ng2-charts';
import {IGraphDesc, IScopeGraphType, IScopeDesc, IScopeCmd, ISigDesc,  ScopeApi  } from 'src/app/api/scope.api';

const SCOPEMSG_TYPE_STATUSUPD=1;   
const SCOPEMSG_TYPE_REFRATE=2;
const SCOPEMSG_TYPE_TIME=3;
const SCOPEMSG_TYPE_DATA=10;


const SCOPEMSG_DATA_IQ=1;
const SCOPEMSG_DATA_LLR=2;
@Component({
  selector: 'app-scope',
  templateUrl: './scope.component.html',
  styleUrls: ['./scope.component.css'],
})

export class ScopeComponent {

  scopetitle = '';
  scopesubtitle = '';
  scopetime = '';
  scopestatus = 'stopped';
  startstop = 'start';
  startstop_color = 'warn';
  rfrate = 2;
  iqmax = 32767;
  iqmin = -32767;

  iqsliderstep=10;
  iqxmin=this.iqmin;
  iqymin=this.iqmin;
  iqxmax=this.iqmax;
  iqymax=this.iqmax;  
  iqgraph_list : IGraphDesc[] = [];
  selected_channels = [""];
  selected_llrchannels = [""];
  llrgraph_list : IGraphDesc[] = [];  
  target_list : number[] = [0,1,2,3];
  selected_sig : ISigDesc ={target_id:0, antenna_id:0};

  
  @Output() ScopeEnabled = new EventEmitter<boolean>();

  //@ViewChild(BaseChartDirective) chart?: BaseChartDirective;
  @ViewChildren(BaseChartDirective) charts?: QueryList<BaseChartDirective>;
  
  public IQDatasets: ChartConfiguration<'scatter'>['data']['datasets'] = [
    {
      data: [],
      label: 'C1',
      pointRadius: 0.5,
      showLine: false,
      animation: false,
      fill:false,
      pointStyle: 'circle',
      pointBackgroundColor:'yellow',
      backgroundColor:'yellow',
      borderWidth:0,
      pointBorderColor:'yellow',
//      parsing: false,
    },
    {
      data: [],
      label: 'C2',
      pointRadius: 0.5,
      showLine: false,
      animation: false,
      pointStyle: 'circle',
      pointBackgroundColor:'cyan',
      backgroundColor:'cyan',
      borderWidth:0,
      pointBorderColor:'cyan',
//      parsing: false,
    },
    {
      data: [],
      label: 'C3',
      pointRadius: 0.5,      
      showLine: false,
      animation: false,
      pointStyle: 'circle',
      pointBackgroundColor:'red',
      backgroundColor:'red',
      borderWidth:0,
      pointBorderColor:'red',
//      parsing: false,
    }       
  ];

  public LLRDatasets: ChartConfiguration<'scatter'>['data']['datasets'] = [
    {
      data: [],
      label: 'LLR1',
      pointRadius: 0.5,
      showLine: false,
      animation: false,
      fill:false,
      pointStyle: 'circle',
      pointBackgroundColor:'yellow',
      backgroundColor:'yellow',
      borderWidth:0,
      pointBorderColor:'yellow',
 //     parsing: false,
    },
    {
      data: [],
      label: 'LL2',
      pointRadius: 0.5,
      showLine: false,
      animation: false,
      pointStyle: 'circle',
      pointBackgroundColor:'cyan',
      backgroundColor:'cyan',
      borderWidth:0,
      pointBorderColor:'cyan',
      parsing: false,
    },
    {
      data: [],
      label: 'LLR3',
      pointRadius: 0.5,      
      showLine: false,
      animation: false,
      pointStyle: 'circle',
      pointBackgroundColor:'red',
      backgroundColor:'red',
      borderWidth:0,
      pointBorderColor:'red',
      parsing: false,
    }       
  ];

  public IQOptions: ChartConfiguration<'scatter'>['options'] = {
    responsive: true,
    aspectRatio: 1,
//    scales: {
//      xAxes: {
//		  min: -5,
//		  max:5,
//      }
//    },
    plugins: { legend: { display: true, labels:{boxWidth: 10, boxHeight: 10}},  tooltip: { enabled: false, }, },
  };

  public LLROptions: ChartConfiguration<'scatter'>['options'] = {
    responsive: true,
    aspectRatio: 3,
    scales: {
      xAxes: {
		  min: 0,
      },
      yAxes: {
		  min: -100,
		  max: 100
      }     
    },
    plugins: { legend: { display: true, labels:{boxWidth: 10, boxHeight: 10}},  tooltip: { enabled: false, }, },
  };
  
  constructor(private wsService: WebSocketService,private scopeApi: ScopeApi ) {
	console.log("Scope constructor ");
    wsService.messages.subscribe((msg: ArrayBuffer) => {
      this.ProcessScopeMsg(this.wsService.DeserializeMessage(msg));
    });  
  }
  
  ngOnInit() {
     console.log("Scope ngOnInit ");
     this.SigChanged(this.selected_sig.target_id);
     this.OnRefrateChange();
     this.OnIQxminsliderChange();
     this.OnIQxmaxsliderChange();
     this.OnIQyminsliderChange();
     this.OnIQymaxsliderChange();    
  } 
 
  DecodScopeBinmsgToString(message: ArrayBuffer) {
     const enc = new TextDecoder("utf-8");
     return enc.decode(message);
  }

  configScope(resp: IScopeDesc) {
	  this.scopetitle=resp.title;
	  this.iqgraph_list.length=0;
	  this.llrgraph_list.length=0;
	  for (let graphIndex = 0; graphIndex < resp.graphs.length; graphIndex++) { 
		  if (resp.graphs[graphIndex].type == IScopeGraphType.IQs ) {
		    this.iqgraph_list.push(resp.graphs[graphIndex]);
		    this.IQDatasets[this.iqgraph_list.length - 1].label=resp.graphs[graphIndex].title;
		  }
		  if (resp.graphs[graphIndex].type == IScopeGraphType.LLR ) {
		    this.llrgraph_list.push(resp.graphs[graphIndex]);
		    this.LLRDatasets[this.llrgraph_list.length - 1].label=resp.graphs[graphIndex].title;	    
		  }
	  }
	  this.charts?.forEach((child) => {
        child.chart?.update() });		  
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
            this.IQDatasets[message.dataid].data.length=0;
            
			switch (message.chartid) {
			  case SCOPEMSG_DATA_IQ:
			    for ( let i=0;i<bufferview.byteLength; i=i+4) {
                  this.IQDatasets[message.dataid].data[i/4]={ x:bufferview.getInt16(i, true) , y: bufferview.getInt16(i+2, true)};
                }
                if(message.update) {
				  console.log("Starting scope update chart " + message.chartid.toString() + ", dataset " + message.dataid.toString());
		          this.charts?.forEach((child,index) => { child.chart?.update() });                  
                  console.log(" scope update completed chart " + message.chartid.toString() + ", dataset " + message.dataid.toString());
			    }
              break;
			  case SCOPEMSG_DATA_LLR:
			    for ( let i=0;i<bufferview.byteLength;i++) {
                  this.LLRDatasets[message.dataid].data[i]={ x: i, y: bufferview.getInt8(i)};
                }
                if(message.update) {
				  console.log("Starting scope update chart " + message.chartid.toString() + ", dataset " + message.dataid.toString());
		          this.charts?.forEach((child,index) => { child.chart?.update() });                  
                  console.log(" scope update completed chart " + message.chartid.toString() + ", dataset " + message.dataid.toString());
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
  
  SendScopeParams(name:string, value:string, graphid:number):boolean {
	let result=false;
	this.scopeApi.setScopeParams$({name:name,value:value,graphid:graphid}).subscribe(
          () => {
			 result=true;
		  },
		  err => {
			 result=false;
		  }
	);
	return result;
  }
  
  startorstop() {
    if (this.scopestatus === 'stopped') {
        this.scopeApi.setScopeParams$({name:"startstop",value:"start"}).subscribe(
          () => {
			 this.IQDatasets.forEach((dataset) => { dataset.data.length=0 }) ;
			 this.LLRDatasets.forEach((dataset) => { dataset.data.length=0 }) ;
             this.charts?.forEach((child,index) => { child.chart?.update() });  			  
			 this.scopestatus='started';
             this.startstop='stop';
             this.startstop_color='started';
		  },
		  err => {
		  }
        );   
    } else {
        this.scopeApi.setScopeParams$({name:"startstop",value:"stop"}).subscribe(
          () => {
			 this.scopestatus='stopped';
             this.startstop='start';
             this.startstop_color='warn';
             this.charts?.forEach((child,index) => { child.chart?.update() });  
		  },
		  err => {
		  }        
        );      
    }
  }
  
  OnRefrateChange() {
	 this.scopeApi.setScopeParams$({name:"refrate",value:(this.rfrate*10).toString()}).subscribe(); 
  }
  
  OnIQxminsliderChange() {
	 this.scopeApi.setScopeParams$({name:"xmin",value:(this.iqxmin).toString()}).subscribe();
  } 
  OnIQxmaxsliderChange() {
	 this.scopeApi.setScopeParams$({name:"xmax",value:(this.iqxmax).toString()}).subscribe();
  }   
  OnIQyminsliderChange() {
	 this.scopeApi.setScopeParams$({name:"ymin",value:(this.iqymin).toString()}).subscribe();
  } 
  OnIQymaxsliderChange() {
	 this.scopeApi.setScopeParams$({name:"ymax",value:(this.iqymax).toString()}).subscribe();
  }   
  channelsChanged(value: string[]) {
    this.selected_channels=value;
    for (let i=0; i<this.iqgraph_list.length; i++) {
	  let enabled="false";
	  for (let j=0; j<value.length; j++) {
		  if( this.iqgraph_list[i].title === value[j]) {
			  enabled="true";
			  break;
		  }
	  }
	  this.SendScopeParams("enabled",enabled,this.iqgraph_list[i].srvidx);	  
    } 
  }  

  llrchannelsChanged(value: string[]) {
    this.selected_llrchannels=value;
    for (let i=0; i<this.llrgraph_list.length; i++) {
	  let enabled="false";
	  for (let j=0; j<value.length; j++) {
		  if( this.llrgraph_list[i].title === value[j]) {
			  enabled="true";
			  break;
		  }
	  }
	  this.SendScopeParams("enabled",enabled,this.llrgraph_list[i].srvidx);	  
    } 
  }    
  SigChanged(value: number) {
    this.selected_sig.target_id=value;
    this.scopesubtitle=  value.toString();
	this.SendScopeParams("TargetSelect",value.toString(),0);
  }    
}
