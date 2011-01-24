#include "qxrdnidaqplugin.h"
#include "NIDAQmx.h"
#include <QVector>
#include <QElapsedTimer>
#include <stdio.h>

#define DAQmxErrChk(functionCall) { if( DAQmxFailed(error=(functionCall)) ) { QxrdNIDAQPlugin::errorCheck(__FILE__,__LINE__,error); goto Error; } }

QxrdNIDAQPlugin::QxrdNIDAQPlugin() :
  m_AOTaskHandle(0),
  m_AITaskHandle(0)
{
  printf("NI-DAQ plugin constructed\n");
  initTaskHandles();
}

QxrdNIDAQPlugin::~QxrdNIDAQPlugin()
{
  printf("NI-DAQ plugin destroyed");
  closeTaskHandles();
}

QString QxrdNIDAQPlugin::name() const
{
  return "NI DAQ Card";
}

void QxrdNIDAQPlugin::errorCheck(const char* file, int line, int err)
{
  if (DAQmxFailed(err)) {
    int sz = DAQmxGetErrorString(err, NULL, 0);

    if (sz > 0) {
      char *buff = (char*) malloc(sz);

      if (DAQmxGetErrorString(err, buff, sz) == 0) {
        printf("%s:%d NI-DAQ Error %d : %s\n", file, line, err, buff);
      }

      free(buff);
    }
  }
}

void QxrdNIDAQPlugin::initTaskHandles()
{
  int error;

  if (m_AOTaskHandle == 0) {
    DAQmxErrChk(DAQmxCreateTask("", &m_AOTaskHandle));
    DAQmxErrChk(DAQmxCreateAOVoltageChan (m_AOTaskHandle, "Dev1/ao0", NULL, -10.0, 10.0, DAQmx_Val_Volts, NULL));
    DAQmxErrChk(DAQmxCreateAOVoltageChan (m_AOTaskHandle, "Dev1/ao1", NULL, -10.0, 10.0, DAQmx_Val_Volts, NULL));
  }

  if (m_AITaskHandle == 0) {
    DAQmxErrChk(DAQmxCreateTask("", &m_AITaskHandle));
    DAQmxErrChk(DAQmxCreateAIVoltageChan (m_AITaskHandle, "Dev1/ai0", NULL, DAQmx_Val_Cfg_Default, -10.0, 10.0, DAQmx_Val_Volts, NULL));
    DAQmxErrChk(DAQmxCreateAIVoltageChan (m_AITaskHandle, "Dev1/ai1", NULL, DAQmx_Val_Cfg_Default, -10.0, 10.0, DAQmx_Val_Volts, NULL));
  }

  return;

Error:
  printf("Error in initTaskHandles\n");

  closeTaskHandles();
}

void QxrdNIDAQPlugin::closeTaskHandles()
{
  if (m_AOTaskHandle) {
    DAQmxClearTask(m_AOTaskHandle);
    m_AOTaskHandle = 0;
  }
  if (m_AITaskHandle) {
    DAQmxClearTask(m_AITaskHandle);
    m_AITaskHandle = 0;
  }
}

void   QxrdNIDAQPlugin::aoSet(double val1, double val2)
{
  QElapsedTimer t;
  t.start();

  double vals[2];
  int32 nWritten;

  vals[0]=val1;
  vals[1]=val2;

  int error;
  DAQmxErrChk(DAQmxWriteAnalogF64(m_AOTaskHandle, 1, true, 10.0,
                                  DAQmx_Val_GroupByScanNumber,
                                  vals, &nWritten,
                                  NULL));

  printf("Analog output took %d msec\n", t.elapsed());

Error:
  return;
}

void   QxrdNIDAQPlugin::aoSet(QString chan, double val)
{
  initTaskHandles();

  int error;
//  DAQmxErrChk(DAQmxCreateAOVoltageChan (m_TaskHandle, qPrintable(chan), NULL, -10.0, 10.0, DAQmx_Val_Volts, NULL));
  DAQmxErrChk(DAQmxWriteAnalogScalarF64(m_AOTaskHandle, true, 10.0, val, NULL));

Error:
  return;
}

double QxrdNIDAQPlugin::aiGet(int chan)
{
  return 0;
}

double QxrdNIDAQPlugin::aiGet(QString chan)
{
  initTaskHandles();

  int error;
  float64 res = 0;

  DAQmxErrChk(DAQmxReadAnalogScalarF64(m_AITaskHandle, 10.0, &res, NULL))

Error:
  return res;
}

void   QxrdNIDAQPlugin::aoWave(QString chan, int type, double freq, double amplitude, double offset)
{
  int error;
  float64 res = 0;
  QVector<float64> waveform(1024);

  for (int i=0; i<1024; i++) {
    waveform[i] = (i>512 ? 1 : -1);
  }

  int32 numWritten;

  DAQmxErrChk(DAQmxWriteAnalogF64(m_AOTaskHandle, 1024, true, 10.0, DAQmx_Val_GroupByChannel, waveform.data(), &numWritten, NULL))

Error:
  return;
}

Q_EXPORT_PLUGIN2(qxrdnidaqplugin, QxrdNIDAQPlugin);


