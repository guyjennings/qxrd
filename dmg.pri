message(dmg $${TARGET} $${PWD})

#dmg.target   = dmg
#dmg.depends  = $${TARGET}.app
#dmg.commands  = touch $$dmg.target &&
dmg.commands += mkdir -p ../$${TARGET}.app/Contents/Frameworks/ &&
dmg.commands += cp ../lib*.dylib ../$${TARGET}.app/Contents/Frameworks/ &&
dmg.commands += install_name_tool -change "libqceplib.1.dylib" "@rpath/libqceplib.1.dylib" ../$${TARGET}.app/Contents/MacOS/$${TARGET} &&
dmg.commands += install_name_tool -change "libqceplib.1.dylib" "@rpath/libqceplib.1.dylib" ../$${TARGET}.app/Contents/Frameworks/libqxrdlib.0.dylib &&
dmg.commands += install_name_tool -change "libqxrdlib.0.dylib" "@rpath/libqxrdlib.0.dylib" ../$${TARGET}.app/Contents/MacOS/$${TARGET} &&
dmg.commands += mkdir -p ../$${TARGET}.app/Contents/MacOS/plugins/ &&
dmg.commands += cp ../plugins/lib*.dylib ../$${TARGET}.app/Contents/MacOS/plugins/ &&
dmg.commands += for f in ../$${TARGET}.app/Contents/MacOS/plugins/lib*.dylib ; do
dmg.commands += install_name_tool -change "libqceplib.1.dylib" "@rpath/libqceplib.1.dylib" "\$\$f" ;
dmg.commands += install_name_tool -change "libqxrdlib.0.dylib" "@rpath/libqxrdlib.0.dylib" "\$\$f" ;
dmg.commands += done &&
dmg.commands += rm -rf ../$${TARGET}.app/Contents/Plugins/ &&
dmg.commands += rm -rf ../$${TARGET}.app/Contents/Resources/qt.conf &&
dmg.commands += $$[QT_INSTALL_BINS]/macdeployqt ../$${TARGET}.app -dmg &&
dmg.commands += rm -rf ../$${TARGET}-$${VERSION}.dmg &&
dmg.commands += mv ../$${TARGET}.dmg ../$${TARGET}-$${VERSION}.dmg

QMAKE_EXTRA_TARGETS += dmg
