#include "qtestceplibmainwindow.h"
#include "qtestceplibdocument.h"
#include <QApplication>
#include "qcepproperty.h"
#include "qcepvector3dproperty.h"
#include "qcepmatrix3x3property.h"
#include "qcepdebug.h"

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);
  QMainWindow *w=NULL;

  g_DebugLevel = QSharedPointer<QcepDebugDictionary>(new QcepDebugDictionary());

  g_DebugLevel->setDebugLevel(DEBUG_PROPERTIES);

  QcepProperty::registerMetaTypes();
  QcepVector3DProperty::registerMetaTypes();
  QcepMatrix3x3Property::registerMetaTypes();

  QtestceplibDocument *doc = new QtestceplibDocument();

  w = new QtestceplibMainWindow(doc);

  w -> show();

  return a.exec();
}

