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
    PHP_NEW_EXTENSION(statsd, php_statsd.c statsd.c, $ext_shared)
fi