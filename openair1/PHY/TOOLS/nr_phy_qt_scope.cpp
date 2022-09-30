#include <QApplication>
#include <QtWidgets>
#include <QPainter>
#include <QtGui>
#include "nr_phy_qt_scope.h"
#include <iostream>
#include <string.h>
#include <unistd.h>
#include <cassert>
#include <cmath>
#include <QtCharts>
#include <qchart.h>
#include <QValueAxis>

extern "C" {
#include "PHY/CODING/nrPolar_tools/nr_polar_defs.h"
#include <openair1/PHY/CODING/nrPolar_tools/nr_polar_defs.h>
}

typedef struct complex16 scopeSample_t;
#define ScaleZone 4;
#define SquaredNorm(VaR) ((VaR).r*(VaR).r+(VaR).i*(VaR).i)

const float Limits_KPI_gNB[4][2]={ // {lower Limit, Upper Limit}
  {0.02, 0.8},    // UL BLER
  {0.2, 10},      // UL Throughput in Mbs
  {0.02, 0.8},    // DL BLER
  {0.2, 10}       // DL Throughput in Mbs
};

const float Limits_KPI_ue[2][2]={ // {lower Limit, Upper Limit}
  {0.02, 0.8},    // DL BLER
  {0.2, 10}       // Throughput in Mbs
};

typedef struct {
  int dataSize;
  int elementSz;
  int colSz;
  int lineSz;
} scopeGraphData_t;

KPIListSelect::KPIListSelect(QWidget *parent) : QComboBox(parent)
{
  this->addItem("I/Q PBCH", 0);
  this->addItem("LLR PBCH", 1);
  this->addItem("I/Q PDSCH", 2);
  this->addItem("LLR PDSCH", 3);
  this->addItem("I/Q PDCCH", 4);
  this->addItem("LLR PDCCH", 5);
  this->addItem("RX Signal-Time", 6);
  this->addItem("Channel Response", 7);
  this->addItem("DL BLER", 8);
  this->addItem("Throughput", 9);
  this->addItem("DL MCS", 10);
  this->addItem("Nof Sched. RBs", 11);
}
KPIListSelect::~KPIListSelect()
{
}

KPIListSelectgNB::KPIListSelectgNB(QWidget *parent) : QComboBox(parent)
{
  this->addItem("I/Q PUSCH", 0);
  this->addItem("LLR PUSCH", 1);
  this->addItem("Channel Response", 2);
  this->addItem("UL BLER", 3);
  this->addItem("DL BLER", 4);
  this->addItem("DL MCS", 5);
  this->addItem("UL MCS", 6);
  this->addItem("UL Throughput", 7);
  this->addItem("DL Throughput", 8);
  this->addItem("Nof Sched. RBs", 9);
  this->addItem("UL SNR", 10);
  this->addItem("DL SNR (CQI)", 11);
  this->addItem("UL Retrans.", 12);
  this->addItem("DL Retrans.", 13);
}
KPIListSelectgNB::~KPIListSelectgNB()
{
}

PainterWidgetgNB::PainterWidgetgNB(QComboBox *parent, scopeData_t *p)
{
    timer = new QTimer(this);
    timerRetrans = new QTimer(this);
    this->chartHight = 300;
    this->chartWidth = 300;
    this->pix = new QPixmap(this->chartWidth,this->chartHight);
    this->pix->fill(QColor(240,240,240));
    this->parentWindow = parent;
    this->p = p;
    this->nb_UEs = 1;

    this->indexToPlot = this->parentWindow->currentIndex();
    this->previousIndex = this->parentWindow->currentIndex();

    // UL BLER
    this->ULBLER.plot_idx = 0;
    resetKPIPlot(&this->ULBLER);

    // UL MCS
    this->ULMCS.plot_idx = 0;
    resetKPIPlot(&this->ULMCS);
    resetKPIValues(&this->ULMCS);

    // DL BLER
    this->DLBLER.plot_idx = 0;
    resetKPIPlot(&this->DLBLER);

    // DL MCS
    this->DLMCS.plot_idx = 0;
    resetKPIPlot(&this->DLMCS);
    resetKPIValues(&this->DLMCS);

    // UL Throughput
    this->ULThrou.plot_idx = 0;
    resetKPIPlot(&this->ULThrou);
    resetKPIValues(&this->ULThrou);

    // DL Throughput
    this->DLThrou.plot_idx = 0;
    resetKPIPlot(&this->DLThrou);
    resetKPIValues(&this->DLThrou);

    // nof scheduled RBs
    this->nofRBs.plot_idx = 0;
    resetKPIPlot(&this->nofRBs);
    resetKPIValues(&this->nofRBs);

    // UL SNR
    this->ULSNR.plot_idx = 0;
    resetKPIPlot(&this->ULSNR);
    resetKPIValues(&this->ULSNR);

    // DL SNR
    this->DLSNR.plot_idx = 0;
    resetKPIPlot(&this->DLSNR);
    resetKPIValues(&this->DLSNR);

    // UL Retrans
    this->ULRetrans[0].plot_idx = 0;
    for (int i=0; i<4; i++)
    {
      resetKPIPlot(&this->ULRetrans[i]);
      resetKPIValues(&this->ULRetrans[i]);
    }
    this->ULRetrans[0].series->setName("R. 1");
    this->ULRetrans[1].series->setName("R. 2");
    this->ULRetrans[2].series->setName("R. 3");
    this->ULRetrans[3].series->setName("R. 4");

    // DL Retrans
    this->DLRetrans[0].plot_idx = 0;
    for (int i=0; i<4; i++)
    {
      resetKPIPlot(&this->DLRetrans[i]);
      resetKPIValues(&this->DLRetrans[i]);
    }
    this->DLRetrans[0].series->setName("R. 1");
    this->DLRetrans[1].series->setName("R. 2");
    this->DLRetrans[2].series->setName("R. 3");
    this->DLRetrans[3].series->setName("R. 4");

    makeConnections();
}

void PainterWidgetgNB::resetKPIPlot(KPI_elements *inputStruct)
{
  inputStruct->series = new QLineSeries();
  inputStruct->series->setColor(QColor(0,0,0));

  inputStruct->seriesMin = new QLineSeries();
  inputStruct->seriesMin->setColor(QColor(255,0,0));
  inputStruct->seriesMin->setName("Min.");

  inputStruct->seriesMax = new QLineSeries();
  inputStruct->seriesMax->setColor(QColor(0,255,0));
  inputStruct->seriesMax->setName("Max.");

  inputStruct->seriesAvg = new QLineSeries();
  inputStruct->seriesAvg->setColor(QColor(0,0,255));
  inputStruct->seriesAvg->setName("Avg.");
}

void PainterWidgetgNB::resetKPIValues(KPI_elements *inputStruct)
{
  inputStruct->max_value = 0.0;
  inputStruct->min_value = 0.0;
  inputStruct->avg_value = 0.0;
  inputStruct->avg_idx = 0;
  inputStruct->nof_retrans = 0;
}

void PainterWidgetgNB::resizeEvent(QResizeEvent *event)
{
  if ((width() != this->chartWidth) || (height() != this->chartHight))
  {
    this->chartHight = height();
    this->chartWidth = width();
    update();
  }
  QWidget::resizeEvent(event);
}

void PainterWidgetgNB::paintEvent(QPaintEvent *)
{
  QPainter painter(this);
  painter.drawPixmap( (this->width()-this->pix->width())/2,
  (this->height()-this->pix->height())/2, *this->pix); // paint pixmap on widget

  this->indexToPlot = this->parentWindow->currentIndex();

  makeConnections();
}

void PainterWidgetgNB::makeConnections()
{
    disconnect(timer, nullptr, nullptr, nullptr);

    if (this->indexToPlot == 0)
    {
      connect(timer, &QTimer::timeout, this, &PainterWidgetgNB::KPI_PuschIQ);
    }
    else if (this->indexToPlot == 1)
    {
      connect(timer, &QTimer::timeout, this, &PainterWidgetgNB::KPI_PuschLLR);   // paintPixmap_uePbchIQ
    }
    else if (this->indexToPlot == 2)
    {
      connect(timer, &QTimer::timeout, this, &PainterWidgetgNB::KPI_ChannelResponse);
    }
    else if (this->indexToPlot == 3)
    {
      connect(timer, &QTimer::timeout, this, &PainterWidgetgNB::KPI_UL_BLER);
    }
    else if (this->indexToPlot == 4)
    {
      connect(timer, &QTimer::timeout, this, &PainterWidgetgNB::KPI_DL_BLER);
    }
    else if (this->indexToPlot == 5)
    {
      connect(timer, &QTimer::timeout, this, &PainterWidgetgNB::KPI_DL_MCS);
    }
    else if (this->indexToPlot == 6)
    {
      connect(timer, &QTimer::timeout, this, &PainterWidgetgNB::KPI_UL_MCS);
    }
    else if (this->indexToPlot == 7)
    {
      connect(timer, &QTimer::timeout, this, &PainterWidgetgNB::KPI_UL_Throu);
    }
    else if (this->indexToPlot == 8)
    {
      connect(timer, &QTimer::timeout, this, &PainterWidgetgNB::KPI_DL_Throu);
    }
    else if (this->indexToPlot == 9)
    {
      connect(timer, &QTimer::timeout, this, &PainterWidgetgNB::KPI_Nof_RBs);
    }
    else if (this->indexToPlot == 10)
    {
      connect(timer, &QTimer::timeout, this, &PainterWidgetgNB::KPI_UL_SNR);
    }
    else if (this->indexToPlot == 11)
    {
      connect(timer, &QTimer::timeout, this, &PainterWidgetgNB::KPI_DL_SNR);
    }
    else if (this->indexToPlot == 12)
    {
      connect(timerRetrans, &QTimer::timeout, this, &PainterWidgetgNB::KPI_UL_Retrans);
    }
    else if (this->indexToPlot == 13)
    {
      connect(timerRetrans, &QTimer::timeout, this, &PainterWidgetgNB::KPI_DL_Retrans);
    }

    timer->start(200);
    timerRetrans->start(1000);
}

void PainterWidgetgNB::KPI_DL_Retrans()
{
  // erase the previous paint
  this->pix->fill(QColor(240,240,240));

  gNB_MAC_INST *gNBMac = (gNB_MAC_INST *)RC.nrmac[0];
  NR_UE_info_t *targetUE = gNBMac->UE_info.list[0];

  if ((this->DLRetrans[0].plot_idx > this->chartWidth) ||
      (this->indexToPlot != this->previousIndex))
  {
    this->DLRetrans[0].plot_idx = 0;
    for (int i=0; i<4; i++)
    {
      resetKPIPlot(&this->DLRetrans[i]);
      resetKPIValues(&this->DLRetrans[i]);
    }
    this->DLRetrans[0].series->setName("R. 1");
    this->DLRetrans[1].series->setName("R. 2");
    this->DLRetrans[2].series->setName("R. 3");
    this->DLRetrans[3].series->setName("R. 4");
  }

  float Xpaint = this->DLRetrans[0].plot_idx;
  float Ypaint[4];
  uint64_t nrRetrans;
  for (int i=0; i<4;i++){
    nrRetrans = targetUE->mac_stats.dl.rounds[i] - this->DLRetrans[i].nof_retrans;
    Ypaint[i] = (float)nrRetrans;
    this->DLRetrans[i].nof_retrans = targetUE->mac_stats.dl.rounds[i];
  }

  if (this->DLRetrans[0].plot_idx != 0)
  {
    this->DLRetrans[0].max_value = std::max(this->DLRetrans[0].max_value, Ypaint[0]);
    for (int i=0; i<4;i++){
      this->DLRetrans[i].series->append(Xpaint, Ypaint[i]);
    }
  }

  QChart *chart = new QChart();
  chart->legend()->setVisible(true);
  chart->legend()->setAlignment(Qt::AlignBottom);

  int nofTicks = 6;
  QValueAxis *axisX = new QValueAxis;
  axisX->setTickCount(nofTicks);
  axisX->setRange(0 , this->chartWidth);
  axisX->setTitleText("Time Index");
  chart->addAxis(axisX, Qt::AlignBottom);

  QValueAxis *axisY = new QValueAxis;
  axisY->setTickCount(nofTicks);
  axisY->setRange(-1, this->DLRetrans[0].max_value + 5);
  axisY->setTitleText("Nof Retrans.");
  chart->addAxis(axisY, Qt::AlignLeft);

  if (this->DLRetrans[0].plot_idx != 0)
  {
    for (int i=0; i<4;i++){
      chart->addSeries(this->DLRetrans[i].series);
      this->DLRetrans[i].series->attachAxis(axisX);
      this->DLRetrans[i].series->attachAxis(axisY);
    }
  }

  QChartView *chartView = new QChartView(chart);
  chartView->resize(this->chartWidth, this->chartHight);

  QPixmap p = chartView->grab();
  *this->pix = p;

  this->previousIndex = this->parentWindow->currentIndex();
  this->DLRetrans[0].plot_idx++;
  update();
}


void PainterWidgetgNB::KPI_UL_Retrans()
{
  // erase the previous paint
  this->pix->fill(QColor(240,240,240));

  gNB_MAC_INST *gNBMac = (gNB_MAC_INST *)RC.nrmac[0];
  NR_UE_info_t *targetUE = gNBMac->UE_info.list[0];

  if ((this->ULRetrans[0].plot_idx > this->chartWidth) ||
      (this->indexToPlot != this->previousIndex))
  {
    this->ULRetrans[0].plot_idx = 0;
    for (int i=0; i<4; i++)
    {
      resetKPIPlot(&this->ULRetrans[i]);
      resetKPIValues(&this->ULRetrans[i]);
    }
    this->ULRetrans[0].series->setName("R. 1");
    this->ULRetrans[1].series->setName("R. 2");
    this->ULRetrans[2].series->setName("R. 3");
    this->ULRetrans[3].series->setName("R. 4");
  }

  float Xpaint = this->ULRetrans[0].plot_idx;
  float Ypaint[4];
  uint64_t nrRetrans;
  for (int i=0; i<4;i++){
    nrRetrans = targetUE->mac_stats.ul.rounds[i] - this->ULRetrans[i].nof_retrans;
    Ypaint[i] = (float)nrRetrans;
    this->ULRetrans[i].nof_retrans = targetUE->mac_stats.ul.rounds[i];
  }

  if (this->ULRetrans[0].plot_idx != 0)
  {
    this->ULRetrans[0].max_value = std::max(this->ULRetrans[0].max_value, Ypaint[0]);
    for (int i=0; i<4;i++){
      this->ULRetrans[i].series->append(Xpaint, Ypaint[i]);
    }
  }

  QChart *chart = new QChart();
  chart->legend()->setVisible(true);
  chart->legend()->setAlignment(Qt::AlignBottom);

  int nofTicks = 6;
  QValueAxis *axisX = new QValueAxis;
  axisX->setTickCount(nofTicks);
  axisX->setRange(0 , this->chartWidth);
  axisX->setTitleText("Time Index");
  chart->addAxis(axisX, Qt::AlignBottom);

  QValueAxis *axisY = new QValueAxis;
  axisY->setTickCount(nofTicks);
  axisY->setRange(-1, this->ULRetrans[0].max_value + 5);
  axisY->setTitleText("Nof Retrans.");
  chart->addAxis(axisY, Qt::AlignLeft);

  if (this->ULRetrans[0].plot_idx != 0)
  {
    for (int i=0; i<4;i++){
      chart->addSeries(this->ULRetrans[i].series);
      this->ULRetrans[i].series->attachAxis(axisX);
      this->ULRetrans[i].series->attachAxis(axisY);
    }
  }

  QChartView *chartView = new QChartView(chart);
  chartView->resize(this->chartWidth, this->chartHight);

  QPixmap p = chartView->grab();
  *this->pix = p;

  this->previousIndex = this->parentWindow->currentIndex();
  this->ULRetrans[0].plot_idx++;
  update();
}


void PainterWidgetgNB::KPI_DL_SNR()
{
  // erase the previous paint
  this->pix->fill(QColor(240,240,240));

  gNB_MAC_INST *gNBMac = (gNB_MAC_INST *)RC.nrmac[0];
  NR_UE_info_t *targetUE = gNBMac->UE_info.list[0];
  NR_UE_sched_ctrl_t *sched_ctrl = &targetUE->UE_sched_ctrl;

  if ((this->DLSNR.plot_idx > this->chartWidth) ||
      (this->indexToPlot != this->previousIndex))
  {
    this->DLSNR.plot_idx = 0;
    resetKPIPlot(&this->DLSNR);
  }

  float Xpaint, Ypaint;
  Xpaint = this->DLSNR.plot_idx;
  Ypaint = (float)sched_ctrl->CSI_report.cri_ri_li_pmi_cqi_report.wb_cqi_1tb;

  this->DLSNR.max_value = std::max(this->DLSNR.max_value, Ypaint);
  this->DLSNR.series->append(Xpaint, Ypaint);

  QChart *chart = new QChart();
  chart->legend()->hide();

  int nofTicks = 6;
  QValueAxis *axisX = new QValueAxis;
  axisX->setTickCount(nofTicks);
  axisX->setRange(0 , this->chartWidth);
  axisX->setTitleText("Time Index");
  chart->addAxis(axisX, Qt::AlignBottom);

  QValueAxis *axisY = new QValueAxis;
  axisY->setTickCount(nofTicks);
  axisY->setRange(-1, this->DLSNR.max_value + 2);
  axisY->setTitleText("DL SNR (CQI)");
  chart->addAxis(axisY, Qt::AlignLeft);

  chart->addSeries(this->DLSNR.series);
  this->DLSNR.series->attachAxis(axisX);
  this->DLSNR.series->attachAxis(axisY);

  QChartView *chartView = new QChartView(chart);
  chartView->resize(this->chartWidth, this->chartHight);

  QPixmap p = chartView->grab();
  *this->pix = p;

  this->previousIndex = this->parentWindow->currentIndex();
  this->DLSNR.plot_idx++;
  update();
}


void PainterWidgetgNB::KPI_UL_SNR()
{
  // erase the previous paint
  this->pix->fill(QColor(240,240,240));

  gNB_MAC_INST *gNBMac = (gNB_MAC_INST *)RC.nrmac[0];
  NR_UE_info_t *targetUE = gNBMac->UE_info.list[0];
  NR_UE_sched_ctrl_t *sched_ctrl = &targetUE->UE_sched_ctrl;

  if ((this->ULSNR.plot_idx > this->chartWidth) ||
      (this->indexToPlot != this->previousIndex))
  {
    this->ULSNR.plot_idx = 0;
    resetKPIPlot(&this->ULSNR);
  }

  float Xpaint, Ypaint;
  Xpaint = this->ULSNR.plot_idx;
  Ypaint = (float)sched_ctrl->pusch_snrx10/10.0;

  this->ULSNR.max_value = std::max(this->ULSNR.max_value, Ypaint);
  this->ULSNR.series->append(Xpaint, Ypaint);

  QChart *chart = new QChart();
  chart->legend()->hide();

  int nofTicks = 6;
  QValueAxis *axisX = new QValueAxis;
  axisX->setTickCount(nofTicks);
  axisX->setRange(0 , this->chartWidth);
  axisX->setTitleText("Time Index");
  chart->addAxis(axisX, Qt::AlignBottom);

  QValueAxis *axisY = new QValueAxis;
  axisY->setTickCount(nofTicks);
  axisY->setRange(-1, 1.2*this->ULSNR.max_value);
  axisY->setTitleText("PUSCH SNR dB");
  chart->addAxis(axisY, Qt::AlignLeft);

  chart->addSeries(this->ULSNR.series);
  this->ULSNR.series->attachAxis(axisX);
  this->ULSNR.series->attachAxis(axisY);

  QChartView *chartView = new QChartView(chart);
  chartView->resize(this->chartWidth, this->chartHight);

  QPixmap p = chartView->grab();
  *this->pix = p;

  this->previousIndex = this->parentWindow->currentIndex();
  this->ULSNR.plot_idx++;
  update();
}


void PainterWidgetgNB::KPI_Nof_RBs()
{
  // erase the previous paint
  this->pix->fill(QColor(240,240,240));

  gNB_MAC_INST *gNBMac = (gNB_MAC_INST *)RC.nrmac[0];
  NR_UE_info_t *targetUE = gNBMac->UE_info.list[0];
  NR_UE_sched_ctrl_t *sched_ctrl = &targetUE->UE_sched_ctrl;


  NR_sched_pdsch_t *sched_pdsch = &sched_ctrl->sched_pdsch;
  int current_harq_pid = (int)sched_pdsch->dl_harq_pid;

  uint16_t rbSize = (uint16_t)sched_ctrl->harq_processes[current_harq_pid].sched_pdsch.rbSize;

  if ((this->nofRBs.plot_idx > this->chartWidth) ||
      (this->indexToPlot != this->previousIndex))
  {
    this->nofRBs.plot_idx = 0;
    resetKPIPlot(&this->nofRBs);
  }

  float Xpaint, Ypaint;
  Xpaint = this->nofRBs.plot_idx;
  Ypaint = (float)rbSize;

  this->nofRBs.max_value = std::max(this->nofRBs.max_value, Ypaint);
  this->nofRBs.series->append(Xpaint, Ypaint);

  QChart *chart = new QChart();
  chart->legend()->hide();

  int nofTicks = 6;
  QValueAxis *axisX = new QValueAxis;
  axisX->setTickCount(nofTicks);
  axisX->setRange(0 , this->chartWidth);
  axisX->setTitleText("Time Index");
  chart->addAxis(axisX, Qt::AlignBottom);

  QValueAxis *axisY = new QValueAxis;
  axisY->setTickCount(nofTicks);
  axisY->setRange(-1, this->nofRBs.max_value + 10);
  axisY->setTitleText("Nof Scheduled RBs");
  chart->addAxis(axisY, Qt::AlignLeft);

  chart->addSeries(this->nofRBs.series);
  this->nofRBs.series->attachAxis(axisX);
  this->nofRBs.series->attachAxis(axisY);

  QChartView *chartView = new QChartView(chart);
  chartView->resize(this->chartWidth, this->chartHight);

  QPixmap p = chartView->grab();
  *this->pix = p;

  this->previousIndex = this->parentWindow->currentIndex();
  this->nofRBs.plot_idx++;
  update();
}


void PainterWidgetgNB::KPI_DL_Throu()
{
  // erase the previous paint
  this->pix->fill(QColor(240,240,240));

  gNB_MAC_INST *gNBMac = (gNB_MAC_INST *)RC.nrmac[0];
  NR_UE_info_t *targetUE = gNBMac->UE_info.list[0];
  NR_UE_sched_ctrl_t *sched_ctrl = &targetUE->UE_sched_ctrl;
  uint32_t blockSize = (uint32_t)targetUE->mac_stats.dl.current_bytes;
  float bler_dl = (float)sched_ctrl->dl_bler_stats.bler;

  NR_DL_FRAME_PARMS *frame_parms = &this->p->gNB->frame_parms;
  uint16_t slots_per_frame = frame_parms->slots_per_frame;
  float slotDuration = 10.0/(float)slots_per_frame;      // slot duration in msec

  double blerTerm = 1.0 - (double)bler_dl;
  double blockSizeBits = (double)(blockSize << 3);

  double ThrouputKBitSec = blerTerm * blockSizeBits / (double)slotDuration;

  if ((this->DLThrou.plot_idx > this->chartWidth) ||
      (this->indexToPlot != this->previousIndex))
  {
    this->DLThrou.plot_idx = 0;
    resetKPIPlot(&this->DLThrou);
  }

  float Xpaint, Ypaint;
  Xpaint = this->DLThrou.plot_idx;
  Ypaint = (float)(ThrouputKBitSec/1000);    // Throughput in MBit/sec

  this->DLThrou.max_value = std::max(this->DLThrou.max_value, Ypaint);
  this->DLThrou.series->append(Xpaint, Ypaint);

  QLineSeries *series_LowLim = new QLineSeries();
  series_LowLim->append(0, Limits_KPI_gNB[3][0]);
  series_LowLim->append(this->chartWidth, Limits_KPI_gNB[3][0]);
  series_LowLim->setColor(QColor(0, 255, 0));

  QLineSeries *series_UppLim = new QLineSeries();
  series_UppLim->append(0, Limits_KPI_gNB[3][1]);
  series_UppLim->append(this->chartWidth, Limits_KPI_gNB[3][1]);
  series_UppLim->setColor(QColor(255, 0, 0));

  QChart *chart = new QChart();
  chart->legend()->hide();

  int nofTicks = 6;
  QValueAxis *axisX = new QValueAxis;
  axisX->setTickCount(nofTicks);
  axisX->setRange(0 , this->chartWidth);
  axisX->setTitleText("Time Index");
  chart->addAxis(axisX, Qt::AlignBottom);

  QValueAxis *axisY = new QValueAxis;
  axisY->setTickCount(nofTicks);
  axisY->setRange(-1, 1.2*this->DLThrou.max_value);
  axisY->setTitleText("UL Throughput Mbit/sec");
  chart->addAxis(axisY, Qt::AlignLeft);

  chart->addSeries(this->DLThrou.series);
  this->DLThrou.series->attachAxis(axisX);
  this->DLThrou.series->attachAxis(axisY);

  chart->addSeries(series_LowLim);
  series_LowLim->attachAxis(axisX);
  series_LowLim->attachAxis(axisY);

  chart->addSeries(series_UppLim);
  series_UppLim->attachAxis(axisX);
  series_UppLim->attachAxis(axisY);

  QChartView *chartView = new QChartView(chart);
  chartView->resize(this->chartWidth, this->chartHight);

  QPixmap p = chartView->grab();
  *this->pix = p;

  this->previousIndex = this->parentWindow->currentIndex();
  this->DLThrou.plot_idx++;
  update();
}

void PainterWidgetgNB::KPI_UL_Throu()
{
  // erase the previous paint
  this->pix->fill(QColor(240,240,240));

  gNB_MAC_INST *gNBMac = (gNB_MAC_INST *)RC.nrmac[0];
  NR_UE_info_t *targetUE = gNBMac->UE_info.list[0];
  NR_UE_sched_ctrl_t *sched_ctrl = &targetUE->UE_sched_ctrl;
  uint32_t blockSize = (uint32_t)targetUE->mac_stats.ul.current_bytes;
  float bler_ul = (float)sched_ctrl->ul_bler_stats.bler;

  NR_DL_FRAME_PARMS *frame_parms = &this->p->gNB->frame_parms;
  uint16_t slots_per_frame = frame_parms->slots_per_frame;
  float slotDuration = 10.0/(float)slots_per_frame;      // slot duration in msec

  double blerTerm = 1.0 - (double)bler_ul;
  double blockSizeBits = (double)(blockSize << 3);

  double ThrouputKBitSec = blerTerm * blockSizeBits / (double)slotDuration;

  if ((this->ULThrou.plot_idx > this->chartWidth) ||
      (this->indexToPlot != this->previousIndex))
  {
    this->ULThrou.plot_idx = 0;
    resetKPIPlot(&this->ULThrou);
  }

  float Xpaint, Ypaint;
  Xpaint = this->ULThrou.plot_idx;
  Ypaint = (float)(ThrouputKBitSec/1000);    // Throughput in MBit/sec

  this->ULThrou.max_value = std::max(this->ULThrou.max_value, Ypaint);
  this->ULThrou.series->append(Xpaint, Ypaint);

  QLineSeries *series_LowLim = new QLineSeries();
  series_LowLim->append(0, Limits_KPI_gNB[1][0]);
  series_LowLim->append(this->chartWidth, Limits_KPI_gNB[1][0]);
  series_LowLim->setColor(QColor(0, 255, 0));

  QLineSeries *series_UppLim = new QLineSeries();
  series_UppLim->append(0, Limits_KPI_gNB[1][1]);
  series_UppLim->append(this->chartWidth, Limits_KPI_gNB[1][1]);
  series_UppLim->setColor(QColor(255, 0, 0));

  QChart *chart = new QChart();
  chart->legend()->hide();

  int nofTicks = 6;
  QValueAxis *axisX = new QValueAxis;
  axisX->setTickCount(nofTicks);
  axisX->setRange(0 , this->chartWidth);
  axisX->setTitleText("Time Index");
  chart->addAxis(axisX, Qt::AlignBottom);

  QValueAxis *axisY = new QValueAxis;
  axisY->setTickCount(nofTicks);
  axisY->setRange(-1, 1.2*this->ULThrou.max_value);
  axisY->setTitleText("UL Throughput Mbit/sec");
  chart->addAxis(axisY, Qt::AlignLeft);

  chart->addSeries(this->ULThrou.series);
  this->ULThrou.series->attachAxis(axisX);
  this->ULThrou.series->attachAxis(axisY);

  chart->addSeries(series_LowLim);
  series_LowLim->attachAxis(axisX);
  series_LowLim->attachAxis(axisY);

  chart->addSeries(series_UppLim);
  series_UppLim->attachAxis(axisX);
  series_UppLim->attachAxis(axisY);

  QChartView *chartView = new QChartView(chart);
  chartView->resize(this->chartWidth, this->chartHight);

  QPixmap p = chartView->grab();
  *this->pix = p;

  this->previousIndex = this->parentWindow->currentIndex();
  this->ULThrou.plot_idx++;
  update();
}

void PainterWidgetgNB::KPI_DL_MCS()
{
  // erase the previous paint
  this->pix->fill(QColor(240,240,240));

  gNB_MAC_INST *gNBMac = (gNB_MAC_INST *)RC.nrmac[0];
  NR_UE_info_t *targetUE = gNBMac->UE_info.list[0];
  NR_UE_sched_ctrl_t *sched_ctrl = &targetUE->UE_sched_ctrl;

  if ((this->DLMCS.plot_idx > this->chartWidth) ||
      (this->indexToPlot != this->previousIndex))
  {
    this->DLMCS.plot_idx = 0;
    resetKPIPlot(&this->DLMCS);

    if(this->indexToPlot != this->previousIndex)
      resetKPIValues(&this->DLMCS);
  }

  float Xpaint, Ypaint;
  Xpaint = this->DLMCS.plot_idx;
  Ypaint = (float)sched_ctrl->dl_bler_stats.mcs;

  this->DLMCS.max_value = std::max(this->DLMCS.max_value, Ypaint);
  this->DLMCS.min_value = std::min(this->DLMCS.min_value, Ypaint);

  this->DLMCS.series->append(Xpaint, Ypaint);
  this->DLMCS.seriesMin->append(Xpaint, this->DLMCS.min_value);
  this->DLMCS.seriesMax->append(Xpaint, this->DLMCS.max_value);

  QChart *chart = new QChart();
  chart->legend()->show();

  int nofTicks = 6;
  QValueAxis *axisX = new QValueAxis;
  axisX->setTickCount(nofTicks);
  axisX->setRange(0 , this->chartWidth);
  axisX->setTitleText("Time Index");
  chart->addAxis(axisX, Qt::AlignBottom);

  QValueAxis *axisY = new QValueAxis;
  axisY->setTickCount(nofTicks);
  axisY->setRange(-1, this->DLMCS.max_value + 2.0);
  axisY->setTitleText("DL MCS");
  chart->addAxis(axisY, Qt::AlignLeft);

  chart->addSeries(this->DLMCS.series);
  this->DLMCS.series->attachAxis(axisX);
  this->DLMCS.series->attachAxis(axisY);

  chart->addSeries(this->DLMCS.seriesMin);
  this->DLMCS.seriesMin->attachAxis(axisX);
  this->DLMCS.seriesMin->attachAxis(axisY);

  chart->addSeries(this->DLMCS.seriesMax);
  this->DLMCS.seriesMax->attachAxis(axisX);
  this->DLMCS.seriesMax->attachAxis(axisY);

  QChartView *chartView = new QChartView(chart);
  chartView->resize(this->chartWidth, this->chartHight);

  QPixmap p = chartView->grab();
  *this->pix = p;

  this->previousIndex = this->parentWindow->currentIndex();
  this->DLMCS.plot_idx++;
  update();
}

void PainterWidgetgNB::KPI_DL_BLER()
{
  // erase the previous paint
  this->pix->fill(QColor(240,240,240));

  gNB_MAC_INST *gNBMac = (gNB_MAC_INST *)RC.nrmac[0];
  NR_UE_info_t *targetUE = gNBMac->UE_info.list[0];
  NR_UE_sched_ctrl_t *sched_ctrl = &targetUE->UE_sched_ctrl;

  if ((this->DLBLER.plot_idx > this->chartWidth) ||
      (this->indexToPlot != this->previousIndex))
  {
    this->DLBLER.plot_idx = 0;
    resetKPIPlot(&this->DLBLER);
  }

  float Xpaint, Ypaint;
  Xpaint = this->DLBLER.plot_idx;
  Ypaint = (float)sched_ctrl->dl_bler_stats.bler;
  this->DLBLER.series->append(Xpaint, Ypaint);

  QLineSeries *series_LowLim = new QLineSeries();
  series_LowLim->append(0, Limits_KPI_gNB[2][0]);
  series_LowLim->append(this->chartWidth, Limits_KPI_gNB[2][0]);
  series_LowLim->setColor(QColor(0, 255, 0));

  QLineSeries *series_UppLim = new QLineSeries();
  series_UppLim->append(0, Limits_KPI_gNB[2][1]);
  series_UppLim->append(this->chartWidth, Limits_KPI_gNB[2][1]);
  series_UppLim->setColor(QColor(255, 0, 0));

  QChart *chart = new QChart();
  chart->legend()->hide();

  int nofTicks = 6;
  QValueAxis *axisX = new QValueAxis;
  axisX->setTickCount(nofTicks);
  axisX->setRange(0 , this->chartWidth);
  axisX->setTitleText("Time Index (calc window: 100 ms)");
  chart->addAxis(axisX, Qt::AlignBottom);

  QValueAxis *axisY = new QValueAxis;
  axisY->setTickCount(nofTicks);
  axisY->setRange(-1, 1.5);
  axisY->setTitleText("DL BLER");
  chart->addAxis(axisY, Qt::AlignLeft);

  chart->addSeries(this->DLBLER.series);
  this->DLBLER.series->attachAxis(axisX);
  this->DLBLER.series->attachAxis(axisY);

  chart->addSeries(series_LowLim);
  series_LowLim->attachAxis(axisX);
  series_LowLim->attachAxis(axisY);

  chart->addSeries(series_UppLim);
  series_UppLim->attachAxis(axisX);
  series_UppLim->attachAxis(axisY);

  QChartView *chartView = new QChartView(chart);
  chartView->resize(this->chartWidth, this->chartHight);

  QPixmap p = chartView->grab();
  *this->pix = p;

  this->previousIndex = this->parentWindow->currentIndex();
  this->DLBLER.plot_idx++;
  update();
}

void PainterWidgetgNB::KPI_UL_BLER()
{
  // erase the previous paint
  this->pix->fill(QColor(240,240,240));

  gNB_MAC_INST *gNBMac = (gNB_MAC_INST *)RC.nrmac[0];
  NR_UE_info_t *targetUE = gNBMac->UE_info.list[0];
  NR_UE_sched_ctrl_t *sched_ctrl = &targetUE->UE_sched_ctrl;

  if ((this->ULBLER.plot_idx > this->chartWidth) ||
      (this->indexToPlot != this->previousIndex))
  {
    this->ULBLER.plot_idx = 0;
    resetKPIPlot(&this->ULBLER);
  }

  float Xpaint, Ypaint;
  Xpaint = this->ULBLER.plot_idx;
  Ypaint = (float)sched_ctrl->ul_bler_stats.bler;
  this->ULBLER.series->append(Xpaint, Ypaint);

  QLineSeries *series_LowLim = new QLineSeries();
  series_LowLim->append(0, Limits_KPI_gNB[0][0]);
  series_LowLim->append(this->chartWidth, Limits_KPI_gNB[0][0]);
  series_LowLim->setColor(QColor(0, 255, 0));

  QLineSeries *series_UppLim = new QLineSeries();
  series_UppLim->append(0, Limits_KPI_gNB[0][1]);
  series_UppLim->append(this->chartWidth, Limits_KPI_gNB[0][1]);
  series_UppLim->setColor(QColor(255, 0, 0));

  QChart *chart = new QChart();
  chart->legend()->hide();

  int nofTicks = 6;
  QValueAxis *axisX = new QValueAxis;
  axisX->setTickCount(nofTicks);
  axisX->setRange(0 , this->chartWidth);
  axisX->setTitleText("Time Index (calc window: 100 ms)");
  chart->addAxis(axisX, Qt::AlignBottom);

  QValueAxis *axisY = new QValueAxis;
  axisY->setTickCount(nofTicks);
  axisY->setRange(-1, 1.5);
  axisY->setTitleText("UL BLER");
  chart->addAxis(axisY, Qt::AlignLeft);


  chart->addSeries(this->ULBLER.series);
  this->ULBLER.series->attachAxis(axisX);
  this->ULBLER.series->attachAxis(axisY);

  chart->addSeries(series_LowLim);
  series_LowLim->attachAxis(axisX);
  series_LowLim->attachAxis(axisY);

  chart->addSeries(series_UppLim);
  series_UppLim->attachAxis(axisX);
  series_UppLim->attachAxis(axisY);

  QChartView *chartView = new QChartView(chart);
  chartView->resize(this->chartWidth, this->chartHight);

  QPixmap p = chartView->grab();
  *this->pix = p;

  this->previousIndex = this->parentWindow->currentIndex();
  this->ULBLER.plot_idx++;
  update();
}

void PainterWidgetgNB::KPI_UL_MCS()
{
  // erase the previous paint
  this->pix->fill(QColor(240,240,240));

  gNB_MAC_INST *gNBMac = (gNB_MAC_INST *)RC.nrmac[0];
  NR_UE_info_t *targetUE = gNBMac->UE_info.list[0];
  NR_UE_sched_ctrl_t *sched_ctrl = &targetUE->UE_sched_ctrl;

  if ((this->ULMCS.plot_idx > this->chartWidth) ||
      (this->indexToPlot != this->previousIndex))
  {
    this->ULMCS.plot_idx = 0;
    resetKPIPlot(&this->ULMCS);

    if (this->indexToPlot != this->previousIndex)
      resetKPIValues(&this->ULMCS);
  }

  float Xpaint, Ypaint;
  Xpaint = this->ULMCS.plot_idx;
  Ypaint = (float)sched_ctrl->ul_bler_stats.mcs;

  this->ULMCS.max_value = std::max(this->ULMCS.max_value, Ypaint);
  this->ULMCS.min_value = std::min(this->ULMCS.min_value, Ypaint);

  this->ULMCS.series->append(Xpaint, Ypaint);
  this->ULMCS.seriesMin->append(Xpaint, this->ULMCS.min_value);
  this->ULMCS.seriesMax->append(Xpaint, this->ULMCS.max_value);

  QChart *chart = new QChart();
  chart->legend()->show();

  int nofTicks = 6;
  QValueAxis *axisX = new QValueAxis;
  axisX->setTickCount(nofTicks);
  axisX->setRange(0 , this->chartWidth);
  axisX->setTitleText("Time Index");
  chart->addAxis(axisX, Qt::AlignBottom);

  QValueAxis *axisY = new QValueAxis;
  axisY->setTickCount(nofTicks);
  axisY->setRange(-1, this->ULMCS.max_value + 2.0);
  axisY->setTitleText("UL MCS");
  chart->addAxis(axisY, Qt::AlignLeft);

  chart->addSeries(this->ULMCS.series);
  this->ULMCS.series->attachAxis(axisX);
  this->ULMCS.series->attachAxis(axisY);

  chart->addSeries(this->ULMCS.seriesMin);
  this->ULMCS.seriesMin->attachAxis(axisX);
  this->ULMCS.seriesMin->attachAxis(axisY);

  chart->addSeries(this->ULMCS.seriesMax);
  this->ULMCS.seriesMax->attachAxis(axisX);
  this->ULMCS.seriesMax->attachAxis(axisY);

  QChartView *chartView = new QChartView(chart);
  chartView->resize(this->chartWidth, this->chartHight);

  QPixmap p = chartView->grab();
  *this->pix = p;

  this->previousIndex = this->parentWindow->currentIndex();
  this->ULMCS.plot_idx++;
  update();
}

void PainterWidgetgNB::KPI_PuschIQ()
{
  // erase the previous paint
  this->pix->fill(QColor(240,240,240));

  //paint the axis and I/Q samples
  NR_DL_FRAME_PARMS *frame_parms=&this->p->gNB->frame_parms;
  int sz=frame_parms->N_RB_UL*12*frame_parms->symbols_per_slot;

  int ue = 0;
  float *I, *Q;
  float FIinit[sz] = { 0 }, FQinit[sz] = { 0 };
  I = FIinit;
  Q = FQinit;

  if ((this->p->gNB->pusch_vars) &&
      (this->p->gNB->pusch_vars[ue]) &&
      (this->p->gNB->pusch_vars[ue]->rxdataF_comp) &&
      (this->p->gNB->pusch_vars[ue]->rxdataF_comp[0]))
  {
    scopeSample_t *pusch_comp = (scopeSample_t *) this->p->gNB->pusch_vars[ue]->rxdataF_comp[0];
    for (int k=0; k<sz; k++ )
    {
      I[k] = pusch_comp[k].r;
      Q[k] = pusch_comp[k].i;
    }
  }

  QColor MarkerColor(0, 255, 0);
  const QString xLabel = QString("Real");
  const QString yLabel = QString("Img");
  createPixMap(I, Q, sz, MarkerColor, xLabel, yLabel, true);

  update();
}

void PainterWidgetgNB::KPI_PuschLLR()
{
    // erase the previous paint
    this->pix->fill(QColor(240,240,240));

    //paint the axis LLRs
    int coded_bits_per_codeword =3*8*6144+12;
    int sz = coded_bits_per_codeword;
    int ue = 0;

    float *llr, *bit;
    float llrBuffer[sz] = { 0 }, bitBuffer[sz] = { 0 };
    llr = llrBuffer;
    bit = bitBuffer;

    if ((this->p->gNB->pusch_vars) &&
        (this->p->gNB->pusch_vars[ue]) &&
        (this->p->gNB->pusch_vars[ue]->llr))
    {
      int16_t *pusch_llr = (int16_t *)p->gNB->pusch_vars[ue]->llr;
      for (int i=0; i<sz; i++)
      {
        llr[i] = (float) pusch_llr[i];
        bit[i] = (float) i;
      }
    }

    QColor MarkerColor(0, 255, 0);
    const QString xLabel = QString("Sample Index");
    const QString yLabel = QString("LLR");
    createPixMap(bit, llr, sz, MarkerColor, xLabel, yLabel, false);

    update();
}

void PainterWidgetgNB::KPI_ChannelResponse()
{
  this->pix->fill(QColor(240,240,240));

  const int len=2*this->p->gNB->frame_parms.ofdm_symbol_size;
  float *values, *time;
  float valuesBuffer[len] = { 0 }, timeBuffer[len] = { 0 };
  values = valuesBuffer;
  time = timeBuffer;
  const int ant=0;

  int ue = 0;

  if ((this->p->gNB->pusch_vars && p->gNB->pusch_vars[ue]) &&
      (this->p->gNB->pusch_vars[ue]->ul_ch_estimates_time) &&
      (this->p->gNB->pusch_vars[ue]->ul_ch_estimates_time[ant]))
  {
    scopeSample_t *data= (scopeSample_t *)this->p->gNB->pusch_vars[ue]->ul_ch_estimates_time[ant];

    if (data != NULL)
    {
      for (int i=0; i<len; i++)
      {
        values[i] = SquaredNorm(data[i]);
        time[i] = (float) i;
      }
    }

  }

  float maxY=0, minY=0;
  for (int k=0; k<len; k++) {
    maxY=std::max(maxY,values[k]);
    minY=std::min(minY,values[k]);
  }

  float maxYAbs = std::max(abs(maxY),abs(minY));

  QLineSeries *series = new QLineSeries();
  QColor MarkerColor(255, 0, 0);
  series->setColor(MarkerColor);

  float Xpaint, Ypaint;

  float minYScaled=0, maxYScaled=0;
  for (int k=0; k<len; k++) {
    Xpaint = time[k];
    Ypaint = values[k]/maxYAbs*50;

    series->append(Xpaint, Ypaint);
    maxYScaled=std::max(maxYScaled,Ypaint);
    minYScaled=std::min(minYScaled,Ypaint);
  }

  QChart *chart = new QChart();
  chart->legend()->hide();

  int nofTicks = 6;
  QValueAxis *axisX = new QValueAxis;
  axisX->setTickCount(nofTicks);
  axisX->setRange(0 , len);
  axisX->setTitleText("Time Index");
  chart->addAxis(axisX, Qt::AlignBottom);

  QValueAxis *axisY = new QValueAxis;
  axisY->setTickCount(nofTicks);
  axisY->setRange((minYScaled + 0.4*minYScaled), (maxYScaled + 0.4*maxYScaled));
  axisY->setTitleText("abs Channel");
  chart->addAxis(axisY, Qt::AlignLeft);

  chart->addSeries(series);

  series->attachAxis(axisX);
  series->attachAxis(axisY);

  QChartView *chartView = new QChartView(chart);
  chartView->resize(this->chartWidth, this->chartHight);

  QPixmap p = chartView->grab();
  *this->pix = p;
  update();
}

void PainterWidgetgNB::createPixMap(float *xData, float *yData, int len, QColor MarkerColor,
                                    const QString xLabel, const QString yLabel, bool scaleX)
{
  float maxX=0, maxY=0, minX=0, minY=0;
  for (int k=0; k<len; k++) {
    maxX=std::max(maxX,xData[k]);
    minX=std::min(minX,xData[k]);
    maxY=std::max(maxY,yData[k]);
    minY=std::min(minY,yData[k]);
  }

  float maxYAbs = std::max(abs(maxY),abs(minY));
  float maxXAbs = std::max(abs(maxX),abs(minX));

  QScatterSeries *series = new QScatterSeries();
  series->setUseOpenGL(true);
  series->setColor(MarkerColor);
  series->setMarkerSize(2);
  series->setBorderColor(Qt::transparent);
  series->setMarkerShape(QScatterSeries::MarkerShapeCircle);

  float Xpaint, Ypaint;

  QVector<QPointF> points(len);

  float minYScaled=0, maxYScaled=0, maxXScaled = 0, minXScaled = 0;
  for (int k=0; k<len; k++) {

    if (maxYAbs != 0)
      Ypaint = yData[k]/maxYAbs*50;
    else
      Ypaint = yData[k];

    if ((maxXAbs != 0) && (scaleX))
      Xpaint = xData[k]/maxXAbs*50;
    else
      Xpaint = xData[k];

    points[k] = QPointF(Xpaint, Ypaint);
    //series->append(Xpaint, Ypaint);
    maxYScaled=std::max(maxYScaled,Ypaint);
    minYScaled=std::min(minYScaled,Ypaint);
    maxXScaled=std::max(maxXScaled,Xpaint);
    minXScaled=std::min(minXScaled,Xpaint);
  }

  series->replace(points);
  QChart *chart = new QChart();
  chart->legend()->hide();

  int nofTicks = 6;
  QValueAxis *axisX = new QValueAxis;
  axisX->setTickCount(nofTicks);

  if (!scaleX)
    axisX->setRange(0 , len);
  else
  {
    if ((minXScaled != 0) && (maxXScaled != 0))
      axisX->setRange((minXScaled + 0.4*minXScaled) , (maxXScaled + 0.4*maxXScaled));
    else
      axisX->setRange(-10,10);
  }

  axisX->setTitleText(xLabel);
  chart->addAxis(axisX, Qt::AlignBottom);

  QValueAxis *axisY = new QValueAxis;
  axisY->setTickCount(nofTicks);
  if ((minYScaled != 0) && (maxYScaled != 0))
    axisY->setRange((minYScaled + 0.4*minYScaled), (maxYScaled + 0.4*maxYScaled));
  else
    axisY->setRange(-10, 10);
  axisY->setTitleText(yLabel);
  chart->addAxis(axisY, Qt::AlignLeft);

  chart->addSeries(series);

  series->attachAxis(axisX);
  series->attachAxis(axisY);

  QChartView *chartView = new QChartView();
  chartView->resize(this->chartWidth, this->chartHight);
  chartView->setChart(chart);

  QPixmap pixFromChart = chartView->grab();
  *this->pix = pixFromChart;
}

void PainterWidget::paintPixmap_ueWaterFallTime()
{
    QPainter PixPainter(this->pix);

    PixPainter.translate(0, this->pix->height()/2);

    scopeSample_t *values = (scopeSample_t *) this->ue->common_vars.rxdata[0];
    const int datasize = this->ue->frame_parms.samples_per_frame;

    if (values == NULL)
      return;

    this->waterFallh = this->chartHight/3 - 15;
    const int samplesPerPixel = datasize/this->chartWidth;
    int displayPart = this->waterFallh - ScaleZone;

    int row = this->iteration%displayPart;
    double avg=0;

    for (int i=0; i < displayPart; i++)
      avg+=this->waterFallAvg[i];

    avg/=displayPart;
    this->waterFallAvg[row]=0;

    for (int pix=0; pix<this->chartWidth; pix++)
    {
      scopeSample_t *end=values+(pix+1)*samplesPerPixel;
      end-=2;

      double val=0;

      for (scopeSample_t *s=values+(pix)*samplesPerPixel; s <end; s++)
        val += SquaredNorm(*s);

      val/=samplesPerPixel;
      this->waterFallAvg[row]+=val/this->chartWidth;

      if (val > avg*2 )
      {
        QColor IQColor(0,0,255);
        PixPainter.setPen(IQColor);
      }

      if (val > avg*10 ){
        QColor IQColor(0,255,0);
        PixPainter.setPen(IQColor);
      }

      if (val > avg*100 ){
        QColor IQColor(255,255,0);
        PixPainter.setPen(IQColor);
      }

      PixPainter.drawEllipse( QPoint(pix, this->iteration%displayPart), 2, 2 );

    }

    this->iteration++;
    repaint();

}

void PainterWidget::resetKPIPlot(KPI_elements *inputStruct)
{
  inputStruct->series = new QLineSeries();
  inputStruct->series->setColor(QColor(0,0,0));

  inputStruct->seriesMin = new QLineSeries();
  inputStruct->seriesMin->setColor(QColor(255,0,0));
  inputStruct->seriesMin->setName("Min.");

  inputStruct->seriesMax = new QLineSeries();
  inputStruct->seriesMax->setColor(QColor(0,255,0));
  inputStruct->seriesMax->setName("Max.");

  inputStruct->seriesAvg = new QLineSeries();
  inputStruct->seriesAvg->setColor(QColor(0,0,255));
  inputStruct->seriesAvg->setName("Avg.");
}

void PainterWidget::resetKPIValues(KPI_elements *inputStruct)
{
  inputStruct->max_value = 0.0;
  inputStruct->min_value = 0.0;
  inputStruct->avg_value = 0.0;
  inputStruct->avg_idx = 0;
  inputStruct->nof_retrans = 0;
}

PainterWidget::PainterWidget(QComboBox *parent, PHY_VARS_NR_UE *ue)
{
    timer = new QTimer(this);
    timerWaterFallTime = new QTimer(this);
    this->chartHight = 300;
    this->chartWidth = 300;
    this->pix = new QPixmap(this->chartWidth,this->chartHight);
    this->pix->fill(QColor(240,240,240));
    this->ue = ue;

    this->parentWindow = parent;

    // settings for waterfall graph
    this->iteration = 0;
    this->waterFallh = this->chartHight/3 - 15;
    this->waterFallAvg= (double*) malloc(sizeof(*this->waterFallAvg) * this->waterFallh);
    this->isWaterFallTimeActive = false;

    for (int i=0; i< this->waterFallh; i++)
      this->waterFallAvg[i]=0;

    this->previousIndex = this->parentWindow->currentIndex();
    this->indexToPlot = this->parentWindow->currentIndex();

    this->extendKPIUE.DL_BLER = -1;
    this->extendKPIUE.blockSize = 0;
    this->extendKPIUE.dl_mcs = 100;
    this->extendKPIUE.nofRBs = 34;

    // DL BLER
    this->DLBLER.plot_idx = 0;
    resetKPIPlot(&this->DLBLER);

    // DL MCS
    this->DLMCS.plot_idx = 0;
    resetKPIPlot(&this->DLMCS);
    resetKPIValues(&this->DLMCS);

    // Throughput
    this->Throu.plot_idx = 0;
    resetKPIPlot(&this->Throu);
    resetKPIValues(&this->Throu);

    // nof scheduled RBs
    this->nofRBs.plot_idx = 0;
    resetKPIPlot(&this->nofRBs);
    resetKPIValues(&this->nofRBs);

    makeConnections();
}

void PainterWidget::paintEvent(QPaintEvent *)
{
  QPainter painter(this);
  painter.drawPixmap( (this->width()-this->pix->width())/2,
  (this->height()-this->pix->height())/2, *this->pix); // paint pixmap on widget

  this->indexToPlot = this->parentWindow->currentIndex();
  makeConnections();
  this->previousIndex = this->parentWindow->currentIndex();
}

void PainterWidget::makeConnections()
{
  getKPIUE(&this->extendKPIUE);
  disconnect(timer, nullptr, nullptr, nullptr);
  disconnect(timerWaterFallTime, nullptr, nullptr, nullptr);

  if ((this->indexToPlot != this->previousIndex) && (this->indexToPlot == 6))   // reset settings
  {
    this->pix->fill(QColor(240,240,240));
    this->iteration = 0;
    for (int i=0; i< this->waterFallh; i++)
      this->waterFallAvg[i]=0;
  }

  if (this->indexToPlot == 2)
  {
    if (this->isWaterFallTimeActive)
      this->iteration++;
    connect(timer, &QTimer::timeout, this, &PainterWidget::paintPixmap_uePdschIQ);   // paintPixmap_uePdschIQ
  }
  else if (this->indexToPlot == 0)
  {
    if (this->isWaterFallTimeActive)
      this->iteration++;
    connect(timer, &QTimer::timeout, this, &PainterWidget::paintPixmap_uePbchIQ);   // paintPixmap_uePbchIQ
  }
  else if (this->indexToPlot == 4)
  {
    if (this->isWaterFallTimeActive)
      this->iteration++;
    connect(timer, &QTimer::timeout, this, &PainterWidget::paintPixmap_uePdcchIQ);
  }
  else if (this->indexToPlot == 3)
  {
    if (this->isWaterFallTimeActive)
      this->iteration++;
    connect(timer, &QTimer::timeout, this, &PainterWidget::paintPixmap_uePdschLLR);    // paintPixmap_uePdschLLR
  }
  else if (this->indexToPlot == 1)
  {
    if (this->isWaterFallTimeActive)
      this->iteration++;
    connect(timer, &QTimer::timeout, this, &PainterWidget::paintPixmap_uePbchLLR);    // paintPixmap_uePbchLLR
  }
  else if (this->indexToPlot == 5)
  {
    if (this->isWaterFallTimeActive)
      this->iteration++;
    connect(timer, &QTimer::timeout, this, &PainterWidget::paintPixmap_uePdcchLLR);
  }
  else if (this->indexToPlot == 6)
  {
    this->isWaterFallTimeActive = true;
    connect(timerWaterFallTime, &QTimer::timeout, this, &PainterWidget::paintPixmap_ueWaterFallTime);    // water fall time domain
  }
  else if (this->indexToPlot == 7)
  {
    if (this->isWaterFallTimeActive)
      this->iteration++;
    connect(timer, &QTimer::timeout, this, &PainterWidget::paintPixmap_ueChannelResponse);    // Channel Response
  }
  else if (this->indexToPlot == 8)
  {
    connect(timer, &QTimer::timeout, this, &PainterWidget::KPI_DL_BLER);
  }
  else if (this->indexToPlot == 9)
  {
    connect(timer, &QTimer::timeout, this, &PainterWidget::KPI_DL_Throu);
  }
  else if (this->indexToPlot == 10)
  {
    connect(timer, &QTimer::timeout, this, &PainterWidget::KPI_DL_MCS);
  }
  else if (this->indexToPlot == 11)
  {
    connect(timer, &QTimer::timeout, this, &PainterWidget::KPI_Nof_RBs);
  }
  timer->start(200);
  timerWaterFallTime->start(10);
}

void PainterWidget::KPI_Nof_RBs()
{
  // erase the previous paint
  this->pix->fill(QColor(240,240,240));

  if ((this->nofRBs.plot_idx > this->chartWidth) ||
      (this->indexToPlot != this->previousIndex))
  {
    this->nofRBs.plot_idx = 0;
    resetKPIPlot(&this->nofRBs);
  }

  float Xpaint, Ypaint;
  Xpaint = this->nofRBs.plot_idx;
  Ypaint = (float)this->extendKPIUE.nofRBs;

  this->nofRBs.max_value = std::max(this->nofRBs.max_value, Ypaint);
  this->nofRBs.series->append(Xpaint, Ypaint);

  QChart *chart = new QChart();
  chart->legend()->hide();

  int nofTicks = 6;
  QValueAxis *axisX = new QValueAxis;
  axisX->setTickCount(nofTicks);
  axisX->setRange(0 , this->chartWidth);
  axisX->setTitleText("Time Index");
  chart->addAxis(axisX, Qt::AlignBottom);

  QValueAxis *axisY = new QValueAxis;
  axisY->setTickCount(nofTicks);
  axisY->setRange(-1, this->nofRBs.max_value + 10);
  axisY->setTitleText("Nof Scheduled RBs");
  chart->addAxis(axisY, Qt::AlignLeft);

  chart->addSeries(this->nofRBs.series);
  this->nofRBs.series->attachAxis(axisX);
  this->nofRBs.series->attachAxis(axisY);

  QChartView *chartView = new QChartView(chart);
  chartView->resize(this->chartWidth, this->chartHight);

  QPixmap p = chartView->grab();
  *this->pix = p;

  this->previousIndex = this->parentWindow->currentIndex();
  this->nofRBs.plot_idx++;
  update();
}

void PainterWidget::KPI_DL_Throu()
{
  // erase the previous paint
  this->pix->fill(QColor(240,240,240));

  uint32_t blockSize = (uint32_t)this->extendKPIUE.blockSize;
  float bler_dl = this->extendKPIUE.DL_BLER;

  NR_DL_FRAME_PARMS *frame_parms = &this->ue->frame_parms;
  uint16_t slots_per_frame = frame_parms->slots_per_frame;
  float slotDuration = 10.0/(float)slots_per_frame;      // slot duration in msec

  double blerTerm = 1.0 - (double)bler_dl;
  double blockSizeBits = (double)(blockSize << 3);

  double ThrouputKBitSec = blerTerm * blockSizeBits / (double)slotDuration;

  if ((this->Throu.plot_idx > this->chartWidth) ||
      (this->indexToPlot != this->previousIndex))
  {
    this->Throu.plot_idx = 0;
    resetKPIPlot(&this->Throu);
  }

  float Xpaint, Ypaint;
  Xpaint = this->Throu.plot_idx;
  Ypaint = (float)(ThrouputKBitSec/1000);    // Throughput in MBit/sec

  this->Throu.max_value = std::max(this->Throu.max_value, Ypaint);
  this->Throu.series->append(Xpaint, Ypaint);

  QLineSeries *series_LowLim = new QLineSeries();
  series_LowLim->append(0, Limits_KPI_gNB[1][0]);
  series_LowLim->append(this->chartWidth, Limits_KPI_gNB[1][0]);
  series_LowLim->setColor(QColor(0, 255, 0));

  QLineSeries *series_UppLim = new QLineSeries();
  series_UppLim->append(0, Limits_KPI_gNB[1][1]);
  series_UppLim->append(this->chartWidth, Limits_KPI_gNB[1][1]);
  series_UppLim->setColor(QColor(255, 0, 0));

  QChart *chart = new QChart();
  chart->legend()->hide();

  int nofTicks = 6;
  QValueAxis *axisX = new QValueAxis;
  axisX->setTickCount(nofTicks);
  axisX->setRange(0 , this->chartWidth);
  axisX->setTitleText("Time Index");
  chart->addAxis(axisX, Qt::AlignBottom);

  QValueAxis *axisY = new QValueAxis;
  axisY->setTickCount(nofTicks);
  axisY->setRange(-1, 1.2*this->Throu.max_value);
  axisY->setTitleText("UL Throughput Mbit/sec");
  chart->addAxis(axisY, Qt::AlignLeft);

  chart->addSeries(this->Throu.series);
  this->Throu.series->attachAxis(axisX);
  this->Throu.series->attachAxis(axisY);

  chart->addSeries(series_LowLim);
  series_LowLim->attachAxis(axisX);
  series_LowLim->attachAxis(axisY);

  chart->addSeries(series_UppLim);
  series_UppLim->attachAxis(axisX);
  series_UppLim->attachAxis(axisY);

  QChartView *chartView = new QChartView(chart);
  chartView->resize(this->chartWidth, this->chartHight);

  QPixmap p = chartView->grab();
  *this->pix = p;

  this->previousIndex = this->parentWindow->currentIndex();
  this->Throu.plot_idx++;
  update();
}

void PainterWidget::KPI_DL_MCS()
{
  // erase the previous paint
  this->pix->fill(QColor(240,240,240));

  if ((this->DLMCS.plot_idx > this->chartWidth) ||
      (this->indexToPlot != this->previousIndex))
  {
    this->DLMCS.plot_idx = 0;
    resetKPIPlot(&this->DLMCS);

    if(this->indexToPlot != this->previousIndex)
      resetKPIValues(&this->DLMCS);
  }

  float Xpaint, Ypaint;
  Xpaint = this->DLMCS.plot_idx;
  Ypaint = (float)this->extendKPIUE.dl_mcs;

  this->DLMCS.max_value = std::max(this->DLMCS.max_value, Ypaint);
  this->DLMCS.min_value = std::min(this->DLMCS.min_value, Ypaint);

  this->DLMCS.series->append(Xpaint, Ypaint);
  this->DLMCS.seriesMin->append(Xpaint, this->DLMCS.min_value);
  this->DLMCS.seriesMax->append(Xpaint, this->DLMCS.max_value);

  QChart *chart = new QChart();
  chart->legend()->show();

  int nofTicks = 6;
  QValueAxis *axisX = new QValueAxis;
  axisX->setTickCount(nofTicks);
  axisX->setRange(0 , this->chartWidth);
  axisX->setTitleText("Time Index");
  chart->addAxis(axisX, Qt::AlignBottom);

  QValueAxis *axisY = new QValueAxis;
  axisY->setTickCount(nofTicks);
  axisY->setRange(-1, this->DLMCS.max_value + 2.0);
  axisY->setTitleText("DL MCS");
  chart->addAxis(axisY, Qt::AlignLeft);

  chart->addSeries(this->DLMCS.series);
  this->DLMCS.series->attachAxis(axisX);
  this->DLMCS.series->attachAxis(axisY);

  chart->addSeries(this->DLMCS.seriesMin);
  this->DLMCS.seriesMin->attachAxis(axisX);
  this->DLMCS.seriesMin->attachAxis(axisY);

  chart->addSeries(this->DLMCS.seriesMax);
  this->DLMCS.seriesMax->attachAxis(axisX);
  this->DLMCS.seriesMax->attachAxis(axisY);

  QChartView *chartView = new QChartView(chart);
  chartView->resize(this->chartWidth, this->chartHight);

  QPixmap p = chartView->grab();
  *this->pix = p;

  this->previousIndex = this->parentWindow->currentIndex();
  this->DLMCS.plot_idx++;
  update();
}

void PainterWidget::KPI_DL_BLER()
{
  // erase the previous paint
  this->pix->fill(QColor(240,240,240));

  if ((this->DLBLER.plot_idx > this->chartWidth) ||
      (this->indexToPlot != this->previousIndex))
  {
    this->DLBLER.plot_idx = 0;
    resetKPIPlot(&this->DLBLER);
  }

  float Xpaint, Ypaint;
  Xpaint = this->DLBLER.plot_idx;
  Ypaint = this->extendKPIUE.DL_BLER;
  this->DLBLER.series->append(Xpaint, Ypaint);

  QLineSeries *series_LowLim = new QLineSeries();
  series_LowLim->append(0, Limits_KPI_ue[0][0]);
  series_LowLim->append(this->chartWidth, Limits_KPI_ue[0][0]);
  series_LowLim->setColor(QColor(0, 255, 0));

  QLineSeries *series_UppLim = new QLineSeries();
  series_UppLim->append(0, Limits_KPI_ue[0][1]);
  series_UppLim->append(this->chartWidth, Limits_KPI_ue[0][1]);
  series_UppLim->setColor(QColor(255, 0, 0));

  QChart *chart = new QChart();
  chart->legend()->hide();

  int nofTicks = 6;
  QValueAxis *axisX = new QValueAxis;
  axisX->setTickCount(nofTicks);
  axisX->setRange(0 , this->chartWidth);
  axisX->setTitleText("Time Index");
  chart->addAxis(axisX, Qt::AlignBottom);

  QValueAxis *axisY = new QValueAxis;
  axisY->setTickCount(nofTicks);
  axisY->setRange(-1, 1.5);
  axisY->setTitleText("DL BLER");
  chart->addAxis(axisY, Qt::AlignLeft);

  chart->addSeries(this->DLBLER.series);
  this->DLBLER.series->attachAxis(axisX);
  this->DLBLER.series->attachAxis(axisY);

  chart->addSeries(series_LowLim);
  series_LowLim->attachAxis(axisX);
  series_LowLim->attachAxis(axisY);

  chart->addSeries(series_UppLim);
  series_UppLim->attachAxis(axisX);
  series_UppLim->attachAxis(axisY);

  QChartView *chartView = new QChartView(chart);
  chartView->resize(this->chartWidth, this->chartHight);

  QPixmap p = chartView->grab();
  *this->pix = p;

  this->previousIndex = this->parentWindow->currentIndex();
  this->DLBLER.plot_idx++;
  update();
}

void PainterWidget::paintPixmap_ueChannelResponse()
{
  scopeData_t *scope=(scopeData_t *) this->ue->scopeData;
  scope->flag_streaming[pbchDlChEstimateTime] = 1;

  scopeGraphData_t **UELiveData = (scopeGraphData_t**)(( scopeData_t *)this->ue->scopeData)->liveDataUE;

  if (!UELiveData[pbchDlChEstimateTime])
    return;

  const scopeSample_t *tmp=(scopeSample_t *)(UELiveData[pbchDlChEstimateTime]+1);
  const scopeSample_t **data=&tmp;
  const int len = UELiveData[pbchDlChEstimateTime]->lineSz;

  float *values, *time;
  float valuesBuffer[len] = { 0 }, timeBuffer[len] = { 0 };
  values = valuesBuffer;
  time = timeBuffer;

  int idx = 0;
  int nb_ant = 1;
  int eNB_id = 0;
  for (int ant=0; ant<nb_ant; ant++)
  {
    if (data[ant] != NULL)
    {
      for (int i=0; i<len; i++)
      {
        values[i] = SquaredNorm(data[ant][i]);
        time[i] = (float) idx;
        idx++;
      }
    }
  }

  float maxY=0, minY=0;
  for (int k=0; k<len; k++) {
    maxY=std::max(maxY,values[k]);
    minY=std::min(minY,values[k]);
  }

  float maxYAbs = std::max(abs(maxY),abs(minY));

  QLineSeries *series = new QLineSeries();
  QColor MarkerColor(255, 0, 0);
  series->setColor(MarkerColor);

  float Xpaint, Ypaint;
  float minYScaled=0, maxYScaled=0;
  for (int k=0; k<len; k++) {
    Xpaint = time[k];
    Ypaint = values[k]/maxYAbs*50;

    series->append(Xpaint, Ypaint);

    maxYScaled=std::max(maxYScaled,Ypaint);
    minYScaled=std::min(minYScaled,Ypaint);
  }

  QChart *chart = new QChart();
  chart->legend()->hide();

  int nofTicks = 6;
  QValueAxis *axisX = new QValueAxis;
  axisX->setTickCount(nofTicks);
  axisX->setRange(0 , len);
  axisX->setTitleText("Time Index");
  chart->addAxis(axisX, Qt::AlignBottom);

  QValueAxis *axisY = new QValueAxis;
  axisY->setTickCount(nofTicks);
  axisY->setRange((minYScaled + 0.4*minYScaled), (maxYScaled + 0.4*maxYScaled));
  axisY->setTitleText("abs Channel");
  chart->addAxis(axisY, Qt::AlignLeft);

  chart->addSeries(series);
  series->attachAxis(axisX);
  series->attachAxis(axisY);

  QChartView *chartView = new QChartView(chart);
  chartView->resize(this->chartWidth, this->chartHight);

  QPixmap p = chartView->grab();
  QPainter PixPainter(&p);
  float freq_offset = (float)this->ue->common_vars.freq_offset;
  std::string strFreq = "Freq. Offset= " + std::to_string(freq_offset);
  QString qstrFreq = QString::fromStdString(strFreq);
  PixPainter.drawText(100, 1.6*maxYScaled, qstrFreq);

  float timing_advance = (float)this->ue->timing_advance;
  std::string strTime = "Timing Advance= " + std::to_string(timing_advance);
  QString qstrTime = QString::fromStdString(strTime);
  PixPainter.drawText(100, 1.3*maxYScaled, qstrTime);
  *this->pix = p;
  update(); //call paintEvent()
}

void PainterWidget::paintPixmap_uePdschIQ()
{
  // erase the previous paint
  this->pix->fill(QColor(240,240,240));

  //paint the axis and I/Q samples
  if (!this->ue->pdsch_vars[0][0]->rxdataF_comp0[0])
      return;

  NR_DL_FRAME_PARMS *frame_parms = &this->ue->frame_parms;
  int sz=7*2*frame_parms->N_RB_DL*12; // size of the malloced buffer
  int base = 0;
  float *I, *Q;
  float FIinit[sz * RX_NB_TH_MAX * sizeof(*I)] = { 0 }, FQinit[sz * RX_NB_TH_MAX * sizeof(*Q)] = { 0 };
  I = FIinit;
  Q = FQinit;
  for (int thr=0 ; thr < RX_NB_TH_MAX ; thr ++ ) {
    scopeSample_t *pdsch_comp = (scopeSample_t *) this->ue->pdsch_vars[thr][0]->rxdataF_comp0[0];
    for (int s=0; s<sz; s++) {
      I[s+base] += pdsch_comp[s].r;
      Q[s+base] += pdsch_comp[s].i;
    }
    base += sz;
  }

  float maxX=0, maxY=0, minX=0, minY=0;
  for (int k=0; k<base; k++) {
    maxX=std::max(maxX,I[k]);
    minX=std::min(minX,I[k]);
    maxY=std::max(maxY,Q[k]);
    minY=std::min(minY,Q[k]);
  }
  float maxXAbs = std::max(abs(maxX),abs(minX));
  float maxYAbs = std::max(abs(maxY),abs(minY));

  float Ipaint, Qpaint;
  QScatterSeries *series = new QScatterSeries();
  series->setUseOpenGL(true);

  float maxXScaled=0, maxYScaled=0, minXScaled=0, minYScaled=0;

  QVector<QPointF> points(base);   // sz * RX_NB_TH_MAX * sizeof(*I)

  for (int k=0; k<base; k++) { //scale the I/Q samples!
    Ipaint = I[k]/maxXAbs*50;
    Qpaint = Q[k]/maxYAbs*50;

    points[k] = QPointF(Ipaint, Qpaint);

    maxXScaled=std::max(maxXScaled,Ipaint);
    minXScaled=std::min(minXScaled,Ipaint);
    maxYScaled=std::max(maxYScaled,Qpaint);
    minYScaled=std::min(minYScaled,Qpaint);
  }

  series->replace(points);
  QColor MarkerColor(0, 255, 0);
  series->setColor(MarkerColor);
  series->setMarkerSize(2);
  series->setBorderColor(Qt::transparent);
  series->setMarkerShape(QScatterSeries::MarkerShapeCircle);

  QChart *chart = new QChart();
  chart->legend()->hide();

  int nofTicks = 6;
  QValueAxis *axisX = new QValueAxis;
  axisX->setTickCount(nofTicks);
  axisX->setRange((minXScaled + 0.4*minXScaled) , (maxXScaled + 0.4*maxXScaled));
  axisX->setTitleText("Real");
  chart->addAxis(axisX, Qt::AlignBottom);

  QValueAxis *axisY = new QValueAxis;
  axisY->setTickCount(nofTicks);
  axisY->setRange((minYScaled + 0.4*minYScaled), (maxYScaled + 0.4*maxYScaled));
  axisY->setTitleText("Imag");
  chart->addAxis(axisY, Qt::AlignLeft);

  chart->addSeries(series);
  series->attachAxis(axisX);
  series->attachAxis(axisY);

  QChartView *chartView = new QChartView();
  chartView->resize(this->chartWidth, this->chartHight);
  chartView->setChart(chart);

  QPixmap p = chartView->grab();
  *this->pix = p;
  update(); //call paintEvent()
}

void PainterWidget::resizeEvent(QResizeEvent *event)
{
  if ((width() != this->chartWidth) || (height() != this->chartHight))
  {
    this->chartHight = height();
    this->chartWidth = width();

    // reset for waterfall plot
    if (this->indexToPlot == 6)
    {
      QPixmap *newPix = new QPixmap(this->chartWidth,this->chartHight);
      this->pix = newPix;
      this->pix->fill(QColor(240,240,240));
      this->iteration = 0;
      this->waterFallh = this->chartHight/3 - 15;
      for (int i=0; i< this->waterFallh; i++)
        this->waterFallAvg[i]=0;
    }

    update();
  }
  QWidget::resizeEvent(event);
}

void PainterWidget::paintPixmap_uePdcchIQ()
{
    this->pix->fill(QColor(240,240,240));

    scopeData_t *scope=(scopeData_t *) this->ue->scopeData;
    scope->flag_streaming[pdcchRxdataF_comp] = 1;

    scopeGraphData_t **UELiveData = (scopeGraphData_t**)(( scopeData_t *)this->ue->scopeData)->liveDataUE;

    if (!UELiveData[pdcchRxdataF_comp])
      return;

    const int len=UELiveData[pdcchRxdataF_comp]->lineSz;

    scopeSample_t *pdcch_comp = (scopeSample_t *) (UELiveData[pdcchRxdataF_comp]+1);

    float *I, *Q;
    float FIinit[len] = { 0 }, FQinit[len] = { 0 };
    I = FIinit;
    Q = FQinit;

    for (int i=0; i<len; i++) {
      I[i] = pdcch_comp[i].r;
      Q[i] = pdcch_comp[i].i;
    }

    float maxX=0, maxY=0, minX=0, minY=0;
    for (int k=0; k<len; k++) {
      maxX=std::max(maxX,I[k]);
      minX=std::min(minX,I[k]);
      maxY=std::max(maxY,Q[k]);
      minY=std::min(minY,Q[k]);
    }
    float maxXAbs = std::max(abs(maxX),abs(minX));
    float maxYAbs = std::max(abs(maxY),abs(minY));

    QScatterSeries *series = new QScatterSeries();

    series->setUseOpenGL(true);

    QColor MarkerColor(0, 0, 255);
    series->setColor(MarkerColor);
    series->setMarkerSize(2);
    series->setBorderColor(Qt::transparent);
    series->setMarkerShape(QScatterSeries::MarkerShapeCircle);

    float maxXScaled=0, maxYScaled=0, minXScaled=0, minYScaled=0;
    float Ipaint, Qpaint;

    QVector<QPointF> points(len);

    for (int k=0; k<len; k++) { //scale the I/Q samples!

      if (abs(maxXAbs - 0.0) > 100e-6)
        Ipaint = I[k]/maxXAbs*50;
      else
        Ipaint = I[k];

      if (abs(maxYAbs - 0.0) > 100e-6)
        Qpaint = Q[k]/maxYAbs*50;
      else
        Qpaint = Q[k];

      //series->append(Ipaint, Qpaint);
      points[k] = QPointF(Ipaint, Qpaint);

      maxXScaled=std::max(maxXScaled,Ipaint);
      minXScaled=std::min(minXScaled,Ipaint);
      maxYScaled=std::max(maxYScaled,Qpaint);
      minYScaled=std::min(minYScaled,Qpaint);
    }

    series->replace(points);

    QChart *chart = new QChart();
    chart->legend()->hide();

    int nofTicks = 6;
    QValueAxis *axisX = new QValueAxis;
    axisX->setTickCount(nofTicks);
    if ( (minXScaled != 0) && (maxXScaled != 0))
    {
      axisX->setRange((minXScaled + 0.4*minXScaled) , (maxXScaled + 0.4*maxXScaled));
    }
    else
    {
      axisX->setRange(-10,10);
    }
    axisX->setTitleText("Real");
    chart->addAxis(axisX, Qt::AlignBottom);

    QValueAxis *axisY = new QValueAxis;
    axisY->setTickCount(nofTicks);
    if ((minYScaled != 0) && (maxYScaled != 0))
    {
      axisY->setRange((minYScaled + 0.4*minYScaled), (maxYScaled + 0.4*maxYScaled));
    }
    else
    {
      axisY->setRange(-10,10);
    }
    axisY->setTitleText("Imag");
    chart->addAxis(axisY, Qt::AlignLeft);

    chart->addSeries(series);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->resize(this->chartWidth, this->chartHight);

    QPixmap p = chartView->grab();
    *this->pix = p;

    update(); //call paintEvent()
}

void PainterWidget::paintPixmap_uePbchIQ()
{
    // erase the previous paint
    this->pix->fill(QColor(240,240,240));

    scopeData_t *scope=(scopeData_t *) this->ue->scopeData;
    scope->flag_streaming[pbchRxdataF_comp] = 1;

    scopeGraphData_t **data = (scopeGraphData_t**)(( scopeData_t *)this->ue->scopeData)->liveDataUE;

    if (!data[pbchRxdataF_comp])
      return;

    const int len=data[pbchRxdataF_comp]->lineSz;
    scopeSample_t *pbch_comp = (scopeSample_t *) (data[pbchRxdataF_comp]+1);

    float *I, *Q;
    float FIinit[len] = { 0 }, FQinit[len] = { 0 };
    I = FIinit;
    Q = FQinit;

    for (int i=0; i<len; i++) {
      I[i] = pbch_comp[i].r;
      Q[i] = pbch_comp[i].i;
    }

    float maxX=0, maxY=0, minX=0, minY=0;
    for (int k=0; k<len; k++) {
      maxX=std::max(maxX,I[k]);
      minX=std::min(minX,I[k]);
      maxY=std::max(maxY,Q[k]);
      minY=std::min(minY,Q[k]);
    }

    float maxXAbs = std::max(abs(maxX),abs(minX));
    float maxYAbs = std::max(abs(maxY),abs(minY));

    QScatterSeries *series = new QScatterSeries();
    series->setUseOpenGL(true);
    QColor MarkerColor(255, 0, 0);
    series->setColor(MarkerColor);
    series->setMarkerSize(2);
    series->setBorderColor(Qt::transparent);
    series->setMarkerShape(QScatterSeries::MarkerShapeCircle);

    int Ipaint, Qpaint;
    QVector<QPointF> points(len);

    for (int k=0; k<len; k++) {
      Ipaint = I[k]/maxXAbs*50;
      Qpaint = Q[k]/maxYAbs*50;

      points[k] = QPointF(Ipaint, Qpaint);
    }

    series->replace(points);
    QChart *chart = new QChart();
    chart->legend()->hide();

    int nofTicks = 6;
    QValueAxis *axisX = new QValueAxis;
    axisX->setTickCount(nofTicks);
    axisX->setRange((minX + 0.4*minX) , (maxX + 0.4*maxX));
    axisX->setTitleText("Real");
    chart->addAxis(axisX, Qt::AlignBottom);

    QValueAxis *axisY = new QValueAxis;
    axisY->setTickCount(nofTicks);
    axisY->setRange((minY + 0.4*minY), (maxY + 0.4*maxY));
    axisY->setTitleText("Imag");
    chart->addAxis(axisY, Qt::AlignLeft);

    chart->addSeries(series);
    series->attachAxis(axisX);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->resize(this->chartWidth, this->chartHight);

    QPixmap p = chartView->grab();
    *this->pix = p;
    update(); //call paintEvent()
}

void PainterWidget::paintPixmap_uePdschLLR()
{
  // erase the previous paint
  this->pix->fill(QColor(240,240,240));

  //paint the axis and LLR values
  if (!this->ue->pdsch_vars[0][0]->llr[0])
      return;

  int num_re = 4500;
  int Qm = 2;
  int coded_bits_per_codeword = num_re*Qm;
  float *llr, *bit;
  float FBitinit[coded_bits_per_codeword * RX_NB_TH_MAX ] = { 0 }, FLlrinit[coded_bits_per_codeword * RX_NB_TH_MAX] = { 0 };
  bit = FBitinit;
  llr = FLlrinit;

  int base=0;
  for (int thr=0 ; thr < RX_NB_TH_MAX ; thr ++ )
  {
    int16_t *pdsch_llr = (int16_t *) this->ue->pdsch_vars[thr][0]->llr[0]; // stream 0
    for (int i=0; i<coded_bits_per_codeword; i++)
    {
      llr[base+i] = (float) pdsch_llr[i];
      bit[base+i] = (float) base+i;
    }
    base+=coded_bits_per_codeword;
  }

  float maxY=0, minY=0;
  for (int k=0; k<base; k++) {
    maxY=std::max(maxY,llr[k]);
    minY=std::min(minY,llr[k]);
  }

  float maxYAbs = std::max(abs(maxY),abs(minY));

  QScatterSeries *series = new QScatterSeries();
  series->setUseOpenGL(true);
  QColor MarkerColor(0, 255, 0);
  series->setColor(MarkerColor);
  series->setMarkerSize(2);
  series->setBorderColor(Qt::transparent);
  series->setMarkerShape(QScatterSeries::MarkerShapeCircle);

  float Xpaint, Ypaint;
  QVector<QPointF> points(base);
  float minYScaled=0, maxYScaled=0;
  for (int k=0; k<base; k++) {
    Xpaint = bit[k];
    Ypaint = llr[k]/maxYAbs*50;

    points[k] = QPointF(Xpaint, Ypaint);
    maxYScaled=std::max(maxYScaled,Ypaint);
    minYScaled=std::min(minYScaled,Ypaint);
  }

  series->replace(points);
  QChart *chart = new QChart();
  chart->legend()->hide();

  int nofTicks = 6;
  QValueAxis *axisX = new QValueAxis;
  axisX->setTickCount(nofTicks);
  axisX->setRange(0 , base);
  axisX->setTitleText("Sample Index");
  chart->addAxis(axisX, Qt::AlignBottom);

  QValueAxis *axisY = new QValueAxis;
  axisY->setTickCount(nofTicks);
  axisY->setRange((minYScaled + 0.4*minYScaled), (maxYScaled + 0.4*maxYScaled));
  axisY->setTitleText("LLR");
  chart->addAxis(axisY, Qt::AlignLeft);

  chart->addSeries(series);
  series->attachAxis(axisX);
  series->attachAxis(axisY);

  QChartView *chartView = new QChartView(chart);
  chartView->resize(this->chartWidth, this->chartHight);

  QPixmap p = chartView->grab();
  *this->pix = p;
  update(); //call paintEvent()
}

void PainterWidget::paintPixmap_uePdcchLLR()
{
  // erase the previous paint
  this->pix->fill(QColor(240,240,240));

  scopeData_t *scope=(scopeData_t *) this->ue->scopeData;
  scope->flag_streaming[pdcchLlr] = 1;

  scopeGraphData_t **data = (scopeGraphData_t**)(( scopeData_t *)this->ue->scopeData)->liveDataUE;

  if (!data[pdcchLlr])
      return;

  const int len=data[pdcchLlr]->lineSz;
  float *llr, *bit;
  float FBitinit[len] = { 0 }, FLlrinit[len] = { 0 };
  bit = FBitinit;
  llr = FLlrinit;

  int16_t *pdcch_llr = (int16_t *)(data[pdcchLlr]+1);

  for (int i=0; i<len; i++)
  {
    llr[i] = (float) pdcch_llr[i];
    bit[i] = (float) i;
  }

  float maxY=0, minY=0;
  for (int k=0; k<len; k++) {
    maxY=std::max(maxY,llr[k]);
    minY=std::min(minY,llr[k]);
  }
  float maxYAbs = std::max(abs(maxY),abs(minY));

  QScatterSeries *series = new QScatterSeries();
  series->setUseOpenGL(true);
  QColor MarkerColor(0, 0, 255);
  series->setColor(MarkerColor);
  series->setMarkerSize(2);
  series->setBorderColor(Qt::transparent);
  series->setMarkerShape(QScatterSeries::MarkerShapeCircle);

  float minYScaled=0, maxYScaled=0;
  float Xpaint, Ypaint;

  QVector<QPointF> points(len);
  for (int k=0; k<len; k++) {
    Xpaint = bit[k];
    Ypaint = llr[k]/maxYAbs*50;

    points[k] = QPointF(Xpaint, Ypaint);
    maxYScaled=std::max(maxYScaled,Ypaint);
    minYScaled=std::min(minYScaled,Ypaint);
  }

  series->replace(points);
  QChart *chart = new QChart();
  chart->legend()->hide();

  int nofTicks = 6;
  QValueAxis *axisX = new QValueAxis;
  axisX->setTickCount(nofTicks);
  axisX->setRange(0 , len);
  axisX->setTitleText("Sample Index");
  chart->addAxis(axisX, Qt::AlignBottom);

  QValueAxis *axisY = new QValueAxis;
  axisY->setTickCount(nofTicks);
  axisY->setRange((minYScaled + 0.4*minYScaled), (maxYScaled + 0.4*maxYScaled));
  axisY->setTitleText("LLR");
  chart->addAxis(axisY, Qt::AlignLeft);

  chart->addSeries(series);
  series->attachAxis(axisX);
  series->attachAxis(axisY);

  QChartView *chartView = new QChartView(chart);
  chartView->resize(this->chartWidth, this->chartHight);

  QPixmap p = chartView->grab();
  *this->pix = p;
  update(); //call paintEvent()
}

void PainterWidget::paintPixmap_uePbchLLR()
{
  this->pix->fill(QColor(240,240,240));

  scopeData_t *scope=(scopeData_t *) this->ue->scopeData;
  scope->flag_streaming[pbchLlr] = 1;

  scopeGraphData_t **data = (scopeGraphData_t**)(( scopeData_t *)this->ue->scopeData)->liveDataUE;

  if (!data[pbchLlr])
      return;

  const int len=data[pbchLlr]->lineSz;
  float *llr, *bit;
  float FBitinit[len] = { 0 }, FLlrinit[len] = { 0 };
  bit = FBitinit;
  llr = FLlrinit;

  int16_t *llr_pbch = (int16_t *)(data[pbchLlr]+1);
  for (int i=0; i<len; i++)
  {
    llr[i] = (float) llr_pbch[i];
    bit[i] = (float) i;
  }
  float maxX=0, maxY=0, minX=0, minY=0;
  for (int k=0; k<len; k++) {
    maxX=std::max(maxX,bit[k]);
    minX=std::min(minX,bit[k]);
    maxY=std::max(maxY,llr[k]);
    minY=std::min(minY,llr[k]);
  }

  float maxYAbs = std::max(abs(maxY),abs(minY));
  QScatterSeries *series = new QScatterSeries();
  series->setUseOpenGL(true);
  QColor MarkerColor(255, 0, 0);
  series->setColor(MarkerColor);
  series->setMarkerSize(2);
  series->setMarkerShape(QScatterSeries::MarkerShapeCircle);
  series->setBorderColor(Qt::transparent);

  QVector<QPointF> points(len);
  float Xpaint, Ypaint;
  float minYScaled=0, maxYScaled=0;
  for (int k=0; k<len; k++) {
    Xpaint = bit[k];
    Ypaint = llr[k]/maxYAbs*50;

    points[k] = QPointF(Xpaint, Ypaint);
    maxYScaled=std::max(maxYScaled,Ypaint);
    minYScaled=std::min(minYScaled,Ypaint);
  }

  series->replace(points);
  QChart *chart = new QChart();
  chart->legend()->hide();

  int nofTicks = 6;
  QValueAxis *axisX = new QValueAxis;
  axisX->setTickCount(nofTicks);
  axisX->setRange(0 , len);
  axisX->setTitleText("Sample Index");
  chart->addAxis(axisX, Qt::AlignBottom);

  QValueAxis *axisY = new QValueAxis;
  axisY->setTickCount(nofTicks);
  axisY->setRange((minYScaled + 0.4*minYScaled), (maxYScaled + 0.4*maxYScaled));
  axisY->setTitleText("LLR");
  chart->addAxis(axisY, Qt::AlignLeft);

  chart->addSeries(series);
  series->attachAxis(axisX);
  series->attachAxis(axisY);

  QChartView *chartView = new QChartView(chart);
  chartView->resize(this->chartWidth, this->chartHight);

  QPixmap p = chartView->grab();
  *this->pix = p;
  update(); //call paintEvent()
}

void *nrUEQtscopeThread(void *arg)
{
	PHY_VARS_NR_UE *ue=(PHY_VARS_NR_UE *)arg;

	int argc = 1;
	char *argv[] = { (char*)"nrqt_scopeUE"};
	QApplication a(argc, argv);

  // Create a main window (widget)
  QWidget *window = new QWidget();
  window->setBaseSize(QSize(600, 900));
  window->resize(600, 900);
  window->setSizeIncrement(20, 20);
  // Window title
  window->setWindowTitle("UE Scope");

  // create the popup lists
  KPIListSelect * combo1 = new KPIListSelect(window);
  combo1->setCurrentIndex(0);
  KPIListSelect * combo2 = new KPIListSelect(window);
  combo2->setCurrentIndex(1);
  KPIListSelect * combo3 = new KPIListSelect(window);
  combo3->setCurrentIndex(2);
  KPIListSelect * combo4 = new KPIListSelect(window);
  combo4->setCurrentIndex(3);
  KPIListSelect * combo5 = new KPIListSelect(window);
  combo5->setCurrentIndex(4);
  KPIListSelect * combo6 = new KPIListSelect(window);
  combo6->setCurrentIndex(7);

  // Initial Plots
  PainterWidget pwidgetueCombo3(combo3, ue);   // I/Q PDSCH
  PainterWidget pwidgetueCombo4(combo4, ue);   // LLR PDSCH */
  PainterWidget pwidgetueCombo1(combo1, ue);   // I/Q PBCH
  PainterWidget pwidgetueCombo2(combo2, ue);   // LLR PBCH
  PainterWidget pwidgetueCombo5(combo5, ue);   // I/Q PDCCH
  PainterWidget pwidgetueCombo6(combo6, ue);   // LLR PDCCH

  while (true)
  {
    // Main layout
    QHBoxLayout *mainLayout = new QHBoxLayout;

    // left and right layouts
    QVBoxLayout *LeftLayout = new QVBoxLayout;
    QVBoxLayout *RightLayout = new QVBoxLayout;


    LeftLayout->addWidget(combo1);
    LeftLayout->addWidget(&pwidgetueCombo1,20);
    LeftLayout->addStretch(); // space for later plots

    LeftLayout->addWidget(combo3);
    LeftLayout->addWidget(&pwidgetueCombo3,20);
    LeftLayout->addStretch(); // space for later plots

    LeftLayout->addWidget(combo5);
    LeftLayout->addWidget(&pwidgetueCombo5,20);
    LeftLayout->addStretch(); // space for later plots

    RightLayout->addWidget(combo2);
    RightLayout->addWidget(&pwidgetueCombo2,20);
    RightLayout->addStretch(); // space for later plots

    RightLayout->addWidget(combo4);
    RightLayout->addWidget(&pwidgetueCombo4,20);
    RightLayout->addStretch(); // space for later plots

    RightLayout->addWidget(combo6);
    RightLayout->addWidget(&pwidgetueCombo6,20);
    RightLayout->addStretch(); // space for later plots

    // add the left and right layouts to the main layout
    mainLayout->addLayout(LeftLayout);
    mainLayout->addLayout(RightLayout);

    // set the layout of the main window
    window->setLayout(mainLayout);

    pwidgetueCombo1.makeConnections();
    pwidgetueCombo2.makeConnections();
    pwidgetueCombo3.makeConnections();
    pwidgetueCombo4.makeConnections();
    pwidgetueCombo5.makeConnections();
    pwidgetueCombo6.makeConnections();

    // display the main window
    window->show();
    a.exec();
  }

  pthread_exit((void *)arg);
	//return NULL;
}

void *nrgNBQtscopeThread(void *arg)
{
  scopeData_t *p=(scopeData_t *)arg;
	int argc = 1;
	char *argv[] = { (char*)"nrqt_scopegNB"};
	QApplication a(argc, argv);

  // Create a main window (widget)
  QWidget *window = new QWidget();
  window->resize(600, 900);

  // Window title
  window->setWindowTitle("gNB Scope");

  // create the popup lists
  KPIListSelectgNB * combo1 = new KPIListSelectgNB(window);
  combo1->setCurrentIndex(0);
  KPIListSelectgNB * combo2 = new KPIListSelectgNB(window);
  combo2->setCurrentIndex(1);
  KPIListSelectgNB * combo3 = new KPIListSelectgNB(window);
  combo3->setCurrentIndex(2);
  KPIListSelectgNB * combo4 = new KPIListSelectgNB(window);
  combo4->setCurrentIndex(3);
  KPIListSelectgNB * combo5 = new KPIListSelectgNB(window);
  combo5->setCurrentIndex(4);
  KPIListSelectgNB * combo6 = new KPIListSelectgNB(window);
  combo6->setCurrentIndex(5);

  PainterWidgetgNB pwidgetgnbCombo1(combo1, p);   // I/Q PUSCH
  PainterWidgetgNB pwidgetgnbCombo2(combo2, p);   // LLR PUSCH
  PainterWidgetgNB pwidgetgnbCombo3(combo3, p);   // Channel Response
  PainterWidgetgNB pwidgetgnbCombo4(combo4, p);   //
  PainterWidgetgNB pwidgetgnbCombo5(combo5, p);   //
  PainterWidgetgNB pwidgetgnbCombo6(combo6, p);   //

  while (true)
  {
    // Main layout
    QHBoxLayout *mainLayout = new QHBoxLayout;

    // left and right layouts
    QVBoxLayout *LeftLayout = new QVBoxLayout;
    QVBoxLayout *RightLayout = new QVBoxLayout;

    LeftLayout->addWidget(combo1);
    LeftLayout->addWidget(&pwidgetgnbCombo1,20);
    LeftLayout->addStretch(); // space for later plots

    LeftLayout->addWidget(combo3);
    LeftLayout->addWidget(&pwidgetgnbCombo3,20);
    LeftLayout->addStretch(); // space for later plots

    LeftLayout->addWidget(combo5);
    LeftLayout->addWidget(&pwidgetgnbCombo5,20);
    LeftLayout->addStretch(); // space for later plots

    RightLayout->addWidget(combo2);
    RightLayout->addWidget(&pwidgetgnbCombo2,20);
    RightLayout->addStretch(); // space for later plots

    RightLayout->addWidget(combo4);
    RightLayout->addWidget(&pwidgetgnbCombo4,20);
    RightLayout->addStretch(); // space for later plots

    RightLayout->addWidget(combo6);      //
    RightLayout->addWidget(&pwidgetgnbCombo6,20);
    RightLayout->addStretch(); // space for later plots

    // add the left and right layouts to the main layout
    mainLayout->addLayout(LeftLayout);
    mainLayout->addLayout(RightLayout);

    // set the layout of the main window
    window->setLayout(mainLayout);

    pwidgetgnbCombo1.makeConnections();
    pwidgetgnbCombo2.makeConnections();
    pwidgetgnbCombo3.makeConnections();
    pwidgetgnbCombo4.makeConnections();
    pwidgetgnbCombo5.makeConnections();
    pwidgetgnbCombo6.makeConnections();

    // display the main window
    window->show();
    a.exec();
  }

	return NULL;
}

void UEcopyData(PHY_VARS_NR_UE *ue, enum UEdataType type, void *dataIn, int elementSz, int colSz, int lineSz) {

  // Local static copy of the scope data bufs
  // The active data buf is alterned to avoid interference between the Scope thread (display) and the Rx thread (data input)
  // Index of "2" could be set to the number of Rx threads + 1
  static scopeGraphData_t *copyDataBufs[UEdataTypeNumberOfItems][2] = {0};
  static int  copyDataBufsIdx[UEdataTypeNumberOfItems] = {0};

  scopeData_t *tmp=(scopeData_t *)ue->scopeData;

  if (tmp) {
    // Begin of critical zone between UE Rx threads that might copy new data at the same time: might require a mutex
    int newCopyDataIdx = (copyDataBufsIdx[type]==0)?1:0;
    copyDataBufsIdx[type] = newCopyDataIdx;
    // End of critical zone between UE Rx threads

    // New data will be copied in a different buffer than the live one
    scopeGraphData_t *copyData= copyDataBufs[type][newCopyDataIdx];

    if (copyData == NULL || copyData->dataSize < elementSz*colSz*lineSz) {
      scopeGraphData_t *ptr= (scopeGraphData_t*) realloc(copyData, sizeof(scopeGraphData_t) + elementSz*colSz*lineSz);

      if (!ptr) {
        LOG_E(PHY,"can't realloc\n");
        return;
      } else {
        copyData=ptr;
      }
    }

    copyData->dataSize=elementSz*colSz*lineSz;
    copyData->elementSz=elementSz;
    copyData->colSz=colSz;
    copyData->lineSz=lineSz;
    memcpy(copyData+1, dataIn,  elementSz*colSz*lineSz);
    copyDataBufs[type][newCopyDataIdx] = copyData;

    // The new data just copied in the local static buffer becomes live now
    ((scopeGraphData_t **)tmp->liveDataUE)[type]=copyData;
  }
}


void nrUEinitQtScope(PHY_VARS_NR_UE *ue) {
  ue->scopeData=malloc(sizeof(scopeData_t));
  scopeData_t *scope=(scopeData_t *) ue->scopeData;
  scope->copyData=UEcopyData;
  scope->liveDataUE=calloc(sizeof(scopeGraphData_t *), UEdataTypeNumberOfItems);
  for (int i=0; i<UEdataTypeNumberOfItems; i++){
    scope->flag_streaming[i] = 0;
  }
  pthread_t qtscope_thread;
  threadCreate(&qtscope_thread, nrUEQtscopeThread, ue, (char*)"qtscope", -1, sched_get_priority_min(SCHED_RR));
}

void nrgNBinitQtScope(scopeParms_t *p) {

  p->gNB->scopeData=malloc(sizeof(scopeData_t));
  scopeData_t *scope=(scopeData_t *) p->gNB->scopeData;
  scope->gNB=p->gNB;
  scope->argc=p->argc;
  scope->argv=p->argv;
  scope->ru=p->ru;

  pthread_t qtscope_thread;
  threadCreate(&qtscope_thread, nrgNBQtscopeThread, p->gNB->scopeData, (char*)"qtscope", -1, sched_get_priority_min(SCHED_RR));
}

extern "C" void nrqtscope_autoinit(void *dataptr) {
  AssertFatal( (IS_SOFTMODEM_GNB_BIT||IS_SOFTMODEM_5GUE_BIT),"Scope cannot find NRUE or GNB context");

  if (IS_SOFTMODEM_GNB_BIT)
    nrgNBinitQtScope((scopeParms_t *)dataptr);
  else
    nrUEinitQtScope((PHY_VARS_NR_UE *)dataptr);

  //nrUEinitQtScope((PHY_VARS_NR_UE *)dataptr);

}
