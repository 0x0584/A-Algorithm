#include "../include/lib.h"
#include "../include/stack.h"

node_t *
initnode(cord_t *c)
{
  node_t *foonode = (node_t *) malloc(sizeof(node_t));

  if(foonode == NULL) return NULL;

  foonode->cord = c;
  foonode->next = NULL;
  
  return foonode;
}

void
push(node_t **h, cord_t *c)
{
  node_t *new = initnode(c);
  new->next = *h;
  *h = new;
}

inline bool
isempty(node_t **h)
{
  if((*h) == NULL) return true;
  else return false;
}

bool
pop(node_t **h, node_t *d)
{
  node_t *foonode;

  if(!isempty(h)) {
    d->cord = (*h)->cord;
    d->mvcost = (*h)->mvcost;

    foonode = (*h);

    (*h) = (*h)->next;

    free(foonode);
    
    return true;
  } else return false;
}

node_t *
check(node_t *h)
{
  if(!h) return NULL;

  node_t *foonode = (node_t *) malloc(sizeof(node_t));
  
  foonode->cord = h->cord;
  
  return foonode;
}
