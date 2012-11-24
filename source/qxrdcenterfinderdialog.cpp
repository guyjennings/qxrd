#include "qxrdcenterfinderdialog.h"
#include "qxrdcenterfinder.h"
#include "qxrddebug.h"

QxrdCenterFinderDialog::QxrdCenterFinderDialog(QxrdCenterFinderPtr cen, QWidget *parent)
  : QDockWidget(parent),
    m_CenterFinder(cen)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdCenterFinderDialog::QxrdCenterFinderDialog(%p)\n", this);
  }

  setupUi(this);

  connect(m_CenterMoveUpLeft,    SIGNAL(clicked()), this, SLOT(centerMoveUpLeft()));
  connect(m_CenterMoveUp,        SIGNAL(clicked()), this, SLOT(centerMoveUp()));
  connect(m_CenterMoveUpRight,   SIGNAL(clicked()), this, SLOT(centerMoveUpRight()));
  connect(m_CenterMoveRight,     SIGNAL(clicked()), this, SLOT(centerMoveRight()));
  connect(m_CenterMoveDownRight, SIGNAL(clicked()), this, SLOT(centerMoveDownRight()));
  connect(m_CenterMoveDown,      SIGNAL(clicked()), this, SLOT(centerMoveDown()));
  connect(m_CenterMoveDownLeft,  SIGNAL(clicked()), this, SLOT(centerMoveDownLeft()));
  connect(m_CenterMoveLeft,      SIGNAL(clicked()), this, SLOT(centerMoveLeft()));

  m_CenterFinder -> prop_CenterX() -> linkTo(m_CenterX);
  m_CenterFinder -> prop_CenterY() -> linkTo(m_CenterY);
  m_CenterFinder -> prop_CenterStep() -> linkTo(m_CenterStep);
  m_CenterFinder -> prop_ImplementTilt() -> linkTo(m_ImplementTilt);
  m_CenterFinder -> prop_DetectorDistance() -> linkTo(m_DetectorDistance);
  m_CenterFinder -> prop_DetectorTilt() -> linkTo(m_DetectorTilt);
  m_CenterFinder -> prop_TiltPlaneRotation() -> linkTo(m_TiltPlaneRotation);
  m_CenterFinder -> prop_DetectorXPixelSize() -> linkTo(m_DetectorXPixelSize);
  m_CenterFinder -> prop_DetectorYPixelSize() -> linkTo(m_DetectorYPixelSize);
  m_CenterFinder -> prop_Energy() -> linkTo(m_Energy);
  m_CenterFinder -> prop_EnableGeometricCorrections() -> linkTo(m_EnableGeometry);
  m_CenterFinder -> prop_EnablePolarizationCorrections() -> linkTo(m_EnablePolarization);
  m_CenterFinder -> prop_Polarization() -> linkTo(m_Polarization);
  m_CenterFinder -> prop_EnableAbsorptionCorrections() -> linkTo(m_EnableAbsorption);
  m_CenterFinder -> prop_AttenuationLength() -> linkTo(m_AttenuationLength);

  connect(m_CenterFinder -> prop_ImplementTilt(), SIGNAL(valueChanged(bool,int)), this, SLOT(onImplementTiltChanged(bool)));
  connect(m_CenterFinder -> prop_EnablePolarizationCorrections(), SIGNAL(valueChanged(bool,int)), this, SLOT(onEnablePolarizationChanged(bool)));
  connect(m_CenterFinder -> prop_EnableAbsorptionCorrections(), SIGNAL(valueChanged(bool,int)), this, SLOT(onEnableAbsorptionChanged(bool)));

  onImplementTiltChanged(m_CenterFinder -> get_ImplementTilt());
  onEnablePolarizationChanged(m_CenterFinder -> get_EnablePolarizationCorrections());
  onEnableAbsorptionChanged(m_CenterFinder -> get_EnableAbsorptionCorrections());
}

QxrdCenterFinderDialog::~QxrdCenterFinderDialog()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdCenterFinderDialog::~QxrdCenterFinderDialog(%p)\n", this);
  }
}

void QxrdCenterFinderDialog::onImplementTiltChanged(bool imp)
{
  m_DetectorDistance  -> setEnabled(true /*imp*/);
  m_DetectorTilt      -> setEnabled(imp);
  m_TiltPlaneRotation -> setEnabled(imp);
}

void QxrdCenterFinderDialog::onEnablePolarizationChanged(bool pol)
{
  m_Polarization      -> setEnabled(pol);
}

void QxrdCenterFinderDialog::onEnableAbsorptionChanged(bool absn)
{
  m_AttenuationLength -> setEnabled(absn);
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
