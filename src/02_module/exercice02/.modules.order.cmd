cmd_/workspace/src/02_module/exercice02/modules.order := {   echo /workspace/src/02_module/exercice02/mymodule.ko; :; } | awk '!x[$$0]++' - > /workspace/src/02_module/exercice02/modules.order
