#! /bin/sh

set -x

ptr_once='s,([^N])(MACRO|NAME|VALUE) \(([^()]*)\),\1\3->\L\2,'
ptr_once='s,([^N])(BYTES|CAR|CDR|CLOSURE|CONTINUATION|LENGTH|MACRO|NAME|PORT|REF|STRING|STRUCT|TYPE|VALUE|VARIABLE|VECTOR) \(([^()]*)\),\1\3->\L\2,'
n_once='s,N(TYPE|CAR|CDR|LENGTH|VALUE|VECTOR) \(([^()]*)\),\2->\L\1,'
ncbytes='s,NC(BYTES) \(([^()]*)\),news_\L\1 (\2),'
cbytes='s,([^N])C(BYTES) \(([^()]*)\),\1cell_\L\2 (\3),'
cstring='s,C(STRING) \(([^()]*)\),cell_bytes (STRING (\2)),'
struct='s,SCM,struct scm*,g'

sed -i -e 's,MES_ARENA=,#MES_ARENA=,' tests/gc.test

sed -ri                                                         \
    -e 's,POINTER_CELLS=0,POINTER_CELLS=1,'                     \
    -e 's,#define POINTER_CELLS 0,#define POINTER_CELLS 1,'     \
    -e "$ncbytes"                                               \
    -e "$cbytes"                                                \
    -e "$cstring"                                               \
                                                                \
    -e "$ncbytes"                                               \
    -e "$cbytes"                                                \
    -e "$cstring"                                               \
                                                                \
    -e "$ptr_once"                                              \
    -e "$ptr_once"                                              \
    -e "$ptr_once"                                              \
    -e 's,->\<struct\>,->structure,g'                           \
    -e "$struct"                                                \
                                                                \
    -e "$n_once"                                                \
    -e "$n_once"                                                \
                                                                \
    -e 's,CAR \(([^()]*)\),\1->cdr,'                            \
    -e 's,CAAR \(([^()]*)\),\1->car->car,'                      \
    -e 's,CADR \(([^()]*)\),\1->cdr->car,'                      \
    -e 's,CDAR \(([^()]*)\),\1->car->cdr,'                      \
    -e 's,CDDR \(([^()]*)\),\1->cdr->cdr,'                      \
    -e 's,CADAR \(([^()]*)\),\1->car->cdr->car,'                \
    -e 's,CADDR \(([^()]*)\),\1->cdr->cdr->car,'                \
    -e 's,CDADR \(([^()]*)\),\1->cdr->car->cdr,'                \
    -e 's,CDDDR \(([^()]*)\),\1->cdr->cdr->cdr,'                \
    -e 's,CDDAR \(([^()]*)\),\1->car->cdr->cdr,'                \
    -e 's,CDADAR \(([^()]*)\),\1->cdr->car->cdr->car,'          \
                                                                \
    $(find .                                    \
       -name builtins.h                         \
    -o -name mes.h                              \
    -o -name mes.h                              \
    -o -name symbols.h                          \
    -o -name builtins.h                         \
    -o -name m2.h                               \
    -o -name builtins.c                         \
    -o -name cc.c                             \
    -o -name core.c                             \
    -o -name display.c                          \
    -o -name eval-apply.c                       \
    -o -name gc.c                               \
    -o -name hash.c                             \
    -o -name lib.c                              \
    -o -name m2.c                             \
    -o -name math.c                             \
    -o -name mes.c                              \
    -o -name module.c                           \
    -o -name posix.c                            \
    -o -name reader.c                           \
    -o -name stack.c                            \
    -o -name string.c                           \
    -o -name struct.c                           \
    -o -name symbol.c                           \
    -o -name vector.c                           \
    -o -name gc.c                          \
    -o -name simple.make)
