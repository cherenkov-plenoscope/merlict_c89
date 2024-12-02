python ./tools/almagamate.py \
    build/almagamate \
    libs/chk \
    libs/mtl \
    libs/mli \
    libs/mli_viewer \
    --header_path build/almagamate/chk-mtl-mli-mli_viewer.h \
    --source_path build/almagamate/chk-mtl-mli-mli_viewer.c

gcc \
    -include build/almagamate/chk-mtl-mli-mli_viewer.h \
    -include build/almagamate/chk-mtl-mli-mli_viewer.c \
    libs/mli_viewer/apps/viewer.main.c \
    -o build/viewer \
    -lm
