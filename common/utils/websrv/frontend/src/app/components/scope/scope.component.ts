import { Component, Output, EventEmitter, QueryList, ViewChild, ViewChildren} from "@angular/core";
import { FormGroup,FormControl } from "@angular/forms";
import { Message, WebSocketService, webSockSrc } from "src/app/services/websocket.service";
import { Observable, Subscription } from 'rxjs';
import { Chart, ChartConfiguration, ChartOptions, ChartEvent, ChartType, ScatterDataPoint } from 'chart.js';
import { BaseChartDirective } from 'ng2-charts';
import {IGraphDesc, IScopeGraphType, IScopeDesc, IScopeCmd, ISigDesc,  ScopeApi  } from 'src/app/api/scope.api';

/*------------------------------------*/
/* constants that must match backend (websrv.h or phy_scope.h) */
const SCOPEMSG_TYPE_STATUSUPD=1;   
const SCOPEMSG_TYPE_REFRATE=2;
const SCOPEMSG_TYPE_TIME=3;
const SCOPEMSG_TYPE_DATA=10;
const SCOPEMSG_TYPE_DATAACK=11;


const SCOPEMSG_DATA_IQ=1;
const SCOPEMSG_DATA_LLR=2;
const SCOPEMSG_DATA_WF=3;
/*---------------------------------------*/
@Component({
  selector: 'app-scope',
  templateUrl: './scope.component.html',
  styleUrls: ['./scope.component.css'],
})

export class ScopeComponent {
//data for scope status area
  scopetitle = '';
  scopesubtitle = '';
  scopetime = '';
  scopestatus = 'stopped';
 //data for scope control area 
  startstop = 'start';
  startstop_color = 'warn';
  rfrate = 2;
  data_ACK=false;
 //data for scope iq constellation area    
  iqgraph_list : IGraphDesc[] = [];
  selected_channels = [""];
  iqmax = 32767;
  iqmin = -32767;
  iqxmin=this.iqmin;
  iqymin=this.iqmin;
  iqxmax=this.iqmax;
  iqymax=this.iqmax;  
 //data for scope LLR area      
  llrgraph_list : IGraphDesc[] = [];
  selected_llrchannels = [""];
  target_list : number[] = [0,1,2,3];
  selected_sig : ISigDesc ={target_id:0, antenna_id:0};
  llrythresh=5;
  llrmin = 0;
  llrmax = 200000;
  llrxmin=this.llrmin; 
  llrxmax=this.llrmax;   
 //data for scope WatterFall area     
  WFgraph_list : IGraphDesc[] = [];
  selected_WF = "";
 
  
  
  wsService?: WebSocketService;
  wsSubscription?: Subscription;
   
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
//    parsing: false,
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

  public WFDatasets: ChartConfiguration<'scatter'>['data']['datasets'] = [
    {
      data: [800*100],
      label: 'WFblue',
      pointRadius: 0.5,
      showLine: false,
      animation: false,
      fill:false,
      pointStyle: 'circle',
      pointBackgroundColor:'blue',
      backgroundColor:'blue',
      borderWidth:0,
      pointBorderColor:'blue',
 //     parsing: false,
    },
    {
      data: [800*100],
      label: 'WFgreen',
      pointRadius: 0.5,
      showLine: false,
      animation: false,
      pointStyle: 'circle',
      pointBackgroundColor:'green',
      backgroundColor:'green',
      borderWidth:0,
      pointBorderColor:'green',
//      parsing: false,
    },
    {
      data: [800*100],
      label: 'WFyellow',
      pointRadius: 0.5,      
      showLine: false,
      animation: false,
      pointStyle: 'circle',
      pointBackgroundColor:'yellow',
      backgroundColor:'yellow',
      borderWidth:0,
      pointBorderColor:'yellow',
//      parsing: false,
    },
    {
      data: [800*100],
      label: 'WFred',
      pointRadius: 0.5,      
      showLine: false,
      animation: false,
      pointStyle: 'circle',
      pointBackgroundColor:'red',
      backgroundColor:'red',
      borderWidth:0,
      pointBorderColor:'red',
 //     parsing: false,
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
//      yAxes: {
//		  min: -100,
//		  max: 100
//      }     
    },
    plugins: { legend: { display: true, labels:{boxWidth: 10, boxHeight: 10}},  tooltip: { enabled: false, }, },
  };

  public WFOptions: ChartConfiguration<'scatter'>['options'] = {
    responsive: true,
    aspectRatio: 5,
    scales: {
    xAxes: {
		  min: 0,
		  max:800,
      },
    yAxes: {
		  min: 0,
		  max: 100,
          reverse: true,
      }
    },           
    plugins: { legend: { display: true, labels:{boxWidth: 10, boxHeight: 10}},  tooltip: { enabled: false, }, },    
  }
  

  constructor(private scopeApi: ScopeApi ) {
	console.log("Scope constructor ");	  
  }
  
  ngOnInit() {
     console.log("Scope ngOnInit "); 
     this.scopeApi.getScopeInfos$().subscribe(resp => {
	 this.configScope(resp);     
     });  
  }
   
  ngOnDestroy() {
    this.wsSubscription?.unsubscribe(); 
  }
  
  DecodScopeBinmsgToString(message: ArrayBuffer) {
     const enc = new TextDecoder("utf-8");
     return enc.decode(message);
  }

  configScope(resp: IScopeDesc) {
	  if (resp.title === "none") {
		  this.ScopeEnabled.emit(false);
	  } else {
		this.ScopeEnabled.emit(true);  
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
		  if (resp.graphs[graphIndex].type == IScopeGraphType.WF ) {
		    this.WFgraph_list.push(resp.graphs[graphIndex]);	    
		  }		  
	    }
	    this.charts?.forEach((child) => {
          child.chart?.update() });
      }		  
  }
  
  ProcessScopeMsg (message: Message) {
	  if (this.scopestatus === "starting") {  
	    this.scopestatus='started';
        this.startstop='stop';
        this.startstop_color='started';	    
	  }	  
	  let d=0;
	  let x=0;
	  let y=0;
	  switch ( message.msgtype ) {		  
          case SCOPEMSG_TYPE_TIME:
            this.scopetime=this.DecodScopeBinmsgToString(message.content);
            break;
          case  SCOPEMSG_TYPE_DATA:   
            const bufferview = new DataView(message.content);
            
            
			switch (message.chartid) {
			  case SCOPEMSG_DATA_IQ:
			  this.IQDatasets[message.dataid].data.length=0;
               if(message.update) {
				  console.log("Starting scope update chart " + message.chartid.toString() + ", dataset " + message.dataid.toString());
			    }			  
			    for ( let i=0;i<bufferview.byteLength; i=i+4) {
                  this.IQDatasets[message.dataid].data[i/4]={ x:bufferview.getInt16(i, true) , y: bufferview.getInt16(i+2, true)};
                }
                
                if(message.update) {
		          this.charts?.forEach((child,index) => { child.chart?.update() });                  
                  console.log(" scope update completed chart " + message.chartid.toString() + ", dataset " + message.dataid.toString());
			    }
              break;
			  case SCOPEMSG_DATA_LLR:
                if(message.update) {
				  console.log("Starting scope update chart " + message.chartid.toString() + ", dataset " + message.dataid.toString());
			    }
			    this.LLRDatasets[message.dataid].data.length=0;	  
			    let xoffset=0;
			    d=0;
			    for ( let i=4;i<(bufferview.byteLength-2);i=i+4) {
				  xoffset=xoffset+bufferview.getInt16(i+2,true);
                  this.LLRDatasets[message.dataid].data[d]={ x: xoffset, y: bufferview.getInt16(i,true)};
                  d++;
                }
                this.LLRDatasets[message.dataid].data[d]={ x:bufferview.getInt32(0, true) , y: 0};
                if(message.update) {
		          this.charts?.forEach((child,index) => { child.chart?.update() });                  
                  console.log(" scope update completed " + d.toString() + "points, ");
			    }
              break;  
			  case SCOPEMSG_DATA_WF:
                if(message.update) {
				  console.log("Starting scope update chart " + message.chartid.toString() + ", dataset " + message.dataid.toString());
			    }			    
			    for ( let i=2;i<(bufferview.byteLength-4);i=i+4) {
			      x=bufferview.getInt16(i,true);
				  y=bufferview.getInt16(i+2,true);  					
				  for(let j=0; j<this.WFDatasets.length; j++){
					if(j==message.dataid) {
                      this.WFDatasets[j].data[x+x*y]={ x: x, y:y};
                      this.WFDatasets[j].data[x+x*y]={ x: x, y:y};                     
				    } else {
                      this.WFDatasets[j].data[x+x*y]={ x: 0, y:0};
                      this.WFDatasets[j].data[x+x*y]={ x: 0, y:0};						
					}
                  }
                }
                if(message.update) {
		          this.charts?.forEach((child,index) => { child.chart?.update() });                  
                  console.log(" scope update completed " + d.toString() + "points, ");
			    }
              break;                           
                default:
                break;
		      }
		      
		      this.sendMsg(SCOPEMSG_TYPE_DATAACK,"Chart " + message.chartid.toString() + ", dataset " + message.dataid.toString() );
          break;
          default:
            break;				
      }
  }
  
  sendMsg(type: number, strmessage : string) {
	if ( this.wsService) {
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
  }

  sendBinMsg(type: number, binmessage: number) {
	if ( this.wsService) {
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
			 this.scopestatus='starting';
             this.SigChanged(this.selected_sig.target_id);
             this.OnRefrateChange();
             this.OnIQxminChange();
             this.OnIQxmaxChange();
             this.OnIQyminChange();
             this.OnIQymaxChange();
             this.OnYthreshChange();
             this.OnLLRxminChange();
             this.OnLLRxmaxChange();
             this.wsService=new(WebSocketService) ;
             if (this.wsService)
               this.wsSubscription=this.wsService.messages.subscribe((msg: ArrayBuffer) => {
				if (this.wsService)   
                  this.ProcessScopeMsg(this.wsService.DeserializeMessage(msg));
             });            
		  },
		  err => {
			  
		  }
        );   
    } else {
        this.scopeApi.setScopeParams$({name:"startstop",value:"stop"}).subscribe(
          () => {
             if ( this.wsSubscription)
               this.wsSubscription.unsubscribe();
             delete this.wsService ;   			  
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
	 this.SendScopeParams("refrate",(this.rfrate*10).toString(),0); 
  }

  OnLLRxminChange() {
	 this.SendScopeParams("llrxmin",(this.llrxmin).toString(),0);
  } 
  OnLLRxmaxChange() {
	 this.SendScopeParams("llrxmax",(this.llrxmax).toString(),0);
  }   
  
  OnIQxminChange() {
	 this.SendScopeParams("xmin",(this.iqxmin).toString(),0);
  } 
  OnIQxmaxChange() {
	 this.SendScopeParams("xmax",(this.iqxmax).toString(),0);
  }   
  OnIQyminChange() {
	 this.SendScopeParams("ymin",(this.iqymin).toString(),0);
  } 
  OnIQymaxChange() {
	 this.SendScopeParams("ymax",(this.iqymax).toString(),0);
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

  WFChanged(value: string) {
    this.selected_WF=value;
    for(let i=0; i<this.WFDatasets.length; i++) {
		this.WFDatasets[i].data.length=0;
	} 
    for (let i=0; i<this.WFgraph_list.length; i++) {
	  if( this.WFgraph_list[i].title === value) {
		this.SendScopeParams("enabled","true",this.WFgraph_list[i].srvidx);
		this.WFDatasets[0].label=value; 
	  } else {
	    this.SendScopeParams("enabled","false",this.WFgraph_list[i].srvidx);
	  }
	}	
  }
    
  onDataACKchange() {
	this.SendScopeParams("DataAck",this.data_ACK.toString(),0);  
  }
   
  OnYthreshChange() {
	this.SendScopeParams("llrythresh",this.llrythresh.toString(),0);  
  }    
}
