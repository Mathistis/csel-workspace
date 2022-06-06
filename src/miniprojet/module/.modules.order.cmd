cmd_/workspace/src/miniprojet/module/modules.order := {   echo /workspace/src/miniprojet/module/mymodule.ko; :; } | awk '!x[$$0]++' - > /workspace/src/miniprojet/module/modules.order
