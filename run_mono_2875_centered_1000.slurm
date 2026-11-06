#!/bin/bash
#SBATCH --job-name=g4-2875-1k
#SBATCH --partition=production
#SBATCH --time=00:30:00
#SBATCH --mem=8G
#SBATCH --cpus-per-task=1
#SBATCH --output=/farm_out/brash/mono_2875_centered_1000_%j.out
#SBATCH --error=/farm_out/brash/mono_2875_centered_1000_%j.err

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
  2875 beam_on_1000.mac 0 0 \
  --mono-electron 7.072 16.991 0.0 10.845 10.845 9.5 \
  < mono_2875_center_input_1000.txt
