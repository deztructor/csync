%{!?cmake_install: %global cmake_install cd build && make install DESTDIR=%{buildroot}}

%if %{undefined suse_version}
%define cmake_custom mkdir -p build && cd build && %cmake ..
%else
%define cmake_custom %cmake
%endif

Summary: Lightweight utility to synchronize files
Name: csync
Version: 0.0.0
Release: 1
License: LGPL2
Group: System Environment/Libraries
URL: http://www.csync.org
Source0: %{name}-%{version}.tar.bz2
BuildRequires: cmake >= 2.8
BuildRequires: pkgconfig(cmocka)
BuildRequires: pkgconfig(sqlite3)

%description
csync is a lightweight utility to synchronize files between two directories on
a system or between multiple systems.

It synchronizes bidirectionally and allows the user to keep two copies of files
and directories in sync. csync uses widely adopted protocols, such as smb or
sftp, so that there is no need for a server component. It is a user-level
program which means you don’t need to be a superuser or administrator.

%package devel
Summary: CSync development files
Group: Development/Libraries
Requires: %{name} = %{version}-%{release}
%description devel
%{summary}

%package doc
Summary: CSync developer documentation
Group: Documenation
BuildRequires: doxygen
%if 0%{?_with_docs:1}
BuildRequires: graphviz
%endif
%description doc
%{summary}

%prep
%setup -q -n %{name}-%{version}/csync

%build
%cmake_custom
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%cmake_install

%files
%defattr(-,root,root,-)
%dir %{_sysconfdir}/%{name}
%{_sysconfdir}/%{name}/*
%{_bindir}/csync
%{_libdir}/*.so*

%files devel
%defattr(-,root,root,-)
%dir %{_includedir}/%{name}
%{_includedir}/%{name}/*.h

%files doc
%defattr(-,root,root,-)
%dir %{_datarootdir}/doc/%{name}
%{_datarootdir}/doc/%{name}/*
%{_mandir}/man1/%{name}.1.gz

%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig
