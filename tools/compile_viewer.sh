python ./tools/almagamate.py \
    build/almagamate \
    libs/mli \
    libs/mli_viewer \
    --header_path build/almagamate/mli-mli_viewer.h \
    --source_path build/almagamate/mli-mli_viewer.c

gcc \
    -include build/almagamate/mli-mli_viewer.h \
    -include build/almagamate/mli-mli_viewer.c \
    libs/mli_viewer/apps/viewer.main.c \
    -o build/viewer \
    -lm
