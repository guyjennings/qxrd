#ifndef QCEPEXPERIMENT_H
#define QCEPEXPERIMENT_H

#include "qceplib_global.h"
#include "qcepserializableobject.h"
#include <QDateTime>
#include <QUndoStack>
#include "qcepproperty.h"
#include "qcepdataobject-ptr.h"
#include "qcepimagedata-ptr.h"
#include "qcepdataexportparameters-ptr.h"
#include "qcepdataimportparameters-ptr.h"
#include "qcepsetrangeofimagecommand-ptr.h"
#include "qcepsetdatavaluerangecommand-ptr.h"
#include "qcepfixupgainmapcommand-ptr.h"

class QCEP_EXPORT QcepExperiment : public QcepSerializableObject
{
  Q_OBJECT
public:
  explicit QcepExperiment(QString path, QString name);
  void initialize();

  virtual void addChildPtr(QcepObjectPtr child);

signals:

public slots:

  QString defaultExperimentDirectory(QString path) const;
  QString defaultExperimentFileName(QString path) const;
  QString defaultExperimentName(QString path) const;

  QcepDataExportParametersWPtr dataExportParameters();
  QcepDataImportParametersWPtr dataImportParameters();

  QcepSetDataValueRangeCommandWPtr setDataValueRangeCommand();
  QcepSetRangeOfImageCommandWPtr   setRangeOfImageCommand();
  QcepFixupGainMapCommandWPtr      fixupGainMapCommand();

  virtual void plotImage(QcepDoubleImageDataPtr img) = 0;

public:
  QUndoStack *undoStack();

protected:
//  QcepSettingsSaverPtr            m_SettingsSaver;

  QUndoStack m_UndoStack;

  virtual void readSettings(QSettings *settings);
  virtual void writeSettings(QSettings *settings);

private:
  QcepDataExportParametersPtr     m_DataExportParameters;
  QcepDataImportParametersPtr     m_DataImportParameters;
  QcepSetDataValueRangeCommandPtr m_SetDataValueRangeCommand;
  QcepSetRangeOfImageCommandPtr   m_SetRangeOfImageCommand;
  QcepFixupGainMapCommandPtr      m_FixupGainMapCommand;

public:
  Q_PROPERTY(int experimentKind READ get_ExperimentKind WRITE set_ExperimentKind)
  QCEP_INTEGER_PROPERTY(ExperimentKind)

  Q_PROPERTY(QString experimentDirectory     READ get_ExperimentDirectory WRITE set_ExperimentDirectory STORED false)
  QCEP_STRING_PROPERTY(ExperimentDirectory)

  Q_PROPERTY(QString experimentFileName     READ get_ExperimentFileName WRITE set_ExperimentFileName STORED false)
  QCEP_STRING_PROPERTY(ExperimentFileName)

  Q_PROPERTY(QString experimentName     READ get_ExperimentName WRITE set_ExperimentName STORED false)
  QCEP_STRING_PROPERTY(ExperimentName)

  Q_PROPERTY(QString experimentDescription     READ get_ExperimentDescription WRITE set_ExperimentDescription)
  QCEP_STRING_PROPERTY(ExperimentDescription)

  Q_PROPERTY(QString newGroupName READ get_NewGroupName WRITE set_NewGroupName)
  QCEP_STRING_PROPERTY(NewGroupName)

  Q_PROPERTY(QString newScanName READ get_NewScanName WRITE set_NewScanName)
  QCEP_STRING_PROPERTY(NewScanName)

  Q_PROPERTY(QStringList newScanColumns READ get_NewScanColumns WRITE set_NewScanColumns)
  QCEP_STRING_LIST_PROPERTY(NewScanColumns)

  Q_PROPERTY(int newScanSize READ get_NewScanSize WRITE set_NewScanSize)
  QCEP_INTEGER_PROPERTY(NewScanSize)

  Q_PROPERTY(QString newArrayName READ get_NewArrayName WRITE set_NewArrayName)
  QCEP_STRING_PROPERTY(NewArrayName)

  Q_PROPERTY(int newArrayNumDimensions READ get_NewArrayNumDimensions WRITE set_NewArrayNumDimensions)
  QCEP_INTEGER_PROPERTY(NewArrayNumDimensions)

  Q_PROPERTY(int newArrayXDimension READ get_NewArrayXDimension WRITE set_NewArrayXDimension)
  QCEP_INTEGER_PROPERTY(NewArrayXDimension)

  Q_PROPERTY(int newArrayYDimension READ get_NewArrayYDimension WRITE set_NewArrayYDimension)
  QCEP_INTEGER_PROPERTY(NewArrayYDimension)

  Q_PROPERTY(int newArrayZDimension READ get_NewArrayZDimension WRITE set_NewArrayZDimension)
  QCEP_INTEGER_PROPERTY(NewArrayZDimension)

  Q_PROPERTY(int newArrayTDimension READ get_NewArrayTDimension WRITE set_NewArrayTDimension)
  QCEP_INTEGER_PROPERTY(NewArrayTDimension)

  Q_PROPERTY(QString newColumnName READ get_NewColumnName WRITE set_NewColumnName)
  QCEP_STRING_PROPERTY(NewColumnName)

  Q_PROPERTY(int newColumnSize READ get_NewColumnSize WRITE set_NewColumnSize)
  QCEP_INTEGER_PROPERTY(NewColumnSize)

  Q_PROPERTY(QString newImageName READ get_NewImageName WRITE set_NewImageName)
  QCEP_STRING_PROPERTY(NewImageName)

  Q_PROPERTY(int newImageWidth READ get_NewImageWidth WRITE set_NewImageWidth)
  QCEP_INTEGER_PROPERTY(NewImageWidth)

  Q_PROPERTY(int newImageHeight READ get_NewImageHeight WRITE set_NewImageHeight)
  QCEP_INTEGER_PROPERTY(NewImageHeight)
};

#endif // QCEPEXPERIMENT_H
