#include <QApplication>
#include <QtWidgets>
#include <QPainter>
#include <QtGui>
#include "nr_phy_qt_scope.h"

typedef struct complex16 scopeSample_t;

KPIListSelect::KPIListSelect(QWidget *parent) : QComboBox(parent)
{
	this->addItem("KPI 1");
    this->addItem("KPI 2");
    this->addItem("KPI 3");
    this->addItem("KPI 4");
    this->addItem("KPI 5");
    this->addItem("KPI 6");
}
KPIListSelect::~KPIListSelect()
{
}


PainterWidget::PainterWidget(PHY_VARS_NR_UE *ue)
{
    timer = new QTimer(this);
    this->pix = new QPixmap(300,300);
    this->pix->fill(QColor(240,240,240));
    this->ue = ue;
    connect(timer, &QTimer::timeout, this, &PainterWidget::paintPixmap);
    timer->start(1000); // paintPixmap every 1000ms

    // paint the frame
    static const QPoint PlotFrame[4] = {
        QPoint( 100, 100),
        QPoint(-100, 100),
        QPoint(-100, -100),
        QPoint( 100, -100)
    };
    QColor FrameColor(0, 255, 120);
    QPainter PixPainter(this->pix);
    PixPainter.translate(this->pix->width()/2, this->pix->height()/2); // move the origin to the center of the Pixmap
    PixPainter.setPen(FrameColor);
    PixPainter.drawConvexPolygon(PlotFrame, 4);
}
void PainterWidget::paintPixmap()
{
    // erase the previous paint
    this->pix->fill(QColor(240,240,240));

    static const QPoint PlotFrame[4] = {
        QPoint( 100, 100),
        QPoint(-100, 100),
        QPoint(-100, -100),
        QPoint( 100, -100)
    };
    QColor FrameColor(0, 255, 120);
    QPainter PixPainter(this->pix);
    PixPainter.translate(this->pix->width()/2, this->pix->height()/2); // move the origin to the center of the Pixmap
    PixPainter.setPen(FrameColor);
    PixPainter.drawConvexPolygon(PlotFrame, 4);

    //paint the axis and I/Q samples
    if (!this->ue->pdsch_vars[0][0]->rxdataF_comp0[0])
        return;

    NR_DL_FRAME_PARMS *frame_parms = &this->ue->frame_parms;
    int sz=7*2*frame_parms->N_RB_DL*12; // size of the malloced buffer
    float FIinit[sz] = { 0 }, FQinit[sz] = { 0 };
    float *I = FIinit, *Q = FQinit;
    for (int thr=0 ; thr < RX_NB_TH_MAX ; thr ++ ) {
      scopeSample_t *pdsch_comp = (scopeSample_t *) this->ue->pdsch_vars[thr][0]->rxdataF_comp0[0];
      for (int s=0; s<sz; s++) {
        I[s] += pdsch_comp[s].r;
        Q[s] += pdsch_comp[s].i;
      }
    }
    
    float maxX=0, maxY=0, minX=0, minY=0;
    for (int k=0; k<sz; k++) {
      maxX=std::max(maxX,I[k]);
      minX=std::min(minX,I[k]);
      maxY=std::max(maxY,Q[k]);
      minY=std::min(minY,Q[k]);
    }
    QColor IQColor(105,105,105);
    PixPainter.setPen(IQColor);
    float maxXAbs = std::max(abs(maxX),abs(minX));
    float maxYAbs = std::max(abs(maxY),abs(minY));
    int Ipaint, Qpaint;
    for (int k=0; k<sz; k++) { //scale the I/Q samples!
      Ipaint = I[k]/maxXAbs*50;
      Qpaint = Q[k]/maxYAbs*50;
      PixPainter.drawEllipse( QPoint(Ipaint,Qpaint), 2, 2 );
      //PixPainter.drawPoint(Ipaint, Qpaint);
    }

    update(); //call paintEvent()

}
void PainterWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap( (this->width()-this->pix->width())/2,
        (this->height()-this->pix->height())/2, *this->pix); // paint pixmap on widget
}


AccumWidget::AccumWidget(PHY_VARS_NR_UE *ue)
{
    timer = new QTimer(this);
    this->pix = new QPixmap(300,300);
    this->pix->fill(QColor(240,240,240));
    this->ue = ue;
    connect(timer, &QTimer::timeout, this, &AccumWidget::paintPixmap);
    timer->start(1000); // paintPixmap every 1000ms
    
    // paint the frame
    static const QPoint PlotFrame[4] = {
        QPoint( 100, 100),
        QPoint(-100, 100),
        QPoint(-100, -100),
        QPoint( 100, -100)
    };
    QColor FrameColor(0, 255, 120);
    QPainter PixPainter(this->pix);
    PixPainter.translate(this->pix->width()/2, this->pix->height()/2); // move the origin to the center of the Pixmap
    PixPainter.setPen(FrameColor);
    PixPainter.drawConvexPolygon(PlotFrame, 4);

}
void AccumWidget::paintPixmap()
{
    // paint on pixmap
    QPainter PixPainter(this->pix);
    PixPainter.translate(this->pix->width()/2, this->pix->height()/2); // move the origin to the center of the Pixmap

    //paint the axis and I/Q samples
    if (!this->ue->pdsch_vars[0][0]->rxdataF_comp0[0])
        return;

    NR_DL_FRAME_PARMS *frame_parms = &this->ue->frame_parms;
    int sz=7*2*frame_parms->N_RB_DL*12; // size of the malloced buffer
    float FIinit[sz] = { 0 }, FQinit[sz] = { 0 };
    float *I = FIinit, *Q = FQinit;
    for (int thr=0 ; thr < RX_NB_TH_MAX ; thr ++ ) {
      scopeSample_t *pdsch_comp = (scopeSample_t *) this->ue->pdsch_vars[thr][0]->rxdataF_comp0[0];
      for (int s=0; s<sz; s++) {
        I[s] += pdsch_comp[s].r;
        Q[s] += pdsch_comp[s].i;
      }
    }
    
    float maxX=0, maxY=0, minX=0, minY=0;
    for (int k=0; k<sz; k++) {
      maxX=std::max(maxX,I[k]);
      minX=std::min(minX,I[k]);
      maxY=std::max(maxY,Q[k]);
      minY=std::min(minY,Q[k]);
    }
    QColor IQColor(105,105,105);
    PixPainter.setPen(IQColor);
    float maxXAbs = std::max(abs(maxX),abs(minX));
    float maxYAbs = std::max(abs(maxY),abs(minY));
    int Ipaint, Qpaint;
    for (int k=0; k<sz; k++) { //scale the I/Q samples!
      Ipaint = I[k]/maxXAbs*50;
      Qpaint = Q[k]/maxYAbs*50;
      PixPainter.drawEllipse( QPoint(Ipaint,Qpaint), 2, 2 );
      //PixPainter.drawPoint(Ipaint, Qpaint);
    }

    update(); //call paintEvent()

}
void AccumWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap( (this->width()-this->pix->width())/2,
        (this->height()-this->pix->height())/2, *this->pix); // paint pixmap on widget
}


void *nrUEQtscopeThread(void *arg)
{
	PHY_VARS_NR_UE *ue=(PHY_VARS_NR_UE *)arg;

	int argc = 1;
	char *argv[] = { (char*)"nrqt_scope"};
	QApplication a(argc, argv);

    // Create a main window (widget)
    QWidget window;
    window.resize(600, 800);
    // Window title
    window.setWindowTitle("UE scope");

    // create the popup lists
    KPIListSelect * combo1 = new KPIListSelect();
    combo1->setCurrentIndex(0);
    KPIListSelect * combo2 = new KPIListSelect();
    combo2->setCurrentIndex(1);
    KPIListSelect * combo3 = new KPIListSelect();
    combo3->setCurrentIndex(2);
    KPIListSelect * combo4 = new KPIListSelect();
    combo4->setCurrentIndex(3);
    KPIListSelect * combo5 = new KPIListSelect();
    combo5->setCurrentIndex(4);
    KPIListSelect * combo6 = new KPIListSelect();
    combo6->setCurrentIndex(5);

    // clock example
    PainterWidget pwidget(ue);
    //PainterExample clock;
    AccumWidget mypixmap(ue);

    // Main layout
    QHBoxLayout *mainLayout = new QHBoxLayout;

    // left and right layouts
    QVBoxLayout *LeftLayout = new QVBoxLayout;
    QVBoxLayout *RightLayout = new QVBoxLayout;
    LeftLayout->addWidget(combo1);
    LeftLayout->addWidget(&pwidget,20);
    //LeftLayout->addStretch(); // space for later plots
    LeftLayout->addWidget(combo2);
    //LeftLayout->addWidget(&clock,10);
    LeftLayout->addStretch(); // space for later plots
    LeftLayout->addWidget(combo3);
    
    
    LeftLayout->addWidget(&mypixmap,20);
    //LeftLayout->addStretch(); // space for later plots
    RightLayout->addWidget(combo4);
    RightLayout->addStretch(); // space for later plots
    RightLayout->addWidget(combo5);
    RightLayout->addStretch(); // space for later plots
    RightLayout->addWidget(combo6);
    RightLayout->addStretch(); // space for later plots

    // add the left and right layouts to the main layout
    mainLayout->addLayout(LeftLayout);
    mainLayout->addLayout(RightLayout);


    // set the layout of the main window
    window.setLayout(mainLayout);


    // display the main window
	window.show();
	a.exec();
	return NULL;

}

void nrUEinitQtScope(PHY_VARS_NR_UE *ue) {
  pthread_t qtscope_thread;
  threadCreate(&qtscope_thread, nrUEQtscopeThread, ue, (char*)"qtscope", -1, sched_get_priority_min(SCHED_RR));
}

extern "C" void nrqtscope_autoinit(void *dataptr) {
  //AssertFatal( (IS_SOFTMODEM_GNB_BIT||IS_SOFTMODEM_5GUE_BIT),"Scope cannot find NRUE or GNB context");

  /*if (IS_SOFTMODEM_GNB_BIT)
    gNBinitScope(dataptr);
  else
    nrUEinitScope(dataptr);*/
  nrUEinitQtScope((PHY_VARS_NR_UE *)dataptr);
  
}
