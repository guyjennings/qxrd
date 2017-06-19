#include "qtestcepliblevmarmainwindow.h"
#include "qtestcepliblevmardocument.h"
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

  QcepProperty::registerMetaTypes();
  QcepVector3DProperty::registerMetaTypes();
  QcepMatrix3x3Property::registerMetaTypes();

  QtestceplibLevMarDocument *doc = new QtestceplibLevMarDocument();

  w = new QtestceplibLevMarMainWindow(doc);
  w -> show();

  return a.exec();
}

