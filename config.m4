PHP_ARG_ENABLE(underscore, whether to enable underscore support,
[  --enable-underscore          Enable underscore support], no)

if test "$PHP_UNDERSCORE" != "no"; then
  AC_DEFINE(HAVE_UNDERSCORE, 1, [ Have underscore support ])
  PHP_NEW_EXTENSION(underscore, underscore.c, $ext_shared)
fi
