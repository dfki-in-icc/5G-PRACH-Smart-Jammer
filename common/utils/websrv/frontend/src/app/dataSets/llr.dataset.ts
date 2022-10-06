import { ChartConfiguration } from "chart.js";


export const LLRDatasets: ChartConfiguration<'scatter'>['data']['datasets'] = [
  {
    data: [],
    label: 'LLR1',
    pointRadius: 0.5,
    showLine: false,
    animation: false,
    fill: false,
    pointStyle: 'circle',
    pointBackgroundColor: 'yellow',
    backgroundColor: 'yellow',
    borderWidth: 0,
    pointBorderColor: 'yellow',
    //     parsing: false,
  },
  {
    data: [],
    label: 'LL2',
    pointRadius: 0.5,
    showLine: false,
    animation: false,
    pointStyle: 'circle',
    pointBackgroundColor: 'cyan',
    backgroundColor: 'cyan',
    borderWidth: 0,
    pointBorderColor: 'cyan',
    parsing: false,
  },
  {
    data: [],
    label: 'LLR3',
    pointRadius: 0.5,
    showLine: false,
    animation: false,
    pointStyle: 'circle',
    pointBackgroundColor: 'red',
    backgroundColor: 'red',
    borderWidth: 0,
    pointBorderColor: 'red',
    parsing: false,
  }
];

export const LLROptions: ChartConfiguration<'scatter'>['options'] = {
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
  plugins: { legend: { display: true, labels: { boxWidth: 10, boxHeight: 10 } }, tooltip: { enabled: false, }, },
};
