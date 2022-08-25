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

typedef struct complex16 scopeSample_t;
#define ScaleZone 4;
#define SquaredNorm(VaR) ((VaR).r*(VaR).r+(VaR).i*(VaR).i)

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
}
KPIListSelect::~KPIListSelect()
{
}

KPIListSelectgNB::KPIListSelectgNB(QWidget *parent) : QComboBox(parent)
{
  this->addItem("I/Q PUSCH", 0);
  this->addItem("LLR PUSCH", 1);
  this->addItem("Channel Response", 2);
  this->addItem("KPI4", 3);
  this->addItem("KPI5", 4);
  this->addItem("KPI6", 5);
}
KPIListSelectgNB::~KPIListSelectgNB()
{
}

PainterWidgetgNB::PainterWidgetgNB(QComboBox *parent, scopeData_t *p)
{
    timer = new QTimer(this);
    this->chartHight = 300;
    this->chartWidth = 300;
    this->pix = new QPixmap(this->chartWidth,this->chartHight);
    this->pix->fill(QColor(240,240,240));
    this->parentWindow = parent;
    this->p = p;
    this->nb_UEs = 1;

    this->indexToPlot = this->parentWindow->currentIndex();

    makeConnections();

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

    timer->start(100); // paintPixmap_xx every 100ms

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


PainterWidget::PainterWidget(QComboBox *parent, PHY_VARS_NR_UE *ue)
{
    timer = new QTimer(this);
    timerWaterFallTime = new QTimer(this);
    this->chartHight = 300;
    this->chartWidth = 300;
    this->chartBaseHeight = 300;
    this->chartBaseWidth = 300;
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


  timer->start(100); // paintPixmap_xx every 100ms
  timerWaterFallTime->start(10);

}


void PainterWidget::paintPixmap_ueChannelResponse()
{
  int nb_ant = 1; //this->ue->frame_parms.nb_antennas_rx;
  int eNB_id = 0;

  const scopeSample_t **data = (const scopeSample_t **) this->ue->pbch_vars[eNB_id]->dl_ch_estimates_time;

  int len = this->ue->frame_parms.ofdm_symbol_size>>3;

  float *values, *time;
  float valuesBuffer[len] = { 0 }, timeBuffer[len] = { 0 };
  values = valuesBuffer;
  time = timeBuffer;

  int idx = 0;

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

  //QVector<QPointF> points(len);

  float minYScaled=0, maxYScaled=0;
  for (int k=0; k<len; k++) {
    Xpaint = time[k];
    Ypaint = values[k]/maxYAbs*50;

    series->append(Xpaint, Ypaint);

    //points[k] = QPointF(Xpaint, Ypaint);
    maxYScaled=std::max(maxYScaled,Ypaint);
    minYScaled=std::min(minYScaled,Ypaint);
  }

  //series->replace(points);
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

    QVector<QPointF> points(sz * RX_NB_TH_MAX * sizeof(*I));

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
    //
    update(); //call paintEvent()

}

void PainterWidget::resizeEvent(QResizeEvent *event)
{
  if ((width() > this->chartWidth) || (height() > this->chartHight))
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
    // erase the previous paint
    this->pix->fill(QColor(240,240,240));

    //paint the axis and I/Q samples
    if (!this->ue->pdcch_vars[0][0]->rxdataF_comp[0])
        return;

    int nb=4*273*12;
    float *I, *Q;
    float FIinit[nb * RX_NB_TH_MAX * sizeof(*I)] = { 0 }, FQinit[nb * RX_NB_TH_MAX * sizeof(*Q)] = { 0 };
    I = FIinit;
    Q = FQinit;
    int base=0;
    for (int thr=0 ; thr < RX_NB_TH_MAX ; thr ++ )
    {
      scopeSample_t *pdcch_comp = (scopeSample_t *) this->ue->pdcch_vars[thr][0]->rxdataF_comp[0];
      for (int i=0; i< nb; i++)
      {
        I[base+i] = pdcch_comp[i].r;
        Q[base+i] = pdcch_comp[i].i;
      }

      base+=nb;
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



    QScatterSeries *series = new QScatterSeries();

    series->setUseOpenGL(true);

    QColor MarkerColor(0, 0, 255);
    series->setColor(MarkerColor);
    series->setMarkerSize(2);
    series->setBorderColor(Qt::transparent);
    series->setMarkerShape(QScatterSeries::MarkerShapeCircle);

    float maxXScaled=0, maxYScaled=0, minXScaled=0, minYScaled=0;
    float Ipaint, Qpaint;

    QVector<QPointF> points(nb * RX_NB_TH_MAX * sizeof(*I));

    for (int k=0; k<base; k++) { //scale the I/Q samples!

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

    //paint the axis and I/Q samples
    if (!this->ue->pbch_vars[0]->rxdataF_comp[0])
        return;

    //NR_DL_FRAME_PARMS *frame_parms = &this->ue->frame_parms;
    int sz=180*3; // size of the malloced buffer
    float *I, *Q;
    float FIinit[sz*sizeof(*I)] = { 0 }, FQinit[sz*sizeof(*Q)] = { 0 };
    I = FIinit;
    Q = FQinit;
    int base = 0;
    int first_symbol = 1;

    scopeSample_t *pbch_comp = (scopeSample_t *) this->ue->pbch_vars[0]->rxdataF_comp[0];

    for (int symbol=first_symbol; symbol<(first_symbol+3); symbol++)
    {
      int nb_re;
      if (symbol == 2 || symbol == 6)
        nb_re = 72;
      else
        nb_re = 180;

      //

      for (int i=0; i<nb_re; i++)
      {
        I[base+i] = pbch_comp[symbol*20*12+i].r;
        Q[base+i] = pbch_comp[symbol*20*12+i].i;
      }

      base+=nb_re;


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

    QScatterSeries *series = new QScatterSeries();
    series->setUseOpenGL(true);
    QColor MarkerColor(255, 0, 0);
    series->setColor(MarkerColor);
    series->setMarkerSize(2);
    series->setBorderColor(Qt::transparent);
    series->setMarkerShape(QScatterSeries::MarkerShapeCircle);

    int Ipaint, Qpaint;
    QVector<QPointF> points(sz * sizeof(*I));

    for (int k=0; k<base; k++) { //scale the I/Q samples!
      Ipaint = I[k]/maxXAbs*50;
      Qpaint = Q[k]/maxYAbs*50;

      points[k] = QPointF(Ipaint, Qpaint);
      //series->append(Ipaint, Qpaint);
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
    //chartView->setRenderHint(QPainter::Antialiasing);
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
    float FBitinit[coded_bits_per_codeword * RX_NB_TH_MAX * sizeof(*bit)] = { 0 }, FLlrinit[coded_bits_per_codeword * RX_NB_TH_MAX * sizeof(*llr)] = { 0 };
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

  QVector<QPointF> points(coded_bits_per_codeword * RX_NB_TH_MAX * sizeof(*bit));

  float minYScaled=0, maxYScaled=0;
  for (int k=0; k<base; k++) { //scale the I/Q samples!
    Xpaint = bit[k]; //bit[k]/maxXAbs*50;
    Ypaint = llr[k]/maxYAbs*50;

    points[k] = QPointF(Xpaint, Ypaint);
    //series->append(Xpaint, Ypaint);
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
  //chartView->setRenderHint(QPainter::Antialiasing);
  chartView->resize(this->chartWidth, this->chartHight);


  QPixmap p = chartView->grab();
  *this->pix = p;

  update(); //call paintEvent()

}



void PainterWidget::paintPixmap_uePdcchLLR()
{
    // erase the previous paint
    this->pix->fill(QColor(240,240,240));

    //paint the axis and LLR values
    if (!this->ue->pdcch_vars[0][0]->llr)
        return;


    int num_re = 4*100*12;
    int Qm = 2;
    int coded_bits_per_codeword = num_re*Qm;
    float *llr, *bit;
    float FBitinit[coded_bits_per_codeword * RX_NB_TH_MAX * sizeof(*bit)] = { 0 }, FLlrinit[coded_bits_per_codeword * RX_NB_TH_MAX * sizeof(*llr)] = { 0 };
    bit = FBitinit;
    llr = FLlrinit;

    int base=0;
    for (int thr=0 ; thr < RX_NB_TH_MAX ; thr ++ )
    {
      int16_t *pdcch_llr = (int16_t *) this->ue->pdcch_vars[thr][0]->llr;
      for (int i=0; i<coded_bits_per_codeword; i++)
      {
        llr[base+i] = (float) pdcch_llr[i];
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
  QColor MarkerColor(0, 0, 255);
  series->setColor(MarkerColor);
  series->setMarkerSize(2);
  series->setBorderColor(Qt::transparent);
  series->setMarkerShape(QScatterSeries::MarkerShapeCircle);

  float minYScaled=0, maxYScaled=0;
  float Xpaint, Ypaint;

  QVector<QPointF> points(coded_bits_per_codeword * RX_NB_TH_MAX * sizeof(*bit));


  for (int k=0; k<base; k++) { //scale the I/Q samples!
    Xpaint = bit[k]; //bit[k]/maxXAbs*50;
    Ypaint = llr[k]/maxYAbs*50;

    points[k] = QPointF(Xpaint, Ypaint);
    //series->append(Xpaint, Ypaint);
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
  //chartView->setRenderHint(QPainter::Antialiasing);
  chartView->resize(this->chartWidth, this->chartHight);


  QPixmap p = chartView->grab();
  *this->pix = p;

  update(); //call paintEvent()

}


void PainterWidget::paintPixmap_uePbchLLR()
{
  int base = 864;
  this->pix->fill(QColor(240,240,240));

  //paint the axis and LLR values
  if (!this->ue->pbch_vars[0]->llr[0])
    return;

  float *llr, *bit;

  float FBitinit[base] = { 0 }, FLlrinit[base] = { 0 };
  bit = FBitinit;
  llr = FLlrinit;

  int16_t *pbch_llr = (int16_t *) this->ue->pbch_vars[0]->llr;

  for (int i=0; i<base; i++)
  {
    llr[i] = (float) pbch_llr[i];
    bit[i] = (float) i;
  }

  float maxX=0, maxY=0, minX=0, minY=0;
  for (int k=0; k<base; k++) {
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

  QVector<QPointF> points(base);
  float Xpaint, Ypaint;

  float minYScaled=0, maxYScaled=0;
  for (int k=0; k<base; k++) { //scale the I/Q samples!
    Xpaint = bit[k]; //bit[k]/maxXAbs*50;
    Ypaint = llr[k]/maxYAbs*50;

    points[k] = QPointF(Xpaint, Ypaint);
    //series->append(Xpaint, Ypaint);
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
  //chartView->setRenderHint(QPainter::Antialiasing);
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

    RightLayout->addWidget(combo6);      //
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

void gNBcopyData(PHY_VARS_gNB *gNB, enum UEdataType type, void *dataIn, int elementSz, int colSz, int lineSz)
{

  std::cout << "WELCOME HERE" << std::endl;

  std::cout << "****elementSz*: " << elementSz << std::endl;
  std::cout << "****colSz*: " << colSz << std::endl;
  std::cout << "****lineSz*: " << lineSz << std::endl;

  static scopeGraphData_t *copyDataBufs[UEdataTypeNumberOfItems][2] = {0};
  static int  copyDataBufsIdx[UEdataTypeNumberOfItems] = {0};

  scopeData_t *tmp=(scopeData_t *)gNB->scopeData;

  if (tmp)
  {
    int newCopyDataIdx = (copyDataBufsIdx[type]==0)?1:0;
    copyDataBufsIdx[type] = newCopyDataIdx;

    scopeGraphData_t *copyData= copyDataBufs[type][newCopyDataIdx];

    if (copyData == NULL || copyData->dataSize < elementSz*colSz*lineSz)
    {
      scopeGraphData_t *ptr = (scopeGraphData_t*)realloc(copyData, sizeof(scopeGraphData_t) + elementSz*colSz*lineSz);

      if (!ptr)
      {
        return;
      }
      else
      {
        copyData = ptr;
      }
    }

    copyData->dataSize=elementSz*colSz*lineSz;
    copyData->elementSz=elementSz;
    copyData->colSz=colSz;
    copyData->lineSz=lineSz;
    memcpy(copyData+1, dataIn,  elementSz*colSz*lineSz);
    copyDataBufs[type][newCopyDataIdx] = copyData;

    ((scopeGraphData_t **)tmp->liveData)[type]=copyData;

  }
}

void nrUEinitQtScope(PHY_VARS_NR_UE *ue) {
  pthread_t qtscope_thread;
  threadCreate(&qtscope_thread, nrUEQtscopeThread, ue, (char*)"qtscope", -1, sched_get_priority_min(SCHED_RR));
}

void nrgNBinitQtScope(scopeParms_t *p) {

  p->gNB->scopeData=malloc(sizeof(scopeData_t));
  scopeData_t *scope=(scopeData_t *) p->gNB->scopeData;
  scope->gNB=p->gNB;
  scope->copyDatagNB = gNBcopyData;
  scope->argc=p->argc;
  scope->argv=p->argv;
  scope->ru=p->ru;
  scope->liveData=calloc(sizeof(scopeGraphData_t *), UEdataTypeNumberOfItems);

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
