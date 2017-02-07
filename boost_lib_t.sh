#!/bin/bash
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/share/boost_extr/boost_1_63_0/stage/lib

#gnome-terminal -e "bash -c \"!!; exec bash\"" echo $LD_LIBRARY_PATH & gnome-terminal & read -rsp $'Press any key to continue...\n' -n 1 key
gnome-terminal -x sh -c "echo $LD_LIBRARY_PATH; bash"

