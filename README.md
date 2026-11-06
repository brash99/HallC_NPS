# HallC_NPS

Hall_C NPS Geant4 Simulation 

For Hall C DVCS (DVCS_evt_gen/), see https://wiki.jlab.org/cuawiki/images/f/fa/User_Guide.pdf for a short description on how to run on JLab/ifarm

## GEp-4 development

This fork preserves the NPS simulation work used for the Hall C GEp-4 reboot
feasibility studies.  The development branch adds:

- external-particle and hybrid event input for transporting SIMC and
  target-background samples through the NPS geometry;
- monoenergetic and beam-electron source modes;
- electronuclear interaction biasing and particle-lineage bookkeeping;
- optional target-length and sweeper-geometry controls for measured-run and
  GEp-4 configurations;
- a standalone `target_phase_space` generator that separates target
  production from detector transport; and
- reproducible validation macros, batch scripts, compact rate summaries, and
  diagnostic plots.

Large ROOT files, event-level CSV files, build products, batch outputs, and
farm executables are intentionally excluded from Git.  They remain in the
working farm area and can be regenerated from the committed source, inputs,
and job scripts.

The original Hall C NPS development repository is maintained by
[hhuang-hep](https://github.com/hhuang-hep/HallC_NPS).  This fork retains that
repository as `upstream`; GEp-specific work is maintained on the
`gep4-backgrounds` branch.
