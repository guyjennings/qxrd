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

  QxrdCenterFinderPtr cf(m_CenterFinder);

  if (cf) {
    cf -> prop_CenterX() -> linkTo(m_CenterX);
    cf -> prop_CenterY() -> linkTo(m_CenterY);
    cf -> prop_CenterStep() -> linkTo(m_CenterStep);
    cf -> prop_ImplementTilt() -> linkTo(m_ImplementTilt);
    cf -> prop_DetectorDistance() -> linkTo(m_DetectorDistance);
    cf -> prop_DetectorDistanceStep() -> linkTo(m_DetectorDistanceStep);
    cf -> prop_DetectorTilt() -> linkTo(m_DetectorTilt);
    cf -> prop_DetectorTiltStep() -> linkTo(m_DetectorTiltStep);
    cf -> prop_TiltPlaneRotation() -> linkTo(m_TiltPlaneRotation);
    cf -> prop_TiltPlaneRotationStep() -> linkTo(m_TiltPlaneRotationStep);
    cf -> prop_DetectorXPixelSize() -> linkTo(m_DetectorXPixelSize);
    cf -> prop_DetectorYPixelSize() -> linkTo(m_DetectorYPixelSize);
    cf -> prop_Energy() -> linkTo(m_Energy);

    cf -> prop_PeakFitRadius() -> linkTo(m_PeakFitRadius);
    cf -> prop_PeakFitDebug() -> linkTo(m_PeakFitDebug);
    cf -> prop_PeakFitIterations() -> linkTo(m_PeakFitIterations);
    cf -> prop_RingIndex() -> linkTo(m_RingIndex);
    cf -> prop_SubtractRingAverages() -> linkTo(m_SubtractRingAverages);
    cf -> prop_RingAverageDisplacement() -> linkTo(m_RingAverageDisplacement);
    cf -> prop_FittedWidthMin() -> linkTo(m_FittedWidthMin);
    cf -> prop_FittedWidthMax() -> linkTo(m_FittedWidthMax);
    cf -> prop_FittedHeightMinRatio() -> linkTo(m_FittedHeightMinRatio);
    cf -> prop_FittedPositionMaxDistance() -> linkTo(m_FittedPositionMaxDistance);
    cf -> prop_FitPowderPointPosition() -> linkTo(m_FitPowderPointPosition);

    connect(cf -> prop_ImplementTilt(), &QcepBoolProperty::valueChanged, this, &QxrdCenterFinderDialog::onImplementTiltChanged);

    onImplementTiltChanged(cf -> get_ImplementTilt());

    connect(cf->prop_CenterStep(),            &QcepDoubleProperty::valueChanged, this, &QxrdCenterFinderDialog::onStepSizesChanged);
    connect(cf->prop_DetectorDistanceStep(),  &QcepDoubleProperty::valueChanged, this, &QxrdCenterFinderDialog::onStepSizesChanged);
    connect(cf->prop_DetectorTiltStep(),      &QcepDoubleProperty::valueChanged, this, &QxrdCenterFinderDialog::onStepSizesChanged);
    connect(cf->prop_TiltPlaneRotationStep(), &QcepDoubleProperty::valueChanged, this, &QxrdCenterFinderDialog::onStepSizesChanged);
  }

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
  QxrdCenterFinderPtr cf(m_CenterFinder);

  if (cf) {
    cf -> set_CenterX(cx);
    cf -> set_CenterY(cy);
  }
//  printf("QxrdCenterFinderDialog::onCenterChanged(%g,%g)\n", cx, cy);
}

void QxrdCenterFinderDialog::moveCenter(int dx, int dy)
{
  QxrdCenterFinderPtr cf(m_CenterFinder);

  if (cf) {
    cf -> set_CenterX(cf -> get_CenterX() + cf -> get_CenterStep()*dx);
    cf -> set_CenterY(cf -> get_CenterY() + cf -> get_CenterStep()*dy);
  }
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

