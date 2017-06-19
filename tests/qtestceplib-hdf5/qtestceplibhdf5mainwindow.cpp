#include "qtestceplibhdf5mainwindow.h"
#include "ui_qtestceplibhdf5mainwindow.h"
#include <QFileDialog>
#include "qcepmutexlocker.h"
#include "qcepimagedata.h"
#include "hdf5.h"
#include "qtestceplibhdf5document.h"
#include "H5Cpp.h"

QtestceplibHdf5MainWindow::QtestceplibHdf5MainWindow(QtestceplibHdf5Document *doc, QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::QtestceplibHdf5MainWindow),
  m_Document(doc),
  m_Mutex(QMutex::Recursive)
{
  ui->setupUi(this);

  connect(ui->m_ActionReadSettings, SIGNAL(triggered()), this, SLOT(doReadSettings()));
  connect(ui->m_ActionWriteSettings, SIGNAL(triggered()), this, SLOT(doWriteSettings()));
  connect(ui->m_ActionLoadImage, SIGNAL(triggered()), this, SLOT(doLoadImage()));
  connect(ui->m_ActionTestHDF, SIGNAL(triggered()), this, SLOT(doTestHDF5Library()));
  connect(ui->m_ActionTestHDFSlab, SIGNAL(triggered()), this, SLOT(doTestHDF5SlabOutput()));

  ui->m_FileMenu->addAction(tr("QCEPLIB Version %1").arg(STR(QCEPLIB_VERSION)));
  ui->m_FileMenu->addAction(tr("HDF5 Version %1").arg(STR(QCEPLIB_HDF5_VERSION)));

  unsigned major, minor, release;

  H5::H5Library::getLibVersion(major, minor, release);

  ui->m_FileMenu->addAction(tr("HDF5 C++ Version %1.%2.%3").arg(major).arg(minor).arg(release));
}

QtestceplibHdf5MainWindow::~QtestceplibHdf5MainWindow()
{
  delete ui;
}

void QtestceplibHdf5MainWindow::printMessage(QString msg)
{
  ui->m_Messages->appendPlainText(msg);
}

QString defPath, defHDFPath;

void QtestceplibHdf5MainWindow::doReadSettings()
{
  QString theFile = QFileDialog::getOpenFileName(
        this, "Read Settings from...", defPath);

  if (theFile.length()) {
    readSettings(theFile);
    defPath = theFile;
  }
}

void QtestceplibHdf5MainWindow::doWriteSettings()
{
  QString theFile = QFileDialog::getSaveFileName(
        this, "Write Settings in...", defPath);

  if (theFile.length()) {
    writeSettings(theFile);
    defPath = theFile;
  }
}

void QtestceplibHdf5MainWindow::readSettings()
{
}

void QtestceplibHdf5MainWindow::writeSettings()
{
}

void QtestceplibHdf5MainWindow::readSettings(QString filePath)
{
  QSettings settings(filePath, QSettings::IniFormat);

  readSettings(&settings);
}

void QtestceplibHdf5MainWindow::writeSettings(QString filePath)
{
  QSettings settings(filePath, QSettings::IniFormat);

  writeSettings(&settings);
}

void QtestceplibHdf5MainWindow::readSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::readSettings(this, "qtestceplib", settings);

  if (m_Document) {
    m_Document->readSettings(settings, "document");
  }
}

void QtestceplibHdf5MainWindow::writeSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::writeSettings(this, "qtestceplib", settings);

  if (m_Document) {
    m_Document->writeSettings(settings, "document");
  }
}

void QtestceplibHdf5MainWindow::doLoadImage()
{
  QString theFile = QFileDialog::getOpenFileName(
        this, "Read Image from...", defPath);

  if (theFile.length()) {
    QcepDoubleImageData *img = new QcepDoubleImageData("image", 1024,1024, 0);

    if (img->readImage(theFile)) {
      img->loadMetaData();

      printMessage(tr("Loaded image from %1").arg(theFile));
      printMessage(tr(" width %1, height %2").arg(img->get_Width()).arg(img->get_Height()));
    } else {
      printMessage(tr("Image load failed from %1").arg(theFile));
    }
  }
}

void QtestceplibHdf5MainWindow::doTestHDF5Library()
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

void QtestceplibHdf5MainWindow::doTestHDF5SlabOutput()
{
  QString theFile = QFileDialog::getSaveFileName(
        this, "Save HDF Slab file...", defHDFPath);

  if (theFile.length()) {
    const int BIGDIM = 128;
    const int CHKDIM = 32;

    hid_t file_id, dataset_id, dataspace_id, memspace_id;

    file_id = H5Fcreate(qPrintable(theFile), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    hsize_t dims[3], count[3], offset[3], stride[3], block[3];

    dims[0] = BIGDIM;
    dims[1] = BIGDIM;
    dims[2] = BIGDIM;

    dataspace_id = H5Screate_simple(3, dims, NULL);

    dataset_id = H5Dcreate(file_id, "data", H5T_NATIVE_FLOAT, dataspace_id, H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    count[0] = CHKDIM;
    count[1] = CHKDIM;
    count[2] = CHKDIM;

    stride[0] = 1;
    stride[1] = 1;
    stride[2] = 1;

    block[0] = 1;
    block[1] = 1;
    block[2] = 1;

    float chk[CHKDIM][CHKDIM][CHKDIM];

    float value = 0;

    memspace_id = H5Screate_simple(3, count, NULL);

    for (int k=0; k<(BIGDIM/CHKDIM); k++) {
      for (int j=0; j<(BIGDIM/CHKDIM); j++) {
        for (int i=0; i<(BIGDIM/CHKDIM); i++) {

          for (int ck=0; ck<CHKDIM; ck++) {
            for (int cj=0; cj<CHKDIM; cj++) {
              for (int ci=0; ci<CHKDIM; ci++) {
                chk[ck][cj][ci] = value;
              }
            }
          }

          offset[0] = i*32;
          offset[1] = j*32;
          offset[2] = k*32;

          fprintf(stderr, "Offset %ld,%ld,%ld\n", (long) offset[0], (long) offset[1], (long) offset[2]);

          H5Sselect_hyperslab(dataspace_id, H5S_SELECT_SET, offset, stride, count, block);

          H5Dwrite(dataset_id, H5T_NATIVE_FLOAT, memspace_id, dataspace_id, H5P_DEFAULT, chk);

          value += 1;
        }
      }
    }

    H5Fclose(file_id);
  }
}

