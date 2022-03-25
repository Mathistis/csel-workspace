setenv bootargs console=ttyS0,115200 earlyprintk root=/dev/mmcblk2p2 rootwait

fatload mmc 0 $kernel_addr_r Image
fatload mmc 0 $fdt_addr_r nanopi-neo-plus2.dtb

booti $kernel_addr_r - $fdt_addr_r
