#include "qxrdcenterfinderdialog.h"
#include "qxrdcenterfinder.h"
#include "qxrddebug.h"
#include <stdio.h>
#include "qxrduserscriptdialog.h"

QxrdCenterFinderDialog::QxrdCenterFinderDialog(QxrdCenterFinderPtr cen, QWidget *parent)
  : QDockWidget(parent),
    m_CenterFinder(cen)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdCenterFinderDialog::QxrdCenterFinderDialog(%p)\n", this);
  }

  setupUi(this);

  connect(m_CenterMoveUpLeft,    &QAbstractButton::clicked, this, &QxrdCenterFinderDialog::centerMoveUpLeft);
  connect(m_CenterMoveUp,        &QAbstractButton::clicked, this, &QxrdCenterFinderDialog::centerMoveUp);
  connect(m_CenterMoveUpRight,   &QAbstractButton::clicked, this, &QxrdCenterFinderDialog::centerMoveUpRight);
  connect(m_CenterMoveRight,     &QAbstractButton::clicked, this, &QxrdCenterFinderDialog::centerMoveRight);
  connect(m_CenterMoveDownRight, &QAbstractButton::clicked, this, &QxrdCenterFinderDialog::centerMoveDownRight);
  connect(m_CenterMoveDown,      &QAbstractButton::clicked, this, &QxrdCenterFinderDialog::centerMoveDown);
  connect(m_CenterMoveDownLeft,  &QAbstractButton::clicked, this, &QxrdCenterFinderDialog::centerMoveDownLeft);
  connect(m_CenterMoveLeft,      &QAbstractButton::clicked, this, &QxrdCenterFinderDialog::centerMoveLeft);

  m_CenterFinder -> prop_CenterX() -> linkTo(m_CenterX);
  m_CenterFinder -> prop_CenterY() -> linkTo(m_CenterY);
  m_CenterFinder -> prop_CenterStep() -> linkTo(m_CenterStep);
  m_CenterFinder -> prop_ImplementTilt() -> linkTo(m_ImplementTilt);
  m_CenterFinder -> prop_DetectorDistance() -> linkTo(m_DetectorDistance);
  m_CenterFinder -> prop_DetectorDistanceStep() -> linkTo(m_DetectorDistanceStep);
  m_CenterFinder -> prop_DetectorTilt() -> linkTo(m_DetectorTilt);
  m_CenterFinder -> prop_DetectorTiltStep() -> linkTo(m_DetectorTiltStep);
  m_CenterFinder -> prop_TiltPlaneRotation() -> linkTo(m_TiltPlaneRotation);
  m_CenterFinder -> prop_TiltPlaneRotationStep() -> linkTo(m_TiltPlaneRotationStep);
  m_CenterFinder -> prop_DetectorXPixelSize() -> linkTo(m_DetectorXPixelSize);
  m_CenterFinder -> prop_DetectorYPixelSize() -> linkTo(m_DetectorYPixelSize);
  m_CenterFinder -> prop_Energy() -> linkTo(m_Energy);

  m_CenterFinder -> prop_PeakFitRadius() -> linkTo(m_PeakFitRadius);
  m_CenterFinder -> prop_PeakFitDebug() -> linkTo(m_PeakFitDebug);
  m_CenterFinder -> prop_PeakFitIterations() -> linkTo(m_PeakFitIterations);
  m_CenterFinder -> prop_RingIndex() -> linkTo(m_RingIndex);
  m_CenterFinder -> prop_SubtractRingAverages() -> linkTo(m_SubtractRingAverages);
  m_CenterFinder -> prop_RingAverageDisplacement() -> linkTo(m_RingAverageDisplacement);
  m_CenterFinder -> prop_FittedWidthMin() -> linkTo(m_FittedWidthMin);
  m_CenterFinder -> prop_FittedWidthMax() -> linkTo(m_FittedWidthMax);
  m_CenterFinder -> prop_FittedHeightMinRatio() -> linkTo(m_FittedHeightMinRatio);
  m_CenterFinder -> prop_FittedPositionMaxDistance() -> linkTo(m_FittedPositionMaxDistance);
  m_CenterFinder -> prop_FitPowderPointPosition() -> linkTo(m_FitPowderPointPosition);

  connect(m_CenterFinder -> prop_ImplementTilt(), &QcepBoolProperty::valueChanged, this, &QxrdCenterFinderDialog::onImplementTiltChanged);

  onImplementTiltChanged(m_CenterFinder -> get_ImplementTilt());

  connect(m_CenterFinder->prop_CenterStep(),            &QcepDoubleProperty::valueChanged, this, &QxrdCenterFinderDialog::onStepSizesChanged);
  connect(m_CenterFinder->prop_DetectorDistanceStep(),  &QcepDoubleProperty::valueChanged, this, &QxrdCenterFinderDialog::onStepSizesChanged);
  connect(m_CenterFinder->prop_DetectorTiltStep(),      &QcepDoubleProperty::valueChanged, this, &QxrdCenterFinderDialog::onStepSizesChanged);
  connect(m_CenterFinder->prop_TiltPlaneRotationStep(), &QcepDoubleProperty::valueChanged, this, &QxrdCenterFinderDialog::onStepSizesChanged);

  onStepSizesChanged();
}

QxrdCenterFinderDialog::~QxrdCenterFinderDialog()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdCenterFinderDialog::~QxrdCenterFinderDialog(%p)\n", this);
  }
}

void QxrdCenterFinderDialog::onStepSizesChanged()
{
  m_CenterX           -> setSingleStep(m_CenterStep->value());
  m_CenterY           -> setSingleStep(m_CenterStep->value());
  m_DetectorDistance  -> setSingleStep(m_DetectorDistanceStep->value());
  m_DetectorTilt      -> setSingleStep(m_DetectorTiltStep->value());
  m_TiltPlaneRotation -> setSingleStep(m_TiltPlaneRotationStep->value());
}

void QxrdCenterFinderDialog::onImplementTiltChanged(bool imp)
{
//  m_DetectorDistance  -> setEnabled(true /*imp*/);
  m_DetectorTilt          -> setEnabled(imp);
  m_DetectorTiltStep      -> setEnabled(imp);
  m_TiltPlaneRotation     -> setEnabled(imp);
  m_TiltPlaneRotationStep -> setEnabled(imp);
}

void QxrdCenterFinderDialog::onCenterChanged(double cx, double cy)
{
  m_CenterFinder -> set_CenterX(cx);
  m_CenterFinder -> set_CenterY(cy);

//  printf("QxrdCenterFinderDialog::onCenterChanged(%g,%g)\n", cx, cy);
}

void QxrdCenterFinderDialog::moveCenter(int dx, int dy)
{
  m_CenterFinder -> set_CenterX(m_CenterFinder -> get_CenterX() + m_CenterFinder -> get_CenterStep()*dx);
  m_CenterFinder -> set_CenterY(m_CenterFinder -> get_CenterY() + m_CenterFinder -> get_CenterStep()*dy);
}

void QxrdCenterFinderDialog::centerMoveUpLeft()
{
  moveCenter(-1,1);
}

void QxrdCenterFinderDialog::centerMoveUp()
{
  moveCenter(0,1);
}

void QxrdCenterFinderDialog::centerMoveUpRight()
{
  moveCenter(1,1);
}

void QxrdCenterFinderDialog::centerMoveRight()
{
  moveCenter(1,0);
}

void QxrdCenterFinderDialog::centerMoveDownRight()
{
  moveCenter(1,-1);
}

void QxrdCenterFinderDialog::centerMoveDown()
{
  moveCenter(0,-1);
}

void QxrdCenterFinderDialog::centerMoveDownLeft()
{
  moveCenter(-1,-1);
}

void QxrdCenterFinderDialog::centerMoveLeft()
{
  moveCenter(-1,0);
}

