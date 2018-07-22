PHP_ARG_ENABLE(epl, whether to enable epl support,
[  --enable-epl          Enable epl support], no)

if test "$PHP_EPL" != "no"; then
  AC_DEFINE(HAVE_EPL, 1, [ Have epl support ])

  PHP_NEW_EXTENSION(epl, epl.c \
    function/array/chunk.c \
    function/array/compact.c \
    function/array/difference.c \
    function/array/differenceBy.c \
    function/array/differenceWith.c \
    function/array/drop.c \
    function/array/dropRight.c \
    function/array/dropRightWhile.c \
    function/array/dropWhile.c, $ext_shared)
  PHP_ADD_BUILD_DIR($ext_builddir/function/array)
fi
