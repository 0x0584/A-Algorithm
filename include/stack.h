#ifndef __STACK_H
#define __STACK_H
#include "types.h"

node_t * initnode(cord_t *cord);
void push(node_t **head, cord_t *cord);
bool isempty(node_t **head);
bool pop(node_t **head, node_t *data);
node_t * check(node_t *head);

#endif	/* __STACK_H */
