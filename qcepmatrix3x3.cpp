#include "qcepmatrix3x3.h"

void QcepMatrix3x3::setSettingsValue(QSettings *settings, QString name)
{
  settings->beginGroup(name);

  for (int r=0; r<3; r++) {
    for (int c=0; c<3; c++) {
      settings->setValue(QString("r%1c%2").arg(r).arg(c), operator()(r,c));
    }
  }

  settings->endGroup();
}
