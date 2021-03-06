# Process this file with automake to produce Makefile.in

SUBDIRS = $(POSUB) src build data man doc

# COMMON_EXTRA_DIST is what used to be the EXTRA_DIST, when adding dist
# files, please add them to the COMMON_EXTRA_DIST list.

COMMON_EXTRA_DIST = .descr \
	AUTHORS \
	build.minix \
	ChangeLog.1 \
	ChangeLog.2 \
	COPYING \
	FEEDBACK \
	Doxyfile \
	INSTALL \
	NEWS \
	vice.spec \
	vice.spec.in \
	vice-gtk.spec \
	vice-gtk.spec.in \
	vice-mint.spec \
	vice-mint.spec.in \
	vice-mint-sdl.spec \
	vice-mint-sdl.spec.in

# PO_EXTRA_DIST is a list of files in the po directory that need to be
# included in the source archive, this list was added to fix the fact that
# 'make dist' only produced a proper/workable source archive when done on a
# *nix platform with nls.

PO_EXTRA_DIST = po/ChangeLog \
	po/Makefile.in.in \
	po/POTFILES.in \
	po/da.po \
	po/de.po \
	po/es.po \
	po/fr.po \
	po/hu.po \
	po/intl2po.c \
	po/it.po \
	po/ko.po \
	po/nl.po \
	po/pl.po \
	po/po2tbl.sed \
	po/ru.po \
	po/sv.po \
	po/tr.po

if PO_SUBDIR_USED

EXTRA_DIST = $(COMMON_EXTRA_DIST)

else

EXTRA_DIST = $(COMMON_EXTRA_DIST) $(PO_EXTRA_DIST)

endif

if SUPPORT_X64SC
  X64SC_INCLUDED = yes
else
  X64SC_INCLUDED = no
endif

.PHONY: vsid x64 x64sc x128 x64dtv xvic xpet xplus4 xcbm2 xcbm5x0 c1541 petcat cartconv
vsid:
	(cd src; $(MAKE) vsid-all)

x64:
	(cd src; $(MAKE) x64-all)

if SUPPORT_X64SC
x64sc:
	(cd src; $(MAKE) x64sc-all)
endif

x128:
	(cd src; $(MAKE) x128-all)

if SUPPORT_X64DTV
x64dtv:
	(cd src; $(MAKE) x64dtv-all)
endif

xvic:
	(cd src; $(MAKE) xvic-all)

xpet:
	(cd src; $(MAKE) xpet-all)

xplus4:
	(cd src; $(MAKE) xplus4-all)

xcbm2:
	(cd src; $(MAKE) xcbm2-all)

xcbm5x0:
	(cd src; $(MAKE) xcbm5x0-all)

c1541:
	(cd src; $(MAKE) c1541-all)

petcat:
	(cd src; $(MAKE) petcat-all)

cartconv:
	(cd src; $(MAKE) cartconv)


install: installvice

# If we have no ChangeLog (which should be generated from SVN), generate a
# dummy one.

ChangeLog:
	touch ChangeLog

if AMIGA_COMPILE
if SDL_COMPILE
bindist:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist_amiga.sh $(STRIP) $(VICE_VERSION) $(host_cpu) $(host_os) @ENABLE_ARCH@ nozip $(X64SC_INCLUDED) $(top_srcdir) $(EXEEXT)

bindistzip:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist_amiga.sh $(STRIP) $(VICE_VERSION) $(host_cpu) $(host_os) @ENABLE_ARCH@ zip $(X64SC_INCLUDED) $(top_srcdir) $(EXEEXT)
else
bindist:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist.sh $(STRIP) $(VICE_VERSION) $(host_cpu) $(host_os) @ENABLE_ARCH@ nozip $(X64SC_INCLUDED) $(top_srcdir) $(EXEEXT)

bindistzip:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist.sh $(STRIP) $(VICE_VERSION) $(host_cpu) $(host_os) @ENABLE_ARCH@ zip $(X64SC_INCLUDED) $(top_srcdir) $(EXEEXT)
endif
endif

if BEOS_COMPILE
if SDL_COMPILE
bindist:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist_beos.sh $(STRIP) $(VICE_VERSION) $(host_cpu) @ENABLE_ARCH@ nozip $(X64SC_INCLUDED) $(top_srcdir)

bindistzip:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist_beos.sh $(STRIP) $(VICE_VERSION) $(host_cpu) @ENABLE_ARCH@ zip $(X64SC_INCLUDED) $(top_srcdir)
else
bindist:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist.sh $(STRIP) $(VICE_VERSION) $(host_cpu) @ENABLE_ARCH@ nozip $(X64SC_INCLUDED) $(top_srcdir)

bindistzip:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist.sh $(STRIP) $(VICE_VERSION) $(host_cpu) @ENABLE_ARCH@ zip $(X64SC_INCLUDED) $(top_srcdir)
endif
endif

if MSDOS_COMPILE
bindist:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist.sh $(STRIP) $(VICE_VERSION_MAJOR) $(VICE_VERSION_MINOR) @ENABLE_ARCH@ nozip $(X64SC_INCLUDED) $(top_srcdir)

bindistzip:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist.sh $(STRIP) $(VICE_VERSION_MAJOR) $(VICE_VERSION_MINOR) @ENABLE_ARCH@ zip $(X64SC_INCLUDED) $(top_srcdir)
endif

if UNIX_GP2X_COMPILE
if SDL_COMPILE
bindist:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist_gp2x.sh $(STRIP) $(VICE_VERSION) @ENABLE_ARCH@ nozip $(top_srcdir)

bindistzip:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist_gp2x.sh $(STRIP) $(VICE_VERSION) @ENABLE_ARCH@ zip $(top_srcdir)
else
bindist:
	$(SHELL) @ARCH_SRC_DIR@/gp2x/make-bindist.sh $(STRIP) $(VICE_VERSION) @ENABLE_ARCH@ nozip $(top_srcdir)

bindistzip:
	$(SHELL) @ARCH_SRC_DIR@/gp2x/make-bindist.sh $(STRIP) $(VICE_VERSION) @ENABLE_ARCH@ zip $(top_srcdir)
endif
endif

if UNIX_DINGUX_COMPILE
bindist:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist_dingoo.sh $(STRIP) $(VICE_VERSION) @ENABLE_ARCH@ nozip $(top_srcdir)

bindistzip:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist_dingoo.sh $(STRIP) $(VICE_VERSION) @ENABLE_ARCH@ zip $(top_srcdir)
endif

if DINGOO_NATIVE_COMPILE
bindist:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist_dingoo_native.sh $(STRIP) $(VICE_VERSION) @ENABLE_ARCH@ nozip $(top_srcdir)

bindistzip:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist_dingoo_native.sh $(STRIP) $(VICE_VERSION) @ENABLE_ARCH@ zip $(top_srcdir)
endif

if UNIX_MACOSX_COMPILE
bindist:
	/bin/bash @ARCH_SRC_DIR@/../unix/macosx/make-bindist.sh $(top_srcdir) $(STRIP) $(VICE_VERSION) @ENABLE_ARCH nozip $(X64SC_INCLUDED) @UNIX_MACOSX_UITYPE@

bindistzip:
	/bin/bash @ARCH_SRC_DIR@/../unix/macosx/make-bindist.sh $(top_srcdir) $(STRIP) $(VICE_VERSION) @ENABLE_ARCH zip $(X64SC_INCLUDED) @UNIX_MACOSX_UITYPE@
endif

if UNIX_SKYOS_COMPILE
if SDL_COMPILE
bindist:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist_skyos.sh $(STRIP) $(VICE_VERSION) $(prefix) @ENABLE_ARCH@ nozip $(X64SC_INCLUDED) $(top_srcdir)

bindistzip:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist_skyos.sh $(STRIP) $(VICE_VERSION) $(prefix) @ENABLE_ARCH@ zip $(X64SC_INCLUDED) $(top_srcdir)
else
bindist:
	$(SHELL) @ARCH_SRC_DIR@/skyos/make-bindist.sh $(STRIP) $(VICE_VERSION) $(prefix) @ENABLE_ARCH@ nozip $(X64SC_INCLUDED) $(top_srcdir)

bindistzip:
	$(SHELL) @ARCH_SRC_DIR@/skyos/make-bindist.sh $(STRIP) $(VICE_VERSION) $(prefix) @ENABLE_ARCH@ zip $(X64SC_INCLUDED) $(top_srcdir)
endif
endif

if UNIX_QNX6_COMPILE
if SDL_COMPILE
bindist:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist_qnx6.sh $(STRIP) $(VICE_VERSION) $(prefix) $(CROSS) @ENABLE_ARCH@ nozip $(X64SC_INCLUDED) $(top_srcdir) $(MAKE) $(host_cpu)

bindistzip:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist_qnx6.sh $(STRIP) $(VICE_VERSION) $(prefix) $(CROSS) @ENABLE_ARCH@ zip $(X64SC_INCLUDED) $(top_srcdir) $(MAKE) $(host_cpu)
else
bindist:
	$(SHELL) @ARCH_SRC_DIR@/qnx6/make-bindist.sh $(STRIP) $(VICE_VERSION) $(prefix) $(CROSS) @ENABLE_ARCH@ nozip $(X64SC_INCLUDED) $(top_srcdir) $(MAKE) $(host_cpu)

bindistzip:
	$(SHELL) @ARCH_SRC_DIR@/qnx6/make-bindist.sh $(STRIP) $(VICE_VERSION) $(prefix) $(CROSS) @ENABLE_ARCH@ zip $(X64SC_INCLUDED) $(top_srcdir) $(MAKE) $(host_cpu)
endif
endif

if UNIX_QNX4_COMPILE
bindist:
	$(SHELL) @ARCH_SRC_DIR@/qnx4/make-bindist.sh $(VICE_VERSION) $(prefix) @ENABLE_ARCH@ nozip $(X64SC_INCLUDED) $(top_srcdir)

bindistzip:
	$(SHELL) @ARCH_SRC_DIR@/qnx4/make-bindist.sh $(VICE_VERSION) $(prefix) @ENABLE_ARCH@ zip $(X64SC_INCLUDED) $(top_srcdir)
endif

if UNIX_HPUX_COMPILE
bindist:
	$(SHELL) @ARCH_SRC_DIR@/hpux/make-bindist.sh $(STRIP) $(PERL) $(VICE_VERSION) $(prefix) $(host_cpu) $(host_os) @ENABLE_ARCH@ nozip $(X64SC_INCLUDED) $(top_srcdir)

bindistzip:
	$(SHELL) @ARCH_SRC_DIR@/hpux/make-bindist.sh $(STRIP) $(PERL) $(VICE_VERSION) $(prefix) $(host_cpu) $(host_os) @ENABLE_ARCH@ zip $(X64SC_INCLUDED) $(top_srcdir)
endif

if UNIX_SOLARIS_COMPILE
if SDL_COMPILE
bindist:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist_scosol.sh $(STRIP) $(VICE_VERSION) $(prefix) $(CROSS) @ENABLE_ARCH@ nozip $(X64SC_INCLUDED) sol $(infodir) $(mandir) $(top_srcdir) $(MAKE)

bindistzip:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist_scosol.sh $(STRIP) $(VICE_VERSION) $(prefix) $(CROSS) @ENABLE_ARCH@ zip $(X64SC_INCLUDED) sol $(infodir) $(mandir) $(top_srcdir) $(MAKE)
else
if GNOMEUI
bindist:
	$(SHELL) @ARCH_SRC_DIR@/sco_sol/make-bindist.sh $(STRIP) $(VICE_VERSION) $(prefix) $(CROSS) @ENABLE_ARCH@ nozip $(X64SC_INCLUDED) sol gtk $(infodir) $(mandir) $(top_srcdir) $(MAKE) $(host_cpu) $(host_os)_

bindistzip:
	$(SHELL) @ARCH_SRC_DIR@/sco_sol/make-bindist.sh $(STRIP) $(VICE_VERSION) $(prefix) $(CROSS) @ENABLE_ARCH@ zip $(X64SC_INCLUDED) sol gtk $(infodir) $(mandir) $(top_srcdir) $(MAKE) $(host_cpu) $(host_os)
else
bindist:
	$(SHELL) @ARCH_SRC_DIR@/sco_sol/make-bindist.sh $(STRIP) $(VICE_VERSION) $(prefix) $(CROSS) @ENABLE_ARCH@ nozip $(X64SC_INCLUDED) sol xaw $(infodir) $(mandir) $(top_srcdir) $(MAKE) $(host_cpu) $(host_os)

bindistzip:
	$(SHELL) @ARCH_SRC_DIR@/sco_sol/make-bindist.sh $(STRIP) $(VICE_VERSION) $(prefix) $(CROSS) @ENABLE_ARCH@ zip $(X64SC_INCLUDED) sol xaw $(infodir) $(mandir) $(top_srcdir) $(MAKE) $(host_cpu) $(host_os)
endif
endif
endif

if UNIX_RHAPSODY_COMPILE
bindist:
	$(SHELL) @ARCH_SRC_DIR@/next_open_rhap/make-bindist.sh $(STRIP) $(VICE_VERSION) $(prefix) @ENABLE_ARCH@ nozip $(X64SC_INCLUDED) rhapsody $(top_srcdir) $(MAKE)

bindistzip:
	$(SHELL) @ARCH_SRC_DIR@/next_open_rhap/make-bindist.sh $(STRIP) $(VICE_VERSION) $(prefix) @ENABLE_ARCH@ zip $(X64SC_INCLUDED) rhapsody $(top_srcdir) $(MAKE)
endif

if UNIX_OPENSTEP_COMPILE
bindist:
	$(SHELL) @ARCH_SRC_DIR@/next_open_rhap/make-bindist.sh $(STRIP) $(VICE_VERSION) $(prefix) @ENABLE_ARCH@ nozip $(X64SC_INCLUDED) openstep $(top_srcdir) $(MAKE)

bindistzip:
	$(SHELL) @ARCH_SRC_DIR@/next_open_rhap/make-bindist.sh $(STRIP) $(VICE_VERSION) $(prefix) @ENABLE_ARCH@ zip $(X64SC_INCLUDED) openstep $(top_srcdir) $(MAKE)
endif

if UNIX_OPENSTEP_COMPILE
bindist:
	$(SHELL) @ARCH_SRC_DIR@/next_open_rhap/make-bindist.sh $(STRIP) $(VICE_VERSION) $(prefix) @ENABLE_ARCH@ nozip $(X64SC_INCLUDED) nextstep $(top_srcdir) $(MAKE)

bindistzip:
	$(SHELL) @ARCH_SRC_DIR@/next_open_rhap/make-bindist.sh $(STRIP) $(VICE_VERSION) $(prefix) @ENABLE_ARCH@ zip $(X64SC_INCLUDED) nextstep $(top_srcdir) $(MAKE)
endif

if UNIX_OPENSERVER5_COMPILE
if SDL_COMPILE
bindist:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist_scosol.sh $(STRIP) $(VICE_VERSION) $(prefix) $(CROSS) @ENABLE_ARCH@ nozip $(X64SC_INCLUDED) sco5 $(infodir) $(mandir) $(top_srcdir) $(MAKE)

bindistzip:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist_scosol.sh $(STRIP) $(VICE_VERSION) $(prefix) $(CROSS) @ENABLE_ARCH@ zip $(X64SC_INCLUDED) sco5 $(infodir) $(mandir) $(top_srcdir) $(MAKE)
else
if GNOMEUI
bindist:
	$(SHELL) @ARCH_SRC_DIR@/sco_sol/make-bindist.sh $(STRIP) $(VICE_VERSION) $(prefix) $(CROSS) @ENABLE_ARCH@ nozip $(X64SC_INCLUDED) sco5 gtk $(infodir) $(mandir) $(top_srcdir) $(MAKE) $(host_cpu) $(host_os)

bindistzip:
	$(SHELL) @ARCH_SRC_DIR@/sco_sol/make-bindist.sh $(STRIP) $(VICE_VERSION) $(prefix) $(CROSS) @ENABLE_ARCH@ zip $(X64SC_INCLUDED) sco5 gtk $(infodir) $(mandir) $(top_srcdir) $(MAKE) $(host_cpu) $(host_os)
else
bindist:
	$(SHELL) @ARCH_SRC_DIR@/sco_sol/make-bindist.sh $(STRIP) $(VICE_VERSION) $(prefix) $(CROSS) @ENABLE_ARCH@ nozip $(X64SC_INCLUDED) sco5 xaw $(infodir) $(mandir) $(top_srcdir) $(MAKE) $(host_cpu) $(host_os)

bindistzip:
	$(SHELL) @ARCH_SRC_DIR@/sco_sol/make-bindist.sh $(STRIP) $(VICE_VERSION) $(prefix) $(CROSS) @ENABLE_ARCH@ zip $(X64SC_INCLUDED) sco5 xaw $(infodir) $(mandir) $(top_srcdir) $(MAKE) $(host_cpu) $(host_os)
endif
endif
endif

if UNIX_OPENSERVER6_COMPILE
if SDL_COMPILE
bindist:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist_scosol.sh $(STRIP) $(VICE_VERSION) $(prefix) $(CROSS) @ENABLE_ARCH@ nozip $(X64SC_INCLUDED) sco6 $(infodir) $(mandir) $(top_srcdir) $(MAKE)

bindistzip:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist_scosol.sh $(STRIP) $(VICE_VERSION) $(prefix) $(CROSS) @ENABLE_ARCH@ zip $(X64SC_INCLUDED) sco6 $(infodir) $(mandir) $(top_srcdir) $(MAKE)
else
if GNOMEUI
bindist:
	$(SHELL) @ARCH_SRC_DIR@/sco_sol/make-bindist.sh $(STRIP) $(VICE_VERSION) $(prefix) $(CROSS) @ENABLE_ARCH@ nozip $(X64SC_INCLUDED) sco6 gtk $(infodir) $(mandir) $(top_srcdir) $(MAKE) $(host_cpu) $(host_os)

bindistzip:
	$(SHELL) @ARCH_SRC_DIR@/sco_sol/make-bindist.sh $(STRIP) $(VICE_VERSION) $(prefix) $(CROSS) @ENABLE_ARCH@ zip $(X64SC_INCLUDED) sco6 gtk $(infodir) $(mandir) $(top_srcdir) $(MAKE) $(host_cpu) $(host_os)
else
bindist:
	$(SHELL) @ARCH_SRC_DIR@/sco_sol/make-bindist.sh $(STRIP) $(VICE_VERSION) $(prefix) $(CROSS) @ENABLE_ARCH@ nozip $(X64SC_INCLUDED) sco6 xaw $(infodir) $(mandir) $(top_srcdir) $(MAKE) $(host_cpu) $(host_os)

bindistzip:
	$(SHELL) @ARCH_SRC_DIR@/sco_sol/make-bindist.sh $(STRIP) $(VICE_VERSION) $(prefix) $(CROSS) @ENABLE_ARCH@ zip $(X64SC_INCLUDED) sco6 xaw $(infodir) $(mandir) $(top_srcdir) $(MAKE) $(host_cpu) $(host_os)
endif
endif
endif

if UNIX_UNIXWARE_COMPILE
if SDL_COMPILE
bindist:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist_scosol.sh $(STRIP) $(VICE_VERSION) $(prefix) $(CROSS) @ENABLE_ARCH@ nozip $(X64SC_INCLUDED) sco7 $(infodir) $(mandir) $(top_srcdir) $(MAKE)

bindistzip:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist_scosol.sh $(STRIP) $(VICE_VERSION) $(prefix) $(CROSS) @ENABLE_ARCH@ zip $(X64SC_INCLUDED) sco7 $(infodir) $(mandir) $(top_srcdir) $(MAKE)
else
if GNOMEUI
bindist:
	$(SHELL) @ARCH_SRC_DIR@/sco_sol/make-bindist.sh $(STRIP) $(VICE_VERSION) $(prefix) $(CROSS) @ENABLE_ARCH@ nozip $(X64SC_INCLUDED) sco7 gtk $(infodir) $(mandir) $(top_srcdir) $(MAKE) $(host_cpu) $(host_os)

bindistzip:
	$(SHELL) @ARCH_SRC_DIR@/sco_sol/make-bindist.sh $(STRIP) $(VICE_VERSION) $(prefix) $(CROSS) @ENABLE_ARCH@ zip $(X64SC_INCLUDED) sco7 gtk $(infodir) $(mandir) $(top_srcdir) $(MAKE) $(host_cpu) $(host_os)
else
bindist:
	$(SHELL) @ARCH_SRC_DIR@/sco_sol/make-bindist.sh $(STRIP) $(VICE_VERSION) $(prefix) $(CROSS) @ENABLE_ARCH@ nozip $(X64SC_INCLUDED) sco7 xaw $(infodir) $(mandir) $(top_srcdir) $(MAKE) $(host_cpu) $(host_os)

bindistzip:
	$(SHELL) @ARCH_SRC_DIR@/sco_sol/make-bindist.sh $(STRIP) $(VICE_VERSION) $(prefix) $(CROSS) @ENABLE_ARCH@ zip $(X64SC_INCLUDED) sco7 xaw $(infodir) $(mandir) $(top_srcdir) $(MAKE) $(host_cpu) $(host_os)
endif
endif
endif

if WIN32_COMPILE
if SDL_COMPILE
bindist:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist_win32.sh $(STRIP) $(VICE_VERSION) @ENABLE_ARCH@ nozip $(X64SC_INCLUDED) $(top_srcdir) $(host_cpu)

bindistzip:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist_win32.sh $(STRIP) $(VICE_VERSION) @ENABLE_ARCH@ zip $(X64SC_INCLUDED) $(top_srcdir) $(host_cpu)
else
if OWCC
bindist:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist.sh $(STRIP) $(VICE_VERSION) @ENABLE_ARCH@ nozip $(X64SC_INCLUDED) $(top_srcdir) $(host_cpu) yes

bindistzip:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist.sh $(STRIP) $(VICE_VERSION) @ENABLE_ARCH@ zip $(X64SC_INCLUDED) $(top_srcdir) $(host_cpu) yes
else
bindist:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist.sh $(STRIP) $(VICE_VERSION) @ENABLE_ARCH@ nozip $(X64SC_INCLUDED) $(top_srcdir) $(host_cpu) no

bindistzip:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist.sh $(STRIP) $(VICE_VERSION) @ENABLE_ARCH@ zip $(X64SC_INCLUDED) $(top_srcdir) $(host_cpu) no
endif
endif
endif

if SYLLABLE_COMPILE
bindist:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist_syllable.sh $(STRIP) $(VICE_VERSION) $(prefix) @ENABLE_ARCH@ nozip $(X64SC_INCLUDED) $(top_srcdir)

bindistzip:
	$(SHELL) @ARCH_SRC_DIR@/make-bindist_syllable.sh $(STRIP) $(VICE_VERSION) $(prefix) @ENABLE_ARCH@ zip $(X64SC_INCLUDED) $(top_srcdir)
endif

@MAKE_BINDIST_TRUE@installvice:
@MAKE_BINDIST_TRUE@	@echo this platform doesn\'t use \"make install\", use \"make bindist\" instead

@MAKE_BINDIST_FALSE@installvice: install-recursive

cleantrans:
	rm `find $(top_srcdir) -name "*.po.c"`

# Manually define the dist targets so the correct tar usage will be used.
dist-gzip:
	$(DOS2UNIX) `find $(top_srcdir) -name "*.sh"`
	$(DOS2UNIX) $(top_srcdir)/src/translate.txt
	$(DOS2UNIX) $(top_srcdir)/doc/vice.texi
	$(MAKE) distdir
	$(AMTAR) chf - $(distdir) | GZIP=$(GZIP_ENV) gzip -c >$(distdir).tar.gz
	$(am__remove_distdir)

dist dist-all:
	$(DOS2UNIX) `find $(top_srcdir) -name "*.sh"`
	$(DOS2UNIX) $(top_srcdir)/src/translate.txt
	$(DOS2UNIX) $(top_srcdir)/doc/vice.texi
	$(MAKE) distdir
	$(AMTAR) chf - $(distdir) | GZIP=$(GZIP_ENV) gzip -c >$(distdir).tar.gz
	$(am__remove_distdir)
