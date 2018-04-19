#include <linux/errno.h>    // For error constants.
#include <linux/kernel.h>   // For printk().
#include <linux/syscalls.h> // For syscall macros.
#include <asm/uaccess.h>

/*
 *  pflag: if non-zero print inside the system call (using printk)
 *  tspec: pointer passed to your system call, the current time
 *         will be "returned" through this parameter
 *  returns: 0 on success, -1 on error
 */
//A system call to hide a process from being listed in the /proc filesystem
SYSCALL_DEFINE1(stealth, pid_t, pid) {

  printk("We want to hide this process from the world thanks\n");

  rcu_read_lock();
  struct task_struct  *ts = find_task_by_vpid(pid);
  //For now let's pretend this will always work
  if(ts){get_task_struct(ts);}
  else{
	printk("whoa oh that is not a thing u can hide kid\n");
	return -ESRCH;}
  rcu_read_unlock();

  unsigned long flags;
  raw_spin_lock_irqsave(&ts->pi_lock, flags);
  if(ts->sflag==0){ts->sflag=1;}
  else{ts->sflag=0;}
  printk("sflag = %d\n",ts->sflag);
  raw_spin_unlock_irqrestore(&ts->pi_lock, flags);
  return 0;
}
