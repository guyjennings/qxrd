#include "qxrdcenterfinder.h"
#include "qxrdcenterfinderdialog.h"

QxrdCenterFinder::QxrdCenterFinder
    (QxrdImagePlot *imgplot, QxrdCenterFinderPlot *cntplot, QxrdCenterFinderDialog *cfdialog, QObject *parent)
  : QObject(parent),
    m_Imageplot(imgplot),
    m_CenterFinderPlot(cntplot),
    m_CenterFinderDialog(cfdialog),
    m_CenterX(0),
    m_CenterY(0),
    m_StepSize(1)
{
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

  connect(this, SIGNAL(centerChanged(double,double)), m_CenterFinderDialog, SLOT(onCenterChanged(double,double)));
}

void QxrdCenterFinder::onCenterXChanged(double cx)
{
  if (m_CenterX != cx) {
    m_CenterX = cx;

    emit centerChanged(m_CenterX, m_CenterY);
  }
}

void QxrdCenterFinder::onCenterYChanged(double cy)
{
  if (m_CenterY != cy) {
    m_CenterY = cy;

    emit centerChanged(m_CenterX, m_CenterY);
  }
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

