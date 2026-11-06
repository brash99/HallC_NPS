#!/bin/bash
set -euo pipefail

source /etc/profile.d/modules.sh
module purge
module use /group/halla/modulefiles
module load geant4/11.2.1
module load root/6.30.04

export DVCS_GEN_DIR=/group/nps/hhuang/software/dvcs_gen
export NPS_SOFT_DIR=/group/nps/hhuang/software/NPS_SOFT

repo=/work/hallc/gep/brash/gep4_nps_geant4
source_dir="$repo/DVCS_evt_gen/DVCS"
build_dir="$repo/DVCS_evt_gen/build_gep"

cmake -S "$source_dir" -B "$build_dir" \
  -DWITH_GEANT4_UIVIS=OFF \
  -DDVCS_GEN_DIR="$DVCS_GEN_DIR" \
  -DNPS_SOFT_DIR="$NPS_SOFT_DIR"
cmake --build "$build_dir" --parallel 4
