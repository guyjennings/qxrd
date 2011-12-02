#include "qxrddocumentthread.h"
#include "qxrdapplication.h"
#include "qxrdperkinelmerdocument.h"
#include "qxrdperkinelmeranalysisdocument.h"
#include "qxrdpilatusdocument.h"
#include "qxrdpilatusanalysisdocument.h"
#include "qxrdsimulateddocument.h"
#include "qxrdgenericanalysisdocument.h"
#include "qxrdsplashscreen.h"

QxrdDocumentThread::QxrdDocumentThread(QxrdDocumentKind kind, QString path, QxrdApplication *app) :
  QxrdThread(app),
  m_DocumentKind(kind),
  m_DocumentPath(path),
  m_Document(NULL),
  m_Application(app)
{
}

QxrdDocumentThreadPtr QxrdDocumentThread::newPerkinElmerDocument(QString path, QxrdApplication *app)
{
  QxrdDocumentThreadPtr res = QxrdDocumentThreadPtr(new QxrdDocumentThread(PerkinElmer, path, app));

  res->start();

  res->document()->init();

  return res;
}

QxrdDocumentThreadPtr QxrdDocumentThread::newPilatusDocument(QString path, QxrdApplication *app)
{
  QxrdDocumentThreadPtr res = QxrdDocumentThreadPtr(new QxrdDocumentThread(Pilatus, path, app));

  res->start();

  res->document()->init();

  return res;
}

QxrdDocumentThreadPtr QxrdDocumentThread::newSimulatedDocument(QString path, QxrdApplication *app)
{
  QxrdDocumentThreadPtr res = QxrdDocumentThreadPtr(new QxrdDocumentThread(Simulated, path, app));

  res->start();

  res->document()->init();

  return res;
}

QxrdDocumentThreadPtr QxrdDocumentThread::newPerkinElmerAnalysisDocument(QString path, QxrdApplication *app)
{
  QxrdDocumentThreadPtr res = QxrdDocumentThreadPtr(new QxrdDocumentThread(PerkinElmerAnalysis, path, app));

  res->start();

  res->document()->init();

  return res;
}

QxrdDocumentThreadPtr QxrdDocumentThread::newPilatusAnalysisDocument(QString path, QxrdApplication *app)
{
  QxrdDocumentThreadPtr res = QxrdDocumentThreadPtr(new QxrdDocumentThread(PilatusAnalysis, path, app));

  res->start();

  res->document()->init();

  return res;
}

QxrdDocumentThreadPtr QxrdDocumentThread::newGenericAnalysisDocument(QString path, QxrdApplication *app)
{
  QxrdDocumentThreadPtr res = QxrdDocumentThreadPtr(new QxrdDocumentThread(GenericAnalysis, path, app));

  res->start();

  res->document()->init();

  return res;
}

QxrdDocumentPtr QxrdDocumentThread::document()
{
  while (m_Document == NULL) {
    msleep(100);
  }

  return m_Document;
}

void QxrdDocumentThread::run()
{
  QxrdSplashScreen splash;
  QxrdDocumentPtr doc;

  switch(m_DocumentKind) {
  case PerkinElmer:
    doc = QxrdDocumentPtr(new QxrdPerkinElmerDocument(m_DocumentPath, m_Application));
    break;

  case Pilatus:
    doc = QxrdDocumentPtr(new QxrdPilatusDocument(m_DocumentPath, m_Application));
    break;

  case Simulated:
    doc = QxrdDocumentPtr(new QxrdSimulatedDocument(m_DocumentPath, m_Application));
    break;

  case PerkinElmerAnalysis:
    doc = QxrdDocumentPtr(new QxrdPerkinElmerAnalysisDocument(m_DocumentPath, m_Application));
    break;

  case PilatusAnalysis:
    doc = QxrdDocumentPtr(new QxrdPilatusAnalysisDocument(m_DocumentPath, m_Application));
    break;

  case GenericAnalysis:
    doc = QxrdDocumentPtr(new QxrdGenericAnalysisDocument(m_DocumentPath, m_Application));
    break;
  }

  m_Document = doc;
}

void QxrdDocumentThread::shutdown()
{
}
