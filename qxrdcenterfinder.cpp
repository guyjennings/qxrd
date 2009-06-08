#include "qxrdcenterfinder.h"
#include "qxrdcenterfinderdialog.h"
#include "qxrdcenterfinderpicker.h"
#include "qxrdwindow.h"
#include <qwt_plot_marker.h>

QxrdCenterFinder::QxrdCenterFinder
    (QxrdWindow *win, QxrdImagePlot *imgplot, QxrdCenterFinderPlot *cntplot, QxrdCenterFinderDialog *cfdialog, QObject *parent)
  : QObject(parent),
    m_Window(win),
    m_ImagePlot(imgplot),
    m_CenterFinderPlot(cntplot),
    m_CenterFinderDialog(cfdialog),
    m_CenterX(0),
    m_CenterY(0),
    m_StepSize(1),
    m_CenterFinderPicker(NULL),
    m_CenterMarker(NULL)
{
  m_CenterFinderPicker = new QxrdCenterFinderPicker(imgplot->canvas(), imgplot, cfdialog);

  m_CenterMarker = new QwtPlotMarker();
  m_CenterMarker -> setLineStyle(QwtPlotMarker::Cross);

  connect(m_CenterFinderPicker, SIGNAL(selected(QwtDoublePoint)),
          this,                 SLOT(onCenterChanged(QwtDoublePoint)));

  connect(m_CenterFinderDialog -> m_CenterMoveUpLeft, SIGNAL(clicked()), this, SLOT(centerMoveUpLeft()));
  connect(m_CenterFinderDialog -> m_CenterMoveUp, SIGNAL(clicked()), this, SLOT(centerMoveUp()));
  connect(m_CenterFinderDialog -> m_CenterMoveUpRight, SIGNAL(clicked()), this, SLOT(centerMoveUpRight()));
  connect(m_CenterFinderDialog -> m_CenterMoveRight, SIGNAL(clicked()), this, SLOT(centerMoveRight()));
  connect(m_CenterFinderDialog -> m_CenterMoveDownRight, SIGNAL(clicked()), this, SLOT(centerMoveDownRight()));
  connect(m_CenterFinderDialog -> m_CenterMoveDown, SIGNAL(clicked()), this, SLOT(centerMoveDown()));
  connect(m_CenterFinderDialog -> m_CenterMoveDownLeft, SIGNAL(clicked()), this, SLOT(centerMoveDownLeft()));
  connect(m_CenterFinderDialog -> m_CenterMoveLeft, SIGNAL(clicked()), this, SLOT(centerMoveLeft()));

  connect(m_CenterFinderDialog -> m_CenterX, SIGNAL(valueChanged(double)), this, SLOT(onCenterXChanged(double)));
  connect(m_CenterFinderDialog -> m_CenterY, SIGNAL(valueChanged(double)), this, SLOT(onCenterYChanged(double)));
  connect(m_CenterFinderDialog -> m_CenterStep, SIGNAL(valueChanged(double)), this, SLOT(onCenterStepChanged(double)));
  connect(m_CenterFinderDialog -> m_AutoScaleButton, SIGNAL(clicked()), m_CenterFinderPlot, SLOT(autoScale()));

  connect(this, SIGNAL(centerChanged(double,double)), m_CenterFinderDialog, SLOT(onCenterChanged(double,double)));
  connect(this, SIGNAL(centerChanged(double,double)), this, SLOT(onCenterChanged(double,double)));

  imgplot -> setCenterFinder(this);
}

void QxrdCenterFinder::setEnabled(bool imgenabled, bool cntrenabled)
{
  m_CenterFinderPicker -> setEnabled(imgenabled);

  if (imgenabled) {
    m_CenterMarker     -> attach(m_ImagePlot);
  } else {
    m_CenterMarker     -> detach();
  }
}

void QxrdCenterFinder::setPen(const QPen &pen)
{
  m_Pen = pen;

  if (m_CenterFinderPicker) {
    m_CenterFinderPicker -> setTrackerPen(pen);
    m_CenterFinderPicker -> setRubberBandPen(pen);
  }

  if (m_CenterMarker) {
    m_CenterMarker -> setLinePen(pen);
  }
}

void QxrdCenterFinder::onCenterXChanged(double cx)
{
  if (m_CenterX != cx) {
    m_CenterX = cx;
    m_CenterMarker -> setXValue(cx);

    m_ImagePlot -> replot();

    emit centerChanged(m_CenterX, m_CenterY);
  }
}

void QxrdCenterFinder::onCenterYChanged(double cy)
{
  if (m_CenterY != cy) {
    m_CenterY = cy;
    m_CenterMarker -> setYValue(cy);

    m_ImagePlot -> replot();

    emit centerChanged(m_CenterX, m_CenterY);
  }
}

void QxrdCenterFinder::onCenterChanged(QwtDoublePoint pt)
{
  if (m_CenterX != pt.x() || m_CenterY != pt.y()) {
    m_CenterX = pt.x();
    m_CenterY = pt.y();
    m_CenterMarker -> setValue(pt);

    m_ImagePlot -> replot();

    emit centerChanged(m_CenterX, m_CenterY);
  }
}

void QxrdCenterFinder::onCenterChanged(double cx, double cy)
{
  QxrdImageData *data = m_Window -> data();

  m_CenterFinderPlot -> onCenterChanged(data, cx, cy);
}

void QxrdCenterFinder::onCenterStepChanged(double stp)
{
  m_StepSize = stp;
}

void QxrdCenterFinder::moveCenter(int dx, int dy)
{
  m_CenterX += m_StepSize*dx;
  m_CenterY += m_StepSize*dy;

  if (dx != 0 || dy != 0) {
    printf("QxrdCenterFinder::moveCenter -> %g, %g\n", m_CenterX, m_CenterY);

    emit centerChanged(m_CenterX, m_CenterY);
  }
}

void QxrdCenterFinder::centerMoveUpLeft()
{
  moveCenter(-1,1);
}

void QxrdCenterFinder::centerMoveUp()
{
  moveCenter(0,1);
}

void QxrdCenterFinder::centerMoveUpRight()
{
  moveCenter(1,1);
}

void QxrdCenterFinder::centerMoveRight()
{
  moveCenter(1,0);
}

void QxrdCenterFinder::centerMoveDownRight()
{
  moveCenter(1,-1);
}

void QxrdCenterFinder::centerMoveDown()
{
  moveCenter(0,-1);
}

void QxrdCenterFinder::centerMoveDownLeft()
{
  moveCenter(-1,-1);
}

void QxrdCenterFinder::centerMoveLeft()
{
  moveCenter(-1,0);
}

