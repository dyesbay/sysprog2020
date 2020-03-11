#include <linux/kernel.h> 
#include <linux/module.h> 
#include <linux/sched.h>
#include <linux/init_task.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/rbtree.h>

int calculate_subtree(struct rb_root *root, struct rb_node *node)
{
	int size ;
	struct vm_area_struct *vma;
	size = 0;
	if(node->rb_left)
		size += calculate_subtree(root, node->rb_left);
	
	vma = container_of(node, struct vm_area_struct, vm_rb);		
	size += (vma->vm_end - vma->vm_start)/1024;	

	if (node->rb_right) {
		size+=calculate_subtree(root, node->rb_right);
	}
	return size;
}

void calculate_vm(struct task_struct *p)
{	
	int size;
	struct rb_root *root = &p->mm->mm_rb;
	size = calculate_subtree(root, root->rb_node); 
	printk (KERN_INFO "Process %s uses %6u KB of virtual memory", 
				   p->comm, size);
}

static void print_mem(struct task_struct *task)
{
        struct mm_struct *mm;
        struct vm_area_struct *vma;
        int count = 0;
        mm = task->mm;
        printk("\nProcess %s has %d vmas.\n", task->comm, mm->map_count);
        
        for (vma = mm->mmap ; vma ; vma = vma->vm_next) {
			count++;
			printk ("VMA # %d stars at %%016lx and ends as %016lx", 
				   count, vma->vm_end, vma->vm_start);
        }

        printk("\nThis mm_struct user %6u KB.\n", size);

}



int init_module(void)
{
    printk(KERN_INFO "We will look into a process and it's memory usage.\n");
    calculate_vm(current);
	print_mem(current);
    return 0;
}

void cleanup_module(void)
{
        printk(KERN_INFO "That's it\n");
}