cmd_/workspace/src/02_module/exercice07/modules.order := {   echo /workspace/src/02_module/exercice07/mymodule.ko; :; } | awk '!x[$$0]++' - > /workspace/src/02_module/exercice07/modules.order
