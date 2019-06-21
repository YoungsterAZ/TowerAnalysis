
echo "send TopoCluster job"
nohup root -b -q executeTopoCluster.C   > vbf2600_topoclus_rapgap_2.0_hemi_iso_1.0.log 2>&1 &
echo "send TopoTower job"
nohup root -b -q executeTopoTower.C     > vbf2600_topotow_rapgap_2.0.log 2>&1 &
echo "send TopoTowerFine job"
nohup root -b -q executeTopoTowerFine.C > vbf2600_topotowf_rapgap_2.0.log 2>&1 &