cmd_/workspace/src/03_kernel/exercice03/modules.order := {   echo /workspace/src/03_kernel/exercice03/mymodule.ko; :; } | awk '!x[$$0]++' - > /workspace/src/03_kernel/exercice03/modules.order
