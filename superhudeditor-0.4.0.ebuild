# Copyright 1999-2006 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

inherit eutils

DESCRIPTION="A Head-up display editor for Quake 3 CPMA"
HOMEPAGE="http://plrf.org/superhudeditor"
SRC_URI="mirror://sourceforge/${PN}/${P}-src.tar.gz"

LICENSE="GPL-3"
SLOT="0"
KEYWORDS="~x86 ~amd64"
IUSE="debug"

DEPEND=">=x11-libs/wxGTK-2.8.7
	virtual/opengl
	"
#RDEPEND=""

pkg_setup() {
	if use debug ; then
		if ! built_with_use wxGTK debug; then
			eerror "Please emerge wxGTK with USE=debug or emerge superhudeditor with USE=-debug"
			die "Package dependencies not set up correctly"
		fi
	fi
	if  ! use debug ; then 
	    if built_with_use wxGTK debug; then
			eerror "Please emerge wxGTK with USE=-debug or emerge superhudeditor with USE=debug"
			die "Package dependencies not set up correctly"
	    fi
	fi

	if ! built_with_use wxGTK opengl; then
		eerror "Please emerge wxGTK with USE=opengl (or add to /etc/portage/package.use: x11-libs/wxGTK opengl)"
		die "Package dependencies not set up correctly"
	fi
}

src_compile() {
	econf $(use_enable debug) || die "econf failed"
	emake || die "emake failed"
}

src_install() {
	emake DESTDIR="${D}" install || die "emake install failed"
	dodoc AUTHORS ChangeLog NEWS README KNOWNBUGS
}

