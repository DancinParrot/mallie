#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/kallsyms.h>
#include <linux/version.h>

#include "ftrace_helper.h"

// Check kernel compiled options
// https://www.walkernews.net/2008/11/21/how-to-check-what-kernel-build-options-enabled-in-the-linux-kernel/
// cat /boot/config-`uname -r` | grep CONFIG_NETFILTER

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mallie");
MODULE_DESCRIPTION("Driver for rtl8192eu wifi adapter.");
MODULE_VERSION("0.1");

static int __init mallie_module_init(void)
{

	printk(KERN_INFO "Mallie loaded.\n");

	return 0;
}

static void __exit mallie_module_exit(void)
{
	printk(KERN_INFO "Mallie: Exiting...\n");
}

module_init(mallie_module_init);
module_exit(mallie_module_exit);
