#!/bin/bash
#SBATCH --job-name=gep-g4-mat-78
#SBATCH --partition=production
#SBATCH --time=00:30:00
#SBATCH --mem=8G
#SBATCH --cpus-per-task=1
#SBATCH --output=/farm_out/brash/simc_material_78_%j.out
#SBATCH --error=/farm_out/brash/simc_material_78_%j.err

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
  6151 beam_on_78.mac 0 0 \
  --simc-events validation/gep_pac37_q11_elastic.hybrid.csv 25.486 3.0 \
  < validation/simc_hybrid_target_off_78.input

/usr/bin/time -v ./DVCS_evt_gen/build_gep/DVCS \
  6151 beam_on_78.mac 0 0 \
  --simc-events-target-on validation/gep_pac37_q11_elastic.hybrid.csv 25.486 3.0 \
  < validation/simc_hybrid_target_on_78.input
