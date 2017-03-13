#ifndef __STACK_H
#define __STACK_H
#include "types.h"

node_t * initnode(cord_t *__cord);
void push(node_t **__head, cord_t *__cord);
bool isempty(node_t **__head);
bool pop(node_t **__head, node_t *__data);
node_t * check(node_t *__head);

#endif	/* __STACK_H */
