#include "qstellarnetplugin.h"

QStellarNetPlugin::QStellarNetPlugin()
  : inherited("stellarNetPlugin")
{
}

int QStellarNetPlugin::openStellarNetLibrary()
{
  return 0;
}

int QStellarNetPlugin::closeStellarNetLibrary()
{
  return 0;
}

void QStellarNetPlugin::initializeSpectrometer(
    int integrationPeriod,
    int scansToAvg,
    int xsmooth,
    int xtrate,
    bool temperatureCompensation)
{
}

bool QStellarNetPlugin::readSpectrometer(
    int chan,
    float *dataBuffer)
{
}

int QStellarNetPlugin::getDeviceCount()
{
  return 0;
}

int QStellarNetPlugin::getBWee(
    int chan,
    double *calCoeff1,
    double *calCoeff2,
    double *calCoeff3,
    double *calCoeff4)
{
  return 0;
}
