#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/timer.h>
#include <linux/unistd.h>
#include <linux/sched.h>
#include <linux/ktime.h>
#include <linux/timekeeping.h>
#include <linux/gfp.h>
#include <linux/mm_types.h>
#include <linux/types.h>

#define PAGE_SIZE 4096

struct mystruct {
   unsigned int arrayOfIntegers[8];
};

struct mystruct m;

static int param = 2000; 

module_param(param, int, 0660);




struct page* mypages;

void print_param(void){
   printk(KERN_INFO "Printing module param: %d.\n \n", param);
}

void calculate_struct_and_num_of_inc(void){
   printk(KERN_INFO "Page size: %d, Struct size : %d. \nInstances:  %d. \n \n",PAGE_SIZE, sizeof(m), PAGE_SIZE/sizeof(m));
}

void pring_pages_and_time(void){
   int temp1 = PAGE_SIZE/sizeof(m);
   struct timespec t1, t2;
   int temp2 = param/temp1 + 1;
   int pow = 0;
   if(temp2 == 8) {
      pow = 3;
   } else if(temp2 == 16) {
      pow = 4;
   } else if(temp2 == 32) {
      pow = 5;
   }

   printk(KERN_INFO "2^%d=%d pages allocated", pow, PAGE_SIZE/sizeof(m));
   getnstimeofday(&t1);

   mypages = alloc_pages(GFP_KERNEL, pow);
   __free_pages(mypages, pow);

   getnstimeofday(&t2);
   printk(KERN_INFO "Start time: %lu \nEnd time: %lu.\nTime gap: %lu.", t1.tv_nsec , t2.tv_nsec ,t2.tv_nsec - t1.tv_nsec);
}

int init_module(void) {
   printk(KERN_INFO "Starting Lab 10 - Page Allocation.\n");   

   print_param();
   
   calculate_struct_and_num_of_inc();
   
   pring_pages_and_time();

   
   return 0;
}

void cleanup_module(void) {
   printk(KERN_INFO "Cleanup module. End of lab 10.\n");
}
