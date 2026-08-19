#
# um variant: harbour-infc plus external hardware (Proxmark) and sound.
# Built from the same source tree with CONFIG+=um.
#

Name:       harbour-infc-um

# >> macros
%define _buildhost localhost
# << macros

%{!?qtc_qmake:%define qtc_qmake %qmake}
%{!?qtc_qmake5:%define qtc_qmake5 %qmake5}
%{!?qtc_make:%define qtc_make make}
%{?qtc_builddir:%define _builddir %qtc_builddir}
Summary:    NFC reader, writer, emulator plus Proxmark
Version:    0.1.0
Release:    1
Group:      Qt/Qt
License:    GPLv3
URL:        https://github.com/JimKnopfIoT/harbour-iNFC
Source0:    %{name}-%{version}.tar.bz2
Requires:   sailfishsilica-qt5 >= 0.10.9
Requires:   nfcd >= 1.2.2
BuildRequires:  pkgconfig(sailfishapp) >= 1.0.2
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  pkgconfig(Qt5DBus)
BuildRequires:  pkgconfig(Qt5Network)
BuildRequires:  pkgconfig(libpulse-simple)
BuildRequires:  desktop-file-utils

%description
iNFC um is the base iNFC app plus support for external hardware: a Proxmark
over USB for the frequencies and cards the phone cannot reach on its own
(125 kHz LF, MIFARE Classic). The base iNFC app has none of this.

%prep
%setup -q -n %{name}-%{version}

%build
%qtc_qmake5 CONFIG+=um

%qtc_make %{?_smp_mflags}

%install
rm -rf %{buildroot}
%qmake5_install

desktop-file-install --delete-original       \
  --dir %{buildroot}%{_datadir}/applications             \
   %{buildroot}%{_datadir}/applications/*.desktop

%post
systemctl restart sailjaild >/dev/null 2>&1 || true

%files
%defattr(644,root,root,755)
%attr(755,root,root) %{_bindir}/%{name}
%{_datadir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/*/apps/%{name}.png
