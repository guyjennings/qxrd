message(qceplib-hdf5 PWD = $${PWD} QCEPLIB_HDF5_VERSION = $${QCEPLIB_HDF5_VERSION})

linux-g++:QMAKE_TARGET.arch = $$QMAKE_HOST.arch
linux-g++-32:QMAKE_TARGET.arch = x86
linux-g++-64:QMAKE_TARGET.arch = x86_64
macx-*-32:QMAKE_TARGET.arch = x86

HDF5BASE = $${PWD}/hdf5-1.8.11/src/

macx {
  HDF5CONF = $${PWD}/hdf5-config/macx/
} else:win32 {
  contains(QMAKE_TARGET.arch, x86_64) {
    message(64 bit build)
    HDF5CONF = $${PWD}/hdf5-config/win64/
  } else {
    message(32 bit build)
    HDF5CONF = $${PWD}/hdf5-config/win32/
  }
} else:unix {
  contains(QMAKE_TARGET.arch, x86_64) {
    message(64 bit build)
    HDF5CONF = $${PWD}/hdf5-config/lin64/
  } else {
    message(32 bit build)
    HDF5CONF = $${PWD}/hdf5-config/lin32/
  }
  DEFINES += _LARGEFILE_SOURCE
  DEFINES += _LARGEFILE64_SOURCE
  LIBS += -ldl
}

INCLUDEPATH += $${HDF5BASE} $${HDF5CONF}

  HEADERS += \
    $${HDF5CONF}/H5config.h \
    $${HDF5CONF}/H5pubconf.h \
    $${HDF5BASE}/H5ACpkg.h \
    $${HDF5BASE}/H5ACprivate.h \
    $${HDF5BASE}/H5ACpublic.h \
    $${HDF5BASE}/H5Apkg.h \
    $${HDF5BASE}/H5Aprivate.h \
    $${HDF5BASE}/H5Apublic.h \
    $${HDF5BASE}/H5B2pkg.h \
    $${HDF5BASE}/H5B2private.h \
    $${HDF5BASE}/H5B2public.h \
    $${HDF5BASE}/H5Bpkg.h \
    $${HDF5BASE}/H5Bprivate.h \
    $${HDF5BASE}/H5Bpublic.h \
    $${HDF5BASE}/H5CSprivate.h \
    $${HDF5BASE}/H5Cpkg.h \
    $${HDF5BASE}/H5Cprivate.h \
    $${HDF5BASE}/H5Cpublic.h \
    $${HDF5BASE}/H5Dpkg.h \
    $${HDF5BASE}/H5Dprivate.h \
    $${HDF5BASE}/H5Dpublic.h \
    $${HDF5BASE}/H5Edefin.h \
    $${HDF5BASE}/H5Einit.h \
    $${HDF5BASE}/H5Epkg.h \
    $${HDF5BASE}/H5Eprivate.h \
    $${HDF5BASE}/H5Epubgen.h \
    $${HDF5BASE}/H5Epublic.h \
    $${HDF5BASE}/H5Eterm.h \
    $${HDF5BASE}/H5FDcore.h \
    $${HDF5BASE}/H5FDdirect.h \
    $${HDF5BASE}/H5FDfamily.h \
    $${HDF5BASE}/H5FDlog.h \
    $${HDF5BASE}/H5FDmpi.h \
    $${HDF5BASE}/H5FDmpio.h \
    $${HDF5BASE}/H5FDmpiposix.h \
    $${HDF5BASE}/H5FDmulti.h \
    $${HDF5BASE}/H5FDpkg.h \
    $${HDF5BASE}/H5FDprivate.h \
    $${HDF5BASE}/H5FDpublic.h \
    $${HDF5BASE}/H5FDsec2.h \
    $${HDF5BASE}/H5FDstdio.h \
    $${HDF5BASE}/H5FDwindows.h \
    $${HDF5BASE}/H5FLprivate.h \
    $${HDF5BASE}/H5FOprivate.h \
    $${HDF5BASE}/H5FSpkg.h \
    $${HDF5BASE}/H5FSprivate.h \
    $${HDF5BASE}/H5FSpublic.h \
    $${HDF5BASE}/H5Fpkg.h \
    $${HDF5BASE}/H5Fprivate.h \
    $${HDF5BASE}/H5Fpublic.h \
    $${HDF5BASE}/H5Gpkg.h \
    $${HDF5BASE}/H5Gprivate.h \
    $${HDF5BASE}/H5Gpublic.h \
    $${HDF5BASE}/H5HFpkg.h \
    $${HDF5BASE}/H5HFprivate.h \
    $${HDF5BASE}/H5HFpublic.h \
    $${HDF5BASE}/H5HGpkg.h \
    $${HDF5BASE}/H5HGprivate.h \
    $${HDF5BASE}/H5HGpublic.h \
    $${HDF5BASE}/H5HLpkg.h \
    $${HDF5BASE}/H5HLprivate.h \
    $${HDF5BASE}/H5HLpublic.h \
    $${HDF5BASE}/H5HPprivate.h \
    $${HDF5BASE}/H5Ipkg.h \
    $${HDF5BASE}/H5Iprivate.h \
    $${HDF5BASE}/H5Ipublic.h \
    $${HDF5BASE}/H5Lpkg.h \
    $${HDF5BASE}/H5Lprivate.h \
    $${HDF5BASE}/H5Lpublic.h \
    $${HDF5BASE}/H5MFpkg.h \
    $${HDF5BASE}/H5MFprivate.h \
    $${HDF5BASE}/H5MMprivate.h \
    $${HDF5BASE}/H5MMpublic.h \
    $${HDF5BASE}/H5MPpkg.h \
    $${HDF5BASE}/H5MPprivate.h \
    $${HDF5BASE}/H5Opkg.h \
    $${HDF5BASE}/H5Oprivate.h \
    $${HDF5BASE}/H5Opublic.h \
    $${HDF5BASE}/H5Oshared.h \
    $${HDF5BASE}/H5PLextern.h \
    $${HDF5BASE}/H5PLprivate.h \
    $${HDF5BASE}/H5Ppkg.h \
    $${HDF5BASE}/H5Pprivate.h \
    $${HDF5BASE}/H5Ppublic.h \
    $${HDF5BASE}/H5RCprivate.h \
    $${HDF5BASE}/H5RSprivate.h \
    $${HDF5BASE}/H5Rpkg.h \
    $${HDF5BASE}/H5Rprivate.h \
    $${HDF5BASE}/H5Rpublic.h \
    $${HDF5BASE}/H5SLprivate.h \
    $${HDF5BASE}/H5SMpkg.h \
    $${HDF5BASE}/H5SMprivate.h \
    $${HDF5BASE}/H5STprivate.h \
    $${HDF5BASE}/H5Spkg.h \
    $${HDF5BASE}/H5Sprivate.h \
    $${HDF5BASE}/H5Spublic.h \
    $${HDF5BASE}/H5TSprivate.h \
    $${HDF5BASE}/H5Tpkg.h \
    $${HDF5BASE}/H5Tprivate.h \
    $${HDF5BASE}/H5Tpublic.h \
    $${HDF5BASE}/H5Vprivate.h \
    $${HDF5BASE}/H5WBprivate.h \
    $${HDF5BASE}/H5Zpkg.h \
    $${HDF5BASE}/H5Zprivate.h \
    $${HDF5BASE}/H5Zpublic.h \
    $${HDF5BASE}/H5api_adpt.h \
    $${HDF5BASE}/H5overflow.h \
    $${HDF5BASE}/H5private.h \
    $${HDF5BASE}/H5public.h \
    $${HDF5BASE}/H5version.h \
    $${HDF5BASE}/H5win32defs.h \
    $${HDF5BASE}/hdf5.h


  SOURCES += \
    $${HDF5CONF}/H5lib_settings.c \
    $${HDF5CONF}/H5Tinit.c \
    $${HDF5BASE}/H5.c \
    $${HDF5BASE}/H5A.c \
    $${HDF5BASE}/H5AC.c \
    $${HDF5BASE}/H5Abtree2.c \
    $${HDF5BASE}/H5Adense.c \
    $${HDF5BASE}/H5Adeprec.c \
    $${HDF5BASE}/H5Aint.c \
    $${HDF5BASE}/H5Atest.c \
    $${HDF5BASE}/H5B.c \
    $${HDF5BASE}/H5B2.c \
    $${HDF5BASE}/H5B2cache.c \
    $${HDF5BASE}/H5B2dbg.c \
    $${HDF5BASE}/H5B2hdr.c \
    $${HDF5BASE}/H5B2int.c \
    $${HDF5BASE}/H5B2stat.c \
    $${HDF5BASE}/H5B2test.c \
    $${HDF5BASE}/H5Bcache.c \
    $${HDF5BASE}/H5Bdbg.c \
    $${HDF5BASE}/H5C.c \
    $${HDF5BASE}/H5CS.c \
    $${HDF5BASE}/H5D.c \
    $${HDF5BASE}/H5Dbtree.c \
    $${HDF5BASE}/H5Dchunk.c \
    $${HDF5BASE}/H5Dcompact.c \
    $${HDF5BASE}/H5Dcontig.c \
    $${HDF5BASE}/H5Ddbg.c \
    $${HDF5BASE}/H5Ddeprec.c \
    $${HDF5BASE}/H5Defl.c \
    $${HDF5BASE}/H5Dfill.c \
    $${HDF5BASE}/H5Dint.c \
    $${HDF5BASE}/H5Dio.c \
    $${HDF5BASE}/H5Dlayout.c \
    $${HDF5BASE}/H5Dmpio.c \
    $${HDF5BASE}/H5Doh.c \
    $${HDF5BASE}/H5Dscatgath.c \
    $${HDF5BASE}/H5Dselect.c \
    $${HDF5BASE}/H5Dtest.c \
    $${HDF5BASE}/H5E.c \
    $${HDF5BASE}/H5Edeprec.c \
    $${HDF5BASE}/H5Eint.c \
    $${HDF5BASE}/H5F.c \
    $${HDF5BASE}/H5FD.c \
    $${HDF5BASE}/H5FDcore.c \
    $${HDF5BASE}/H5FDdirect.c \
    $${HDF5BASE}/H5FDfamily.c \
    $${HDF5BASE}/H5FDint.c \
    $${HDF5BASE}/H5FDlog.c \
    $${HDF5BASE}/H5FDmpi.c \
    $${HDF5BASE}/H5FDmpio.c \
    $${HDF5BASE}/H5FDmpiposix.c \
    $${HDF5BASE}/H5FDmulti.c \
    $${HDF5BASE}/H5FDsec2.c \
    $${HDF5BASE}/H5FDspace.c \
    $${HDF5BASE}/H5FDstdio.c \
    $${HDF5BASE}/H5FDwindows.c \
    $${HDF5BASE}/H5FL.c \
    $${HDF5BASE}/H5FO.c \
    $${HDF5BASE}/H5FS.c \
    $${HDF5BASE}/H5FScache.c \
    $${HDF5BASE}/H5FSdbg.c \
    $${HDF5BASE}/H5FSsection.c \
    $${HDF5BASE}/H5FSstat.c \
    $${HDF5BASE}/H5FStest.c \
    $${HDF5BASE}/H5Faccum.c \
    $${HDF5BASE}/H5Fcwfs.c \
    $${HDF5BASE}/H5Fdbg.c \
    $${HDF5BASE}/H5Fefc.c \
    $${HDF5BASE}/H5Ffake.c \
    $${HDF5BASE}/H5Fio.c \
    $${HDF5BASE}/H5Fmount.c \
    $${HDF5BASE}/H5Fmpi.c \
    $${HDF5BASE}/H5Fquery.c \
    $${HDF5BASE}/H5Fsfile.c \
    $${HDF5BASE}/H5Fsuper.c \
    $${HDF5BASE}/H5Fsuper_cache.c \
    $${HDF5BASE}/H5Ftest.c \
    $${HDF5BASE}/H5G.c \
    $${HDF5BASE}/H5Gbtree2.c \
    $${HDF5BASE}/H5Gcache.c \
    $${HDF5BASE}/H5Gcompact.c \
    $${HDF5BASE}/H5Gdense.c \
    $${HDF5BASE}/H5Gdeprec.c \
    $${HDF5BASE}/H5Gent.c \
    $${HDF5BASE}/H5Gint.c \
    $${HDF5BASE}/H5Glink.c \
    $${HDF5BASE}/H5Gloc.c \
    $${HDF5BASE}/H5Gname.c \
    $${HDF5BASE}/H5Gnode.c \
    $${HDF5BASE}/H5Gobj.c \
    $${HDF5BASE}/H5Goh.c \
    $${HDF5BASE}/H5Groot.c \
    $${HDF5BASE}/H5Gstab.c \
    $${HDF5BASE}/H5Gtest.c \
    $${HDF5BASE}/H5Gtraverse.c \
    $${HDF5BASE}/H5HF.c \
    $${HDF5BASE}/H5HFbtree2.c \
    $${HDF5BASE}/H5HFcache.c \
    $${HDF5BASE}/H5HFdbg.c \
    $${HDF5BASE}/H5HFdblock.c \
    $${HDF5BASE}/H5HFdtable.c \
    $${HDF5BASE}/H5HFhdr.c \
    $${HDF5BASE}/H5HFhuge.c \
    $${HDF5BASE}/H5HFiblock.c \
    $${HDF5BASE}/H5HFiter.c \
    $${HDF5BASE}/H5HFman.c \
    $${HDF5BASE}/H5HFsection.c \
    $${HDF5BASE}/H5HFspace.c \
    $${HDF5BASE}/H5HFstat.c \
    $${HDF5BASE}/H5HFtest.c \
    $${HDF5BASE}/H5HFtiny.c \
    $${HDF5BASE}/H5HG.c \
    $${HDF5BASE}/H5HGcache.c \
    $${HDF5BASE}/H5HGdbg.c \
    $${HDF5BASE}/H5HGquery.c \
    $${HDF5BASE}/H5HL.c \
    $${HDF5BASE}/H5HLcache.c \
    $${HDF5BASE}/H5HLdbg.c \
    $${HDF5BASE}/H5HLint.c \
    $${HDF5BASE}/H5HP.c \
    $${HDF5BASE}/H5I.c \
    $${HDF5BASE}/H5Itest.c \
    $${HDF5BASE}/H5L.c \
    $${HDF5BASE}/H5Lexternal.c \
    $${HDF5BASE}/H5MF.c \
    $${HDF5BASE}/H5MFaggr.c \
    $${HDF5BASE}/H5MFdbg.c \
    $${HDF5BASE}/H5MFsection.c \
    $${HDF5BASE}/H5MM.c \
    $${HDF5BASE}/H5MP.c \
    $${HDF5BASE}/H5MPtest.c \
    $${HDF5BASE}/H5O.c \
    $${HDF5BASE}/H5Oainfo.c \
    $${HDF5BASE}/H5Oalloc.c \
    $${HDF5BASE}/H5Oattr.c \
    $${HDF5BASE}/H5Oattribute.c \
    $${HDF5BASE}/H5Obogus.c \
    $${HDF5BASE}/H5Obtreek.c \
    $${HDF5BASE}/H5Ocache.c \
    $${HDF5BASE}/H5Ochunk.c \
    $${HDF5BASE}/H5Ocont.c \
    $${HDF5BASE}/H5Ocopy.c \
    $${HDF5BASE}/H5Odbg.c \
    $${HDF5BASE}/H5Odrvinfo.c \
    $${HDF5BASE}/H5Odtype.c \
    $${HDF5BASE}/H5Oefl.c \
    $${HDF5BASE}/H5Ofill.c \
    $${HDF5BASE}/H5Oginfo.c \
    $${HDF5BASE}/H5Olayout.c \
    $${HDF5BASE}/H5Olinfo.c \
    $${HDF5BASE}/H5Olink.c \
    $${HDF5BASE}/H5Omessage.c \
    $${HDF5BASE}/H5Omtime.c \
    $${HDF5BASE}/H5Oname.c \
    $${HDF5BASE}/H5Onull.c \
    $${HDF5BASE}/H5Opline.c \
    $${HDF5BASE}/H5Orefcount.c \
    $${HDF5BASE}/H5Osdspace.c \
    $${HDF5BASE}/H5Oshared.c \
    $${HDF5BASE}/H5Oshmesg.c \
    $${HDF5BASE}/H5Ostab.c \
    $${HDF5BASE}/H5Otest.c \
    $${HDF5BASE}/H5Ounknown.c \
    $${HDF5BASE}/H5P.c \
    $${HDF5BASE}/H5PL.c \
    $${HDF5BASE}/H5Pacpl.c \
    $${HDF5BASE}/H5Pdapl.c \
    $${HDF5BASE}/H5Pdcpl.c \
    $${HDF5BASE}/H5Pdeprec.c \
    $${HDF5BASE}/H5Pdxpl.c \
    $${HDF5BASE}/H5Pfapl.c \
    $${HDF5BASE}/H5Pfcpl.c \
    $${HDF5BASE}/H5Pfmpl.c \
    $${HDF5BASE}/H5Pgcpl.c \
    $${HDF5BASE}/H5Pint.c \
    $${HDF5BASE}/H5Plapl.c \
    $${HDF5BASE}/H5Plcpl.c \
    $${HDF5BASE}/H5Pocpl.c \
    $${HDF5BASE}/H5Pocpypl.c \
    $${HDF5BASE}/H5Pstrcpl.c \
    $${HDF5BASE}/H5Ptest.c \
    $${HDF5BASE}/H5R.c \
    $${HDF5BASE}/H5RC.c \
    $${HDF5BASE}/H5RS.c \
    $${HDF5BASE}/H5Rdeprec.c \
    $${HDF5BASE}/H5S.c \
    $${HDF5BASE}/H5SL.c \
    $${HDF5BASE}/H5SM.c \
    $${HDF5BASE}/H5SMbtree2.c \
    $${HDF5BASE}/H5SMcache.c \
    $${HDF5BASE}/H5SMmessage.c \
    $${HDF5BASE}/H5SMtest.c \
    $${HDF5BASE}/H5ST.c \
    $${HDF5BASE}/H5Sall.c \
    $${HDF5BASE}/H5Sdbg.c \
    $${HDF5BASE}/H5Shyper.c \
    $${HDF5BASE}/H5Smpio.c \
    $${HDF5BASE}/H5Snone.c \
    $${HDF5BASE}/H5Spoint.c \
    $${HDF5BASE}/H5Sselect.c \
    $${HDF5BASE}/H5Stest.c \
    $${HDF5BASE}/H5T.c \
    $${HDF5BASE}/H5TS.c \
    $${HDF5BASE}/H5Tarray.c \
    $${HDF5BASE}/H5Tbit.c \
    $${HDF5BASE}/H5Tcommit.c \
    $${HDF5BASE}/H5Tcompound.c \
    $${HDF5BASE}/H5Tconv.c \
    $${HDF5BASE}/H5Tcset.c \
    $${HDF5BASE}/H5Tdbg.c \
    $${HDF5BASE}/H5Tdeprec.c \
    $${HDF5BASE}/H5Tenum.c \
    $${HDF5BASE}/H5Tfields.c \
    $${HDF5BASE}/H5Tfixed.c \
    $${HDF5BASE}/H5Tfloat.c \
    $${HDF5BASE}/H5Tnative.c \
    $${HDF5BASE}/H5Toffset.c \
    $${HDF5BASE}/H5Toh.c \
    $${HDF5BASE}/H5Topaque.c \
    $${HDF5BASE}/H5Torder.c \
    $${HDF5BASE}/H5Tpad.c \
    $${HDF5BASE}/H5Tprecis.c \
    $${HDF5BASE}/H5Tstrpad.c \
    $${HDF5BASE}/H5Tvisit.c \
    $${HDF5BASE}/H5Tvlen.c \
    $${HDF5BASE}/H5V.c \
    $${HDF5BASE}/H5WB.c \
    $${HDF5BASE}/H5Z.c \
    $${HDF5BASE}/H5Zdeflate.c \
    $${HDF5BASE}/H5Zfletcher32.c \
    $${HDF5BASE}/H5Znbit.c \
    $${HDF5BASE}/H5Zscaleoffset.c \
    $${HDF5BASE}/H5Zshuffle.c \
    $${HDF5BASE}/H5Zszip.c \
    $${HDF5BASE}/H5Ztrans.c \
    $${HDF5BASE}/H5checksum.c \
    $${HDF5BASE}/H5dbg.c \
#    $${HDF5BASE}/H5detect.c \
#    $${HDF5BASE}/H5make_libsettings.c \
    $${HDF5BASE}/H5system.c \
    $${HDF5BASE}/H5timer.c \
    $${HDF5BASE}/H5trace.c
