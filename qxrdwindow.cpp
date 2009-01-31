#include "qavrgwindow.h"
#include "qavrgapplication.h"
#include "qavrgacquisition.h"

#include <qwt_plot.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_zoomer.h>
#include <qwt_legend.h>
#include <qwt_symbol.h>
#include <QPen>
#include <QFile>
#include <QCheckBox>
#include <QSignalMapper>

QxrdWindow::QxrdWindow(QxrdApplication *app, QWidget *parent)
  : QMainWindow(parent),
    m_Application(app),
    m_Acquisition(NULL),
    m_XUnits(0),
    m_XStep(1.0),
    m_Highlighter(NULL)
{
  setupUi(this);

  m_Plot->setCanvasBackground(QColor(Qt::white));

  m_Zoomer = new QwtPlotZoomer(QwtPlot::xBottom, QwtPlot::yLeft, m_Plot->canvas());
  m_Zoomer -> setSelectionFlags(QwtPicker::DragSelection | QwtPicker::CornerToCorner);
  m_Zoomer -> setTrackerMode(QwtPicker::ActiveOnly);
  m_Zoomer -> setRubberBand(QwtPicker::RectRubberBand);

  m_Zoomer -> setMousePattern(QwtEventPattern::MouseSelect2,
			      Qt::LeftButton, Qt::ControlModifier | Qt::ShiftModifier);
  m_Zoomer -> setMousePattern(QwtEventPattern::MouseSelect3,
			      Qt::LeftButton, Qt::ControlModifier);

  m_Legend = new QwtLegend;
  m_Legend -> setFrameStyle(QFrame::Box|QFrame::Sunken);
  m_Legend -> setItemMode(QwtLegend::CheckableItem);

  m_Plot -> insertLegend(m_Legend, QwtPlot::BottomLegend);

//   connect(m_ViewChan01,   SIGNAL(clicked()), this, SLOT(viewParametersChanged()));
//   connect(m_ViewChan02,   SIGNAL(clicked()), this, SLOT(viewParametersChanged()));
  connect(m_XAxisUnits,   SIGNAL(currentIndexChanged(int)), this, SLOT(viewScalingChanged()));
  connect(m_XViewChoice,   SIGNAL(currentIndexChanged(int)), this, SLOT(viewParametersChanged()));
  connect(m_ViewOrbit,    SIGNAL(valueChanged(double)), this, SLOT(viewParametersChanged()));
  connect(m_ViewBunch,    SIGNAL(valueChanged(double)), this, SLOT(viewParametersChanged()));
  connect(m_AutoScale,    SIGNAL(clicked()), this, SLOT(autoScale()));
  connect(m_ViewAllData,    SIGNAL(clicked()), this, SLOT(viewAllData()));

  connect(m_ActionAutoScale, SIGNAL(triggered()), this, SLOT(autoScale()));
  connect(m_ActionViewAllData, SIGNAL(triggered()), this, SLOT(viewAllData()));
  connect(m_ActionQuit, SIGNAL(triggered()), m_Application, SLOT(quit()));
  connect(m_ActionLoadData, SIGNAL(triggered()), m_Application, SLOT(loadData()));
  connect(m_ActionSaveData, SIGNAL(triggered()), m_Application, SLOT(saveData()));

  m_Highlighter = new QxrdHighlighter(m_InputScript->document());

  m_HelpText -> setReadOnly(true);

  QFile file(":/qavrghelptext.html");

  file.open(QIODevice::ReadOnly);

  QString contents = file.readAll();

  m_HelpText -> setHtml(contents);
}

void QxrdWindow::setAcquisition(QxrdAcquisition *acq)
{
  m_Acquisition = acq;

  QGridLayout * grid = new QGridLayout(m_GraphChooser);

  grid -> addWidget(new QLabel("Chan"), 0,0, Qt::AlignHCenter);
  grid -> addWidget(new QLabel("Data"), 0,1, Qt::AlignHCenter);
  grid -> addWidget(new QLabel("Ref"), 0,2, Qt::AlignHCenter);
  grid -> addWidget(new QLabel("Fit"), 0,3, Qt::AlignHCenter);

  int nchan = m_Acquisition -> totalChannels();

  QSignalMapper *mapper = new QSignalMapper(this);

  for (int i=0; i<nchan; i++) {
    grid -> addWidget(new QLabel(tr("ch%1").arg(i)), i+1,0, Qt::AlignHCenter);

    QCheckBox *dataCkb = new QCheckBox();
    QCheckBox *refCkb = new QCheckBox();
    QCheckBox *fitCkb = new QCheckBox();

    grid -> addWidget(dataCkb, i+1,1, Qt::AlignHCenter);
    grid -> addWidget(refCkb, i+1,2, Qt::AlignHCenter);
    grid -> addWidget(fitCkb, i+1,3, Qt::AlignHCenter);

    connect(dataCkb, SIGNAL(clicked()), mapper, SLOT(map()));
    connect(refCkb, SIGNAL(clicked()), mapper, SLOT(map()));
    connect(fitCkb, SIGNAL(clicked()), mapper, SLOT(map()));

    mapper -> setMapping(dataCkb, i*3+0);
    mapper -> setMapping(refCkb, i*3+1);
    mapper -> setMapping(fitCkb, i*3+2);

    m_CheckBoxes.append(dataCkb);
    m_CheckBoxes.append(refCkb);
    m_CheckBoxes.append(fitCkb);
  }

  connect(mapper, SIGNAL(mapped(int)), this, SLOT(chooserClicked(int)));
}

void QxrdWindow::chooserClicked(int n)
{
  updateGraph();
}

const QPushButton *QxrdWindow::acquireButton()
{
  return m_AcquireButton;
}

const QAction *QxrdWindow::acquireAction()
{
  return m_ActionAcquire;
}

void QxrdWindow::newDataAvailable()
{
  printf("New data available...\n");

  printf("%d channels, %d samples\n", 
	 m_Application->data().count(), 
	 m_Application->data()[0].count());

  QVector< QVector<double> > data = m_Application->data();
  QxrdAcquisitionParms *acqParms = m_Application->acquisitionParms();

  int nsamples = data[0].count();
  int norbits  = nsamples / acqParms->samplesPerOrbit();
  int nbunches = acqParms -> bunchesPerOrbit();

  m_ViewOrbit -> setMaximum(norbits);
  m_ViewBunch -> setMaximum(nbunches);

  updateGraph();
}

void QxrdWindow::viewParametersChanged()
{
  updateGraph();
}

void QxrdWindow::viewScalingChanged()
{
  updateGraph();
}

QPen QxrdWindow::curvePen(int ch)
{
  QPen pen1(Qt::red);
  QPen pen2(Qt::green);
  QPen pen3(Qt::blue);
  QPen pen4(Qt::magenta);

  switch (ch & 3) {
  case 0:
    return pen1;
    break;
  case 1:
    return pen2;
    break;
  case 2:
    return pen3;
    break;
  default:
    return pen4;
    break;
  }
//   if (ch & 1) {
//     return pen1;
//   } else {
//     return pen2;
//   }
}

void QxrdWindow::updateGraph()
{
//   printf("Update graph\n");

  QVector< QVector<double> > data = m_Application->data();
  QVector< QVector<double> > ref  = m_Application->referenceData();
  QVector< QVector<double> > dark = m_Application->darkData();

//   printf("data[%d], ref[%d], dark[%d]\n", data.size(), ref.size(), dark.size());

  QxrdAcquisition      *acq      = m_Application->acquisition();
  QxrdAcquisitionParms *acqParms = m_Application->acquisitionParms();

  int viewChoice = m_XViewChoice -> currentIndex();
  int xunits = m_XAxisUnits -> currentIndex();
  double viewOrbit = m_ViewOrbit -> value();
  double viewBunch = m_ViewBunch -> value();

//   printf("Bunch Separation = %g nsec\n", acqParms->bunchSeparation());
//   printf("Samples per orbit = %g samp\n", acqParms->samplesPerOrbit());
//   printf("Bunches per orbit = %g bunch\n", acqParms->bunchesPerOrbit());
//   int bunches = true;

  const char *xlabel; double xstep;

  switch (xunits) {
  case 0:
  default:
    xstep = 1.0;
    xlabel = "Time (nsec)";
    break;

  case 1:
    xstep = 1.0e-3;
    xlabel = "Time (usec)";
    break;

  case 2:
    xstep = 1.0e-6;
    xlabel = "Time (msec)";
    break;

  case 3:
    xstep = 1.0/(acqParms->bunchSeparation()*acqParms->bunchesPerOrbit());
    xlabel = "Orbit Number";
    break;

  case 4:
    xstep = 1.0/acqParms->bunchSeparation();
    xlabel = "Bunch Number";
    break;
  }

  m_Plot -> clear();
  m_Plot -> setAxisTitle(QwtPlot::xBottom, xlabel);

  switch (viewChoice) {
  case 0: // View all data
    {
      int npts=data[0].count();

//       printf("Update graph, points %d, data channels %d\n", npts, data.count());

      QVector<double> timeb(npts);
      QVector<double> ydata(npts);

      for (int i=0; i<npts; i++) {
	timeb[i] = i*xstep;
      }

      for (int ch = 1; ch < data.count(); ch++) {
	QCheckBox *cb = m_CheckBoxes[(ch-1)*3];
	if (cb && cb->isChecked()) {
	  npts = data[ch].count();
	  QwtPlotCurve *crv = new QwtPlotCurve(tr("Channel %1 pts %2").arg(ch-1).arg(npts));
	
	  if (m_Application->darkAvailable()) {
	    for (int i=0; i<npts; i++) {
	      ydata[i] = data[ch][i] - dark[ch-1][i];
	    }
	  } else {
	    for (int i=0; i<npts; i++) {
	      ydata[i] = data[ch][i];
	    }
	  }

	  crv -> setData(timeb.data(), ydata.data(), npts);
	  crv -> setPen(curvePen(ch));
	
	  crv -> attach(m_Plot);
	}
      }
    }
    break;

  case 1: // View one orbit
    {
      double npts = acqParms->samplesPerOrbit();
      int inpts = npts;
      double p0 = viewOrbit*npts;
      int i0 = (int) p0;
      double dx = p0-i0;

      QVector<double> timeb(inpts);
      QVector<double> ydata(inpts);

      for (int i=0; i<inpts; i++) {
	timeb[i] = (i-dx)*xstep;
      }

      for (int ch = 1; ch < data.count(); ch++) {
	QCheckBox *cb = m_CheckBoxes[(ch-1)*3];
	if (cb && cb->isChecked()) {
	  QwtPlotCurve *crv = new QwtPlotCurve(tr("ch%1 orb%2").arg(ch).arg(viewOrbit));

	  QVector<double> dat = data[ch].mid(i0, inpts);

	  if (m_Application->darkAvailable()) {
	    QVector<double> drk = dark[ch-1].mid(i0, inpts);
	  
	    for (int i=0; i<inpts; i++) {
	      ydata[i] = dat[i]-drk[i];
	      // 	    printf("ydata[%d] = %g - %g = %g\n", i, dat[i], drk[i], ydata[i]);
	    }
	  } else {
	    for (int i=0; i<inpts; i++) {
	      ydata[i] = dat[i];
	    }
	  }

	  crv -> setData(timeb.data(), ydata.data(), inpts);
	  crv -> setPen(curvePen(ch));
	
	  crv -> attach(m_Plot);
	}
      }
    }
    break;

  case 2: // View one bunch
    {
      double npts = acqParms->samplesPerOrbit()/24;
      int inpts = npts;
      double sporb = acqParms->samplesPerOrbit();
      double spb = acqParms->bunchSeparation();
      double p0 = viewOrbit*sporb+viewBunch*spb;
      int i0 = (int) p0;
      double dx = p0-i0;

      QVector<double> timeb(inpts);
      QVector<double> ydata(inpts);

      for (int i=0; i<inpts; i++) {
	timeb[i] = (i-dx)*xstep;
      }

      for (int ch = 1; ch < data.count(); ch++) {
	QCheckBox *cb = m_CheckBoxes[(ch-1)*3];
	if (cb && cb->isChecked()) {
	  QwtPlotCurve *crv = new QwtPlotCurve(tr("ch%1 orb%2 bnch%3").arg(ch).arg(viewOrbit).arg(viewBunch));

	  QVector<double> dat = data[ch].mid(i0, inpts);
	  
	  if (m_Application->darkAvailable()) {
	    QVector<double> drk = dark[ch-1].mid(i0, inpts);
	    for (int i=0; i<inpts; i++) {
	      ydata[i] = dat[i]-drk[i];
	    }
	  } else {
	    for (int i=0; i<inpts; i++) {
	      ydata[i] = dat[i];
	    }
	  }
	  
	  crv -> setData(timeb.data(), ydata.data(), inpts);
	  crv -> setPen(curvePen(ch));
	  
	  crv -> attach(m_Plot);
	}	    
      }   
    }
    break;

  case 3: // View one bunch, all orbits
    {
      double npts = acqParms->samplesPerOrbit()/24;
      double sporb = acqParms->samplesPerOrbit();
      double spb = acqParms->bunchSeparation();
      
      int inpts = npts;
      int pen = 0;

      QVector<double> timeb(inpts);
      QVector<double> ydata(inpts);

      int norbits = data[0].count()/acqParms->samplesPerOrbit();

      for (int orb = 0; orb < norbits; orb++) {
	double p0 = orb*sporb + viewBunch*spb;
	int i0 = (int) p0;
	double dx = p0-i0;

	for (int i=0; i<inpts; i++) {
	  timeb[i] = (i-dx)*xstep;
	}

	for (int ch = 1; ch < data.count(); ch++) {
	  QCheckBox *cb = m_CheckBoxes[(ch-1)*3];
	  if (cb && cb->isChecked()) {
	    QwtPlotCurve *crv = new QwtPlotCurve(tr("ch%1 orb%2 bnch%3").arg(ch).arg(orb).arg(viewBunch));

	    QVector<double> dat = data[ch].mid(i0, inpts);

	    if (m_Application->darkAvailable()) {
	      QVector<double> drk = dark[ch-1].mid(i0, inpts);
	      for (int i=0; i<inpts; i++) {
		ydata[i] = dat[i]-drk[i];
	      }
	    } else {
	      for (int i=0; i<inpts; i++) {
		ydata[i] = dat[i];
	      }
	    }
	    
	    crv -> setData(timeb.data(), ydata.data(), inpts);
	    crv -> setPen(curvePen(pen++));

	    crv -> attach(m_Plot);

	    if (pen > 200) break;
	  }
	}

	if (pen > 200) break;
      }
    }
    break;
  }

//   if (bunches) {
//     QByteArray fill = m_Application->fillPattern()->fillPattern();
//     double bunchSep = acqParms->bunchSeparation();
//     int nbunches = acqParms->bunchesPerOrbit();

//     int nfilled = fill.count(1);
//     int norbits = (int)((double)NPOINTS)/(bunchSep*nbunches);

//     printf("%d filled bunches per orbit\n", nfilled);
//     printf("%g bunch time (nsec)\n", bunchSep);
//     printf("%d orbits displayed\n", norbits);

//     if (norbits*nfilled) {
//       QwtPlotCurve *bn1 = new QwtPlotCurve("Bunches Ch1");
//       QwtPlotCurve *bn2 = new QwtPlotCurve("Bunches Ch2");

//       QVector<double> xdata(norbits*nfilled);
//       QVector<double> y1data(norbits*nfilled);
//       QVector<double> y2data(norbits*nfilled);

//       int p=0;
//       for (int i=0; i<norbits; i++) {
// 	for (int j=0; j<nbunches; j++) {
// 	  if (fill[j]) {
// 	    int bin = (int)((double)(i*nbunches+j)*bunchSep);
// 	    xdata[p] = timeb[bin];
// 	    y1data[p] = chan1[bin];
// 	    y2data[p] = chan2[bin];
// 	    p++;
// 	  }
// 	}
//       }

//       QwtSymbol sym1(QwtSymbol::Ellipse, QBrush(Qt::white), QPen(Qt::black), QSize(6,6));

//       bn1 -> setData(xdata.data(), y1data.data(), norbits*nfilled);
//       bn1 -> setStyle(QwtPlotCurve::NoCurve);
//       bn1 -> setSymbol(sym1);

//       bn1 -> attach(m_Plot);

//       QwtSymbol sym2(QwtSymbol::Ellipse, QBrush(Qt::white), QPen(Qt::black), QSize(6,6));

//       bn2 -> setData(xdata.data(), y2data.data(), norbits*nfilled);
//       bn2 -> setStyle(QwtPlotCurve::NoCurve);
//       bn2 -> setSymbol(sym2);

//       bn2 -> attach(m_Plot);
//     }
//   }

  if (xunits != m_XUnits) {
    // If the x scaling changes, go and update the coordinates in the zoom stack...
    double ratio = xstep/m_XStep;

    for (int i=0; i<m_Zoomer->zoomStack().count(); i++) {
      QwtDoubleRect &zr = const_cast<QwtDoubleRect&>(m_Zoomer->zoomStack()[i]);

      zr.setLeft(zr.left()*ratio);
      zr.setRight(zr.right()*ratio);
    }

    m_XUnits = xunits;
    m_XStep = xstep;

    m_Zoomer->zoom(m_Zoomer -> zoomRectIndex());
  }

  if (m_Zoomer -> zoomRectIndex() == 0) {
    m_Plot -> setAxisAutoScale(QwtPlot::xBottom);
    m_Plot -> setAxisAutoScale(QwtPlot::yLeft);
  }

  m_Plot->replot();

  if (m_Zoomer -> zoomRectIndex() == 0) {
    m_Zoomer -> setZoomBase();
  }
}

void QxrdWindow::autoScale()
{
  m_Plot -> setAxisAutoScale(QwtPlot::xBottom);
  m_Plot -> setAxisAutoScale(QwtPlot::yLeft);
  
  m_Plot->replot();

  m_Zoomer -> setZoomBase();
}

void QxrdWindow::viewAllData()
{
  m_XViewChoice -> setCurrentIndex(0);

  updateGraph();
}

double QxrdWindow::integrationTime()
{
  return m_IntegrationTime -> value();
}

int QxrdWindow::nRepetitions()
{
  return m_NRepetitions -> value();
}
