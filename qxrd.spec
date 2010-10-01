Name:		qxrd
Version: 0.5.2
Release:        1%{?dist}
Summary:        qxrd - a readout and control program for perkin elmer x ray detector

%if 0%{?fedora} >= 8
%define qtversion 4.3.4
%else
%if 0%{?fedora} >= 7
%define qtversion 4.3.3
%else
%define qtversion 4.3.2
%endif
%endif

Group:       	Applications/Engineering   
License:        GPL
URL:            http://www.bessrc.aps.anl.gov/software/qxrd/index.html
Source0:        http://www.bessrc.aps.anl.gov/software/qxrd/%{name}-%{version}.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires:  desktop-file-utils qt4 >= %{qtversion}, qt4-x11 qt4-devel 
BuildRequires:  libtiff-devel
#BuildRequires:  epics-base-devel, epics-base, acqiris
Requires:       qt4 >= %{qtversion}, qt4-x11, libtiff
#Requires:       epics-base, acqiris
Requires(post): desktop-file-utils
Requires(postun): desktop-file-utils

%description
qxrd is a readout and control program for perkin elmer x ray detector

%prep
%setup -n %{name}-%{version}

%build
/usr/bin/qmake-qt4 %{name}.pro -o Makefile "QXRD_PLUGINS_DIR=%{_libdir}/%{name}-%{version}"
make clean
make %{?_smp_mflags}


%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT%{_bindir}
mkdir -p $RPM_BUILD_ROOT%{_libdir}/%{name}-%{version}
mkdir -p $RPM_BUILD_ROOT%{_libdir}/%{name}-%{version}/plugins/

install source/qxrd $RPM_BUILD_ROOT%{_bindir}
install plugins/*/*.so  $RPM_BUILD_ROOT%{_libdir}/%{name}-%{version}/plugins/

#install images/qscan345.png $RPM_BUILD_ROOT%{_datadir}/icons/hicolor/96x96/apps/
#desktop-file-install --vendor=bessrc \
#	--dir $RPM_BUILD_ROOT%{_datadir}/applications \
#	qscan345.desktop

%clean
rm -rf $RPM_BUILD_ROOT

#%post
#update-desktop-database &> /dev/null || :
#touch --no-create %{_datadir}/icons/hicolor || :
#%{_bindir}/gtk-update-icon-cache --quiet %{_datadir}/icons/hicolor || :

#%postun
#update-desktop-database &> /dev/null || :
#touch --no-create %{_datadir}/icons/hicolor || :
#%{_bindir}/gtk-update-icon-cache --quiet %{_datadir}/icons/hicolor || :


%files
%defattr(-,root,root,-)
%doc
%{_bindir}/%{name}
%{_libdir}/%{name}-%{version}/plugins/*.so
#%{_datadir}/applications/*%{name}.desktop
#%{_datadir}/icons/hicolor/*/*/*

%changelog
* Fri Oct 10 2008  <jennings@weimar.bessrc.aps.anl.gov> - 0.4.10-3
- added extra 'mock' and 'rpmsource' targets to project file

* Mon Jul  2 2007  Guy Jennings <jennings@anl.gov> 0.0.1-0.cep.1
- Initial RPM Release
