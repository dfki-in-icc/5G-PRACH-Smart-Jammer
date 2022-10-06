import { ChartConfiguration } from "chart.js";

export const WFDatasets: ChartConfiguration<'scatter'>['data']['datasets'] = [
  {
    data: [800 * 100],
    label: 'WFblue',
    pointRadius: 0.5,
    showLine: false,
    animation: false,
    fill: false,
    pointStyle: 'circle',
    pointBackgroundColor: 'blue',
    backgroundColor: 'blue',
    borderWidth: 0,
    pointBorderColor: 'blue',
    //     parsing: false,
  },
  {
    data: [800 * 100],
    label: 'WFgreen',
    pointRadius: 0.5,
    showLine: false,
    animation: false,
    pointStyle: 'circle',
    pointBackgroundColor: 'green',
    backgroundColor: 'green',
    borderWidth: 0,
    pointBorderColor: 'green',
    //      parsing: false,
  },
  {
    data: [800 * 100],
    label: 'WFyellow',
    pointRadius: 0.5,
    showLine: false,
    animation: false,
    pointStyle: 'circle',
    pointBackgroundColor: 'yellow',
    backgroundColor: 'yellow',
    borderWidth: 0,
    pointBorderColor: 'yellow',
    //      parsing: false,
  },
  {
    data: [800 * 100],
    label: 'WFred',
    pointRadius: 0.5,
    showLine: false,
    animation: false,
    pointStyle: 'circle',
    pointBackgroundColor: 'red',
    backgroundColor: 'red',
    borderWidth: 0,
    pointBorderColor: 'red',
    //     parsing: false,
  }
];

export const WFOptions: ChartConfiguration<'scatter'>['options'] = {
  responsive: true,
  aspectRatio: 5,
  scales: {
    xAxes: {
      min: 0,
      max: 800,
    },
    yAxes: {
      min: 0,
      max: 100,
      reverse: true,
    }
  },
  plugins: { legend: { display: true, labels: { boxWidth: 10, boxHeight: 10 } }, tooltip: { enabled: false, }, },
}
