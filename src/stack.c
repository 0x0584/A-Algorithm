#include "../include/lib.h"
#include "../include/stack.h"

node_t *
initnode(cord_t *cord)
{
  node_t *foonode = (node_t *) malloc(sizeof(node_t));

  if(foonode == NULL) return NULL;

  foonode->cord = cord;
  foonode->next = NULL;
  
  return foonode;
}

void
push(node_t **head, cord_t *cord)
{
  node_t *new = initnode(cord);
  new->next = *head;
  *head = new;
}

inline bool
isempty(node_t **head)
{
  if((*head) == NULL) return true;
  else return false;
}

bool
pop(node_t **head, node_t *data)
{
  node_t *foonode;

  if(!isempty(head)) {
    data->cord = (*head)->cord;
    data->mvcost = (*head)->mvcost;

    foonode = (*head);

    (*head) = (*head)->next;

    free(foonode);
    
    return true;
  }  else return false;
}

node_t *
check(node_t *head)
{
  if(!head) return NULL;

  node_t *foonode = (node_t *) malloc(sizeof(node_t));
  
  foonode->cord = head->cord;
  
  return foonode;
}
