cmd_/workspace/src/02_module/exercice08/Module.symvers := sed 's/\.ko$$/\.o/' /workspace/src/02_module/exercice08/modules.order | scripts/mod/modpost    -o /workspace/src/02_module/exercice08/Module.symvers -e -i Module.symvers   -T -