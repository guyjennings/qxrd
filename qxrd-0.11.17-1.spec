Name:		qxrd-qt5
Version: 0.11.17
Release:        1%{?dist}
Summary:        qxrd - a readout and control program for perkin elmer x ray detector

Group:       	Applications/Engineering   
License:        GPL
URL:            http://www.bessrc.aps.anl.gov/software/qxrd/index.html
#Source0:        http://www.bessrc.aps.anl.gov/software/qxrd/%{name}-%{version}.tar.gz
Source0:        http://www.bessrc.aps.anl.gov/software/qxrd/qxrd-%{version}.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

Obsoletes:      qxrd

BuildRequires:  desktop-file-utils
#BuildRequires:  qt4, qt4-x11 qt4-devel
BuildRequires:  qt5-qtbase-devel, qt5-qtscript-devel, qt5-qtwebkit-devel, qt5-qtsvg-devel
BuildRequires:  libtiff-devel, CBFlib-devel
#BuildRequires:  hdf5-devel
#Requires:       qt4, qt4-x11, libtiff, CBFlib
Requires:       qt5-qtbase, qt5-qtscript, qt5-qtwebkit, qt5-qtsvg
#Requires:       hdf5
Requires(post): desktop-file-utils
Requires(postun): desktop-file-utils

%description
qxrd is a readout and control program for perkin elmer x ray detector

%prep
#%setup -n %{name}-%{version}
%setup -n qxrd-%{version}

%build
#/usr/bin/qmake-qt4 %{name}.pro -recursive "QXRD_PLUGINS_DIR=%{_libdir}/%{name}-%{version}"
#/usr/bin/qmake-qt5 %{name}.pro -recursive "QXRD_PLUGINS_DIR=%{_libdir}/%{name}-%{version}"
/usr/bin/qmake-qt5 qxrd.pro -recursive "QXRD_PLUGINS_DIR=%{_libdir}/qxrd-%{version}"
make clean
make %{?_smp_mflags}


%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT%{_bindir}
#mkdir -p $RPM_BUILD_ROOT%{_libdir}/%{name}-%{version}
#mkdir -p $RPM_BUILD_ROOT%{_libdir}/%{name}-%{version}/plugins/
mkdir -p $RPM_BUILD_ROOT%{_libdir}/qxrd-%{version}
mkdir -p $RPM_BUILD_ROOT%{_libdir}/qxrd-%{version}/plugins/
mkdir -p $RPM_BUILD_ROOT%{_datadir}/icons/hicolor/32x32/apps/
mkdir -p $RPM_BUILD_ROOT%{_datadir}/icons/hicolor/64x64/apps/
mkdir -p $RPM_BUILD_ROOT%{_datadir}/icons/hicolor/128x128/apps/
mkdir -p $RPM_BUILD_ROOT%{_datadir}/icons/hicolor/256x256/apps/

install qxrd $RPM_BUILD_ROOT%{_bindir}
#install plugins/*.so  $RPM_BUILD_ROOT%{_libdir}/%{name}-%{version}/plugins/
install plugins/*.so  $RPM_BUILD_ROOT%{_libdir}/qxrd-%{version}/plugins/

cp source/images/qxrd-icon-32x32.png $RPM_BUILD_ROOT%{_datadir}/icons/hicolor/32x32/apps/qxrd-icon.png
cp source/images/qxrd-icon-64x64.png $RPM_BUILD_ROOT%{_datadir}/icons/hicolor/64x64/apps/qxrd-icon.png
cp source/images/qxrd-icon-128x128.png $RPM_BUILD_ROOT%{_datadir}/icons/hicolor/128x128/apps/qxrd-icon.png
cp source/images/qxrd-icon-256x256.png $RPM_BUILD_ROOT%{_datadir}/icons/hicolor/256x256/apps/qxrd-icon.png

desktop-file-install --vendor=bessrc \
        --dir $RPM_BUILD_ROOT%{_datadir}/applications \
        qxrd.desktop

%clean
rm -rf $RPM_BUILD_ROOT

%post
update-desktop-database &> /dev/null || :
touch --no-create %{_datadir}/icons/hicolor || :
%{_bindir}/gtk-update-icon-cache --quiet %{_datadir}/icons/hicolor || :

%postun
update-desktop-database &> /dev/null || :
touch --no-create %{_datadir}/icons/hicolor || :
%{_bindir}/gtk-update-icon-cache --quiet %{_datadir}/icons/hicolor || :


%files
%defattr(-,root,root,-)
%doc
#%{_bindir}/%{name}
#%{_libdir}/%{name}-%{version}/plugins/*.so
%{_bindir}/qxrd
%{_libdir}/qxrd-%{version}/plugins/*.so
#%{_datadir}/applications/*%{name}.desktop
%{_datadir}/applications/*qxrd.desktop
%{_datadir}/icons/hicolor/*/*/*

%changelog
* Fri Oct 10 2008  <jennings@weimar.bessrc.aps.anl.gov> - 0.4.10-3
- added extra 'mock' and 'rpmsource' targets to project file

* Mon Jul  2 2007  Guy Jennings <jennings@anl.gov> 0.0.1-0.cep.1
- Initial RPM Release
