#ifndef QXRDFORWARDTYPES_H
#define QXRDFORWARDTYPES_H

#include <QSharedPointer>

class QxrdApplication;             typedef QxrdApplication *QxrdApplicationPtr;

class QxrdAcquisitionThread;       typedef QxrdAcquisitionThread *QxrdAcquisitionThreadPtr;
class QxrdAcquisition;             typedef QxrdAcquisition *QxrdAcquisitionPtr;

class QxrdDataProcessorThread;     typedef QxrdDataProcessorThread *QxrdDataProcessorThreadPtr;
class QxrdDataProcessor;           typedef QxrdDataProcessor *QxrdDataProcessorPtr;

class QxrdAllocatorThread;         typedef QxrdAllocatorThread *QxrdAllocatorThreadPtr;
class QxrdAllocator;               typedef QxrdAllocator *QxrdAllocatorPtr;

class QxrdFileSaverThread;         typedef QxrdFileSaverThread *QxrdFileSaverThreadPtr;
class QxrdFileSaver;               typedef QxrdFileSaver *QxrdFileSaverPtr;

class QxrdSettingsSaverThread;     typedef QxrdSettingsSaverThread *QxrdSettingsSaverThreadPtr;
class QxrdSettingsSaver;           typedef QxrdSettingsSaver *QxrdSettingsSaverPtr;

class QxrdServerThread;            typedef QxrdServerThread *QxrdServerThreadPtr;
class QxrdServer;                  typedef QxrdServer *QxrdServerPtr;

class QxrdSimpleServerThread;      typedef QxrdSimpleServerThread *QxrdSimpleServerThreadPtr;
class QxrdSimpleServer;            typedef QxrdSimpleServer *QxrdSimpleServerPtr;

class QxrdScriptEngineThread;      typedef QxrdScriptEngineThread *QxrdScriptEngineThreadPtr;
class QxrdScriptEngine;            typedef QxrdScriptEngine *QxrdScriptEnginePtr;
class QScriptEngine;               typedef QScriptEngine *QScriptEnginePtr;

class QxrdWindow;                  typedef QxrdWindow *QxrdWindowPtr;

class QxrdImageCalculator;         typedef QxrdImageCalculator *QxrdImageCalculatorPtr;

class QwtPlotCanvas;               typedef QwtPlotCanvas *QwtPlotCanvasPtr;
class QwtPlotCurve;                typedef QwtPlotCurve *QwtPlotCurvePtr;
class QwtPlotSpectrogram;          typedef QwtPlotSpectrogram *QwtPlotSpectrogramPtr;
class QwtPlotMarker;               typedef QwtPlotMarker *QwtPlotMarkerPtr;
class QxrdPlot;                    typedef QxrdPlot *QxrdPlotPtr;
class QxrdPlotMeasurer;            typedef QxrdPlotMeasurer *QxrdPlotMeasurerPtr;
class QxrdPlotSlicer;              typedef QxrdPlotSlicer *QxrdPlotSlicerPtr;
class QwtPlotZoomer;               typedef QwtPlotZoomer *QwtPlotZoomerPtr;
class QxrdPlotZoomer;              typedef QxrdPlotZoomer *QxrdPlotZoomerPtr;

class QwtPlotPanner;               typedef QwtPlotPanner *QwtPlotPannerPtr;
class QwtPlotMagnifier;            typedef QwtPlotMagnifier *QwtPlotMagnifierPtr;
class QwtPlotRescaler;             typedef QwtPlotRescaler *QwtPlotRescalerPtr;
class QwtLegend;                   typedef QwtLegend *QwtLegendPtr;

class QxrdImagePlot;               typedef QxrdImagePlot *QxrdImagePlotPtr;
class QxrdImagePlotMeasurer;       typedef QxrdImagePlotMeasurer *QxrdImagePlotMeasurerPtr;

class QxrdIntegrator;              typedef QxrdIntegrator *QxrdIntegratorPtr;
class QxrdIntegratorDialog;        typedef QxrdIntegratorDialog *QxrdIntegratorDialogPtr;

class QxrdAcquireDialog;           typedef QxrdAcquireDialog *QxrdAcquireDialogPtr;

class QxrdCenterFinder;            typedef QxrdCenterFinder *QxrdCenterFinderPtr;
class QxrdCenterFinderDialog;      typedef QxrdCenterFinderDialog *QxrdCenterFinderDialogPtr;
class QxrdCenterFinderPlot;        typedef QxrdCenterFinderPlot *QxrdCenterFinderPlotPtr;
class QxrdCenterFinderPicker;      typedef QxrdCenterFinderPicker *QxrdCenterFinderPickerPtr;

class QxrdMaskDialog;              typedef QxrdMaskDialog *QxrdMaskDialogPtr;

class QxrdMaskPicker;              typedef QxrdMaskPicker *QxrdMaskPickerPtr;
class QxrdCircularMaskPicker;      typedef QxrdCircularMaskPicker *QxrdCircularMaskPickerPtr;
class QxrdPolygonalMaskPicker;     typedef QxrdPolygonalMaskPicker *QxrdPolygonalMaskPickerPtr;

class QxrdPowderFitDialog;         typedef QxrdPowderFitDialog *QxrdPowderFitDialogPtr;

class QLabel;                      typedef QLabel *QLabelPtr;
class QProgressBar;                typedef QProgressBar *QProgressBarPtr;
class QFileSystemModel;            typedef QFileSystemModel *QFileSystemModelPtr;

class QxrdGenerateTestImage;       typedef QxrdGenerateTestImage *QxrdGenerateTestImagePtr;

//class QxrdMaskData;                typedef QSharedPointer<QxrdMaskData>             QxrdMaskDataPtr;

#endif // QXRDFORWARDTYPES_H
