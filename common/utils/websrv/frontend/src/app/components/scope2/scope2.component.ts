import { Component, EventEmitter, OnDestroy, OnInit, Output, QueryList, ViewChildren } from "@angular/core";
import { BaseChartDirective } from 'ng2-charts';
import { Subscription } from 'rxjs';
import { IGraphDesc, IScopeDesc, IScopeGraphType, ISigDesc, ScopeApi } from 'src/app/api/scope.api';
import { IQDatasets, IQOptions } from "src/app/charts/iq.dataset";
import { LLRDatasets, LLROptions } from "src/app/charts/llr.dataset";
import { WFDatasets, WFOptions } from "src/app/charts/wf.dataset";
import { arraybuf_data_offset, Message, WebSocketService2, webSockSrc } from "src/app/services/websocket2.service";

export interface RxScopeMessage {
  msgtype: number;
  chartid: number;
  dataid: number;
  segnum: number;
  update: boolean;
  content: ArrayBuffer;
}

const deserialize = (buff: ArrayBuffer): RxScopeMessage => {
  const header = new DataView(buff, 0, arraybuf_data_offset);
  return {
    // source: src.getUint8(0),  //header
    msgtype: header.getUint8(1),  //header
    chartid: header.getUint8(3),  //header
    dataid: header.getUint8(4),  //header
    segnum: header.getUint8(2),  //header
    update: (header.getUint8(5) == 1) ? true : false,  //header
    content: buff.slice(arraybuf_data_offset)  // data
  };
}

const serialize = (msg: TxScopeMessage): ArrayBuffer => {

  const byteArray = new TextEncoder().encode(msg.content);

  let buff = new Uint8Array(byteArray.byteLength + arraybuf_data_offset);
  buff.set(byteArray, arraybuf_data_offset) //data
  let buffview = new DataView(buff);
  buffview.setUint8(1, msg.msgtype); //header

  return buffview.buffer;
}

export interface TxScopeMessage {
  msgtype: number;
  content: string;
}

/*------------------------------------*/
/* constants that must match backend (websrv.h or phy_scope.h) */
const SCOPEMSG_TYPE_STATUSUPD = 1;
const SCOPEMSG_TYPE_REFRATE = 2;
const SCOPEMSG_TYPE_TIME = 3;
const SCOPEMSG_TYPE_DATA = 10;
const SCOPEMSG_TYPE_DATAACK = 11;


const SCOPEMSG_DATA_IQ = 1;
const SCOPEMSG_DATA_LLR = 2;
const SCOPEMSG_DATA_WF = 3;
/*---------------------------------------*/
@Component({
  selector: 'app-scope2',
  templateUrl: './scope2.component.html',
  styleUrls: ['./scope2.component.css'],
})

export class Scope2Component implements OnInit, OnDestroy {

  WFDatasets = WFDatasets
  WFOptions = WFOptions
  LLROptions = LLROptions
  IQDatasets = IQDatasets
  IQOptions = IQOptions
  LLRDatasets = LLRDatasets

  //data for scope status area
  scopetitle = '';
  scopesubtitle = '';
  scopetime = '';
  scopestatus = 'stopped';
  //data for scope control area
  startstop = 'start';
  startstop_color = 'warn';
  rfrate = 2;
  data_ACK = false;
  //data for scope iq constellation area
  iqgraph_list: IGraphDesc[] = [];
  selected_channels = [""];
  iqmax = 32767;
  iqmin = -32767;
  iqxmin = this.iqmin;
  iqymin = this.iqmin;
  iqxmax = this.iqmax;
  iqymax = this.iqmax;
  //data for scope LLR area
  llrgraph_list: IGraphDesc[] = [];
  selected_llrchannels = [""];
  target_list: number[] = [0, 1, 2, 3];
  selected_sig: ISigDesc = { target_id: 0, antenna_id: 0 };
  llrythresh = 5;
  llrmin = 0;
  llrmax = 200000;
  llrxmin = this.llrmin;
  llrxmax = this.llrmax;
  //data for scope WatterFall area
  WFgraph_list: IGraphDesc[] = [];
  selected_WF = "";

  wsSubscription?: Subscription;

  @Output() ScopeEnabled = new EventEmitter<boolean>();

  @ViewChildren(BaseChartDirective) charts?: QueryList<BaseChartDirective>;


  constructor(
    private scopeApi: ScopeApi,
    private wsService: WebSocketService2
  ) { }

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
      this.scopetitle = resp.title;
      this.iqgraph_list.length = 0;
      this.llrgraph_list.length = 0;
      for (let graphIndex = 0; graphIndex < resp.graphs.length; graphIndex++) {
        if (resp.graphs[graphIndex].type == IScopeGraphType.IQs) {
          this.iqgraph_list.push(resp.graphs[graphIndex]);
          IQDatasets[this.iqgraph_list.length - 1].label = resp.graphs[graphIndex].title;
        }
        if (resp.graphs[graphIndex].type == IScopeGraphType.LLR) {
          this.llrgraph_list.push(resp.graphs[graphIndex]);
          LLRDatasets[this.llrgraph_list.length - 1].label = resp.graphs[graphIndex].title;
        }
        if (resp.graphs[graphIndex].type == IScopeGraphType.WF) {
          this.WFgraph_list.push(resp.graphs[graphIndex]);
        }
      }
      this.charts?.forEach((child) => {
        child.chart?.update()
      });
    }
  }

  ProcessScopeMsg(message: RxScopeMessage) {
    if (this.scopestatus === "starting") {
      this.scopestatus = 'started';
      this.startstop = 'stop';
      this.startstop_color = 'started';
    }
    let d = 0;
    let x = 0;
    let y = 0;
    switch (message.msgtype) {
      case SCOPEMSG_TYPE_TIME:
        this.scopetime = this.DecodScopeBinmsgToString(message.content);
        break;
      case SCOPEMSG_TYPE_DATA:
        const bufferview = new DataView(message.content);


        switch (message.chartid) {
          case SCOPEMSG_DATA_IQ:
            this.IQDatasets[message.dataid].data.length = 0;
            if (message.update) {
              console.log("Starting scope update chart " + message.chartid.toString() + ", dataset " + message.dataid.toString());
            }
            for (let i = 0; i < bufferview.byteLength; i = i + 4) {
              this.IQDatasets[message.dataid].data[i / 4] = { x: bufferview.getInt16(i, true), y: bufferview.getInt16(i + 2, true) };
            }

            if (message.update) {
              this.charts?.forEach((child, index) => { child.chart?.update() });
              console.log(" scope update completed chart " + message.chartid.toString() + ", dataset " + message.dataid.toString());
            }
            break;
          case SCOPEMSG_DATA_LLR:
            if (message.update) {
              console.log("Starting scope update chart " + message.chartid.toString() + ", dataset " + message.dataid.toString());
            }
            this.LLRDatasets[message.dataid].data.length = 0;
            let xoffset = 0;
            d = 0;
            for (let i = 4; i < (bufferview.byteLength - 2); i = i + 4) {
              xoffset = xoffset + bufferview.getInt16(i + 2, true);
              this.LLRDatasets[message.dataid].data[d] = { x: xoffset, y: bufferview.getInt16(i, true) };
              d++;
            }
            this.LLRDatasets[message.dataid].data[d] = { x: bufferview.getInt32(0, true), y: 0 };
            if (message.update) {
              this.charts?.forEach((child, index) => { child.chart?.update() });
              console.log(" scope update completed " + d.toString() + "points, ");
            }
            break;
          case SCOPEMSG_DATA_WF:
            if (message.update) {
              console.log("Starting scope update chart " + message.chartid.toString() + ", dataset " + message.dataid.toString());
            }
            for (let i = 2; i < (bufferview.byteLength - 4); i = i + 4) {
              x = bufferview.getInt16(i, true);
              y = bufferview.getInt16(i + 2, true);
              for (let j = 0; j < this.WFDatasets.length; j++) {
                if (j == message.dataid) {
                  this.WFDatasets[j].data[x + x * y] = { x: x, y: y };
                  this.WFDatasets[j].data[x + x * y] = { x: x, y: y };
                } else {
                  this.WFDatasets[j].data[x + x * y] = { x: 0, y: 0 };
                  this.WFDatasets[j].data[x + x * y] = { x: 0, y: 0 };
                }
              }
            }
            if (message.update) {
              this.charts?.forEach((child, index) => { child.chart?.update() });
              console.log(" scope update completed " + d.toString() + "points, ");
            }
            break;
          default:
            break;
        }

        this.sendMsg(SCOPEMSG_TYPE_DATAACK, "Chart " + message.chartid.toString() + ", dataset " + message.dataid.toString());
        break;
      default:
        break;
    }
  }

  sendMsg(type: number, strmessage: string) {
    this.wsService.send({
      source: webSockSrc.softscope,
      fullbuff: serialize({
        msgtype: type,
        content: strmessage
      })
    });
    console.log("Scope sent msg type " + type.toString() + " " + strmessage);
  }

  SendScopeParams(name: string, value: string, graphid: number) {
    this.scopeApi.setScopeParams$({ name: name, value: value, graphid: graphid }).subscribe();
  }

  startorstop() {
    if (this.scopestatus === 'stopped') {

      this.scopeApi.setScopeParams$({ name: "startstop", value: "start" }).subscribe(
        () => {
          IQDatasets.forEach((dataset) => { dataset.data.length = 0 });
          LLRDatasets.forEach((dataset) => { dataset.data.length = 0 });
          this.charts?.forEach((child) => { child.chart?.update() });
          this.scopestatus = 'starting';
          this.SigChanged(this.selected_sig.target_id);
          this.OnRefrateChange();
          this.OnIQxminChange();
          this.OnIQxmaxChange();
          this.OnIQyminChange();
          this.OnIQymaxChange();
          this.OnYthreshChange();
          this.OnLLRxminChange();
          this.OnLLRxmaxChange();

          this.wsSubscription = this.wsService.registerScopeSocket().subscribe((msg: Message) => {
            this.ProcessScopeMsg(deserialize(msg.fullbuff));
          });
        }
      );
    } else {
      this.scopeApi.setScopeParams$({ name: "startstop", value: "stop" }).subscribe(
        () => {
          if (this.wsSubscription) this.wsSubscription.unsubscribe();
          this.scopestatus = 'stopped';
          this.startstop = 'start';
          this.startstop_color = 'warn';
          this.charts?.forEach(child => child.chart?.update());

        }
      );
    }
  }

  OnRefrateChange() {
    this.SendScopeParams("refrate", (this.rfrate * 10).toString(), 0);
  }

  OnLLRxminChange() {
    this.SendScopeParams("llrxmin", (this.llrxmin).toString(), 0);
  }
  OnLLRxmaxChange() {
    this.SendScopeParams("llrxmax", (this.llrxmax).toString(), 0);
  }

  OnIQxminChange() {
    this.SendScopeParams("xmin", (this.iqxmin).toString(), 0);
  }
  OnIQxmaxChange() {
    this.SendScopeParams("xmax", (this.iqxmax).toString(), 0);
  }
  OnIQyminChange() {
    this.SendScopeParams("ymin", (this.iqymin).toString(), 0);
  }
  OnIQymaxChange() {
    this.SendScopeParams("ymax", (this.iqymax).toString(), 0);
  }

  channelsChanged(titles: string[]) {
    this.selected_channels = titles;
    this.iqgraph_list.forEach(graph => {
      const [enabled] = titles.filter(value => graph.title === value)
      this.SendScopeParams("enabled", enabled ? "true" : "false", graph.srvidx);
    })
  }

  llrchannelsChanged(titles: string[]) {
    this.selected_llrchannels = titles;
    this.llrgraph_list.forEach(graph => {
      const [enabled] = titles.filter(value => graph.title === value)
      this.SendScopeParams("enabled", enabled ? "true" : "false", graph.srvidx);
    })
  }

  SigChanged(value: number) {
    this.selected_sig.target_id = value;
    this.scopesubtitle = value.toString();
    this.SendScopeParams("TargetSelect", value.toString(), 0);
  }

  WFChanged(value: string) {
    this.selected_WF = value;
    this.WFDatasets.forEach(item => item.data.length = 0)
    this.WFgraph_list.forEach(graph => {
      if (graph.title === value) {
        this.SendScopeParams("enabled", "true", graph.srvidx);
        this.WFDatasets[0].label = value;
      } else {
        this.SendScopeParams("enabled", "false", graph.srvidx);
      }
    })
  }

  onDataACKchange() {
    this.SendScopeParams("DataAck", this.data_ACK.toString(), 0);
  }

  OnYthreshChange() {
    this.SendScopeParams("llrythresh", this.llrythresh.toString(), 0);
  }
}
