#ifndef QXRDFILEBROWSER_H
#define QXRDFILEBROWSER_H

#include "qcepmacros.h"

#include <QDockWidget>
#include "qxrdforwardtypes.h"
#include "qcepproperty.h"
#include "qxrdsettings.h"
#include "qxrddataprocessor.h"
#include "ui_qxrdfilebrowser.h"

class QxrdFileBrowser : public QDockWidget, public Ui::QxrdFileBrowser
{
  Q_OBJECT;

public:
  QxrdFileBrowser(QxrdDataProcessorPtr processor, QWidget *parent=0);

public:
  Q_PROPERTY(int browserFilter READ get_BrowserFilter WRITE set_BrowserFilter);
  QCEP_INTEGER_PROPERTY(BrowserFilter);

  Q_PROPERTY(QString browserSelector READ get_BrowserSelector WRITE set_BrowserSelector);
  QCEP_STRING_PROPERTY(BrowserSelector);

public:
  void readSettings(QxrdSettings &settings, QString section);
  void writeSettings(QxrdSettings &settings, QString section);

public slots:
  void onFilterChanged(int newfilter);
  void onSelectorChanged(QString str, QModelIndex = QModelIndex());
  void doOpen();
  void doProcess();
  void doIntegrate();
  void doAccumulate();
  void onRootDirectoryChanged(QString dir);
  void mousePressed(QModelIndex index);
  void onRowCountChanged(int oldCount, int newCount);

signals:
  void printMessage(QDateTime ts, QString msg) const;
  void statusMessage(QDateTime ts, QString msg) const;
  void criticalMessage(QDateTime ts, QString msg) const;

private:
  mutable QMutex       m_Mutex;
  QxrdDataProcessorPtr m_Processor;
  QFileSystemModelPtr  m_Model;
};

#endif // QXRDFILEBROWSER_H
