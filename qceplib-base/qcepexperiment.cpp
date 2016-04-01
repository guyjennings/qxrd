#include "qcepexperiment.h"
#include "qcepsettingssaver.h"
#include <QFileInfo>
#include <QDir>
#include "qcepdataexportparameters.h"
#include "qcepdataimportparameters.h"

QcepExperiment::QcepExperiment(QString path, QString name, QcepObject *parent) :
  QcepObject(name, parent),
  m_SettingsSaver(new QcepSettingsSaver(this)),
  m_ExperimentKind(m_SettingsSaver, this, "experimentKind", -1, "Kind of Experiment"),
  m_ExperimentDirectory(QcepSettingsSaverPtr(), this, "experimentDirectory", defaultExperimentDirectory(path), "Experiment Directory"),
  m_ExperimentFileName(QcepSettingsSaverPtr(), this, "experimentFileName", defaultExperimentFileName(path), "Experiment File"),
  m_ExperimentName(QcepSettingsSaverPtr(), this, "experimentName", defaultExperimentName(path), "Experiment Name"),
  m_ExperimentDescription(m_SettingsSaver, this, "experimentDescription", "", "Experiment Description"),
  m_NewGroupName(m_SettingsSaver, this, "newGroupName", "group", "Default name of newly created data group"),
  m_NewScanName(m_SettingsSaver, this, "newScanName", "scan", "Default name of newly created scans"),
  m_NewScanColumns(m_SettingsSaver, this, "newScanColumns", QStringList(), "Column names for new scan"),
  m_NewScanSize(m_SettingsSaver, this, "newScanSize", 0, "Number of points in new scan"),
  m_NewArrayName(m_SettingsSaver, this, "newArrayName", "array", "Default name of newly created array"),
  m_NewArrayNumDimensions(m_SettingsSaver, this, "newArrayNumDimensions", 2, "Dimensionality of newly created array"),
  m_NewArrayXDimension(m_SettingsSaver, this, "newArrayXDimension", 2048, "X Dimension of new array"),
  m_NewArrayYDimension(m_SettingsSaver, this, "newArrayYDimension", 2048, "Y Dimension of new array"),
  m_NewArrayZDimension(m_SettingsSaver, this, "newArrayZDimension", 1, "Z Dimension of new array"),
  m_NewArrayTDimension(m_SettingsSaver, this, "newArrayTDimension", 1, "T Dimension of new array"),
  m_NewColumnName(m_SettingsSaver, this, "newColumnName", "colummn", "Default name of newly created column"),
  m_NewColumnSize(m_SettingsSaver, this, "newColumnSize", 1024, "Size of newly created column"),
  m_NewImageName(m_SettingsSaver, this, "newImageName", "image", "Name of newly created image"),
  m_NewImageWidth(m_SettingsSaver, this, "newImageWidth", 2048, "Width of new image"),
  m_NewImageHeight(m_SettingsSaver, this, "newImageHeight", 2048, "Height of new image")
{
}

void QcepExperiment::initialize(QSettings *settings)
{
  m_DataExportParameters = QcepDataExportParametersPtr(
        new QcepDataExportParameters(m_SettingsSaver, "exportParameters"));

  m_DataImportParameters = QcepDataImportParametersPtr(
        new QcepDataImportParameters(m_SettingsSaver, "importParameters"));
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

QcepDataExportParametersPtr QcepExperiment::dataExportParameters()
{
  return m_DataExportParameters;
}

QcepDataImportParametersPtr QcepExperiment::dataImportParameters()
{
  return m_DataImportParameters;
}

void QcepExperiment::readSettings(QSettings *settings, QString section)
{
  if (settings) {
    QcepObject::readSettings(settings, section);

    if (m_DataExportParameters) {
      m_DataExportParameters -> readSettings(settings, "exportParameters");
    }

    if (m_DataImportParameters) {
      m_DataImportParameters -> readSettings(settings, "importParameters");
    }
  }
}

void QcepExperiment::writeSettings(QSettings *settings, QString section)
{
  if (settings) {
    QcepObject::writeSettings(settings, section);

    if (m_DataExportParameters) {
      m_DataExportParameters -> writeSettings(settings, "exportParameters");
    }

    if (m_DataImportParameters) {
      m_DataImportParameters -> writeSettings(settings, "importParameters");
    }
  }
}
