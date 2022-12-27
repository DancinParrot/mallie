#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kallsyms.h>
#include <linux/syscalls.h>
#include <linux/namei.h>

// Check kernel compiled options
// https://www.walkernews.net/2008/11/21/how-to-check-what-kernel-build-options-enabled-in-the-linux-kernel/
// cat /boot/config-`uname -r` | grep CONFIG_NETFILTER

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mallie");
MODULE_DESCRIPTION("A simple Linux module.");
MODULE_VERSION("0.01");

static unsigned long * __sys_call_table;

/* asmlinkage is important here -- the kernel expects syscall parameters to be
 * on the stack at this point, not inside registers.
 */
typedef asmlinkage long (*orig_mkdir_t)(const struct pt_regs *);
orig_mkdir_t orig_mkdir;

static int __init mallie_module_init(void)
{
	__sys_call_table = kallsyms_lookup_name("sys_call_table");

	printk(KERN_INFO "Mallie loaded.\n");
	if (sys_call_table == NULL) {
		printk(KERN_INFO "sys_call_table not found");
		return -1;
	}
	printk(KERN_INFO "sys_call_table found at 0x%1x", __sys_call_table);
	return 0;
}

static void __exit mallie_module_exit(void)
{
	printk(KERN_INFO "Byee!\n");
}

module_init(mallie_module_init);
module_exit(mallie_module_exit);
