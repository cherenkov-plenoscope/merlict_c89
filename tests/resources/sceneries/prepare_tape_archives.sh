#!/bin/bash
# prepare tape-archives

for scenery_id in 001
do
    echo $scenery_id
    cd $scenery_id
    tar -cf ../$scenery_id.tar *
done