#include "qcepexperiment.h"
#include <QFileInfo>
#include <QDir>
#include "qcepdataexportparameters.h"
#include "qcepdataimportparameters.h"
#include "qcepsetdatavaluerangecommand.h"
#include "qcepsetrangeofimagecommand.h"
#include "qcepfixupgainmapcommand.h"
#include "qcepmainwindowsettings.h"

QcepExperiment::QcepExperiment(QString path, QString name) :
  QcepObject(name),
//  m_SettingsSaver(new QcepSettingsSaver(this)),
  m_ExperimentKind        (this, "experimentKind", -1, "Kind of Experiment"),
  m_ExperimentDirectory   (this, "experimentDirectory", defaultExperimentDirectory(path), "Experiment Directory"),
  m_ExperimentFileName    (this, "experimentFileName", defaultExperimentFileName(path), "Experiment File"),
  m_ExperimentName        (this, "experimentName", defaultExperimentName(path), "Experiment Name"),
  m_ExperimentDescription (this, "experimentDescription", "", "Experiment Description"),
  m_NewGroupName          (this, "newGroupName", "group", "Default name of newly created data group"),
  m_NewScanName           (this, "newScanName", "scan", "Default name of newly created scans"),
  m_NewScanColumns        (this, "newScanColumns", QStringList(), "Column names for new scan"),
  m_NewScanSize           (this, "newScanSize", 0, "Number of points in new scan"),
  m_NewArrayName          (this, "newArrayName", "array", "Default name of newly created array"),
  m_NewArrayNumDimensions (this, "newArrayNumDimensions", 2, "Dimensionality of newly created array"),
  m_NewArrayXDimension    (this, "newArrayXDimension", 2048, "X Dimension of new array"),
  m_NewArrayYDimension    (this, "newArrayYDimension", 2048, "Y Dimension of new array"),
  m_NewArrayZDimension    (this, "newArrayZDimension", 1, "Z Dimension of new array"),
  m_NewArrayTDimension    (this, "newArrayTDimension", 1, "T Dimension of new array"),
  m_NewColumnName         (this, "newColumnName", "column", "Default name of newly created column"),
  m_NewColumnSize         (this, "newColumnSize", 1024, "Size of newly created column"),
  m_NewImageName          (this, "newImageName", "image", "Name of newly created image"),
  m_NewImageWidth         (this, "newImageWidth", 2048, "Width of new image"),
  m_NewImageHeight        (this, "newImageHeight", 2048, "Height of new image")
{
  m_DataExportParameters = QcepDataExportParametersPtr(
        new QcepDataExportParameters("exportParameters"));

  m_DataImportParameters = QcepDataImportParametersPtr(
        new QcepDataImportParameters("importParameters"));

  m_SetDataValueRangeCommand = QcepSetDataValueRangeCommandPtr(
        new QcepSetDataValueRangeCommand());

  m_SetRangeOfImageCommand = QcepSetRangeOfImageCommandPtr(
        new QcepSetRangeOfImageCommand());

  m_FixupGainMapCommand = QcepFixupGainMapCommandPtr(
        new QcepFixupGainMapCommand());
}

void QcepExperiment::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  m_DataExportParameters -> initialize(sharedFromThis());

  m_DataImportParameters -> initialize(sharedFromThis());

  m_SetDataValueRangeCommand -> initialize(sharedFromThis());

  m_SetRangeOfImageCommand -> initialize(sharedFromThis());

  m_FixupGainMapCommand -> initialize(sharedFromThis());

  defaultWindowSettings();

  int nOpened = 0;

  for (int i=0; i<windowSettingsCount(); i++) {
    QcepMainWindowSettingsPtr set = windowSettings(i);

    if (set) {
      set -> initialize(sharedFromThis());

      if (set -> get_WindowOpen()) {
        nOpened += 1;
      }
    }
  }

  if (nOpened == 0) {
    // If no windows are open, open the first...

    QcepMainWindowSettingsPtr set = windowSettings(0);

    if (set) {
      set -> set_WindowOpen(true);
    }
  }
}

QcepExperimentWPtr QcepExperiment::findExperiment(QcepObjectWPtr p)
{
  QcepExperimentWPtr res =
      qSharedPointerDynamicCast<QcepExperiment>(p);

  if (res == NULL) {
    QcepObjectPtr objp(p);

    if (objp) {
      res = findExperiment(objp->parentPtr());
    }
  }

  return res;
}

QString QcepExperiment::defaultExperimentDirectory(QString path) const
{
  QFileInfo info(path);

  QString directory = info.dir().canonicalPath();

  return directory;
}

QString QcepExperiment::defaultExperimentFileName(QString path) const
{
  QFileInfo info(path);
  QString baseName = info.completeBaseName();
//  QString suffix   = info.suffix();

  if (baseName.length() == 0) {
    baseName = "Untitled";
  }
//  if (suffix == "qxrdp") {
//    return info.fileName();
//  } else {
//    return info.fileName()+".qxrdp";
//  }

  return baseName + ".qxrdp";
}

QString QcepExperiment::defaultExperimentName(QString path) const
{
  QFileInfo info(path);
  QString baseName = info.completeBaseName();
//  QString suffix   = info.suffix();

  if (baseName.length() == 0) {
    baseName = "Untitled";
  }

//  if (info.suffix() == "qxrdp") {
//    return info.completeBaseName();
//  } else {
//    return info.fileName();
//  }

  return baseName;
}

QUndoStack *QcepExperiment::undoStack()
{
  return &m_UndoStack;
}

QcepDataExportParametersWPtr QcepExperiment::dataExportParameters()
{
  return m_DataExportParameters;
}

QcepDataImportParametersWPtr QcepExperiment::dataImportParameters()
{
  return m_DataImportParameters;
}

QcepSetDataValueRangeCommandWPtr QcepExperiment::setDataValueRangeCommand()
{
  return m_SetDataValueRangeCommand;
}

QcepSetRangeOfImageCommandWPtr QcepExperiment::setRangeOfImageCommand()
{
  return m_SetRangeOfImageCommand;
}

QcepFixupGainMapCommandWPtr QcepExperiment::fixupGainMapCommand()
{
  return m_FixupGainMapCommand;
}

void QcepExperiment::readSettings(QSettings *settings)
{
  if (settings) {
    QcepObject::readSettings(settings);

    if (m_DataExportParameters) {
      settings->beginGroup("exportParameters");
      m_DataExportParameters -> readSettings(settings);
      settings->endGroup();
    }

    if (m_DataImportParameters) {
      settings->beginGroup("importParameters");
      m_DataImportParameters -> readSettings(settings);
      settings->endGroup();
    }

    if (m_SetDataValueRangeCommand) {
      settings->beginGroup("setDataValueRange");
      m_SetDataValueRangeCommand -> readSettings(settings);
      settings->endGroup();
    }

    if (m_SetRangeOfImageCommand) {
      settings->beginGroup("setRangeOfImage");
      m_SetRangeOfImageCommand -> readSettings(settings);
      settings->endGroup();
    }

    if (m_FixupGainMapCommand) {
      settings->beginGroup("fixupGainMap");
      m_FixupGainMapCommand -> readSettings(settings);
      settings->endGroup();
    }
  }
}

void QcepExperiment::writeSettings(QSettings *settings)
{
  if (settings) {
    QcepObject::writeSettings(settings);

    if (m_DataExportParameters) {
      settings->beginGroup("exportParameters");
      m_DataExportParameters -> writeSettings(settings);
      settings->endGroup();
    }

    if (m_DataImportParameters) {
      settings->beginGroup("importParameters");
      m_DataImportParameters -> writeSettings(settings);
      settings->endGroup();
    }

    if (m_SetDataValueRangeCommand) {
      settings->beginGroup("setDataValueRange");
      m_SetDataValueRangeCommand -> writeSettings(settings);
      settings->endGroup();
    }

    if (m_SetRangeOfImageCommand) {
      settings->beginGroup("setRangeOfImage");
      m_SetRangeOfImageCommand -> writeSettings(settings);
      settings->endGroup();
    }

    if (m_FixupGainMapCommand) {
      settings->beginGroup("fixupGainMap");
      m_FixupGainMapCommand -> writeSettings(settings);
      settings->endGroup();
    }
  }
}
