win32 {
  INCLUDEPATH += ../qxrd/qwt-5.2/src/

  HEADERS += \
    ../qxrd/qwt-5.2/src/qwt.h \
    ../qxrd/qwt-5.2/src/qwt_abstract_scale_draw.h \
    ../qxrd/qwt-5.2/src/qwt_array.h \
    ../qxrd/qwt-5.2/src/qwt_color_map.h \
    ../qxrd/qwt-5.2/src/qwt_clipper.h \
    ../qxrd/qwt-5.2/src/qwt_double_interval.h \
    ../qxrd/qwt-5.2/src/qwt_double_rect.h \
    ../qxrd/qwt-5.2/src/qwt_dyngrid_layout.h \
    ../qxrd/qwt-5.2/src/qwt_global.h \
    ../qxrd/qwt-5.2/src/qwt_layout_metrics.h \
    ../qxrd/qwt-5.2/src/qwt_math.h \
    ../qxrd/qwt-5.2/src/qwt_magnifier.h \
    ../qxrd/qwt-5.2/src/qwt_paint_buffer.h \
    ../qxrd/qwt-5.2/src/qwt_painter.h \
    ../qxrd/qwt-5.2/src/qwt_panner.h \
    ../qxrd/qwt-5.2/src/qwt_picker.h \
    ../qxrd/qwt-5.2/src/qwt_picker_machine.h \
    ../qxrd/qwt-5.2/src/qwt_polygon.h \
    ../qxrd/qwt-5.2/src/qwt_round_scale_draw.h \
    ../qxrd/qwt-5.2/src/qwt_scale_div.h \
    ../qxrd/qwt-5.2/src/qwt_scale_draw.h \
    ../qxrd/qwt-5.2/src/qwt_scale_engine.h \
    ../qxrd/qwt-5.2/src/qwt_scale_map.h \
    ../qxrd/qwt-5.2/src/qwt_spline.h \
    ../qxrd/qwt-5.2/src/qwt_symbol.h \
    ../qxrd/qwt-5.2/src/qwt_text_engine.h \
    ../qxrd/qwt-5.2/src/qwt_text_label.h \
    ../qxrd/qwt-5.2/src/qwt_text.h \
    ../qxrd/qwt-5.2/src/qwt_valuelist.h

  SOURCES += \
    ../qxrd/qwt-5.2/src/qwt_abstract_scale_draw.cpp \
    ../qxrd/qwt-5.2/src/qwt_color_map.cpp \
    ../qxrd/qwt-5.2/src/qwt_clipper.cpp \
    ../qxrd/qwt-5.2/src/qwt_double_interval.cpp \
    ../qxrd/qwt-5.2/src/qwt_double_rect.cpp \
    ../qxrd/qwt-5.2/src/qwt_dyngrid_layout.cpp \
    ../qxrd/qwt-5.2/src/qwt_layout_metrics.cpp \
    ../qxrd/qwt-5.2/src/qwt_math.cpp \
    ../qxrd/qwt-5.2/src/qwt_magnifier.cpp \
    ../qxrd/qwt-5.2/src/qwt_paint_buffer.cpp \
    ../qxrd/qwt-5.2/src/qwt_panner.cpp \
    ../qxrd/qwt-5.2/src/qwt_painter.cpp \
    ../qxrd/qwt-5.2/src/qwt_picker.cpp \
    ../qxrd/qwt-5.2/src/qwt_round_scale_draw.cpp \
    ../qxrd/qwt-5.2/src/qwt_scale_div.cpp \
    ../qxrd/qwt-5.2/src/qwt_scale_draw.cpp \
    ../qxrd/qwt-5.2/src/qwt_scale_map.cpp \
    ../qxrd/qwt-5.2/src/qwt_spline.cpp \
    ../qxrd/qwt-5.2/src/qwt_text_engine.cpp \
    ../qxrd/qwt-5.2/src/qwt_text_label.cpp \
    ../qxrd/qwt-5.2/src/qwt_text.cpp \
    ../qxrd/qwt-5.2/src/qwt_event_pattern.cpp \
    ../qxrd/qwt-5.2/src/qwt_picker_machine.cpp \
    ../qxrd/qwt-5.2/src/qwt_scale_engine.cpp \
    ../qxrd/qwt-5.2/src/qwt_symbol.cpp


  HEADERS += \
        ../qxrd/qwt-5.2/src/qwt_curve_fitter.h \
        ../qxrd/qwt-5.2/src/qwt_data.h \
        ../qxrd/qwt-5.2/src/qwt_event_pattern.h \
        ../qxrd/qwt-5.2/src/qwt_interval_data.h \
        ../qxrd/qwt-5.2/src/qwt_legend.h \
        ../qxrd/qwt-5.2/src/qwt_legend_item.h \
        ../qxrd/qwt-5.2/src/qwt_legend_itemmanager.h \
        ../qxrd/qwt-5.2/src/qwt_plot.h \
        ../qxrd/qwt-5.2/src/qwt_plot_curve.h \
        ../qxrd/qwt-5.2/src/qwt_plot_dict.h \
        ../qxrd/qwt-5.2/src/qwt_plot_grid.h \
        ../qxrd/qwt-5.2/src/qwt_plot_item.h \
        ../qxrd/qwt-5.2/src/qwt_plot_layout.h \
        ../qxrd/qwt-5.2/src/qwt_plot_marker.h \
        ../qxrd/qwt-5.2/src/qwt_plot_printfilter.h \
        ../qxrd/qwt-5.2/src/qwt_plot_rasteritem.h \
        ../qxrd/qwt-5.2/src/qwt_plot_spectrogram.h \
        ../qxrd/qwt-5.2/src/qwt_plot_scaleitem.h \
        ../qxrd/qwt-5.2/src/qwt_plot_canvas.h \
        ../qxrd/qwt-5.2/src/qwt_plot_rescaler.h \
        ../qxrd/qwt-5.2/src/qwt_plot_panner.h \
        ../qxrd/qwt-5.2/src/qwt_plot_picker.h \
        ../qxrd/qwt-5.2/src/qwt_plot_zoomer.h \
        ../qxrd/qwt-5.2/src/qwt_plot_magnifier.h \
        ../qxrd/qwt-5.2/src/qwt_raster_data.h \
        ../qxrd/qwt-5.2/src/qwt_scale_widget.h

  SOURCES += \
        ../qxrd/qwt-5.2/src/qwt_curve_fitter.cpp \
        ../qxrd/qwt-5.2/src/qwt_data.cpp \
        ../qxrd/qwt-5.2/src/qwt_interval_data.cpp \
        ../qxrd/qwt-5.2/src/qwt_legend.cpp \
        ../qxrd/qwt-5.2/src/qwt_legend_item.cpp \
        ../qxrd/qwt-5.2/src/qwt_plot.cpp \
        ../qxrd/qwt-5.2/src/qwt_plot_print.cpp \
        ../qxrd/qwt-5.2/src/qwt_plot_xml.cpp \
        ../qxrd/qwt-5.2/src/qwt_plot_axis.cpp \
        ../qxrd/qwt-5.2/src/qwt_plot_curve.cpp \
        ../qxrd/qwt-5.2/src/qwt_plot_dict.cpp \
        ../qxrd/qwt-5.2/src/qwt_plot_grid.cpp \
        ../qxrd/qwt-5.2/src/qwt_plot_item.cpp \
        ../qxrd/qwt-5.2/src/qwt_plot_spectrogram.cpp \
        ../qxrd/qwt-5.2/src/qwt_plot_scaleitem.cpp \
        ../qxrd/qwt-5.2/src/qwt_plot_marker.cpp \
        ../qxrd/qwt-5.2/src/qwt_plot_layout.cpp \
        ../qxrd/qwt-5.2/src/qwt_plot_printfilter.cpp \
        ../qxrd/qwt-5.2/src/qwt_plot_rasteritem.cpp \
        ../qxrd/qwt-5.2/src/qwt_plot_canvas.cpp \
        ../qxrd/qwt-5.2/src/qwt_plot_rescaler.cpp \
        ../qxrd/qwt-5.2/src/qwt_plot_panner.cpp \
        ../qxrd/qwt-5.2/src/qwt_plot_picker.cpp \
        ../qxrd/qwt-5.2/src/qwt_plot_zoomer.cpp \
        ../qxrd/qwt-5.2/src/qwt_plot_magnifier.cpp \
        ../qxrd/qwt-5.2/src/qwt_raster_data.cpp \
        ../qxrd/qwt-5.2/src/qwt_scale_widget.cpp

    HEADERS += \
        ../qxrd/qwt-5.2/src/qwt_abstract_slider.h \
        ../qxrd/qwt-5.2/src/qwt_abstract_scale.h \
        ../qxrd/qwt-5.2/src/qwt_arrow_button.h \
        ../qxrd/qwt-5.2/src/qwt_analog_clock.h \
        ../qxrd/qwt-5.2/src/qwt_compass.h \
        ../qxrd/qwt-5.2/src/qwt_compass_rose.h \
        ../qxrd/qwt-5.2/src/qwt_counter.h \
        ../qxrd/qwt-5.2/src/qwt_dial.h \
        ../qxrd/qwt-5.2/src/qwt_dial_needle.h \
        ../qxrd/qwt-5.2/src/qwt_double_range.h \
        ../qxrd/qwt-5.2/src/qwt_knob.h \
        ../qxrd/qwt-5.2/src/qwt_slider.h \
        ../qxrd/qwt-5.2/src/qwt_thermo.h \
        ../qxrd/qwt-5.2/src/qwt_wheel.h

    SOURCES += \
        ../qxrd/qwt-5.2/src/qwt_abstract_slider.cpp \
        ../qxrd/qwt-5.2/src/qwt_abstract_scale.cpp \
        ../qxrd/qwt-5.2/src/qwt_arrow_button.cpp \
        ../qxrd/qwt-5.2/src/qwt_analog_clock.cpp \
        ../qxrd/qwt-5.2/src/qwt_compass.cpp \
        ../qxrd/qwt-5.2/src/qwt_compass_rose.cpp \
        ../qxrd/qwt-5.2/src/qwt_counter.cpp \
        ../qxrd/qwt-5.2/src/qwt_dial.cpp \
        ../qxrd/qwt-5.2/src/qwt_dial_needle.cpp \
        ../qxrd/qwt-5.2/src/qwt_double_range.cpp \
        ../qxrd/qwt-5.2/src/qwt_knob.cpp \
        ../qxrd/qwt-5.2/src/qwt_slider.cpp \
        ../qxrd/qwt-5.2/src/qwt_thermo.cpp \
        ../qxrd/qwt-5.2/src/qwt_wheel.cpp

}
