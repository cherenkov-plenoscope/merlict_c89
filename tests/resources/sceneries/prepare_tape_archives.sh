#!/bin/bash
# prepare tape-archives

for scenery_id in 001
do
    cd $scenery_id
    tar -cf ../$scenery_id.tar *
done
