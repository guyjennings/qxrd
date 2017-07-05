#include "qtestceplibnexusmainwindow.h"
#include "ui_qtestceplibnexusmainwindow.h"
#include <QFileDialog>
#include "qcepmutexlocker.h"
#include "qcepimagedata.h"
#include "hdf5.h"
#include "qtestceplibnexusdocument.h"
#include "napi.h"
#include "zlib.h"

QtestceplibNexusMainWindow::QtestceplibNexusMainWindow(QtestceplibNexusDocument *doc, QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::QtestceplibNexusMainWindow),
  m_Document(doc),
  m_Mutex(QMutex::Recursive)
{
  ui->setupUi(this);

  connect(ui->m_ActionReadSettings, SIGNAL(triggered()), this, SLOT(doReadSettings()));
  connect(ui->m_ActionWriteSettings, SIGNAL(triggered()), this, SLOT(doWriteSettings()));
  connect(ui->m_ActionLoadImage, SIGNAL(triggered()), this, SLOT(doLoadImage()));
  connect(ui->m_ActionTestHDF, SIGNAL(triggered()), this, SLOT(doTestHDF5Library()));
  connect(ui->m_ActionTestHDFSlab, SIGNAL(triggered()), this, SLOT(doTestHDF5SlabOutput()));
  connect(ui->m_ActionTestNexus, SIGNAL(triggered()), this, SLOT(doTestNexusLibrary()));

  ui->m_FileMenu->addAction(tr("QT Version %1").arg(qVersion()));
  ui->m_FileMenu->addAction(tr("QCEPLIB Version %1").arg(qceplibVersion()));
  ui->m_FileMenu->addAction(tr("ZLIB Version %1").arg(zlibVersion()));
  ui->m_FileMenu->addAction(tr("HDF5 Version %1").arg(qcepH5Version()));
  ui->m_FileMenu->addAction(tr("NeXus Version %1").arg(NXgetversion()));
}

QtestceplibNexusMainWindow::~QtestceplibNexusMainWindow()
{
  delete ui;
}

void QtestceplibNexusMainWindow::printMessage(QString msg)
{
  ui->m_Messages->appendPlainText(msg);
}

QString defPath, defHDFPath, defNexusPath;

void QtestceplibNexusMainWindow::doReadSettings()
{
  QString theFile = QFileDialog::getOpenFileName(
        this, "Read Settings from...", defPath);

  if (theFile.length()) {
    readSettings(theFile);
    defPath = theFile;
  }
}

void QtestceplibNexusMainWindow::doWriteSettings()
{
  QString theFile = QFileDialog::getSaveFileName(
        this, "Write Settings in...", defPath);

  if (theFile.length()) {
    writeSettings(theFile);
    defPath = theFile;
  }
}

void QtestceplibNexusMainWindow::readSettings()
{
}

void QtestceplibNexusMainWindow::writeSettings()
{
}

void QtestceplibNexusMainWindow::readSettings(QString filePath)
{
  QSettings settings(filePath, QSettings::IniFormat);

  readSettings(&settings);
}

void QtestceplibNexusMainWindow::writeSettings(QString filePath)
{
  QSettings settings(filePath, QSettings::IniFormat);

  writeSettings(&settings);
}

void QtestceplibNexusMainWindow::readSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::readSettings(this, settings);

  if (m_Document) {
    m_Document->readSettings(settings);
  }
}

void QtestceplibNexusMainWindow::writeSettings(QSettings *settings)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::writeSettings(this, settings);

  if (m_Document) {
    m_Document->writeSettings(settings);
  }
}

void QtestceplibNexusMainWindow::doLoadImage()
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

void QtestceplibNexusMainWindow::doTestHDF5Library()
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

void QtestceplibNexusMainWindow::doTestHDF5SlabOutput()
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

void QtestceplibNexusMainWindow::doTestNexusLibrary()
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
