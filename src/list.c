#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "utils.h"
#include "option_list.h"

list *make_list()
{
    list* l = (list*)xmalloc(sizeof(list));
    l->size = 0;
    l->front = 0;
    l->back = 0;
    return l;
}

/*
void transfer_node(list *s, list *d, node *n)
{
    node *prev, *next;
    prev = n->prev;
    next = n->next;
    if(prev) prev->next = next;
    if(next) next->prev = prev;
    --s->size;
    if(s->front == n) s->front = next;
    if(s->back == n) s->back = prev;
}
*/

void *list_pop(list *l){
    if(!l->back) return 0;
    node *b = l->back;
    void *val = b->val;
    l->back = b->prev;
    if(l->back) l->back->next = 0;
    free(b);
    --l->size;

    return val;
}

/*
 * 简介: 将 val 指针插入 list 结构体 l 中，这里相当于是用 C 实现了 C++ 中的 
 *         list 的元素插入功能
 * 
 * 参数: l    链表指针
 *         val  链表节点的元素值
 * 
 * 流程： list 中保存的是 node 指针. 因此，需要用 node 结构体将 val 包裹起来后才可以
 *       插入 list 指针 l 中
 * 
 * 注意: 此函数类似 C++ 的 insert() 插入方式；
 *      而 opion_insert() 函数类似 C++ map 的按值插入方式，比如 map[key]= value
 *      
 *      两个函数操作对象都是 list 变量， 只是操作方式略有不同。
*/
void list_insert(list *l, void *val)
{
    node* newnode = (node*)xmalloc(sizeof(node));
    newnode->val = val;
    newnode->next = 0;
    // 如果 list 的 back 成员为空(初始化为 0), 说明 l 到目前为止，还没有存入数据  
    // 另外, 令 l 的 front 为 new （此后 front 将不会再变，除非删除） 
    if(!l->back){
        l->front = newnode;
        newnode->prev = 0;
    }else{
        l->back->next = newnode;
        newnode->prev = l->back;
    }
    l->back = newnode;
    ++l->size;
}

void free_node(node *n)
{
    node *next;
    while(n) {
        next = n->next;
        free(n);
        n = next;
    }
}

void free_list_val(list *l)
{
    node *n = l->front;
    node *next;
    while (n) {
        next = n->next;
        free(n->val);
        n = next;
    }
}

void free_list(list *l)
{
    free_node(l->front);
    free(l);
}

void free_list_contents(list *l)
{
    node *n = l->front;
    while(n){
        free(n->val);
        n = n->next;
    }
}

void free_list_contents_kvp(list *l)
{
    node *n = l->front;
    while (n) {
        kvp* p = (kvp*)n->val;
        free(p->key);
        free(n->val);
        n = n->next;
    }
}

void **list_to_array(list *l)
{
    void** a = (void**)xcalloc(l->size, sizeof(void*));
    int count = 0;
    node *n = l->front;
    while(n){
        a[count++] = n->val;
        n = n->next;
    }
    return a;
}
