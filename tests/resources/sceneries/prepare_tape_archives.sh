#!/bin/bash
# prepare tape-archives

cwd=$(pwd)
for scenery_id in 000 001
do
    cd $scenery_id
    tar -cf "../$scenery_id.tar" *
    if [ ! -f "../$scenery_id.tar" ]; then
        echo "$scenery_id.tar does not exist."
        exit 1
    fi
    cd $cwd
done
exit 0
