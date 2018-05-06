#ifndef QSTELLARNETPLUGIN_H
#define QSTELLARNETPLUGIN_H

#include "qcepobject.h"
#include "qstellarnetplugininterface.h"

class QStellarNetPlugin :
    public QStellarNetPluginInterface
{
  Q_OBJECT
  Q_PLUGIN_METADATA(IID StellarNetPluginInterface_iid FILE "stellarnet.json")
  Q_INTERFACES(QStellarNetPluginInterface)

private:
  typedef QStellarNetPluginInterface inherited;

public:
  QStellarNetPlugin();

  int openStellarNetLibrary();

  int closeStellarNetLibrary();

  void initializeSpectrometer(
      int integrationPeriod,
      int scansToAvg,
      int xsmooth,
      int xtrate,
      bool temperatureCompensation);

  bool readSpectrometer(
      int chan,
      float *dataBuffer);

  int getDeviceCount(void);

  int getBWee(
      int chan,
      double *calCoeff1,
      double *calCoeff2,
      double *calCoeff3,
      double *calCoeff4);
};

Q_DECLARE_METATYPE(QStellarNetPlugin*)

#endif // QSTELLARNETPLUGIN_H
