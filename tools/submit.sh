#!/bin/bash


CFGS=( jetanalysis_rapgap_0.5 jetanalysis_rapgap_1.0 jetanalysis_rapgap_2.0 )

for cf in "${CFGS[@]}"
do
    ln -sf ./config/$cf.cfg jetanalysis.cfg
    nohup root -b -q execute.C > $cf.log 2>&1 &
    echo "Submitted configuration ${cf}"
    set -x; sleep 60; set +x
done
