AC_C_BIGENDIAN(
  [AC_SUBST(BSON_BYTE_ORDER, 4321)],
  [AC_SUBST(BSON_BYTE_ORDER, 1234)],
  [AC_MSG_ERROR([unknown endianness])],
  [AC_MSG_ERROR([universal endianness is not supported])]
)
