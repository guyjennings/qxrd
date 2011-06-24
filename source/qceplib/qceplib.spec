Name:		qceplib
Version:	0.0.5
Release:        1%{?dist}
Summary:        qceplib - a common library for qt-based programs from cep

#%if 0%{?fedora} >= 8
#%define qtversion 4.3.4
#%else
#%if 0%{?fedora} >= 7
#%define qtversion 4.3.3
#%else
#%define qtversion 4.3.2
#%endif
#%endif

#%if 0%{?fedora} >= 8
#%define qwtlib qwt
#%define qwtdevel qwt-devel
#%else
#%define qwtlib qwt-qt4
#%define qwtdevel qwt-qt4-devel
#%endif

Group:       	Development/Libraries   
License:        GPL
URL:            http://cep.bessrc.aps.anl.gov/software/qceplib/index.html
Source0:        http://cep.bessrc.aps.anl.gov/software/qceplib/%{name}%{version}.tar.gz
BuildRoot:      %{_tmppath}/%{name}-%{version}-%{release}-root-%(%{__id_u} -n)

BuildRequires:  desktop-file-utils qt4 >= %{qtversion}, qt4-x11 qt4-devel libtiff-devel
#BuildRequires: %{qwtlib} %{qwtdevel}
#BuildRequires:  epics-base-devel, epics-base, acqiris
Requires:       qt4 >= %{qtversion}, qt4-x11, libtiff
#Requires:       epics-base, acqiris %{qwtlib}
Requires(post): desktop-file-utils
Requires(postun): desktop-file-utils

%package devel
Summary:	Headers and libraries for building apps that use qceplib
Group:		Development/Libraries
Requires:	%{name} = %{version}-%{release}

%description
%{name} is a common library for qt-based programs from cep

%description devel
%{name}-devel contains headers and libraries for building apps that use qceplib

%prep
%setup -n %{name}%{version}

%build
/usr/bin/qmake-qt4 %{name}.pro -o Makefile
make clean
make %{?_smp_mflags}


%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT%{_bindir}
mkdir -p $RPM_BUILD_ROOT%{_libdir}
mkdir -p $RPM_BUILD_ROOT%{_includedir}
install *.h $RPM_BUILD_ROOT%{_includedir}
cp -d lib%{name}* $RPM_BUILD_ROOT%{_libdir}
#mkdir -p $RPM_BUILD_ROOT%{_datadir}/icons/hicolor/96x96/apps/
#mkdir -p $RPM_BUILD_ROOT%{_datadir}/applications/
#install qxrd $RPM_BUILD_ROOT%{_bindir}
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
%{_libdir}/*.so.*

%files devel
%defattr(-,root,root,-)
%doc
%{_includedir}/*.h
#%{_libdir}/*.a
#%{_libdir}/*.la
%{_libdir}/*.so

%changelog
* Fri Oct 10 2008  <jennings@weimar.bessrc.aps.anl.gov> - 0.0.5-3
- added extra 'mock' and 'rpmsource' targets to project file

* Mon Jul  2 2007  Guy Jennings <jennings@anl.gov> 0.0.1-0.cep.1
- Initial RPM Release