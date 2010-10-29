#include "qxrdpowderfitidentifypage.h"
#include "qwt_symbol.h"
#include "qwt_double_rect.h"
#include "qwt_plot_marker.h"
#include "levmar.h"

QxrdPowderFitIdentifyPage::QxrdPowderFitIdentifyPage(QxrdDataProcessorPtr proc, QWidget *parent) :
    QxrdPowderFitWidget(proc, parent),
    m_IdCurrentRing(this,"idCurrentRing",0),
    m_Picker(NULL),
    m_SampledData(proc -> initialRingSetData())
{
  m_Picker = new QwtPlotPicker(m_ImagePlot->canvas());
  m_Picker -> setEnabled(true);
  m_Picker -> setTrackerMode(QwtPicker::AlwaysOn);
  m_Picker -> setSelectionFlags(QwtPicker::PointSelection);

  connect(m_Picker, SIGNAL(selected(QwtDoublePoint)), this, SLOT(pointClicked(QwtDoublePoint)));
  connect(m_IdNextRingButton, SIGNAL(clicked()), this, SLOT(selectNextRing()));
  connect(m_IdPrevRingButton, SIGNAL(clicked()), this, SLOT(selectPrevRing()));
  connect(m_IdAddRingButton, SIGNAL(clicked()), this, SLOT(appendRing()));
  connect(m_IdRemoveRingButton, SIGNAL(clicked()), this, SLOT(removeRing()));
  connect(m_IdPerformFitButton, SIGNAL(clicked()), this, SLOT(idPerformFit()));

  prop_IdCurrentRing() -> linkTo(m_IdRingNumber);
}

void QxrdPowderFitIdentifyPage::selectIdentifyPage()
{
  m_PowderFitSteps->setCurrentIndex(0);

  removeGraphMarkers();

  for (int i=0; i<m_SampledData->count(); i++) {
    QxrdRingSampledDataPtr p = m_SampledData->ring(i);

    if (p) {
      for (int j=0; j<p->count(); j++) {
        QwtDoublePoint pt = p->point(j);
        appendGraphMarker(i, pt);
      }
    }
  }

  m_ImagePlot -> replot();
}

void QxrdPowderFitIdentifyPage::pointClicked(QwtDoublePoint pt)
{
  appendOutputString(tr("Point Clicked (%1, %2)").arg(pt.x()).arg(pt.y()));

  appendGraphMarker(get_IdCurrentRing(), pt);

  appendPoint(pt);

  m_ImagePlot->replot();
}

void QxrdPowderFitIdentifyPage::appendRing()
{
  m_SampledData -> append();

  prop_IdCurrentRing() -> incValue(1);

  m_IdRemoveRingButton -> setEnabled(true);
}

void QxrdPowderFitIdentifyPage::removeRing()
{
  m_SampledData -> remove(get_IdCurrentRing());

  m_IdRemoveRingButton -> setEnabled(m_SampledData->count());
}

void QxrdPowderFitIdentifyPage::selectRing(int n)
{
  if (n >= 0 && n < m_SampledData->count()) {
    set_IdCurrentRing(n);
  }
}

void QxrdPowderFitIdentifyPage::selectNextRing()
{
  selectRing(get_IdCurrentRing() + 1);
}

void QxrdPowderFitIdentifyPage::selectPrevRing()
{
  selectRing(get_IdCurrentRing() - 1);
}

void QxrdPowderFitIdentifyPage::appendPoint(QwtDoublePoint pt)
{
  QxrdRingSampledDataPtr p = m_SampledData -> ring(get_IdCurrentRing());

  if (p) {
    p -> append(pt);
  }
}

static void initialFit(double *parm, double *x, int np, int nx, void *adata)
{
  QxrdPowderFitIdentifyPage *p = (QxrdPowderFitIdentifyPage*) adata;

  p -> evaluateFit(parm, x, np, nx);
}

static double deg2rad(double deg)
{
  return deg*M_PI/180.0;
}

void QxrdPowderFitIdentifyPage::idPerformFit()
{
  appendOutputString(tr("%1 rings defined").arg(m_SampledData->count()));

  int npts = 0;
  int nparm = 5 + m_SampledData->count();

  for (int i=0; i<m_SampledData->count(); i++) {
    QxrdRingSampledDataPtr p = m_SampledData->ring(i);

    if (p) {
      appendOutputString(tr("Ring %1 has %2 points").arg(i).arg(p->count()));

      npts += p->count();
    }
  }

  for (int i=0; i<m_SampledData->count(); i++) {
    QxrdRingSampledDataPtr p = m_SampledData->ring(i);

    if (p) {
      for (int j=0; j<p->count(); j++) {
        QwtDoublePoint pt = p->point(j);
        appendOutputString(tr("%1\t%2\t%3").arg(i).arg(pt.x()).arg(pt.y()));
      }
    }
  }


  QxrdRingSetFitParameters *p = m_Processor->initialRingSetFitParameters();

  m_FittedParameters.clear();

  if (p -> prop_CenterX() -> isFitted())  m_FittedParameters.append(p -> prop_CenterX());
  if (p -> prop_CenterY() -> isFitted())  m_FittedParameters.append(p -> prop_CenterY());
  if (p -> prop_Distance() -> isFitted()) m_FittedParameters.append(p -> prop_Distance());
  if (p -> prop_Tilt() -> isFitted())     m_FittedParameters.append(p -> prop_Tilt());
  if (p -> prop_Rotation() -> isFitted()) m_FittedParameters.append(p -> prop_Rotation());

  for (int i=0; i < p->count(); i++) {
    QxrdFitParameter *tth = p->ring(i)->prop_TwoTheta();

    if (tth -> isFitted()) m_FittedParameters.append(tth);
  }

  nparm = m_FittedParameters.count();

  QVector<double> parms(nparm), measured(npts);
  double info[LM_INFO_SZ];

  for (int i=0; i<nparm; i++) {
    parms[i] = m_FittedParameters[i]->value();
  }

  appendOutputString(tr("Initial fit parameters"));

  for (int i=0; i<nparm; i++) {
    appendOutputString(tr("P[%1](%2)=%3").arg(i).arg(m_FittedParameters[i]->name()).arg(parms[i]));
  }

  int ret = dlevmar_dif(initialFit, parms.data(), measured.data(), nparm, npts, 100, NULL, info, NULL, NULL, this);

  appendOutputString(tr("Fitted parameters"));

  for (int i=0; i<nparm; i++) {
    appendOutputString(tr("P[%1](%2)=%3").arg(i).arg(m_FittedParameters[i]->name()).arg(parms[i]));
  }

  for (int i=0; i<LM_INFO_SZ; i++) {
    appendOutputString(tr("INFO[%1]=%2").arg(i).arg(info[i]));
  }
}

void QxrdPowderFitIdentifyPage::evaluateFit(double *parm, double *x, int np, int nx)
{
  QVector<double> saved(np);

  for (int i=0; i<np; i++) {
    saved[i] = m_FittedParameters[i]->value();
    m_FittedParameters[i]->setValue(parm[i]);
  }

  QxrdRingSetFitParameters *pf = m_Processor->initialRingSetFitParameters();

  double cx       = pf -> get_CenterX();                // Beam center coords
  double cy       = pf -> get_CenterY();
  double d        = pf -> get_Distance();                // Sample-Detector distance
  double alpha    = deg2rad(pf -> get_Tilt());       // Detector Tilt angle
  double chi0     = deg2rad(pf -> get_Rotation());       // Rotation of tilt plane

  for (int i=0, n=0; i<m_SampledData->count(); i++) {
    QxrdRingSampledDataPtr p = m_SampledData->ring(i);

    double tth = deg2rad(pf->ring(i)->get_TwoTheta());                 // tth value of ring i

    double A = d*sin(tth)/cos(tth+alpha);   // Max ellipse R (aphelion)
    double B = d*sin(tth)/cos(tth-alpha);   // Min ellipse R (perihelion)
    double a = (A+B)/2;                     // Ellipse semi-major axis
    double e = (A-B)/(A+B);                 // Ellipse eccentricity

    if (p) {
      for (int j=0; j<p->count(); j++) {
        QwtDoublePoint pt = p->point(j);

        double dy = pt.y()-cy;
        double dx = pt.x()-cx;
        double chi = atan2(dy, dx);

        double rdat = sqrt(dy*dy + dx*dx);
        double rcalc = (1-e*e)*a/(1-e*cos(chi-chi0));

        x[n++] = rcalc-rdat;
      }
    }
  }

  for (int i=0; i<np; i++) {
    m_FittedParameters[i]->setValue(saved[i]);
  }
}
