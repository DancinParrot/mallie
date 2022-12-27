#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/kallsyms.h>

// Check kernel compiled options
// https://www.walkernews.net/2008/11/21/how-to-check-what-kernel-build-options-enabled-in-the-linux-kernel/
// cat /boot/config-`uname -r` | grep CONFIG_NETFILTER

static unsigned long *sys_call_table;

static int __init mallie_module_init(void)
{
	printk(KERN_INFO "Hi!\n");
	sys_call_table = (void *)kallsyms_lookup_name("sys_call_table");

	if (sys_call_table == NULL) {
		printk(KERN_INFO "sys_call_table not found");
		return -1;
	}
	printk(KERN_INFO "sys_call_table found at %p");
	return 0;
}

static void __exit mallie_module_exit(void)
{
	printk(KERN_INFO "Byee!\n");
}

module_init(mallie_module_init);
module_exit(mallie_module_exit);
