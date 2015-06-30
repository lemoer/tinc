## Produce this file with automake to get Makefile.in

sbin_PROGRAMS = tincd tinc sptps_test sptps_keypair

## Make sure version.c is always rebuilt with the latest git information
.PHONY: ${srcdir}/version.c version_git.h
version_git.h:
	echo >$@
	-(cd $(srcdir) && git describe) && echo '#define GIT_DESCRIPTION "'`(cd $(srcdir) && git describe) | sed 's/release-//'`'"' >$@
${srcdir}/version.c: version_git.h

if LINUX
sbin_PROGRAMS += sptps_speed
endif

lib_LTLIBRARIES = libtincd.la libed25519.la libchacha_poly1305.la libtincconf.la

libchacha_poly1305_la_SOURCES = \
	chacha-poly1305/chacha.c chacha-poly1305/chacha.h \
	chacha-poly1305/chacha-poly1305.c chacha-poly1305/chacha-poly1305.h \
	chacha-poly1305/poly1305.c chacha-poly1305/poly1305.h

libed25519_la_SOURCES = \
	ed25519/add_scalar.c \
	ed25519/ed25519.h \
	ed25519/fe.c ed25519/fe.h \
	ed25519/fixedint.h \
	ed25519/ge.c ed25519/ge.h \
	ed25519/key_exchange.c \
	ed25519/keypair.c \
	ed25519/precomp_data.h \
	ed25519/sc.c ed25519/sc.h \
	ed25519/sha512.c ed25519/sha512.h \
  ed25519/ecdsa.c \
	ed25519/sign.c \
	ed25519/verify.c

libtincconf_la_SOURCES = \
	conf.h

libtincd_la_SOURCES = \
  conf.h \
  conf.c \
	buffer.c buffer.h \
	cipher.h \
	connection.c connection.h \
	control.c control.h \
	control_common.h \
	crypto.h \
	device.h \
	digest.h \
	dropin.c \
	dummy_device.c \
	ecdh.h \
	ecdsa.h \
	ecdsagen.h \
	edge.c edge.h \
	ethernet.h \
	event.c event.h \
	fake-gai-errnos.h \
	fake-getaddrinfo.c fake-getaddrinfo.h \
	fake-getnameinfo.c fake-getnameinfo.h \
	graph.c graph.h \
	hash.c hash.h \
	have.h \
	ipv4.h \
	ipv6.h \
	list.c list.h \
	logger.c logger.h \
	meta.c meta.h \
	multicast_device.c \
	names.c names.h \
	net.c net.h \
	net_packet.c \
	net_setup.c \
	net_socket.c \
	netutl.c netutl.h \
	node.c node.h \
	prf.h \
	protocol.c protocol.h \
	protocol_auth.c \
	protocol_edge.c \
	protocol_key.c \
	protocol_misc.c \
	protocol_subnet.c \
	raw_socket_device.c \
	route.c route.h \
	rsa.h \
	rsagen.h \
	script.c script.h \
	splay_tree.c splay_tree.h \
	sptps.c sptps.h \
	subnet.c subnet.h \
	subnet_parse.c \
	system.h \
	utils.c utils.h \
	xalloc.h \
	version.c version.h \
	ed25519/ecdh.c \
	ed25519/ecdsa.c \
	getopt.c getopt.h \
	process.h process.c \
	getopt1.c

## Conditionally compile device drivers

if LINUX
libtincd_la_SOURCES += linux/device.c
endif

if BSD
libtincd_la_SOURCES += bsd/device.c
if TUNEMU
libtincd_la_SOURCES += bsd/tunemu.c bsd/tunemu.h
endif
endif

if SOLARIS
libtincd_la_SOURCES += solaris/device.c
endif

if MINGW
libtincd_la_SOURCES += mingw/device.c mingw/common.h
endif

if CYGWIN
libtincd_la_SOURCES += cygwin/device.c
endif

if UML
libtincd_la_SOURCES += uml_device.c
endif

if VDE
libtincd_la_SOURCES += vde_device.c
endif

if OPENSSL
libtincd_la_SOURCES += \
	openssl/cipher.c \
	openssl/crypto.c \
	openssl/digest.c openssl/digest.h \
	openssl/prf.c \
	openssl/rsa.c \
	openssl/rsagen.c
else
if GCRYPT
libtincd_la_SOURCES += \
	gcrypt/cipher.c \
	gcrypt/crypto.c \
	gcrypt/digest.c gcrypt/digest.h \
	gcrypt/prf.c \
	gcrypt/rsa.c \
	gcrypt/rsagen.c
else
libtincd_la_SOURCES += \
	nolegacy/crypto.c \
	nolegacy/prf.c
endif
endif

tincd_SOURCES = \
	conf.h \
	tincd.c

tinc_SOURCES = \
	invitation.c invitation.h \
	top.c top.h \
	fsck.c fsck.h \
	info.c info.h \
	ed25519/ecdsagen.c \
	tincctl.c tincctl.h

sptps_test_SOURCES = \
	sptps_test.c

sptps_keypair_SOURCES = \
	ed25519/ecdsagen.c \
	sptps_keypair.c

sptps_speed_SOURCES = \
	ed25519/ecdsagen.c \
	sptps_speed.c


tincd_LDADD = libtincd.la libed25519.la libchacha_poly1305.la libtincconf.la
tinc_LDADD = $(READLINE_LIBS) $(CURSES_LIBS) libtincd.la libtincconf.la libed25519.la libchacha_poly1305.la
sptps_speed_LDADD = -lrt libtincd.la libed25519.la libtincconf.la libchacha_poly1305.la
sptps_test_LDADD = -lrt libtincd.la libed25519.la libtincconf.la libchacha_poly1305.la
sptps_keypair_LDADD = -lrt libed25519.la libtincconf.la libchacha_poly1305.la libtincd.la

LIBS = @LIBS@ -lm

if TUNEMU
LIBS += -lpcap
endif

AM_CFLAGS = -DCONFDIR=\"$(sysconfdir)\" -DLOCALSTATEDIR=\"$(localstatedir)\" -DSBINDIR=\"$(sbindir)\" -iquote. \
	-Weverything -Wno-reserved-id-macro -Wno-padded -Wno-disabled-macro-expansion -Wno-shorten-64-to-32 -Wno-format-nonliteral \
	-Werror=conditional-uninitialized \
	-Werror=uninitialized \
	-Werror=missing-field-initializers \
	-Werror=missing-variable-declarations \
	-Wno-conversion \
	-Wno-date-time \
	-Wno-shadow \
	-Wno-vla \
	-Wno-unused-macros \
	-Wno-cast-qual \
	-ferror-limit=10
#	-Werror \
#-fsanitize=address
