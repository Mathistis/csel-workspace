// skeleton.c
#include <linux/module.h>	// needed by all modules
#include <linux/init.h>		// needed for macros
#include <linux/kernel.h>	// needed for debugging

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
struct elem* head;

static int __init skeleton_init(void)
{
	int i = elements;
	struct elem* it; //iterator
	pr_debug ("  text: %s\n  elements: %d\n", text, elements);

	if (i > 0){
		head = (struct elem* ) kmalloc(sizeof( struct elem), GFP_KERNEL);
		head->id = i;
		head->text = text;
		head->next = NULL;
		it = head;
		i--;
		pr_info("Head created\n");
	}
	for (; i > 0 ; i--){
		it->next =  (struct elem* ) kmalloc(sizeof( struct elem), GFP_KERNEL);
		it = it->next;
		it->id = i;
		it->text = text;
		it->next = NULL;
		pr_info ("Elem created\n");
	}
	return 0;
}

static void __exit skeleton_exit(void)
{
	int i = 0;
	for(;i < elements; i++){
		struct elem* elem_to_free = head;
		head = head->next;
		kfree(elem_to_free);
		pr_info("Il a free, il a tout compris %d\n", i);
	}
	pr_info ("Linux module skeleton unloaded\n");
}

module_init (skeleton_init);
module_exit (skeleton_exit);

MODULE_AUTHOR ("Raemy Mathis , Macherel Rémy");
MODULE_DESCRIPTION ("Module skeleton");
MODULE_LICENSE ("GPL");