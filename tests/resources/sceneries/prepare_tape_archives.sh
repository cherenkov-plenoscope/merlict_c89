#!/bin/bash
# prepare tape-archives

cwd=$(pwd)
for scenery_id in 000 001
do
    cd $scenery_id
    tar -cf ../$scenery_id.tar *
    cd $cwd
done
