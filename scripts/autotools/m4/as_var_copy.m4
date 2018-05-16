dnl AS_VAR_COPY is available in AC 2.64 and on, but we only require 2.59.
dnl If we're on an older version, we define it ourselves:
m4_ifndef([AS_VAR_COPY],
          [m4_define([AS_VAR_COPY],
          [AS_LITERAL_IF([$1[]$2], [$1=$$2], [eval $1=\$$2])])])
