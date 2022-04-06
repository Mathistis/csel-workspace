cmd_/workspace/src/02_module/exercice08/modules.order := {   echo /workspace/src/02_module/exercice08/mymodule.ko; :; } | awk '!x[$$0]++' - > /workspace/src/02_module/exercice08/modules.order
