#!/bin/bash
#SBATCH --job-name=gep-g4-6151-95m
#SBATCH --partition=production
#SBATCH --time=00:30:00
#SBATCH --mem=8G
#SBATCH --cpus-per-task=1
#SBATCH --output=/farm_out/brash/mono_6151_95m_centered_10_%j.out
#SBATCH --error=/farm_out/brash/mono_6151_95m_centered_10_%j.err

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
  6151 beam_on_10.mac 0 0 \
  --mono-electron 4.598 20.58 0.0 10.845 10.845 9.5 \
  < mono_6151_95m_center_input.txt
