#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/random.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/spinlock.h>

#define TAG "Lab8"
#define MAX_NUM 1000
int final[MAX_NUM];
int initial[MAX_NUM];

static struct task_struct *t1;
static struct task_struct *t2;
static struct task_struct *t3;
static struct task_struct *t4;

DEFINE_SPINLOCK(lock);



struct pair {
    int first;
    int second;
};


void sortedPrint(void) {
    int i = 0;
    int j = 0;

    printk(KERN_INFO "Number after the sorting :\n");
    for(i = 0; i < MAX_NUM && i<1000; i++) {
        if (i%20==0)
            printk("");
        printk(KERN_CONT "%d ", final[i]);
    }
}

void initialPrint(void) {
    int i = 0;
    printk(KERN_INFO "Initial array: \n");
    for(i = 0; i < MAX_NUM && i<1000; i++) {
        if (i%20==0)
            printk("");
        printk(KERN_CONT "%d ", initial[i]);
        
    }
}


static int sort(void *args) {
    spin_lock(&lock);
    struct pair *params = (struct pair*) args;
    int first = params->first;
    int second = params->second;
    int i = params->first;
    int temp_element;
    int j;
    while(i <= second) {
        temp_element = initial[i];
        j = i-1;
        while(j >= first && temp_element < initial[j]){
            initial[j+1] = initial[j];
            j--;
        }
        initial[j+1] = temp_element;
        i++;
    }
    spin_unlock(&lock);
    return 0;
}

static int merge(void *unused) {
    spin_lock(&lock);
    int i = 0;
    int j = MAX_NUM/2 + 1;
    int n = MAX_NUM - 1;
    int k = 0;
    while(i < MAX_NUM/2 + 1 && j <= n) {
        if(initial[i] < initial[j]) {
            final[k++] = initial[i++];
        }
        else {
            final[k++] = initial[j++];
        }
    }
    while(i < (MAX_NUM/2 + 1)) {
        final[k++] = initial[i++];
    }
    while(j <= n) {
        final[k++] = initial[j++];
    }
    spin_unlock(&lock);
    return 0;
}

void runAlgo (void){
    struct pair pair[2]; 

    pair[0].first = 0;
    pair[0].second = MAX_NUM/2;
    pair[1].first = MAX_NUM/2 + 1;
    pair[1].second = MAX_NUM - 1; 

    t1 = kthread_run(sort, &pair[0], "one");
    t2 = kthread_run(sort, &pair[1], "two");
    t3 = kthread_run(merge, NULL, "three");   
    t4 = kthread_run(merge, NULL, "four");

kthread_stop(t1);
    kthread_stop(t2);
    kthread_stop(t3);
    kthread_stop(t4);
}

int randomize (void){
    int n;
    get_random_bytes(&n, sizeof(n));
    n = n*n;
    n = n < 0 ? n*-1 : n;
    n = n % 10000;
    return n;
}

int init_module(void)
{
    printk(KERN_INFO "START %s\n", TAG);

    int i = 0;
    for(i = 0; i < MAX_NUM; i++) {
        initial[i] = randomize();
    }

    initialPrint();

    runAlgo();

    sortedPrint();

    
    return 0;
}

void cleanup_module(void)
{
    printk(KERN_DEBUG "END %s\n", TAG);
}