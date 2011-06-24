#
# spec file for package easyimagesizer
#

# norootforbuild
Name:           photosdd
Version:        0.2.0
Release:        1
AutoReqProv:    on
License:        LGPLv3+
Summary:        Web Images Downloader
Group:          Utilities/File
Url:	        http://photosdd.sourceforge.net/
Source0:        %{name}.tar.gz
BuildRoot:      %{_tmppath}/%{name}
BuildRequires:	gcc-c++
BuildRequires:  libqt4-devel >= 4.7
BuildRequires:  desktop-file-utils
BuildRequires:  cmake >= 2.8.1
BuildRequires:	libQtWebKit-devel
%if 0%{?suse_version}
BuildRequires:	update-desktop-files
%endif

%if 0%{?fedora_version}
Requires:		qt >= 4.5
%endif
%if 0%{?mandriva_version}
%ifarch x86_64
Requires:		lib64qtcore4 >= 4.5
%endif
%ifarch i586
Requires:		libqtcore4 >= 4.5
%endif
%endif

%if 0%{?suse_version}
Requires:       libqt4 >= 4.5
%endif

%if 0%{?sles_version}
Requires:       libqt4 >= 4.5
%endif

%if 0%{?centos_version}
Requires:       libqt4 >= 4.5
%endif

%if 0%{?rhel_version}
Requires:       libqt4 >= 4.5
%endif

Obsoletes:      photosdd
Provides:       photosdd = %{version}
#Requires:       lib%{name} = %{version}

%description
%{name} is a Library to download images from the web.

Authors:
--------
    FalseCAM <FalseCAM@googlemail.com>
 
%package -n lib%{name}
License:        LGPLv3+
Summary:        Library to batch convert images
Group:          Development/Libraries/C and C++
 
%description -n lib%{name}
%{name} is a Library to download images from the web.

Authors:
--------
    FalseCAM <FalseCAM@googlemail.com>

%package -n lib%{name}-devel
License:        LGPLv3+
Summary:        Development Headers for %{name}
Group:          Development/Libraries/C and C++
Requires:       lib%{name} = %{version} libqt4-devel libexiv2-devel
 
%description -n lib%{name}-devel
%{name} is a Library to download images from the web.

Authors:
--------
    FalseCAM <FalseCAM@googlemail.com>

%prep
%setup -q -n %{name}

%build
# create Makefile

cmake . -DCMAKE_INSTALL_PREFIX=/usr
make

%install

make DESTDIR=$RPM_BUILD_ROOT install
#bin

%ifarch x86_64
mkdir -p $RPM_BUILD_ROOT/%{_libdir}
mkdir -p $RPM_BUILD_ROOT/%{_libdir}/%{name}
mv $RPM_BUILD_ROOT/usr/lib/lib%{name}.so $RPM_BUILD_ROOT/%{_libdir}/
mv $RPM_BUILD_ROOT/usr/lib/%{name}/plugins $RPM_BUILD_ROOT/%{_libdir}/%{name}
%endif

install -m 644 data/%{name}.xpm $RPM_BUILD_ROOT/%{_datadir}/pixmaps/
install -m 644 data/%{name}.desktop $RPM_BUILD_ROOT/%{_datadir}/applications
#menu
%if 0%{?suse_version}
#%suse_update_desktop_file -i %{name} Graphics Photography
%endif
%if 0%{?fedora_version}
#desktop-file-validate $RPM_BUILD_ROOT/%{_datadir}/applications/%{name}.desktop
%endif

%clean
rm -rf $RPM_BUILD_ROOT


%files -n lib%{name}
%defattr(-,root,root)
%{_libdir}/lib%{name}.so

%files -n %{name}
%defattr(-, root, root)
%{_bindir}/%{name}-gui
%{_libdir}/%{name}/plugins/*
%dir %{_libdir}/%{name}
%dir %{_libdir}/%{name}/plugins
%{_datadir}/pixmaps/%{name}.xpm  
%{_datadir}/applications/%{name}.desktop

%files  -n lib%{name}-devel
%defattr(-,root,root)
%{_includedir}/%{name}


%changelog