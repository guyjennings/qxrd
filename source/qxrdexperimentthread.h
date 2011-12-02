#ifndef QXRDDOCUMENTTHREAD_H
#define QXRDDOCUMENTTHREAD_H

#include <QThread>
#include <QSharedPointer>
#include "qxrdthread.h"
#include "qxrddocument.h"

class QxrdDocumentThread;
typedef QSharedPointer<QxrdDocumentThread> QxrdDocumentThreadPtr;

class QxrdDocumentThread : public QxrdThread
{
  Q_OBJECT
public:
  typedef enum {
    PerkinElmer,
    Pilatus,
    Simulated,
    PerkinElmerAnalysis,
    PilatusAnalysis,
    GenericAnalysis
  } QxrdDocumentKind;

  QxrdDocumentThread(QxrdDocumentKind kind, QString path, QxrdApplication *app);

  static QxrdDocumentThreadPtr newPerkinElmerDocument(QString path, QxrdApplication *app);
  static QxrdDocumentThreadPtr newPilatusDocument(QString path, QxrdApplication *app);
  static QxrdDocumentThreadPtr newSimulatedDocument(QString path, QxrdApplication *app);
  static QxrdDocumentThreadPtr newPerkinElmerAnalysisDocument(QString path, QxrdApplication *app);
  static QxrdDocumentThreadPtr newPilatusAnalysisDocument(QString path, QxrdApplication *app);
  static QxrdDocumentThreadPtr newGenericAnalysisDocument(QString path, QxrdApplication *app);

  QxrdDocumentPtr document();

  void shutdown();

signals:

public slots:

protected:
  void run();

private:
  QxrdDocumentKind m_DocumentKind;
  QString          m_DocumentPath;
  QxrdDocumentPtr  m_Document;
  QxrdApplication *m_Application;
};

#endif // QXRDDOCUMENTTHREAD_H
