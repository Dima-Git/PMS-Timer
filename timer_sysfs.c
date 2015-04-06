#include <linux/kobject.h>
#include <linux/string.h>
#include <linux/sysfs.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/timer.h>

static struct kobject *timer_file_kobj;
static struct timer_list timer;
int timeout = 0;

ssize_t timer_sysfs_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n", timeout);
}

ssize_t timer_sysfs_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
	sscanf(buf, "%d", &timeout);
	if (timeout == 0)
		del_timer(&timer);
	else
		mod_timer(&timer, jiffies + msecs_to_jiffies(timeout));
	return count;
}

static struct kobj_attribute timer_attribute = 
	__ATTR(timer_period, 0666, timer_sysfs_show, timer_sysfs_store);

static struct attribute *attrs[] = { &timer_attribute.attr, NULL };

static struct attribute_group timer_attr_group = { 
	.attrs = attrs, 
};

void timer_sysfs_callback(unsigned long data)
{
	printk("Timer SysFS Message.\n");
	mod_timer(&timer, jiffies + msecs_to_jiffies(timeout));
}

static int __init timer_sysfs_init(void)
{
	int err;
	timer_file_kobj = kobject_create_and_add("timer_dir", NULL);
	err = sysfs_create_group(timer_file_kobj, &timer_attr_group);
	if (err)
		kobject_put(timer_file_kobj);
	setup_timer(&timer, timer_sysfs_callback, 0);
	return err;
}

static void __exit timer_sysfs_exit(void)
{
	del_timer(&timer);
	kobject_put(timer_file_kobj);
}

module_init(timer_sysfs_init);
module_exit(timer_sysfs_exit);
MODULE_LICENSE("GPL");