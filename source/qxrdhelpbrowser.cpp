#include "qxrdhelpbrowser.h"
#include "qxrdscriptengine.h"
#include "qxrdexperiment.h"
#include <stdio.h>
#include "qxrddebug.h"

QxrdHelpBrowser::QxrdHelpBrowser(QWidget *parent) :
  QTextBrowser(parent)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdHelpBrowser::QxrdHelpBrowser(%p)\n", this);
  }
}

QxrdHelpBrowser::~QxrdHelpBrowser()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdHelpBrowser::~QxrdHelpBrowser(%p)\n", this);
  }
}

void QxrdHelpBrowser::init(QxrdExperimentWPtr expt)
{
  m_Experiment = expt;
}

QVariant QxrdHelpBrowser::loadResource(int type, const QUrl &name)
{
//  printf("QxrdHelpBrowser::loadResource(%d,\"%s\",\"%s\",\"%s\")\n",
//         type, qPrintable(name.scheme()), qPrintable(name.path()), name.encodedQuery().data());

  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    QxrdScriptEnginePtr engine(expt->scriptEngine());

    if (engine) {
      if (type == QTextDocument::HtmlResource) {
        if (name.scheme() == "qrc") {
          if (name.path() == "/help/autohelp") {

            QByteArray res = engine->helpText(name.encodedQuery());

//            printf("QxrdHelpBrowser::loadResource res.length = %d\n", res.count());

            return res;
          }
        }
      }
    }
  }

  QVariant res = QTextBrowser::loadResource(type, name);

//  printf("QxrdHelpBrowser::loadResource res.typeName = %s\n", res.typeName());

  return res;
}
