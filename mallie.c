#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init mallie_module_init(void)
{
	printk(KERN_INFO "Driver version %s\n", EI_VERSION);
	return 0;
}

static void __exit mallie_module_exit(void)
{
	printk(KERN_INFO "Byee!\n");
}

module_init(mallie_module_init);
module_exit(mallie_module_exit);
