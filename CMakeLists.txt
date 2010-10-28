cmake_minimum_required(VERSION 2.8)
project (qxrd)

find_package(Qt4   COMPONENTS QtCore QtGui QtNetwork QtScript REQUIRED)
find_package(CUDA  REQUIRED)

message(QT: ${QT_USE_FILE})
message(QTLIBS: ${QT_LIBRARY_DIR})
message(BINDIR: ${PROJECT_BINARY_DIR})

include(${QT_USE_FILE})

add_definitions(-DVERSION="0.4.0")

INCLUDE_DIRECTORIES(${PROJECT_BINARY_DIR} ${CMAKE_SOURCE_DIR} qceplib tiff-3.8.2/libtiff qwt-5.2/src)

set(qceplib_SRCS
    qceplib/qspecserver.cpp
    qceplib/qcepproperty.cpp
    qceplib/qcepimagedata.cpp
    qceplib/qcepimagedataformat.cpp
    qceplib/qcepimagedataformattiff.cpp
    qceplib/qcepimagedataformatmar345.cpp
    qceplib/qcepimagedataformatfactory.cpp
)

set(qceplib_HDRS
    qceplib/spec_server.h
    qceplib/qspecserver.h
    qceplib/qcepmacros.h 
    qceplib/qcepproperty.h
    qceplib/qcepimagedata.h
    qceplib/qcepimagedataformat.h
    qceplib/qcepimagedataformattiff.h
    qceplib/qcepimagedataformatmar345.h
    qceplib/qcepimagedataformatfactory.h
)

set(libtiff_SRCS
    tiff-3.8.2/libtiff/tif_aux.c
    tiff-3.8.2/libtiff/tif_close.c
    tiff-3.8.2/libtiff/tif_codec.c
    tiff-3.8.2/libtiff/tif_color.c
    tiff-3.8.2/libtiff/tif_compress.c
    tiff-3.8.2/libtiff/tif_dir.c
    tiff-3.8.2/libtiff/tif_dirinfo.c
    tiff-3.8.2/libtiff/tif_dirread.c
    tiff-3.8.2/libtiff/tif_dirwrite.c
    tiff-3.8.2/libtiff/tif_dumpmode.c
    tiff-3.8.2/libtiff/tif_error.c
    tiff-3.8.2/libtiff/tif_extension.c
    tiff-3.8.2/libtiff/tif_fax3.c
    tiff-3.8.2/libtiff/tif_fax3sm.c
    tiff-3.8.2/libtiff/tif_flush.c
    tiff-3.8.2/libtiff/tif_jpeg.c
    tiff-3.8.2/libtiff/tif_luv.c
    tiff-3.8.2/libtiff/tif_lzw.c
    tiff-3.8.2/libtiff/tif_next.c
    tiff-3.8.2/libtiff/tif_ojpeg.c
    tiff-3.8.2/libtiff/tif_open.c
    tiff-3.8.2/libtiff/tif_packbits.c
    tiff-3.8.2/libtiff/tif_pixarlog.c
    tiff-3.8.2/libtiff/tif_predict.c
    tiff-3.8.2/libtiff/tif_print.c
    tiff-3.8.2/libtiff/tif_read.c
    tiff-3.8.2/libtiff/tif_strip.c
    tiff-3.8.2/libtiff/tif_swab.c
    tiff-3.8.2/libtiff/tif_thunder.c
    tiff-3.8.2/libtiff/tif_tile.c
    tiff-3.8.2/libtiff/tif_version.c
    tiff-3.8.2/libtiff/tif_warning.c
    tiff-3.8.2/libtiff/tif_write.c
    tiff-3.8.2/libtiff/tif_zip.c
    tiff-3.8.2/libtiff/tif_win32.c
)

set(libtiff_HDRS
    tif_config.h
    tiffconf.h
    tiff-3.8.2/libtiff/tiff.h
    tiff-3.8.2/libtiff/tiffio.h
    tiff-3.8.2/libtiff/tiffiop.h
    tiff-3.8.2/libtiff/tiffvers.h
    tiff-3.8.2/libtiff/uvcode.h
    tiff-3.8.2/libtiff/tif_dir.h
    tiff-3.8.2/libtiff/tif_fax3.h
    tiff-3.8.2/libtiff/tif_predict.h
)

set(qwt_SRCS
    qwt-5.2/src/qwt_abstract_scale_draw.cpp
    qwt-5.2/src/qwt_color_map.cpp
    qwt-5.2/src/qwt_clipper.cpp
    qwt-5.2/src/qwt_double_interval.cpp
    qwt-5.2/src/qwt_double_rect.cpp
    qwt-5.2/src/qwt_dyngrid_layout.cpp
    qwt-5.2/src/qwt_layout_metrics.cpp
    qwt-5.2/src/qwt_math.cpp
    qwt-5.2/src/qwt_magnifier.cpp
    qwt-5.2/src/qwt_paint_buffer.cpp
    qwt-5.2/src/qwt_panner.cpp
    qwt-5.2/src/qwt_painter.cpp
    qwt-5.2/src/qwt_picker.cpp
    qwt-5.2/src/qwt_round_scale_draw.cpp
    qwt-5.2/src/qwt_scale_div.cpp
    qwt-5.2/src/qwt_scale_draw.cpp
    qwt-5.2/src/qwt_scale_map.cpp
    qwt-5.2/src/qwt_spline.cpp
    qwt-5.2/src/qwt_text_engine.cpp
    qwt-5.2/src/qwt_text_label.cpp
    qwt-5.2/src/qwt_text.cpp
    qwt-5.2/src/qwt_event_pattern.cpp
    qwt-5.2/src/qwt_picker_machine.cpp
    qwt-5.2/src/qwt_scale_engine.cpp
    qwt-5.2/src/qwt_symbol.cpp
    qwt-5.2/src/qwt_curve_fitter.cpp
    qwt-5.2/src/qwt_data.cpp
    qwt-5.2/src/qwt_interval_data.cpp
    qwt-5.2/src/qwt_legend.cpp
    qwt-5.2/src/qwt_legend_item.cpp
    qwt-5.2/src/qwt_plot.cpp
    qwt-5.2/src/qwt_plot_print.cpp
    qwt-5.2/src/qwt_plot_xml.cpp
    qwt-5.2/src/qwt_plot_axis.cpp
    qwt-5.2/src/qwt_plot_curve.cpp
    qwt-5.2/src/qwt_plot_dict.cpp
    qwt-5.2/src/qwt_plot_grid.cpp
    qwt-5.2/src/qwt_plot_item.cpp
    qwt-5.2/src/qwt_plot_spectrogram.cpp
    qwt-5.2/src/qwt_plot_scaleitem.cpp
    qwt-5.2/src/qwt_plot_marker.cpp
    qwt-5.2/src/qwt_plot_layout.cpp
    qwt-5.2/src/qwt_plot_printfilter.cpp
    qwt-5.2/src/qwt_plot_rasteritem.cpp
    qwt-5.2/src/qwt_plot_canvas.cpp
    qwt-5.2/src/qwt_plot_rescaler.cpp
    qwt-5.2/src/qwt_plot_panner.cpp
    qwt-5.2/src/qwt_plot_picker.cpp
    qwt-5.2/src/qwt_plot_zoomer.cpp
    qwt-5.2/src/qwt_plot_magnifier.cpp
    qwt-5.2/src/qwt_raster_data.cpp
    qwt-5.2/src/qwt_scale_widget.cpp
    qwt-5.2/src/qwt_abstract_slider.cpp
    qwt-5.2/src/qwt_abstract_scale.cpp
    qwt-5.2/src/qwt_arrow_button.cpp
    qwt-5.2/src/qwt_analog_clock.cpp
    qwt-5.2/src/qwt_compass.cpp
    qwt-5.2/src/qwt_compass_rose.cpp
    qwt-5.2/src/qwt_counter.cpp
    qwt-5.2/src/qwt_dial.cpp
    qwt-5.2/src/qwt_dial_needle.cpp
    qwt-5.2/src/qwt_double_range.cpp
    qwt-5.2/src/qwt_knob.cpp
    qwt-5.2/src/qwt_slider.cpp
    qwt-5.2/src/qwt_thermo.cpp
    qwt-5.2/src/qwt_wheel.cpp
)

set(qwt_HDRS
    qwt-5.2/src/qwt.h
    qwt-5.2/src/qwt_abstract_scale_draw.h
    qwt-5.2/src/qwt_array.h
    qwt-5.2/src/qwt_color_map.h
    qwt-5.2/src/qwt_clipper.h
    qwt-5.2/src/qwt_double_interval.h
    qwt-5.2/src/qwt_double_rect.h
    qwt-5.2/src/qwt_dyngrid_layout.h
    qwt-5.2/src/qwt_global.h
    qwt-5.2/src/qwt_layout_metrics.h
    qwt-5.2/src/qwt_math.h
    qwt-5.2/src/qwt_magnifier.h
    qwt-5.2/src/qwt_paint_buffer.h
    qwt-5.2/src/qwt_painter.h
    qwt-5.2/src/qwt_panner.h
    qwt-5.2/src/qwt_picker.h
    qwt-5.2/src/qwt_picker_machine.h
    qwt-5.2/src/qwt_polygon.h
    qwt-5.2/src/qwt_round_scale_draw.h
    qwt-5.2/src/qwt_scale_div.h
    qwt-5.2/src/qwt_scale_draw.h
    qwt-5.2/src/qwt_scale_engine.h
    qwt-5.2/src/qwt_scale_map.h
    qwt-5.2/src/qwt_spline.h
    qwt-5.2/src/qwt_symbol.h
    qwt-5.2/src/qwt_text_engine.h
    qwt-5.2/src/qwt_text_label.h
    qwt-5.2/src/qwt_text.h
    qwt-5.2/src/qwt_valuelist.h
    qwt-5.2/src/qwt_curve_fitter.h
    qwt-5.2/src/qwt_data.h
    qwt-5.2/src/qwt_event_pattern.h
    qwt-5.2/src/qwt_interval_data.h
    qwt-5.2/src/qwt_legend.h
    qwt-5.2/src/qwt_legend_item.h
    qwt-5.2/src/qwt_legend_itemmanager.h
    qwt-5.2/src/qwt_plot.h
    qwt-5.2/src/qwt_plot_curve.h
    qwt-5.2/src/qwt_plot_dict.h
    qwt-5.2/src/qwt_plot_grid.h
    qwt-5.2/src/qwt_plot_item.h
    qwt-5.2/src/qwt_plot_layout.h
    qwt-5.2/src/qwt_plot_marker.h
    qwt-5.2/src/qwt_plot_printfilter.h
    qwt-5.2/src/qwt_plot_rasteritem.h
    qwt-5.2/src/qwt_plot_spectrogram.h
    qwt-5.2/src/qwt_plot_scaleitem.h
    qwt-5.2/src/qwt_plot_canvas.h
    qwt-5.2/src/qwt_plot_rescaler.h
    qwt-5.2/src/qwt_plot_panner.h
    qwt-5.2/src/qwt_plot_picker.h
    qwt-5.2/src/qwt_plot_zoomer.h
    qwt-5.2/src/qwt_plot_magnifier.h
    qwt-5.2/src/qwt_raster_data.h
    qwt-5.2/src/qwt_scale_widget.h
    qwt-5.2/src/qwt_abstract_slider.h
    qwt-5.2/src/qwt_abstract_scale.h
    qwt-5.2/src/qwt_arrow_button.h
    qwt-5.2/src/qwt_analog_clock.h
    qwt-5.2/src/qwt_compass.h
    qwt-5.2/src/qwt_compass_rose.h
    qwt-5.2/src/qwt_counter.h
    qwt-5.2/src/qwt_dial.h
    qwt-5.2/src/qwt_dial_needle.h
    qwt-5.2/src/qwt_double_range.h
    qwt-5.2/src/qwt_knob.h
    qwt-5.2/src/qwt_slider.h
    qwt-5.2/src/qwt_thermo.h
    qwt-5.2/src/qwt_wheel.h
)

set(qxrd_SRCS
    qxrd.cpp
    qxrdapplication.cpp
    qxrdsettings.cpp
    qxrdimagedata.cpp
    qxrdimagequeue.cpp
    qxrdimagedataformathis.cpp
    qxrdimagedataformattiff.cpp
    qxrdimagedataformatmar345.cpp
    qxrdrasterdata.cpp
    qxrdmaskrasterdata.cpp
    qxrdmaskcolormap.cpp
    qxrdwindow.cpp
    qxrddataprocessor.cpp
    qxrdcenterfinder.cpp
    qxrdcenterfinderplot.cpp
    qxrdcenterfinderdialog.cpp
    qxrdcenterfinderpicker.cpp
    qxrdintegrator.cpp
    qxrdintegratorplot.cpp
    qxrdintegratordialog.cpp
    qxrdimageplot.cpp
    qxrdplotzoomer.cpp
    qxrdplottracker.cpp
    qxrdserver.cpp
    qxrdserverthread.cpp
    qxrdspecserver.cpp
    qxrdacquisitionthread.cpp
    qxrdacquisition.cpp
    qxrdacquisitionparameters.cpp
    qxrdacquisitionscripting.cpp
    qxrdacquisitionoperations.cpp
    qxrdacquisitionperkinelmer.cpp
    qxrdplot.cpp
    qxrdplotslicer.cpp
    qxrdplotmeasurer.cpp
    qxrdscriptengine.cpp
    qxrdscriptenginethread.cpp
    qxrddataprocessorthread.cpp
    qxrdacquisitionsimulated.cpp
    qxrdmaskpicker.cpp
    qxrdmaskdata.cpp
    qxrdimageplotmeasurer.cpp
    qxrdfilebrowser.cpp
    qxrdimagecalculator.cpp
    qxrdmutexlocker.cpp
    xisl_dummy.cpp
)

set(qxrd_HDRS
    TODO.h
    qxrdapplication.h
    qxrdsettings.h
    qxrdimagedata.h
    qxrdimagequeue.h
    qxrdimagedataformathis.h
    qxrdimagedataformattiff.h
    qxrdimagedataformatmar345.h
    qxrdrasterdata.h
    qxrdmaskrasterdata.h
    qxrdmaskcolormap.h
    qxrdwindow.h
    qxrddataprocessor.h
    qxrdcenterfinder.h
    qxrdcenterfinderplot.h
    qxrdcenterfinderdialog.h
    qxrdcenterfinderpicker.h
    qxrdintegrator.h
    qxrdintegratorplot.h
    qxrdintegratordialog.h
    qxrdimageplot.h
    qxrdplotzoomer.h
    qxrdplottracker.h
    qxrdserver.h
    qxrdserverthread.h
    qxrdspecserver.h
    qxrdacquisitionthread.h
    qxrdacquisition.h
    Acq.h
    qxrdacquisitionparameters.h
    qxrdacquisitionscripting.h
    qxrdacquisitionoperations.h
    qxrdacquisitionperkinelmer.h
    qxrdplot.h
    qxrdplotslicer.h
    qxrdplotmeasurer.h
    qxrdscriptengine.h
    qxrdscriptenginethread.h
    qxrddataprocessorthread.h
    qxrdacquisitionsimulated.h
    qxrdmaskpicker.h
    qxrdmaskdata.h
    qxrdimageplotmeasurer.h
    qxrdfilebrowser.h
    qxrdimagecalculator.h
    qxrdmutexlocker.h
    xisl_dummy.h
)

set(qxrd_UI
    qxrdwindow.ui
    qxrdcenterfinderdialog.ui
    qxrdintegratordialog.ui
    qxrdfilebrowser.ui
    qxrdimagecalculator.ui
)

set(qxrd_RSRC
	qxrdhelptext.qrc
	qxrdresources.qrc
)

set(qxrd_CUS
)

QT4_WRAP_CPP(qwt_MOC  ${qwt_HDRS})
QT4_WRAP_CPP(qceplib_MOC ${qceplib_HDRS})
QT4_WRAP_CPP(qxrd_MOC ${qxrd_HDRS})
QT4_WRAP_UI(qxrd_UIS   ${qxrd_UI})
QT4_ADD_RESOURCES(qxrd_RSRCS ${qxrd_RSRC})

message(MOC: ${qwt_MOC})
message(BIN: ${CMAKE_CURRENT_BINARY_DIR})

CUDA_ADD_EXECUTABLE(qxrd 
	${qceplib_SRCS} 
	${qwt_SRCS} 
	${libtiff_SRCS} 
	${qxrd_SRCS}
	${qxrd_CUS}
	${qceplib_MOC} 
	${qwt_MOC} 
	${qxrd_MOC}
	${qxrd_RSRCS}
	${qxrd_UIS})

target_link_libraries(qxrd ${QT_LIBRARIES})

file (INSTALL ${QT_BINARY_DIR}/QtCore4.dll DESTINATION Release)
file (INSTALL ${QT_BINARY_DIR}/QtGui4.dll  DESTINATION Release)
file (INSTALL ${QT_BINARY_DIR}/QtNetwork4.dll DESTINATION Release)
file (INSTALL ${QT_BINARY_DIR}/QtScript4.dll  DESTINATION Release)

file (INSTALL ${QT_BINARY_DIR}/QtCored4.dll DESTINATION Debug)
file (INSTALL ${QT_BINARY_DIR}/QtGuid4.dll  DESTINATION Debug)
file (INSTALL ${QT_BINARY_DIR}/QtNetworkd4.dll DESTINATION Debug)
file (INSTALL ${QT_BINARY_DIR}/QtScriptd4.dll  DESTINATION Debug)

install(TARGETS qxrd DESTINATION bin)

install(FILES	${QT_BINARY_DIR}/QtCore4.dll 
				${QT_BINARY_DIR}/QtGui4.dll 
				${QT_BINARY_DIR}/QtNetwork4.dll 
				${QT_BINARY_DIR}/QtScript4.dll
		DESTINATION bin)
		
include(CPack)
