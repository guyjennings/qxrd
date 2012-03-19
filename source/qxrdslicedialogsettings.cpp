#include "qxrdslicedialogsettings.h"

QxrdSliceDialogSettings::QxrdSliceDialogSettings(QxrdSettingsSaverWPtr saver, QObject *parent) :
  QObject(parent)
{
}

void QxrdSliceDialog::readSettings(QSettings *settings, QString section)
{
  if (settings) {
    m_SlicePlot->readSettings(settings, section+"/plot");

    QcepProperty::readSettings(this, &staticMetaObject, section, settings);

    m_Polygon.clear();

    int sz = settings->beginReadArray(section+"/polygon");

    for (int i=0; i<sz; i++) {
      settings->setArrayIndex(i);
      bool xok, yok;
      double x = settings->value("x").toDouble(&xok);
      double y = settings->value("y").toDouble(&yok);
      if (xok && yok) {
        m_Polygon.append(QwtDoublePoint(x,y));
      }
    }

    settings->endArray();
  }
}

void QxrdSliceDialog::writeSettings(QSettings *settings, QString section)
{
  if (settings) {
    m_SlicePlot->writeSettings(settings, section+"/plot");

    QcepProperty::writeSettings(this, &staticMetaObject, section, settings);

    settings->beginWriteArray(section+"/polygon", m_Polygon.count());

    for (int i=0; i<m_Polygon.count(); i++) {
      QwtDoublePoint pt = m_Polygon[i];

      settings->setArrayIndex(i);
      settings->setValue("x", pt.x());
      settings->setValue("y", pt.y());
    }

    settings->endArray();
  }
}

