PHP_ARG_ENABLE(epl, whether to enable epl support,
[  --enable-epl          Enable epl support], no)

if test "$PHP_EPL" != "no"; then
  AC_DEFINE(HAVE_EPL, 1, [ Have epl support ])
  PHP_NEW_EXTENSION(epl, epl.c, $ext_shared)
fi
