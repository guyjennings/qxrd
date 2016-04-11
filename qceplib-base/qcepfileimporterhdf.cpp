#include "qcepfileimporterhdf.h"
#include "hdf5.h"
#include "qcepdatasetmodel.h"
#include "qcepallocator.h"
#include "qcepdatacolumn.h"
#include "qcepimagedata.h"
#include "qcepdataarray.h"

QcepFileImporterHDF::QcepFileImporterHDF(QcepDatasetModelPtr model,
                                         QModelIndexList &indexes,
                                         QString path)
  : QcepFileImporter(model, indexes, path)
{
}

void QcepFileImporterHDF::exec()
{
  hid_t file;
  hid_t group;
  herr_t status;

  file = H5Fopen(qPrintable(m_Path), H5F_ACC_RDONLY, H5P_DEFAULT);

//  m_Model -> printMessage(tr("Scanning %1").arg(m_Path));

  group = H5Gopen(file, "/", H5P_DEFAULT);

//  m_Model -> printMessage(tr("/"));

  scanGroup(QModelIndex(), group);

  status = H5Fclose(file);

  emit importCompleted();
}

#define MAX_NAME 1024

void QcepFileImporterHDF::scanGroup(QModelIndex dest, hid_t gid, int level)
{
  char group_name[MAX_NAME];
  char memb_name[MAX_NAME];

  ssize_t len = H5Iget_name(gid, group_name, MAX_NAME);

  hsize_t nobj;
  herr_t err = H5Gget_num_objs(gid, &nobj);

  for (int i=0; i<nobj; i++) {
    len = H5Gget_objname_by_idx(gid, i, memb_name, MAX_NAME);
    int otype = H5Gget_objtype_by_idx(gid, i);

//    m_Model->printMessage(tr("%1%2").arg(i,-level*3).arg(memb_name));

    if (otype == H5G_LINK  || otype == H5G_UDLINK) {
      scanLink(dest, gid, memb_name);
    } else if (otype == H5G_GROUP) {
//      QcepDataGroupPtr dgrp = m_Model->group(dest);
      QcepDataGroupPtr ngrp = QcepAllocator::newGroup(memb_name);
      QModelIndex      inst = m_Model->append(dest, ngrp);

      hid_t grpid = H5Gopen(gid, memb_name, H5P_DEFAULT);
      scanGroup(inst, grpid, level+1);
      H5Gclose(grpid);
    } else if (otype == H5G_DATASET) {
      hid_t dsid = H5Dopen(gid, memb_name, H5P_DEFAULT);
      scanDataset(dest, dsid, memb_name);
      H5Dclose(dsid);
    } else {
      m_Model->printMessage(tr("Other object type %1").arg(otype));
    }

    if (level == 0) {
      emit importProgress(100.0*((double) i)/((double) nobj));
    }
  }
}

void QcepFileImporterHDF::scanDataset(QModelIndex dest, hid_t dsid, char *name)
{
//  m_Model -> printMessage("Scan Dataset");

  char ds_name[MAX_NAME];
  H5Iget_name(dsid, ds_name, MAX_NAME);

  hid_t sid = H5Dget_space(dsid);
  int rank  = H5Sget_simple_extent_ndims(sid);

  QVector<hsize_t> dims(rank), maxdims(rank);

  H5Sget_simple_extent_dims(sid, dims.data(), maxdims.data());

  QString res;

  for (int i=0; i<rank; i++) {
    if (i == 0) {
      res = tr("%1").arg(dims.value(i));
    } else {
      res += tr(" x %1").arg(dims.value(i));
    }
  }

//  m_Model -> printMessage(tr("Dataset: %1 : %2").arg(name).arg(res));

  if (rank == 0) {
  } else if (rank == 1) {
    QcepDataColumnPtr ncol = QcepAllocator::newColumn(name, dims.value(0));

    if (ncol) {
      readColumnData(ncol, dsid);

      m_Model->append(dest, ncol);
    }
  } else if (rank == 2) {
    QcepDoubleImageDataPtr nimg = QcepAllocator::newDoubleImage(name, dims.value(1), dims.value(0));

    if (nimg) {
      readImageData(nimg, dsid);

      m_Model->append(dest, nimg);
    }
  } else {
    QVector<int> idims(rank);

    for (int i=0; i<rank; i++) {
      idims[rank-i-1]=dims[i];
    }

    QcepDataArrayPtr narr = QcepAllocator::newArray(name, idims);

    if (narr) {
      readArrayData(narr, dsid);

      m_Model->append(dest, narr);
    }
  }
}

void QcepFileImporterHDF::scanLink(QModelIndex dest, hid_t gid, char *name)
{
  herr_t status;
  char target[MAX_NAME];

  status = H5Gget_linkval(gid, name, MAX_NAME, target);

  m_Model->printMessage(tr("Scan link %1 -> %2").arg(name).arg(target));
}

void QcepFileImporterHDF::readColumnData(QcepDataColumnPtr col, hid_t dsid)
{
  hid_t sid = H5Dget_space(dsid);
  hsize_t size[1];
  size[0] = col->rowCount();
  hid_t memspace = H5Screate_simple(1, size, NULL);
  herr_t rderr   = H5Dread(dsid, H5T_NATIVE_DOUBLE, memspace, sid, H5P_DEFAULT, col->data());

  if (rderr) {
    m_Model->printMessage(tr("Error reading column data %1").arg(rderr));
  }
}

void QcepFileImporterHDF::readImageData(QcepDoubleImageDataPtr img, hid_t dsid)
{
  hid_t sid = H5Dget_space(dsid);
  hsize_t size[2];
  size[0] = img->get_Width();
  size[1] = img->get_Height();
  hid_t memspace = H5Screate_simple(2, size, NULL);
  herr_t rderr   = H5Dread(dsid, H5T_NATIVE_DOUBLE, memspace, sid, H5P_DEFAULT, img->data());

  if (rderr) {
    m_Model->printMessage(tr("Error reading image data %1").arg(rderr));
  }
}

void QcepFileImporterHDF::readArrayData(QcepDataArrayPtr arr, hid_t dsid)
{
  m_Model->printMessage("QcepFileImporterHDF::readArrayData not yet implemented");
}
