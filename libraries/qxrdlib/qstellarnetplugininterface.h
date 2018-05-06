#ifndef QSTELLARNETPLUGININTERFACE_H
#define QSTELLARNETPLUGININTERFACE_H

#include "qxrdlib_global.h"
#include "qcepobject.h"

class QXRD_EXPORT QStellarNetPluginInterface : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  QStellarNetPluginInterface(QString name);

  virtual int openStellarNetLibrary() = 0;
  virtual int closeStellarNetLibrary() = 0;
  virtual void initializeSpectrometer(int IntegrationPeriod,
                                      int ScansToAvg,
                                      int xsmooth,
                                      int xtrate,
                                      bool TemperatureCompensation) = 0;
  virtual bool readSpectrometer(int chan, float *DataBuffer) = 0;
  virtual int getDeviceCount(void) = 0;
  virtual int getBWee(int chan,
                      double *CalCoeff1,
                      double *CalCoeff2,
                      double *CalCoeff3,
                      double *CalCoeff4) = 0;

};

#define StellarNetPluginInterface_iid "gov.anl.aps.cep.Qxrd.StellarNetPlugin"
Q_DECLARE_INTERFACE(QStellarNetPluginInterface, StellarNetPluginInterface_iid)

#endif // QSTELLARNETPLUGININTERFACE_H
