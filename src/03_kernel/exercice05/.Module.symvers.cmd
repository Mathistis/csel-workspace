cmd_/workspace/src/03_kernel/exercice05/Module.symvers := sed 's/\.ko$$/\.o/' /workspace/src/03_kernel/exercice05/modules.order | scripts/mod/modpost    -o /workspace/src/03_kernel/exercice05/Module.symvers -e -i Module.symvers   -T -