#/bin/bash

DD=data_transfer
CONFN=( JAPU+INCL JAPU+TIME CSPU+INCL CSPU+TIME CSSK+INCL CSSK+TIME )

for CF in ${CONFN[@]}; do
    FILEN=(`ls $CF`)
    for FN in ${FILEN[@]}; do
	NF=$DD"/"${CF}"_"${FN}
	OF=${CF}"/"${FN}
	echo "Copy "${OF}" to "${NF}
	cp ${OF} ${NF} 
    done
done
