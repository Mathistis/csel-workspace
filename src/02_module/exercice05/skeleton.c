// skeleton.c
#include <linux/module.h>	// needed by all modules
#include <linux/init.h>		// needed for macros
#include <linux/kernel.h>	// needed for debugging


#include <linux/ioport.h>	/* needed for memory region handling */
#include <linux/io.h>		/* needed for mmio handling */

#include <linux/moduleparam.h>	// needed for module parameters
#include <linux/slab.h>
static char* text = "dummy text";
module_param(text, charp, 0664);
static int  elements = 1;
module_param(elements, int, 0);
struct elem {
	int id;
	char* text;
	struct elem* next;
};

static struct resource* res[3]={[0]=0,};

static int __init skeleton_init(void)
{
	unsigned char* regs[3]={[0]=0,};
	unsigned int chipid[4]={[0]=0,};
	int temp = 0;
	unsigned int addr[2] = {[0]=0,};
	pr_debug ("  text: %s\n  elements: %d\n", text, elements);

	/**
	 * cat /proc/iomem :
	 * ...
	 * 01c14000-01c143ff : 1c14000.eeprom eeprom@1c14000
	 * ...
	 * 01c25000-01c253ff : 1c25000.thermal-sensor thermal-sensor@1c25000
	 * ...
	 * 01c30000-01c3ffff : 1c30000.ethernet ethernet@1c30000
	 * 
	 */
	res[0] = request_mem_region(0x01c14000, 0x1000, "allwiner h5 sid");
	res[1] = request_mem_region (0x01C25000, 0x1000, "allwiner h5 ths");
	res[2] = request_mem_region (0x01C30000, 0x1000, "allwiner h5 emac");
	if ((res[0] == 0) | (res[1] == 0) | (res[2] == 0))
		pr_info ("Error while reserving memory region... [0]=%d, [1]=%d, [2]=%d\n",	res[0]==0, res[1]==0, res[2]==0);

	regs[0] = ioremap (0x01c14200, 0x10); 	//Chip-ID
	regs[1] = ioremap (0x01C25080, 0x4);	//temperature
	regs[2] = ioremap (0x01C30050, 0x8);	//MAC

	if ((regs[0] == 0) || (regs[1] == 0) ||(regs[2] == 0)) {
		pr_info ("Error while trying to map processor register...\n");
		return -EFAULT;
	}


	temp = -1191 * (int) ioread32(regs[1]) / 10 + 223000;
	pr_debug("Temperature = %d", temp);
	

	addr[0]=ioread32(regs[2]);
	addr[1]=ioread32(regs[2]+4);
	pr_info("mac-addr=%02x:%02x:%02x:%02x:%02x:%02x\n",
			(addr[1]>> 0) & 0xff,
			(addr[1]>> 8) & 0xff,
			(addr[1]>>16) & 0xff,
			(addr[1]>>24) & 0xff,
			(addr[0]>> 0) & 0xff,
			(addr[0]>> 8) & 0xff
	);

	chipid[0] = ioread32 (regs[0]);
	chipid[1] = ioread32 (regs[0]+0x4);
	chipid[2] = ioread32 (regs[0]+0x8);
	chipid[3] = ioread32 (regs[0]+0xc);

	pr_info("chipid=%08x'%08x'%08x'%08x\n",
	 	chipid[0], chipid[1], chipid[2], chipid[3]);
	return 0;
}

static void __exit skeleton_exit(void)
{
	
	pr_info ("Linux module skeleton unloaded\n");
}

module_init (skeleton_init);
module_exit (skeleton_exit);

MODULE_AUTHOR ("Raemy Mathis , Macherel Rémy");
MODULE_DESCRIPTION ("Module skeleton");
MODULE_LICENSE ("GPL");