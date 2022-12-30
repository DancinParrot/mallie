#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/kprobes.h>

// Check kernel compiled options
// https://www.walkernews.net/2008/11/21/how-to-check-what-kernel-build-options-enabled-in-the-linux-kernel/
// cat /boot/config-`uname -r` | grep CONFIG_NETFILTER

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mallie");
MODULE_DESCRIPTION("Provides functions to supplement logging of kernel activities.");
MODULE_VERSION("0.01");

static int __init mallie_module_init(void)
{
	// kallsyms_lookup_name not exported in latest kernel versions, even if CONFIG_KALLSYMS is enabled when compiling
	//__sys_call_table = kallsyms_lookup_name("sys_call_table");

	printk(KERN_INFO "Mallie loaded.\n");

	register_kprobe(&kp);
	pr_alert("kprobe registered at 0x%px\n", kp.addr);

	if (sys_call_table == NULL) {
		printk(KERN_INFO "sys_call_table not found");
		return -1;
	}

	//printk(KERN_INFO "sys_call_table found at 0x%1x", __sys_call_table);
	return 0;
}

static void __exit mallie_module_exit(void)
{
	unregister_kprobe(&kp);
	printk(KERN_INFO "Mallie: Exiting...\n");
}

module_init(mallie_module_init);
module_exit(mallie_module_exit);
