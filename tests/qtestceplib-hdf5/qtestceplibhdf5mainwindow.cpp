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
  connect(ui->m_ActionIterateHDF, SIGNAL(triggered()), this, SLOT(doIterateHDF5File()));
  connect(ui->m_ActionTestHDFSlab, SIGNAL(triggered()), this, SLOT(doTestHDF5SlabOutput()));

  connect(ui->m_FileMenu->addAction("Iterate over objects in HDF file..."), SIGNAL(triggered()), this, SLOT(doIterateHDF5File2()));

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
    if (H5Fis_hdf5(qPrintable(theFile))) {
      hid_t file, data;

      file = H5Fopen(qPrintable(theFile), H5F_ACC_RDONLY, H5P_DEFAULT);
      H5Fclose(file);

      defHDFPath=theFile;
    } else {
      printMessage(tr("File %1 is not an HDF5 file").arg(theFile));
    }
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

class QtestHDF5Command {
public:
  QtestHDF5Command(QtestceplibHdf5MainWindow *win);

  static herr_t staticPrintOneError(unsigned n,
                                    const H5E_error2_t *err_desc,
                                    void *data);
  herr_t printOneError(unsigned n,
                       const H5E_error2_t *err_desc);

  void printMessage(QString msg);
  void printError();

  QString linkTarget(hid_t             loc_id,
                     const char       *name,
                     const H5L_info_t *info);

  QString softLinkTarget(hid_t         loc_id,
                         const char   *name,
                         const H5L_info_t *info);

  QString externalLinkTarget(hid_t       loc_id,
                             const char *name,
                             const H5L_info_t *info);

protected:
  QtestceplibHdf5MainWindow   *m_Window;
};

QtestHDF5Command::QtestHDF5Command(QtestceplibHdf5MainWindow *win)
  : m_Window(win)
{
}


void QtestHDF5Command::printMessage(QString msg)
{
  if (m_Window) {
    m_Window -> printMessage(msg);
  }
}

void QtestHDF5Command::printError()
{
  H5Ewalk(H5E_DEFAULT, H5E_WALK_UPWARD, QtestHDF5Command::staticPrintOneError, (void*) this);
}

herr_t QtestHDF5Command::staticPrintOneError(unsigned n, const H5E_error2_t *err_desc, void *data)
{
  QtestHDF5Command *iter = reinterpret_cast<QtestHDF5Command*>(data);

  if (iter) {
    return iter->printOneError(n, err_desc);
  } else {
    return -1;
  }
}

herr_t QtestHDF5Command::printOneError(unsigned n, const H5E_error2_t *err_desc)
{
  if (err_desc) {
    printMessage(QObject::tr("%1: %2 line %3 in %4: %5")
                 .arg(n)
                 .arg(err_desc->file_name)
                 .arg(err_desc->line)
                 .arg(err_desc->func_name)
                 .arg(err_desc->desc));
  }
}

QString QtestHDF5Command::linkTarget(hid_t       loc_id,
                                     const char *name,
                                     const H5L_info_t *info)
{
  QString res = "<invalid>";

  if (info) {
    switch (info->type) {
    case H5L_TYPE_SOFT:
      res = softLinkTarget(loc_id, name, info);
      break;

    case H5L_TYPE_EXTERNAL:
      res = externalLinkTarget(loc_id, name, info);
      break;
    }
  }

  return res;
}

QString QtestHDF5Command::softLinkTarget(hid_t       loc_id,
                                         const char *name,
                                         const H5L_info_t *info)
{
  QString res = "<invalid>";

  if (info && info->type == H5L_TYPE_SOFT) {
    QByteArray buff(info->u.val_size, '*');

    herr_t s = H5Lget_val(loc_id, name, buff.data(), buff.size(), H5P_DEFAULT);

    if (s == 0) {
      res = buff;
    } else {
      printError();
    }
  }

  return res;
}

QString QtestHDF5Command::externalLinkTarget(hid_t loc_id,
                                             const char *name,
                                             const H5L_info_t *info)
{
  QString res = "<invalid>";

  if (info && info->type == H5L_TYPE_EXTERNAL) {
    QByteArray buff(info->u.val_size, '*');

    herr_t s = H5Lget_val(loc_id, name, buff.data(), buff.size(), H5P_DEFAULT);

    if (s == 0) {
      const char *file, *data;
      unsigned flags;

      herr_t s = H5Lunpack_elink_val(buff.data(), buff.size(), &flags, &file, &data);

      if (s == 0) {
        res = QObject::tr("%1#%2").arg(file).arg(data);
      }
    } else {
      printError();
    }
  }

  return res;
}

class QtestHDF5Iterator : public QtestHDF5Command {
public:
  QtestHDF5Iterator(QtestceplibHdf5MainWindow *win,
                    hid_t                      file,
                    int                        level,
                    QtestHDF5Iterator         *parent,
                    haddr_t                    addr);

  int visitAll();
  int visitAll(const char *inGroup);

  int group_check(haddr_t target_addr);

  static herr_t staticVisitOne(
      hid_t             loc_id,
      const char       *name,
      const H5L_info_t *info,
      void             *data);

  herr_t visitOne(
      hid_t             loc_id,
      const char       *name,
      const H5L_info_t *info);

  static herr_t staticOneAttribute(hid_t id,
                                   const char *name,
                                   const H5A_info_t *ainfo,
                                   void *data);

  herr_t oneAttribute(hid_t id,
                      const char *name,
                      const H5A_info_t *ainfo);

protected:
  hid_t                        m_File;
  int                          m_Level;
  QtestHDF5Iterator           *m_Parent;
  haddr_t                      m_Address;
};

QtestHDF5Iterator::QtestHDF5Iterator(
    QtestceplibHdf5MainWindow *win,
    hid_t                      file,
    int                        level,
    QtestHDF5Iterator         *parent,
    haddr_t                    addr) :
  QtestHDF5Command(win),
  m_File(file),
  m_Level(level),
  m_Parent(parent),
  m_Address(addr)
{
}

int QtestHDF5Iterator::visitAll()
{
  printMessage(QObject::tr("QtestHDFIterator::visitAll(): Begin"));

  herr_t status = H5Literate(m_File,
                             H5_INDEX_NAME,
                             H5_ITER_NATIVE,
                             NULL,
                             QtestHDF5Iterator::staticVisitOne,
                             (void*) this);

  if (status) {
    printMessage(QObject::tr("QtestHDFIterator::visitAll(): Error %1").arg(status));
    printError();
  } else {
    printMessage(QObject::tr("QtestHDFIterator::visitAll(): Success"));
  }

  return status;
}

int QtestHDF5Iterator::visitAll(const char *name)
{
  printMessage(QObject::tr("QtestHDFIterator::visitAll(%1): Begin").arg(name));

  herr_t status = H5Literate_by_name(m_File,
                                     name,
                                     H5_INDEX_NAME,
                                     H5_ITER_NATIVE,
                                     NULL,
                                     QtestHDF5Iterator::staticVisitOne,
                                     (void*) this,
                                     H5P_DEFAULT);

  if (status) {
    printMessage(QObject::tr("QtestHDFIterator::visitAll(%1): Error %2").arg(name).arg(status));
    printError();
  } else {
    printMessage(QObject::tr("QtestHDFIterator::visitAll(%1): Success").arg(name));
  }

  return status;
}

herr_t QtestHDF5Iterator::staticVisitOne(hid_t loc_id, const char *name, const H5L_info_t *info, void *data)
{
  QtestHDF5Iterator *iter = reinterpret_cast<QtestHDF5Iterator*>(data);

  if (iter) {
    return iter->visitOne(loc_id, name, info);
  } else {
    return -1;
  }
}

herr_t QtestHDF5Iterator::visitOne(hid_t loc_id, const char *name, const H5L_info_t *info)
{
  herr_t      status,
              return_val = 0;
  H5O_info_t  infobuf;
  unsigned    spaces = 2*(m_Level+1);

  status = H5Oget_info_by_name(loc_id, name, &infobuf, H5P_DEFAULT);

  herr_t      status2;
  H5L_info_t  linkInfo;

  status2 = H5Lget_info(loc_id, name, &linkInfo, H5P_DEFAULT);

//  switch (linkInfo.type) {
//  case H5L_TYPE_HARD:
//    printMessage(QObject::tr("%1Hard Link").arg(" ",spaces));
//    break;

//  case H5L_TYPE_SOFT:
//    printMessage(QObject::tr("%1Soft Link").arg(" ",spaces));
//    break;

//  case H5L_TYPE_EXTERNAL:
//    printMessage(QObject::tr("%1External Link").arg(" ",spaces));
//    break;

//  default:
//    printMessage(QObject::tr("%1Other: %2").arg(" ",spaces).arg(linkInfo.type));
//  }

  switch (infobuf.type) {
  case H5O_TYPE_GROUP:
    if (linkInfo.type == H5L_TYPE_HARD) {
      printMessage(QObject::tr("%1Group: %2 {").arg(' ',spaces).arg(name));

      if (group_check(infobuf.addr)) {
        printMessage(QObject::tr("%1  Warning: Loop Detected!").arg(' ',spaces));
      } else {
        QtestHDF5Iterator inGroup(m_Window, loc_id, m_Level+1, this, infobuf.addr);

        inGroup.visitAll(name);
      }

      printMessage(QObject::tr("%1}").arg(' ',spaces));
    } else {
      printMessage(QObject::tr("%1Link: %2").arg(' ',spaces).arg(name));
    }
    break;

  case H5O_TYPE_DATASET:
    printMessage(QObject::tr("%1Dataset: %2").arg(' ',spaces).arg(name));
    break;

  case H5O_TYPE_NAMED_DATATYPE:
    printMessage(QObject::tr("%1Datatype: %2").arg(' ',spaces).arg(name));
    break;

  default:
    printMessage(QObject::tr("%1Unknown: %2 (type %3)").arg(' ',spaces).arg(name).arg(infobuf.type));
    break;
  }

  herr_t status3;
  hsize_t sz;

  status3 = H5Aiterate(loc_id, H5_INDEX_NAME, H5_ITER_INC, &sz, QtestHDF5Iterator::staticOneAttribute, this);

  return return_val;
}

int QtestHDF5Iterator::group_check(haddr_t target_addr)
{
  if (m_Address == target_addr) {
    return 1;
  } else if (m_Level == 0) {
    return 0;
  } else {
    return m_Parent->group_check(target_addr);
  }
}

herr_t QtestHDF5Iterator::staticOneAttribute(hid_t id, const char *name, const H5A_info_t *ainfo, void *data)
{
  QtestHDF5Iterator *iter = reinterpret_cast<QtestHDF5Iterator*>(data);

  if (iter) {
    return iter->oneAttribute(id, name, ainfo);
  } else {
    return -1;
  }
}

herr_t QtestHDF5Iterator::oneAttribute(hid_t id, const char *name, const H5A_info_t *ainfo)
{
  unsigned    spaces = 2*(m_Level+1);

  printMessage(QObject::tr("%1  Attr: %2").arg(' ', spaces).arg(name));
}

void QtestceplibHdf5MainWindow::doIterateHDF5File()
{
  QString theFile = QFileDialog::getOpenFileName(
        this, "Read HDF5 File...", defHDFPath);

  if (theFile.length()) {
    if (H5Fis_hdf5(qPrintable(theFile))) {
      hid_t         file;
      herr_t        status;
      H5O_info_t    infobuf;

      file = H5Fopen(qPrintable(theFile), H5F_ACC_RDONLY, H5P_DEFAULT);
      status = H5Oget_info(file, &infobuf);

      QtestHDF5Iterator iter(this, file, 0, NULL, infobuf.addr);

      printMessage(tr("Iterating over %1").arg(theFile));
      iter.visitAll();
      printMessage(tr("Done"));

      defHDFPath = theFile;
    } else {
      printMessage(tr("File %1 is not an HDF5 file").arg(theFile));
    }
  }
}

class QtestHDF5Iterator2 : public QtestHDF5Command {
public:
  QtestHDF5Iterator2(QtestceplibHdf5MainWindow *win,
                    hid_t                      file,
                    int                        level,
                    QtestHDF5Iterator2         *parent,
                    haddr_t                    addr);

  herr_t visitAll();

  static herr_t staticVisitOne(hid_t             loc_id,
      const char       *name,
      const H5L_info_t *info,
      void             *data);

  herr_t visitOne(
      hid_t             loc_id,
      const char       *name,
      const H5L_info_t *info);

protected:
  hid_t                        m_File;
  int                          m_Level;
  QtestHDF5Iterator2          *m_Parent;
  haddr_t                      m_Address;
};

QtestHDF5Iterator2::QtestHDF5Iterator2(
    QtestceplibHdf5MainWindow *win,
    hid_t                      file,
    int                        level,
    QtestHDF5Iterator2        *parent,
    haddr_t                    addr) :
  QtestHDF5Command(win),
  m_File(file),
  m_Level(level),
  m_Parent(parent),
  m_Address(addr)
{
}

herr_t QtestHDF5Iterator2::visitAll()
{
  printMessage(QObject::tr("QtestHDFIterator2::visitAll(): Begin"));

  herr_t status = H5Lvisit(m_File,
                             H5_INDEX_NAME,
                             H5_ITER_NATIVE,
                             QtestHDF5Iterator2::staticVisitOne,
                             (void*) this);

  if (status) {
    printMessage(QObject::tr("QtestHDFIterator2::visitAll(): Error %1").arg(status));
    printError();
  } else {
    printMessage(QObject::tr("QtestHDFIterator2::visitAll(): Success"));
  }

  return status;
}

herr_t QtestHDF5Iterator2::staticVisitOne(hid_t loc_id, const char *name, const H5L_info_t *info, void *data)
{
  QtestHDF5Iterator2 *iter = reinterpret_cast<QtestHDF5Iterator2*>(data);

  if (iter) {
    return iter->visitOne(loc_id, name, info);
  } else {
    return -1;
  }
}

herr_t QtestHDF5Iterator2::visitOne(hid_t loc_id, const char *name, const H5L_info_t *info)
{
  if (info) {
    switch (info->type) {
    case H5L_TYPE_HARD:
      printMessage(QObject::tr("Hard: %1").arg(name));
      break;

    case H5L_TYPE_SOFT:
      printMessage(QObject::tr("Soft: %1 -> %2").arg(name).arg(softLinkTarget(loc_id, name, info)));
      break;

    case H5L_TYPE_EXTERNAL:
      printMessage(QObject::tr("Ext: %1 -> %2").arg(name).arg(externalLinkTarget(loc_id, name, info)));
      break;

    default:
      printMessage(QObject::tr("Other: %1").arg(name));
      break;
    }
  }

  return 0;
}

void QtestceplibHdf5MainWindow::doIterateHDF5File2()
{
  QString theFile = QFileDialog::getOpenFileName(
        this, "Read HDF5 File...", defHDFPath);

  if (theFile.length()) {
    if (H5Fis_hdf5(qPrintable(theFile))) {
      hid_t         file;
      herr_t        status;
      H5O_info_t    infobuf;

      file = H5Fopen(qPrintable(theFile), H5F_ACC_RDONLY, H5P_DEFAULT);
      status = H5Oget_info(file, &infobuf);

      QtestHDF5Iterator2 iter(this, file, 0, NULL, infobuf.addr);

      printMessage(tr("Iterating over %1").arg(theFile));
      iter.visitAll();
      printMessage(tr("Done"));

      defHDFPath = theFile;
    } else {
      printMessage(tr("File %1 is not an HDF5 file").arg(theFile));
    }
  }
}
