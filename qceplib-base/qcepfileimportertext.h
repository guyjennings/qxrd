#ifndef QCEPFILEIMPORTERTEXT_H
#define QCEPFILEIMPORTERTEXT_H

#include "qcepfileimporter.h"
#include "qcepdatacolumnscan-ptr.h"
#include <QFile>

class QcepFileImporterText : public QcepFileImporter
{
  Q_OBJECT

public:
  QcepFileImporterText(QcepDatasetModelPtr model,
                       QModelIndexList &indexes,
                       QString path);

  void exec();

private:
  void processLine(QString aLine);
  void processFile(QString aLine);
  void processEpoch(QString aLine);
  void processScan(QString aLine);
  void processDate(QString aLine);
  void processColumnCount(QString aLine);
  void processColumnNames(QString aLine);
  void processCountTime(QString aLine);
  void processComment(QString aLine);
  void processHeaderLine(QString aLine);
  void processDataLine(QString aLine);

  QFile                 m_InputFile;
  QcepDataColumnScanPtr m_LatestScan;
  QRegExp               m_ScanLine;
  QRegExp               m_ColumnCountLine;
  QRegExp               m_ColumnNamesLine;
  QRegExp               m_DataLine;
  int                   m_ColumnCount;
  int                   m_RowCount;
};

#endif // QCEPFILEIMPORTERTEXT_H
