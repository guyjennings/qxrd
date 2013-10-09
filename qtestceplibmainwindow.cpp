#include "qtestceplibmainwindow.h"
#include "ui_qtestceplibmainwindow.h"
#include <QFileDialog>
#include "qcepmutexlocker.h"
#include "qcepimagedataformattiff.h"
#include "qcepimagedata.h"
#include "hdf5.h"
#include "napi.h"

QtestceplibMainWindow::QtestceplibMainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::QtestceplibMainWindow),
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
  connect(ui->m_ActionLoadTIFFImage, SIGNAL(triggered()), this, SLOT(doLoadTIFFImage()));
  connect(ui->m_ActionTestHDF, SIGNAL(triggered()), this, SLOT(doTestHDF5Library()));
  connect(ui->m_ActionTestNexus, SIGNAL(triggered()), this, SLOT(doTestNexusLibrary()));
}

QtestceplibMainWindow::~QtestceplibMainWindow()
{
  delete ui;
}

QString defPath, defHDFPath, defNexusPath;

void QtestceplibMainWindow::doReadSettings()
{
  QString theFile = QFileDialog::getOpenFileName(
        this, "Read Settings from...", defPath);

  if (theFile.length()) {
    readSettings(theFile);
    defPath = theFile;
  }
}

void QtestceplibMainWindow::doWriteSettings()
{
  QString theFile = QFileDialog::getSaveFileName(
        this, "Write Settings in...", defPath);

  if (theFile.length()) {
    writeSettings(theFile);
    defPath = theFile;
  }
}

void QtestceplibMainWindow::readSettings()
{
}

void QtestceplibMainWindow::writeSettings()
{
}

void QtestceplibMainWindow::readSettings(QString filePath)
{
  QSettings settings(filePath, QSettings::IniFormat);

  readSettings(&settings);
}

void QtestceplibMainWindow::writeSettings(QString filePath)
{
  QSettings settings(filePath, QSettings::IniFormat);

  writeSettings(&settings);
}

void QtestceplibMainWindow::readSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::readSettings(this, "qtestceplib", settings);
}

void QtestceplibMainWindow::writeSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::writeSettings(this, "qtestceplib", settings);
}

void QtestceplibMainWindow::doLoadTIFFImage()
{
  QString theFile = QFileDialog::getOpenFileName(
        this, "Read TIFF Image from...", defPath);

  if (theFile.length()) {
    QcepImageDataFormatTiff<double> fmt("tiff");

    QcepImageData<double> *img = new QcepImageData<double>(QcepSettingsSaverWPtr(), 1024,1024);

    fmt.loadFile(theFile, img);
  }
}

void QtestceplibMainWindow::doTestHDF5Library()
{
  QString theFile = QFileDialog::getOpenFileName(
        this, "Read HDF5 File...", defHDFPath);

  if (theFile.length()) {
    hid_t file, data;

    file = H5Fopen(qPrintable(theFile), H5F_ACC_RDONLY, H5P_DEFAULT);
    H5Fclose(file);

    defHDFPath=theFile;
  }
}

void QtestceplibMainWindow::doTestNexusLibrary()
{
  QString theFile = QFileDialog::getOpenFileName(
        this, "Read Nexus File...", defNexusPath);

  if (theFile.length()) {
    NXhandle nxHandle;

    NXopen(qPrintable(theFile), NXACC_READ, &nxHandle);

    NXclose(&nxHandle);

    defNexusPath=theFile;
  }
}
