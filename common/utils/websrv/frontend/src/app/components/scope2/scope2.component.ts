import { Component, EventEmitter, Output, QueryList, ViewChildren } from "@angular/core";
import { BaseChartDirective } from 'ng2-charts';
import { Subscription } from 'rxjs';
import { IGraphDesc, IScopeDesc, IScopeGraphType, ISigDesc, ScopeApi } from 'src/app/api/scope.api';
import { IQDatasets } from "src/app/dataSets/iq.dataset";
import { LLRDatasets } from "src/app/dataSets/llr.dataset";
import { WFDatasets } from "src/app/dataSets/wf.dataset";
import { WebSocketService, webSockSrc } from "src/app/services/websocket.service";

export interface LogMessage {
  msgtype: number;
  chartid: number;
  dataid: number;
  segnum: number;
  update: boolean;
  content: ArrayBuffer;
}

const deserialize = (msg: ArrayBuffer): LogMessage => {
  const src = new DataView(msg, 0, 8);
  return {
    msgtype: src.getUint8(1),
    chartid: src.getUint8(3),
    dataid: src.getUint8(4),
    segnum: src.getUint8(2),
    update: (src.getUint8(5) == 1) ? true : false,
    content: msg.slice(8)
  };
}

const serialize = (msg: LogMessage): ArrayBuffer => {
  let buff = new ArrayBuffer(msg.content.byteLength + 8);   // 64 bits (8 bytes) header
  let fullbuff = new Uint8Array(buff, 0, buff.byteLength).set(new Uint8Array(msg.content), 8);
  let buffview = new DataView(buff);
  buffview.setUint8(0, msg.source);
  buffview.setUint8(1, msg.msgtype);
  return buffview.buffer;
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

export class Scope2Component {
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
    private wsService: WebSocketService
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

  ProcessScopeMsg(message: LogMessage) {
    if (this.scopestatus === "starting") {
      this.scopestatus = 'started';
      this.startstop = 'stop';
      this.startstop_color = 'started';
    }
    switch (message.msgtype) {
      case SCOPEMSG_TYPE_TIME:
        this.scopetime = this.DecodScopeBinmsgToString(message.content);
        break;
      case SCOPEMSG_TYPE_DATA:
        const bufferview = new DataView(message.content);
        IQDatasets[message.dataid].data.length = 0;

        switch (message.chartid) {
          case SCOPEMSG_DATA_IQ:
            if (message.update) {
              console.log("Starting scope update chart " + message.chartid.toString() + ", dataset " + message.dataid.toString());
            }
            for (let i = 0; i < bufferview.byteLength; i = i + 4) {
              IQDatasets[message.dataid].data[i / 4] = { x: bufferview.getInt16(i, true), y: bufferview.getInt16(i + 2, true) };
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
            LLRDatasets[message.dataid].data.length = 0;
            let xoffset = 0;
            let d = 0
            for (let i = 4; i < (bufferview.byteLength - 2); i = i + 4) {
              xoffset = xoffset + bufferview.getInt16(i + 2, true);
              LLRDatasets[message.dataid].data[d] = { x: xoffset, y: bufferview.getInt16(i, true) };
              d++;
            }
            LLRDatasets[message.dataid].data[d] = { x: bufferview.getInt32(0, true), y: 0 };
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
    buffview.setUint32(0, binmessage);
    let message = {
      source: webSockSrc.softscope,
      msgtype: type,
      chartid: 0,
      dataid: 0,
      segnum: 0,
      update: false,
      content: buffview.buffer
    };
    this.wsService.messages.next(this.wsService.SerializeMessage(message));
    console.log("Scope sent msg type " + type.toString() + " (binary content)");
  }

  SendScopeParams(name: string, value: string, graphid: number): boolean {
    let result = false;
    this.scopeApi.setScopeParams$({ name: name, value: value, graphid: graphid }).subscribe(
      () => {
        result = true;
      },
      err => {
        result = false;
      }
    );
    return result;
  }

  startorstop() {
    if (this.scopestatus === 'stopped') {

      this.scopeApi.setScopeParams$({ name: "startstop", value: "start" }).subscribe(
        () => {
          IQDatasets.forEach((dataset) => { dataset.data.length = 0 });
          LLRDatasets.forEach((dataset) => { dataset.data.length = 0 });
          this.charts?.forEach((child, index) => { child.chart?.update() });
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
          this.wsService = new (WebSocketService);

          this.wsSubscription = this.wsService.messages.subscribe((msg: ArrayBuffer) => {
            this.ProcessScopeMsg(this.wsService.DeserializeMessage(msg));
          });
        },
        err => {

        }
      );
    } else {
      this.scopeApi.setScopeParams$({ name: "startstop", value: "stop" }).subscribe(
        () => {
          if (this.wsSubscription) this.wsSubscription.unsubscribe();
          this.scopestatus = 'stopped';
          this.startstop = 'start';
          this.startstop_color = 'warn';
          this.charts?.forEach((child, index) => { child.chart?.update() });

        },
        err => {
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

  channelsChanged(value: string[]) {
    this.selected_channels = value;
    for (let i = 0; i < this.iqgraph_list.length; i++) {
      let enabled = "false";
      for (let j = 0; j < value.length; j++) {
        if (this.iqgraph_list[i].title === value[j]) {
          enabled = "true";
          break;
        }
      }
      this.SendScopeParams("enabled", enabled, this.iqgraph_list[i].srvidx);
    }
  }

  llrchannelsChanged(value: string[]) {
    this.selected_llrchannels = value;
    for (let i = 0; i < this.llrgraph_list.length; i++) {
      let enabled = "false";
      for (let j = 0; j < value.length; j++) {
        if (this.llrgraph_list[i].title === value[j]) {
          enabled = "true";
          break;
        }
      }
      this.SendScopeParams("enabled", enabled, this.llrgraph_list[i].srvidx);
    }
  }

  SigChanged(value: number) {
    this.selected_sig.target_id = value;
    this.scopesubtitle = value.toString();
    this.SendScopeParams("TargetSelect", value.toString(), 0);
  }

  WFChanged(value: string) {
    this.selected_WF = value;
    for (let i = 0; i < this.WFgraph_list.length; i++) {
      if (this.WFgraph_list[i].title === value) {
        this.SendScopeParams("enabled", "true", this.WFgraph_list[i].srvidx);
        WFDatasets[0].label = value;
      } else {
        this.SendScopeParams("enabled", "false", this.WFgraph_list[i].srvidx);
      }
    }
  }

  onDataACKchange() {
    this.SendScopeParams("DataAck", this.data_ACK.toString(), 0);
  }

  OnYthreshChange() {
    this.SendScopeParams("llrythresh", this.llrythresh.toString(), 0);
  }
}
