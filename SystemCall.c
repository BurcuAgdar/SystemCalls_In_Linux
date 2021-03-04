
#include <linux/uaccess.h>
#include <linux/syscalls.h>
#include <linux/slab.h>
#include <linux/init.h>
#include <linux/sched/signal.h>
#include <linux/sched.h>
#include <linux/cred.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/uidgid.h>
#include <linux/types.h>
#include <linux/string.h>
MODULE_LICENSE("GPL");
MODULE_AUTHOR("BURCU AGDAR");
struct task_struct *task;
struct mm_struct *mm;
struct task_struct task_child;
struct list_head *list;

#define PARAM_LEN 10

void process_module(char *user_p1, char *user_p2)
{
	int flag = 1;

	if (strcmp(user_p1, "-all") == 0)
	{
		for_each_process(task)
		{
			struct file *f;
			char buf[128];
			mm_segment_t fs;
			int i;

			int c = task->pid;
			char tplm[50];
			sprintf(tplm, "/proc/%d/status", c);
			printk("%s", tplm);

			for (i = 0; i < 128; i++)
				buf[i] = 0;
			f = filp_open(tplm, O_RDONLY, 0);
			if (f == NULL)
				printk(KERN_ALERT "filp_open error!!.\n");
			else
			{

				fs = get_fs();
				set_fs(get_ds());
				f->f_op->read(f, buf, 128, &f->f_pos);
				set_fs(fs);
				printk(KERN_INFO "buf:%s\n", buf);
			}
			filp_close(f, NULL);
		}
	}

	else if (strcmp(user_p1, "-p") == 0)
	{
		struct file *f;
		for_each_process(task)
		{
			int control = task->pid;
			char convert[10];
			sprintf(convert, "%d", control);
			if (strcmp(convert, user_p2)==0)
			{
				flag = 0;
				char buf[128];
				mm_segment_t fs;
				int i;

				int c = task->pid;
				char tplm[50];
				sprintf(tplm, "/proc/%d/status", c);
				//printk("%s",tplm);

				for (i = 0; i < 128; i++)
					buf[i] = 0;
				f = filp_open(tplm, O_RDONLY, 0);
				if (f == NULL)
					printk(KERN_ALERT "filp_open error!!.\n");
				else
				{

					fs = get_fs();
					set_fs(get_ds());
					f->f_op->read(f, buf, 128, &f->f_pos);
					set_fs(fs);
					printk(KERN_INFO "buf:%s\n", buf);
				}
				filp_close(f, NULL);
			}
		}
		if (flag == 1)
		{
			printk("\nThis process not avaliable.\n");
		}
	}

	else if (strcmp(user_p1, "-k") == 0)
	{
		for_each_process(task)
		{
			int control = task->pid;
			char convert[10];
			sprintf(convert, "%d", control);
			if (strcmp(convert, user_p2)==0)
			{

				if (kill_pid(find_vpid(control), SIGKILL, 1) == 0)
				{
					printk("\nkilled process : %d\n", control);
				}
			}
		}
	}
	else
	{
		printk("\n-all prints some information (process id and its argument/s) about all processes.\n -p takes process id and prints the 	details of it.\n -k takes process id.\n");
	}
}
SYSCALL_DEFINE2(SystemCall, char __user *, user_1, char __user *, user_2)
{

	char param_1[PARAM_LEN] = {'\0'};
	char param_2[PARAM_LEN] = {'\0'};

	copy_from_user(param_1, user_1, PARAM_LEN); // k,p,all

	copy_from_user(param_2, user_2, PARAM_LEN); //pid

	process_module(param_1, param_2);
}

