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

//    m_Model->printMessage(tr("%1: %2").arg(i).arg(memb_name));

    if (otype == H5G_LINK) {
      scanLink(dest, gid, memb_name);
    } else if (otype == H5G_GROUP) {
//      QcepDataGroupPtr dgrp = m_Model->group(dest);
      QcepDataGroupPtr ngrp = QcepDataGroup::newDataGroup(QcepSettingsSaverWPtr(), memb_name, NULL);
      QModelIndex      inst = m_Model->append(dest, ngrp);

      hid_t grpid = H5Gopen(gid, memb_name, H5P_DEFAULT);
      scanGroup(inst, grpid, level+1);
      H5Gclose(grpid);
    } else if (otype == H5G_DATASET) {
      hid_t dsid = H5Dopen(gid, memb_name, H5P_DEFAULT);
      scanDataset(dest, dsid);
      H5Dclose(dsid);
    }

    if (level == 0) {
      emit importProgress(100.0*((double) i)/((double) nobj));
    }
  }
}

void QcepFileImporterHDF::scanDataset(QModelIndex dest, hid_t dsid)
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

  m_Model -> printMessage(tr("Dataset: %1 : %2").arg(ds_name).arg(res));

  if (rank == 0) {
  } else if (rank == 1) {
    QcepDataColumnPtr ncol = QcepAllocator::newColumn(ds_name, dims.value(0));
    if (ncol) {
      m_Model->append(dest, ncol);
    }
  } else if (rank == 2) {
    QcepDoubleImageDataPtr nimg = QcepAllocator::newDoubleImage(ds_name, dims.value(0), dims.value(1), NULL);

    if (nimg) {
      m_Model->append(dest, nimg);
    }
  } else {
    QVector<int> idims(rank);

    for (int i=0; i<rank; i++) {
      idims[i]=dims[i];
    }

    QcepDataArrayPtr narr = QcepAllocator::newArray(ds_name, idims);

    if (narr) {
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
