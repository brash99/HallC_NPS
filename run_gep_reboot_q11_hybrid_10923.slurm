#!/bin/bash
#SBATCH --job-name=gep-g4-q11-10923
#SBATCH --partition=production
#SBATCH --time=01:00:00
#SBATCH --mem=8G
#SBATCH --cpus-per-task=1
#SBATCH --output=/farm_out/brash/gep_q11_hybrid_10923_%j.out
#SBATCH --error=/farm_out/brash/gep_q11_hybrid_10923_%j.err

set -euo pipefail

source /etc/profile.d/modules.sh
module purge
module use /group/halla/modulefiles
module load geant4/11.2.1
module load root/6.30.04

export DVCS_GEN_DIR=/group/nps/hhuang/software/dvcs_gen
export NPS_SOFT_DIR=/group/nps/hhuang/software/NPS_SOFT

repo=/work/hallc/gep/brash/gep4_nps_geant4
cd "$repo"

/usr/bin/time -v ./DVCS_evt_gen/build_gep/DVCS \
  6151 beam_on_10923.mac 0 0 \
  --simc-events validation/gep_reboot_q11_hybrid.hybrid.csv 26.651 3.0 \
  < validation/gep_reboot_q11_hybrid_10923.input
