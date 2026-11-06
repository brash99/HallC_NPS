#!/bin/bash
#SBATCH --job-name=g4-face-scan
#SBATCH --partition=production
#SBATCH --time=00:30:00
#SBATCH --mem=8G
#SBATCH --cpus-per-task=1
#SBATCH --array=0-9%4
#SBATCH --output=/farm_out/brash/nps_face_scan_200_%A_%a.out
#SBATCH --error=/farm_out/brash/nps_face_scan_200_%A_%a.err

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

positions=(center midface near_edge edge corner)
x_values=(10.845 5.0 2.0 0.0 0.0)
y_values=(10.845 10.845 10.845 10.845 0.0)

energy_index=$((SLURM_ARRAY_TASK_ID / 5))
position_index=$((SLURM_ARRAY_TASK_ID % 5))

if (( energy_index == 0 )); then
  run=6151
  energy=4.598
  angle=20.58
  energy_tag=4598
else
  run=2875
  energy=7.072
  angle=16.991
  energy_tag=7072
fi

position=${positions[$position_index]}
local_x=${x_values[$position_index]}
local_y=${y_values[$position_index]}
output="$repo/validation/face_scan_${energy_tag}_${position}_200.root"

printf '%s\n17\n12345\n67890\n' "$output" | \
  /usr/bin/time -v ./DVCS_evt_gen/build_gep/DVCS \
    "$run" beam_on_200.mac 0 0 \
    --mono-electron "$energy" "$angle" 0.0 "$local_x" "$local_y" 9.5
