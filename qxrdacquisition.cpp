#include "qavrgacquisition.h"

#include <QTime>
#include <QTimer>
#include <QMetaObject>
#include <QList>
#include <QtConcurrentRun>
#include "AcqirisD1Import.h"
#include "qavrgapplication.h"
#include "qavrgfillpattern.h"
#include "qavrgfitter.h"
#include "qavrgsettings.h"
#include <math.h>

Q_DECLARE_METATYPE(QVector<double>);
Q_DECLARE_METATYPE(QVector<QString>);

QavrgAcquisition::QavrgAcquisition(QavrgApplication *app)
  : QObject(),
    m_Mutex(QMutex::Recursive),
    m_Application(app),
    m_NInstruments(0),
    m_InstrumentID(NULL),
    m_AcquisitionParms(NULL),
    m_NChannels(NULL),
    m_TotalChannels(0),
    m_SaveFile(NULL),
    m_ParametersChanged(false),
    m_ReferenceChanged(false),
    m_DarkChanged(false),
    m_ReferenceAvailable(false),
    m_DarkAvailable(false)
{
  printf("Enter QavrgAcquisition::QavrgAcquisition\n");

  m_InstrumentID = new ViSession[20];
  m_NChannels = new long[20];
  m_AcquisitionParms = new QavrgAcquisitionParms();

  newSaveFile("saveddata",1);

  findDevices();

  loadParameters();
  loadReferenceData();
  loadDarkData();

  printf("Leave QavrgAcquisition::QavrgAcquisition\n");
}

QavrgAcquisition::~QavrgAcquisition()
{
  delete [] m_InstrumentID;
  delete [] m_NChannels;
  delete m_AcquisitionParms;

  if (m_ParametersChanged) {
    saveParameters();
  }

  if (m_ReferenceChanged) {
    saveReferenceData();
  }

  if (m_DarkChanged) {
    saveDarkData();
  }

  QavrgFitter *t;

  foreach(t, m_Fitters) {
    delete t;
  }

  fclose(m_SaveFile);
}

void QavrgAcquisition::referenceloaded(int t)
{
  m_ReferenceAvailable = t;
}

void QavrgAcquisition::darkloaded(int t)
{
  m_DarkAvailable = t;
}

QavrgFitter *QavrgAcquisition::fitter(int chan)
{
  return m_Fitters.value(chan);
}

int QavrgAcquisition::concurrent() const
{
  return m_Application -> acquisitionParms() -> concurrent();
}

int QavrgAcquisition::resultSize()
{
  QMutexLocker lock(&m_Mutex);

  int maxsize = 0;

  QavrgFitter *t;

  foreach(t, m_Fitters) {
    if (t) {
      int resultsize = t -> resultSize();
      if (resultsize > maxsize) {
	maxsize = resultsize;
      }
    }
  }

  return maxsize;
}

int QavrgAcquisition::totalChannels()
{
  return m_TotalChannels;
}

QavrgAcquisitionParms *QavrgAcquisition::parms()
{
  return m_AcquisitionParms;
}

void QavrgAcquisition::setParms(QavrgAcquisitionParms *parms)
{
  m_AcquisitionParms -> copy(parms);
}

void QavrgAcquisition::fillPatternChanged()
{
  QList<double> patt = m_Application -> fillPattern() -> fillPattern();

  m_AcquisitionParms -> setFillPattern(patt);
}

void QavrgAcquisition::acquire(double integ, int nreps)
{
  {
    QMutexLocker lock(&m_Mutex);

    printf("QavrgAcquisition::acquire(%g,%d)\n", integ, nreps);

    m_AcquisitionParms -> setNAverages((int)(integ*1000));
    m_AcquisitionParms -> setNRepetitions(nreps);
  }

  configureAverager();

  for (int i=0; i<nreps; i++) {
    startAcquisition(i);

    waitAcquisition(i);
    readoutAcquisition(i);
  }
}

void QavrgAcquisition::setReferenceData()
{
  QMutexLocker lock(&m_Mutex);

  m_ReferenceData.resize(m_TotalChannels);

  for (int i=0; i<m_TotalChannels; i++) {
    int sz = m_Data[i].size();
    m_ReferenceData[i].resize(sz);

    for (int j=0; j<sz; j++) {
      m_ReferenceData[i][j] = m_Data[i][j];
    }
  }    

  m_ReferenceAvailable = true;

  saveReferenceData();
}

void QavrgAcquisition::setDarkData()
{
  QMutexLocker lock(&m_Mutex);

  m_DarkData.resize(m_TotalChannels);

  for (int i=0; i<m_TotalChannels; i++) {
    int sz = m_Data[i].size();
    m_DarkData[i].resize(sz);

    for (int j=0; j<sz; j++) {
      m_DarkData[i][j] = m_Data[i][j];
    }
  }    

  m_DarkAvailable = true;

  saveDarkData();
}

QVector< QVector<double> > QavrgAcquisition::readData()
{
  return m_Data;
}

QVector<double> QavrgAcquisition::readData(int chan, int start, int nbins)
{
  return m_Data.value(chan).mid(start,nbins);
}

QVector<double> QavrgAcquisition::readData(int chan)
{
  return m_Data.value(chan);
}

double QavrgAcquisition::readData(int chan, int bin)
{
  return m_Data.value(chan).value(bin);
}

int QavrgAcquisition::referenceAvailable() const
{
  return m_ReferenceAvailable;
}

int QavrgAcquisition::darkAvailable() const
{
  return m_DarkAvailable;
}

QVector< QVector<double> > QavrgAcquisition::readReference()
{
  return m_ReferenceData;
}

QVector<double> QavrgAcquisition::readReference(int chan, int start, int nbins)
{
  return m_ReferenceData.value(chan).mid(start,nbins);
}

QVector<double> QavrgAcquisition::readReference(int chan)
{
  return m_ReferenceData.value(chan);
}

double QavrgAcquisition::readReference(int chan, int bin)
{
  return m_ReferenceData.value(chan).value(bin);
}

QVector< QVector<double> > QavrgAcquisition::readDark()
{
  return m_DarkData;
}

QVector<double> QavrgAcquisition::readDark(int chan, int start, int nbins)
{
  return m_DarkData.value(chan).mid(start,nbins);
}

QVector<double> QavrgAcquisition::readDark(int chan)
{
  return m_DarkData.value(chan);
}

double QavrgAcquisition::readDark(int chan, int bin)
{
  return m_DarkData.value(chan).value(bin);
}

void QavrgAcquisition::writeData(int chan, QVector<double> data)
{
  QMutexLocker lock(&m_Mutex);

  if (chan >= 0 && chan < m_TotalChannels) {
    m_Data[chan] = data;

    emit newDataAvailable();
  } else {
    if (chan == 0 || chan == 1) {
      m_TotalChannels = 2;
      m_Data.resize(m_TotalChannels);
      m_Data[chan] = data;
      emit newDataAvailable();
    } else {
      printf("Didn't write data, chan=%d, m_TotalChannels=%ld\n", chan, m_TotalChannels);
    }
  }
}

void QavrgAcquisition::checkStatus(ViStatus status)
{
  if (status != VI_SUCCESS) {
    printf("checkStatus(%lx)\n", status);
  }
}

void QavrgAcquisition::findDevices()
{
  QMutexLocker lock(&m_Mutex);

  ViStatus status;
  ViString options = "";

  options = "cal=0 dma=1";

  status = AcqrsD1_multiInstrAutoDefine(options, &m_NInstruments);
  checkStatus(status);

  printf("Found %ld Acqiris Instruments\n", m_NInstruments);

  for (int i=0; i<m_NInstruments; i++) {
    char resourceName[20];
    sprintf(resourceName, "PCI::INSTR%d",i);

    status = AcqrsD1_InitWithOptions(resourceName, VI_FALSE, VI_FALSE,
				     options, &m_InstrumentID[i]);
    checkStatus(status);

    status = AcqrsD1_getNbrChannels(m_InstrumentID[i], &m_NChannels[i]);
    checkStatus(status);

    printf("Instrument %d has %ld channels\n", i, m_NChannels[i]);

    checkStatus(AcqrsD1_configMode(m_InstrumentID[i], 2, 0, 0)); // 0 for dig, 2 for avg

    m_TotalChannels += m_NChannels[i];
  }

  printf("Starting fitter threads...\n");

  m_Fitters.resize(m_TotalChannels);

  for (int i=0; i < m_TotalChannels; i++) {
    m_Fitters[i] = new QavrgFitter(this, i);

    connect(m_Fitters[i], SIGNAL(resultsAvailable(int)),
	    this, SLOT(resultsAvailable(int)));

    connect(m_Fitters[i], SIGNAL(savingComplete(int)),
	    this, SLOT(savingComplete(int)));

    m_Fitters[i] -> setSaveFile(m_SaveFile);
  }

  printf("Ready..\n");
}

void QavrgAcquisition::resultsAvailable(int chan)
{
  emit resultsChanged();
}

void QavrgAcquisition::savingComplete(int chan)
{
  if (chan < (m_TotalChannels-1)) {
    m_Fitters[chan+1] -> startSave();
  } else {
    fprintf(m_SaveFile, "\n");
  }
}

void QavrgAcquisition::newSaveFile(QString f, int n)
{
  FILE* nf = fopen(qPrintable(f),"a+");

  if (nf) {
    if (m_SaveFile) {
      fclose(m_SaveFile);
      m_SaveFile = NULL;
    }

    m_SaveFile = nf;
    m_SaveFileName = f;
    m_ScanNumber = n;

    fprintf(m_SaveFile, "#F %s\n", qPrintable(f));
    fprintf(m_SaveFile, "#E %d\n", ::time(NULL));
    fprintf(m_SaveFile, "#D %s\n", qPrintable(QDateTime::currentDateTime().toString()));
    fprintf(m_SaveFile, "\n");
  }
}

void QavrgAcquisition::startNewScan(QVector<QString> header)
{
  fprintf(m_SaveFile, "#S %d qavrgscan\n", m_ScanNumber++);
  fprintf(m_SaveFile, "#D %s\n", qPrintable(QDateTime::currentDateTime().toString()));

  int ncols = 0;

  for (int ch=0; ch < m_TotalChannels; ch++) {
    QavrgFitter *f = fitter(ch);

    if (f) {
      ncols += 3*(f->resultSize());
    }
  }

  int nhdr  = header.size();

  fprintf(m_SaveFile, "#N %d\n", ncols+nhdr);

  for (int i=0; i<nhdr; i++) {
    if (i) {
      fprintf(m_SaveFile, "\t%s", qPrintable(header[i]));
    } else {
      fprintf(m_SaveFile, "#L %s", qPrintable(header[i]));
    }
  }

  for (int ch=0; ch < m_TotalChannels; ch++) {
    QavrgFitter *f = fitter(ch);

    if (f) {
      int nbins = f->resultSize();

      for (int bin=0; bin < nbins; bin++) {
	for (int parm=0; parm<3; parm++) {
	  fprintf(m_SaveFile, "\t%s", qPrintable(resultHeader(ch, parm, bin)));
	}
      }
    }
  }

  fprintf(m_SaveFile, "\n");
}

void QavrgAcquisition::saveAcquisitionResult(QVector<double> data)
{
  printf("acquisition data ready\n");

  int nhdr = data.size();

  for (int i=0; i<nhdr; i++) {
    if (i) {
      fprintf(m_SaveFile, "\t%g", data[i]);
    } else {
      fprintf(m_SaveFile, "%g",   data[i]);
    }
  }

  for (int ch=0; ch < m_TotalChannels; ch++) {
    QavrgFitter *f = fitter(ch);

    if (f) {
      int nbins = f->resultSize();

      for (int bin=0; bin < nbins; bin++) {
	for (int parm=0; parm<3; parm++) {
	  fprintf(m_SaveFile, "\t%g", readResult(ch, parm, bin));
	}
      }
    }
  }

  fprintf(m_SaveFile, "\n");
}

double QavrgAcquisition::readResultAverage(int chan, int parm, int start, int nbins)
{
  QavrgFitter *f = fitter(chan);

  if (f) {
    return f->readResultAverage(parm,start,nbins);
  } else {
    return 0;
  }
}
  
double QavrgAcquisition::readResultBunchAverage(int chan, int parm, int bunch, int start, int norbits)
{
  QavrgFitter *f = fitter(chan);

  if (f) {
    return f->readResultBunchAverage(parm, bunch, start, norbits);
  } else {
    return 0;
  }
}

QVector<double> QavrgAcquisition::readResult(int chan, int parm, int start, int nbins)
{
  QavrgFitter *f = fitter(chan);

  if (f) {
    return f->readResult(parm,start,nbins);
  } else {
    return QVector<double>();
  }
}

QVector<double> QavrgAcquisition::readResult(int chan, int parm)
{
  QavrgFitter *f = fitter(chan);

  if (f) {
    return f->readResult(parm);
  } else {
    return QVector<double>();
  }
}

double QavrgAcquisition::readResult(int chan, int parm, int bin)
{
  QavrgFitter *f = fitter(chan);

  if (f) {
    return f->readResult(parm,bin);
  } else {
    return 0;
  }
}

QString QavrgAcquisition::resultHeader(int chan, int parm, int bin)
{
  char * labels = "BSP";
  
  return tr("Ch%1%2%3").arg(chan).arg(labels[parm]).arg(bin);
}

void QavrgAcquisition::configureAverager()
{
  printf("configure averager (thread %p, this-thread() %p\n", QThread::currentThread(), thread());

//   QMutexLocker lock(&m_Mutex);

  double sampInterval = 1.e-9, delayTime = 0.0;
  // long nbrSamples = 1000;
  long nbrSegments = 1;
  long coupling = 3, bandwidth = 0;
  double fullScale = 5, offset = 0.0;
  long trigCoupling = 0, trigSlope = 0;
  //  double trigLevel = 20.0; // In % of vertical Full Scale !!

  // ### Configuration parameters for averager ###
  long nbrSamples = m_AcquisitionParms->nSamples(), nbrWaveForms = m_AcquisitionParms->nAverages();
  long ditherRange = 0, trigResync = 1;
  long startDelay = 0, stopDelay = 0;
  
  int i = m_AcquisitionParms->instrument();

  // if (i >= 0 && i < m_NInstruments) {
  for (int i=0; i<m_NInstruments; i++) {

    // Configure timebase
    checkStatus(AcqrsD1_configHorizontal(m_InstrumentID[i], sampInterval, delayTime));
    checkStatus(AcqrsD1_configMemory(m_InstrumentID[i], nbrSamples, nbrSegments));

    // Configure edge trigger on external trigger 1
    checkStatus(AcqrsD1_configTrigClass(m_InstrumentID[i], 0, 0x80000000, 0, 0, 0.0, 0.0));
    // Configure external trigger 1
    checkStatus(AcqrsD1_configTrigSource(m_InstrumentID[i], -1, trigCoupling, trigSlope, 1000, 0.0));
		
    for (long channel = 1; channel <= m_NChannels[i]; channel++) {
      // Configure vertical settings
      checkStatus(AcqrsD1_configVertical(m_InstrumentID[i], channel, fullScale, offset, coupling, bandwidth));
      // number of samples
      checkStatus(AcqrsD1_configAvgConfig(m_InstrumentID[i], channel, "NbrSamples", &nbrSamples));
      // number of waveforms
      checkStatus(AcqrsD1_configAvgConfig(m_InstrumentID[i], channel, "NbrWaveforms", &nbrWaveForms));
      // dithering range
      checkStatus(AcqrsD1_configAvgConfig(m_InstrumentID[i], channel, "DitherRange", &ditherRange));
      // trigger resync
      checkStatus(AcqrsD1_configAvgConfig(m_InstrumentID[i], channel, "TrigResync", &trigResync));
      // startDelay
      checkStatus(AcqrsD1_configAvgConfig(m_InstrumentID[i], channel, "StartDelay", &startDelay));
      // stopDelay
      checkStatus(AcqrsD1_configAvgConfig(m_InstrumentID[i], channel, "StopDelay", &stopDelay));
    }
  }
}

void QavrgAcquisition::startAcquisition(int rep)
{
  printf("start acquisition (thread %p, this-thread() %p\n", QThread::currentThread(), thread());

//   QMutexLocker lock(&m_Mutex);

  ViStatus status;

  int i = m_AcquisitionParms->instrument();

  for (int i=0; i<m_NInstruments; i++) {
    m_Time.start();
  
    checkStatus(AcqrsD1_acquire(m_InstrumentID[i]));
  }
}

void QavrgAcquisition::waitAcquisition(int rep)
{
  printf("wait acquisition (thread %p, this-thread() %p\n", QThread::currentThread(), thread());

//   QMutexLocker lock(&m_Mutex);

  ViStatus status;

  int i = m_AcquisitionParms->instrument();

  for (int i=0; i<m_NInstruments; i++) {
    status = AcqrsD1_waitForEndOfAcquisition(m_InstrumentID[i], m_AcquisitionParms->nAverages()+5000);

    if (status == ACQIRIS_ERROR_ACQ_TIMEOUT) {
      checkStatus(AcqrsD1_stopAcquisition(m_InstrumentID[i]));
      printf("Acquisition timeout\n");
    }

    printf("Acquisition took %d msec\n", m_Time.restart());
  }
}

void QavrgAcquisition::readoutAcquisition(int rep)
{
  printf("readout acquisition (thread %p, this-thread() %p\n", QThread::currentThread(), thread());

//   QMutexLocker lock(&m_Mutex);

  QTime timer;
  AqReadParameters readParams;
  AqDataDescriptor wfDesc;
  AqSegmentDescriptorAvg segDesc[1];
  int chan=0;

  int nsamples = m_AcquisitionParms->nSamples();
  int nrepetitions = m_AcquisitionParms->nRepetitions();

  int i = m_AcquisitionParms->instrument();

  for (int i=0; i<m_NInstruments; i++) {
    timer.start();

    for (int ch=0; ch<m_NChannels[i]; ch++, chan++) {
      if (chan >= m_Data.count()) {
	m_Data.resize(chan+1);
	m_Accumulator.resize(chan+1);
      }
      
      if (chan >= m_Futures.count()) {
	m_Futures.resize(chan+1);
      }

      m_Data[chan].resize(m_AcquisitionParms->nSamples());
      m_Accumulator[chan].resize(m_AcquisitionParms->nSamples());

      readParams.dataType	    = ReadReal64;
      readParams.readMode	    = ReadModeAvgW; // ReadModeStdW;
      readParams.nbrSegments	    = 1;
      readParams.firstSampleInSeg   = 0;
      readParams.firstSegment	    = 0;
      readParams.segmentOffset	    = 0;
      readParams.segDescArraySize   = (long)sizeof(AqSegmentDescriptorAvg) * 1;
      readParams.nbrSamplesInSeg    = m_AcquisitionParms->nSamples();
      readParams.dataArraySize	    = sizeof(double)*(m_AcquisitionParms->nSamples());
      readParams.flags              = 0;
      readParams.reserved           = 0;
      readParams.reserved2          = 0.0;
      readParams.reserved3          = 0.0;
      
      checkStatus(AcqrsD1_readData(m_InstrumentID[i], ch+1, &readParams, m_Data[chan].data(),
				   &wfDesc, &segDesc));

      printf("rep = %d, nrepetitions = %d\n", rep, nrepetitions);

      if (rep == 0) {
     	for (int i=0; i<nsamples; i++) {
	  m_Accumulator[chan][i] = 0;
	}
      }

      if (rep >= (nrepetitions-1)) {
     	for (int i=0; i<nsamples; i++) {
	  m_Accumulator[chan][i] += m_Data[chan][i];
	  m_Data[chan][i] = (m_Accumulator[chan][i])/nrepetitions;
	}

	printf("m_ReferenceAvailable = %d\n", m_ReferenceAvailable);

	if (m_ReferenceAvailable) {
	  if (concurrent() & 1) {
	    m_Futures[chan] = QtConcurrent::run(fitter(chan), &QavrgFitter::performCalculation);
	  } else {
	    fitter(chan) -> performCalculation();
	  }
	}
      } else {
     	for (int i=0; i<nsamples; i++) {
	  m_Accumulator[chan][i] += m_Data[chan][i];
	}
      }

      printf("Readout took %d msec\n", timer.restart());
    }
  }

  if (rep >= (nrepetitions-1)) {
    if (concurrent() & 1) {
      if (m_ReferenceAvailable) {
	QFuture<void> f;

	foreach(f, m_Futures) {
	  f.waitForFinished();
	}
      }
    }
    

    emit newDataAvailable();

    printf("Readout complete...\n");
  }
}

void QavrgAcquisition::parametersChanged()
{
  m_ParametersChanged = true;
}

QString QavrgAcquisition::parametersPath(int chan)
{
  return QavrgSettings::settingsDirectory().filePath(tr("parms%1").arg(chan));
}

void QavrgAcquisition::loadParameters()
{
  QMutexLocker writelock(&m_Mutex);

  for (int i=0; i<m_TotalChannels; i++) {
    printf("loadParameters from %s\n", qPrintable(parametersPath(i)));

    QFile file(parametersPath(i));

    if (!file.open(QIODevice::ReadOnly)) {
      printf("Couldn't load parameters for channel %d\n", i);
      return;
    }
  
    double nparms, inputOffset, startOffset, endOffset, edgeOffset, samplesPerOrbit, bunchesPerOrbit;

    file.read((char*) &nparms, sizeof(double));

    file.read((char*) &inputOffset, sizeof(double));
    file.read((char*) &startOffset, sizeof(double));
    file.read((char*) &endOffset, sizeof(double));
    file.read((char*) &edgeOffset, sizeof(double));
    file.read((char*) &samplesPerOrbit, sizeof(double));
    file.read((char*) &bunchesPerOrbit, sizeof(double));

    fitter(i)->setinputoffset(inputOffset);
    fitter(i)->setstartoffset(startOffset);
    fitter(i)->setendoffset(endOffset);
    fitter(i)->setedgeoffset(edgeOffset);
    fitter(i)->setsamplesperorbit(samplesPerOrbit);
    fitter(i)->setbunchesperorbit(bunchesPerOrbit);
  }

  m_ParametersChanged = false;
}

void QavrgAcquisition::saveParameters()
{
  QMutexLocker writelock(&m_Mutex);

  for (int i=0; i<m_TotalChannels; i++) {
    printf("saveParameters to %s\n", qPrintable(parametersPath(i)));

    QFile file(parametersPath(i));

    if (!file.open(QIODevice::WriteOnly)) {
      printf("Couldn't save parameters for channel %d\n", i);
      return;
    }

    double nparms = 6;
    double inputOffset = fitter(i)->inputoffset();
    double startOffset = fitter(i)->startoffset();
    double endOffset = fitter(i)->endoffset();
    double edgeOffset = fitter(i)->edgeoffset();
    double samplesPerOrbit = fitter(i)->samplesperorbit();
    double bunchesPerOrbit = fitter(i)->bunchesperorbit();

    file.write((char*) &nparms, sizeof(double));
    file.write((char*) &inputOffset, sizeof(double));
    file.write((char*) &startOffset, sizeof(double));
    file.write((char*) &endOffset, sizeof(double));
    file.write((char*) &edgeOffset, sizeof(double));
    file.write((char*) &samplesPerOrbit, sizeof(double));
    file.write((char*) &bunchesPerOrbit, sizeof(double));
  }

  m_ParametersChanged = false;
}

QString QavrgAcquisition::referenceDataPath(int chan)
{
  return QavrgSettings::settingsDirectory().filePath(tr("ref%1").arg(chan));
}

void QavrgAcquisition::loadReferenceData()
{
  QMutexLocker writelock(&m_Mutex);

  m_ReferenceData.resize(m_TotalChannels);

  for (int i = 0; i<m_TotalChannels; i++) {
    printf("loadReferenceData from %s\n", qPrintable(referenceDataPath(i)));

    QFile file(referenceDataPath(i));

    if (!file.open(QIODevice::ReadOnly)) {
      printf("Couldn't load reference data for channel %d\n", i);
      return;
    }
    
    int sz = file.size();

    int nsamp = sz / sizeof(double);
    int nread = nsamp*sizeof(double);

//   printf("File size %d, nsamp %d, nread %d\n", sz, nsamp, nread);

    m_ReferenceData[i].resize(nsamp);
    m_ReferenceData[i].fill(0);

//   printf("File size %d, nsamp %d, nread %d\n", sz, nsamp, nread);

    int szrd = file.read((char*) m_ReferenceData[i].data(), nread);

//   printf("File size %d, nsamp %d, nread %d\n", sz, nsamp, nread);
  
    if (szrd != nread) {
      printf("Incomplete read, read %d of %d.\n", szrd, nread);
    }
  }

  referenceloaded(true);

  m_ReferenceChanged = false;
}

void QavrgAcquisition::saveReferenceData()
{
  QMutexLocker lock(&m_Mutex);

  for (int i = 0; i<m_TotalChannels; i++) {
    printf("saveReferenceData to %s\n", qPrintable(referenceDataPath(i)));

    QFile file(referenceDataPath(i));

    if (!file.open(QIODevice::WriteOnly)) {
      printf("Couldn't save reference data for channel %d\n", i);
      return;
    }

//   printf("File size = %ld\n", file.size());

    qint64 sz = m_ReferenceData[i].size()*sizeof(double);

//   printf("Reference data size = %ld elems\n", m_ReferenceData.size());

    qint64 szwrt = file.write((char*) m_ReferenceData[i].data(), sz);

    //   printf("New file size = %ld\n", file.size());
    
    if (szwrt != sz) {
      printf("Incomplete write, wrote %ld of %ld.\n", szwrt, sz);
    }
  }

  m_ReferenceChanged = false;
}

QString QavrgAcquisition::darkDataPath(int chan)
{
  return QavrgSettings::settingsDirectory().filePath(tr("dark%1").arg(chan));
}

void QavrgAcquisition::loadDarkData()
{
  QMutexLocker writelock(&m_Mutex);

  m_DarkData.resize(m_TotalChannels);

  for (int i = 0; i<m_TotalChannels; i++) {
    printf("loadDarkData from %s\n", qPrintable(darkDataPath(i)));

    QFile file(darkDataPath(i));

    if (!file.open(QIODevice::ReadOnly)) {
      printf("Couldn't load dark data for channel %d\n", i);
      return;
    }

    int sz = file.size();

    int nsamp = sz / sizeof(double);
    int nread = nsamp*sizeof(double);

    //   printf("File size %d, nsamp %d, nread %d\n", sz, nsamp, nread);

    m_DarkData[i].resize(nsamp);
    m_DarkData[i].fill(0);
  
    //   printf("File size %d, nsamp %d, nread %d\n", sz, nsamp, nread);
    
    int szrd = file.read((char*) m_DarkData[i].data(), nread);
  
    //   printf("File size %d, nsamp %d, nread %d\n", sz, nsamp, nread);

    if (szrd != nread) {
      printf("Incomplete read, read %ld of %ld.\n", szrd, nread);
    }
  }

  darkloaded(true);

  m_DarkChanged = false;
}

void QavrgAcquisition::saveDarkData()
{
  QMutexLocker lock(&m_Mutex);

  for (int i = 0; i<m_TotalChannels; i++) {
    printf("saveDarkData to %s\n", qPrintable(darkDataPath(i)));

    QFile file(darkDataPath(i));

    if (!file.open(QIODevice::WriteOnly)) {
      printf("Couldn't save reference data for channel %d\n", i);
      return;
    }

    //   printf("File size = %ld\n", file.size());

    qint64 sz = m_DarkData[i].size()*sizeof(double);

    //   printf("Dark data size = %ld elems\n", m_DarkData.size());

    qint64 szwrt = file.write((char*) m_DarkData[i].data(), sz);

//   printf("New file size = %ld\n", file.size());

    if (szwrt != sz) {
      printf("Incomplete write, wrote %ld of %ld.\n", szwrt, sz);
    }
  }

  m_DarkChanged = false;
}
