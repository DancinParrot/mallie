#include <linux/version.h>
#include <linux/kallsyms.h>

#if LINUX_VERSION_CODE >= KERNEL_VERSION(5, 7, 0)
#define KPROBE_LOOKUP 1
#include <linux/kprobes.h>
static struct kprobe kp = {
	.symbol_name = "kallsyms_lookup_name",
};
#endif

static unsigned long * __sys_call_table;

/* We pack all the information we need (name, hooking function, original function)
 * into this struct. This makes is easier for setting up the hook and just passing
 * the entire struct off to fh_install_hook() later on.
 * */
struct ftrace_hook {
    const char *name;
    void *function;
    void *original;

    unsigned long address;
    struct ftrace_ops ops;
};

/* Ftrace needs to know the address of the original function that we
 * are going to hook. As before, we just use kallsyms_lookup_name() 
 * to find the address in kernel memory.
 * */
static int fh_resolve_hook_address(struct ftrace_hook *hook)
{
	#ifdef KPROBE_LOOKUP
    	typedef unsigned long (*kallsyms_lookup_name_t)(const char *name);
    	kallsyms_lookup_name_t kallsyms_lookup_name;
    	register_kprobe(&kp);
    	kallsyms_lookup_name = (kallsyms_lookup_name_t) kp.addr;
    	unregister_kprobe(&kp);
	#endif
		__sys_call_table = kallsyms_lookup_name("sys_call_table");
		print("sys_call_table found at 0x%1x", __sys_call_table);

    	hook->address = kallsyms_lookup_name(hook->name);

    	if (!hook->address)
    	{
        	printk(KERN_DEBUG "mallie: unresolved symbol: %s\n", hook->name);
        	return -ENOENT;
    	}

	#if USE_FENTRY_OFFSET
    	*((unsigned long*) hook->original) = hook->address + MCOUNT_INSN_SIZE;
	#else
    	*((unsigned long*) hook->original) = hook->address;
	#endif

    return 0;
}