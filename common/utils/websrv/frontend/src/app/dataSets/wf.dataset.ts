import { ChartConfiguration } from "chart.js";

export const WFDatasets: ChartConfiguration<'scatter'>['data']['datasets'] = [
  {
    data: [],
    label: 'WF1',
    pointRadius: 0.5,
    showLine: false,
    animation: false,
    fill: false,
    pointStyle: 'circle',
    pointBackgroundColor: 'yellow',
    backgroundColor: 'yellow',
    borderWidth: 0,
    pointBorderColor: 'yellow',
    //    parsing: false,
  },
  {
    data: [],
    label: 'WF2',
    pointRadius: 0.5,
    showLine: false,
    animation: false,
    pointStyle: 'circle',
    pointBackgroundColor: 'cyan',
    backgroundColor: 'cyan',
    borderWidth: 0,
    pointBorderColor: 'cyan',
    //      parsing: false,
  },
  {
    data: [],
    label: 'WF3',
    pointRadius: 0.5,
    showLine: false,
    animation: false,
    pointStyle: 'circle',
    pointBackgroundColor: 'red',
    backgroundColor: 'red',
    borderWidth: 0,
    pointBorderColor: 'red',
    //      parsing: false,
  }
];

export const WFOptions: ChartConfiguration<'scatter'>['options'] = {
  responsive: true,
  aspectRatio: 5,
  plugins: { legend: { display: true, labels: { boxWidth: 10, boxHeight: 10 } }, tooltip: { enabled: false, }, },
}
