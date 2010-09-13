QMAKE_EXTRA_TARGETS += nsis

nsis.depends = install

nsis.commands = makensis.exe /V3 /DVERSION=$${VERSION} /DPREFIX="\"$${QXRDSUFFIX}\"" /DPREFIXSTR="\"$${QXRDSUFFIXSTR}\"" ../qxrd.nsi
