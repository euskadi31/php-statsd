dnl
dnl $Id$
dnl

PHP_ARG_ENABLE(
    statsd, 
    whether to enable StatsD support,
    [  --enable-statsd         Enable StatsD support]
)

if test "$PHP_STATSD" = "yes"; then

    AC_DEFINE(HAVE_STATSD, 1, [Whether you want StatsD support])

    PHP_ADD_INCLUDE($STATSD_DIR/lib/statsd/src)
    PHP_NEW_EXTENSION(statsd, php_statsd.c lib/statsd/src/statsd.c, $ext_shared)
    PHP_ADD_BUILD_DIR($ext_builddir/lib/statsd/src, 1)
fi