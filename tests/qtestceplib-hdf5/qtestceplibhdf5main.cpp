#include "qtestceplibhdf5mainwindow.h"
#include "qtestceplibhdf5document.h"
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

  QtestceplibHdf5Document *doc = new QtestceplibHdf5Document();

  w = new QtestceplibHdf5MainWindow(doc);
  w -> show();

  return a.exec();
}

