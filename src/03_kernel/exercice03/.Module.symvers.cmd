cmd_/workspace/src/03_kernel/exercice03/Module.symvers := sed 's/\.ko$$/\.o/' /workspace/src/03_kernel/exercice03/modules.order | scripts/mod/modpost    -o /workspace/src/03_kernel/exercice03/Module.symvers -e -i Module.symvers   -T -
