#include "qtestcepliblevmarmainwindow.h"
#include "ui_qtestcepliblevmarmainwindow.h"
#include <QFileDialog>
#include "qcepmutexlocker.h"
#include "qcepimagedata.h"

QtestceplibLevMarMainWindow::QtestceplibLevMarMainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::QtestceplibLevMarMainWindow),
  m_Mutex(QMutex::Recursive),
  m_IntProp(QcepSettingsSaverWPtr(), this, "intProp", 42, "Integer Property"),
  m_DblProp(QcepSettingsSaverWPtr(), this, "dblProp", 42.0, "Double Property"),
  m_StrProp(QcepSettingsSaverWPtr(), this, "strProp", "42", "String Property"),
  m_SListProp(QcepSettingsSaverWPtr(), this, "sListProp", QcepStringList(), "String List Property"),
  m_Vec3dPropA(QcepSettingsSaverWPtr(), this, "vec3dPropA", QcepVector3D(1,2,3), "Vector 3D Property A"),
  m_Vec3dPropB(QcepSettingsSaverWPtr(), this, "vec3dPropB", 1,2,3, "Vector 3D Property B"),
  m_Mat3x3PropA(QcepSettingsSaverWPtr(), this, "mat3x3PropA", QcepMatrix3x3(), "Matrix 3x3 Property A"),
  m_Mat3x3PropB(QcepSettingsSaverWPtr(), this, "mat3x3PropB",
                1, 0, 0,
                0, 1, 0,
                0, 0, 1,
                "Matrix 3x3 Property B")
{
  ui->setupUi(this);

  connect(ui->m_ActionReadSettings, SIGNAL(triggered()), this, SLOT(doReadSettings()));
  connect(ui->m_ActionWriteSettings, SIGNAL(triggered()), this, SLOT(doWriteSettings()));
  connect(ui->m_ActionLoadImage, SIGNAL(triggered()), this, SLOT(doLoadImage()));
  connect(ui->m_ActionTestLevMar, SIGNAL(triggered()), this, SLOT(doTestLevMar()));

  ui->m_FileMenu->addAction(tr("QCEPLIB Version %1").arg(STR(QCEPLIB_VERSION)));
  ui->m_FileMenu->addAction(tr("LevMar Version %1").arg(STR(QCEPLIB_LEVMAR_VERSION)));
}

QtestceplibLevMarMainWindow::~QtestceplibLevMarMainWindow()
{
  delete ui;
}

void QtestceplibLevMarMainWindow::printMessage(QString msg)
{
  ui->m_Messages->appendPlainText(msg);
}

QString defPath;

void QtestceplibLevMarMainWindow::doReadSettings()
{
  QString theFile = QFileDialog::getOpenFileName(
        this, "Read Settings from...", defPath);

  if (theFile.length()) {
    readSettings(theFile);
    defPath = theFile;
  }
}

void QtestceplibLevMarMainWindow::doWriteSettings()
{
  QString theFile = QFileDialog::getSaveFileName(
        this, "Write Settings in...", defPath);

  if (theFile.length()) {
    writeSettings(theFile);
    defPath = theFile;
  }
}

void QtestceplibLevMarMainWindow::readSettings()
{
}

void QtestceplibLevMarMainWindow::writeSettings()
{
}

void QtestceplibLevMarMainWindow::readSettings(QString filePath)
{
  QSettings settings(filePath, QSettings::IniFormat);

  readSettings(&settings);
}

void QtestceplibLevMarMainWindow::writeSettings(QString filePath)
{
  QSettings settings(filePath, QSettings::IniFormat);

  writeSettings(&settings);
}

void QtestceplibLevMarMainWindow::readSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::readSettings(this, "qtestceplib", settings);
}

void QtestceplibLevMarMainWindow::writeSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::writeSettings(this, "qtestceplib", settings);
}

void QtestceplibLevMarMainWindow::doLoadImage()
{
  QString theFile = QFileDialog::getOpenFileName(
        this, "Read Image from...", defPath);

  if (theFile.length()) {
    QcepImageData<double> *img = new QcepImageData<double>(QcepSettingsSaverWPtr(), 1024,1024);

    if (img->readImage(theFile)) {
      img->loadMetaData();

      printMessage(tr("Loaded image from %1").arg(theFile));
      printMessage(tr(" width %1, height %2").arg(img->get_Width()).arg(img->get_Height()));
    } else {
      printMessage(tr("Image load failed from %1").arg(theFile));
    }
  }
}

void QtestceplibLevMarMainWindow::doTestLevMar()
{
}

