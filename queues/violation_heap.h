#ifndef VIOLATION_HEAP
#define VIOLATION_HEAP

//==============================================================================
// DEFINES AND INCLUDES
//==============================================================================

#include "heap_common.h"

//==============================================================================
// STRUCTS
//==============================================================================

/**
* Holds an inserted element, as well as pointers to maintain tree
* structure.  Acts as a handle to clients for the purpose of
* mutability.  Tracks rank of node as well as pointers to this node's
* first child and the next and previous nodes in the list of siblings.
* The last node in the list of siblings will have a null prev pointer
* and the first node's next pointer will point to their parent.
*/
typedef struct violation_node_t
{
    //! Last child of this node
    struct violation_node_t *child;
    //! Next node in the list of this node's siblings
    struct violation_node_t *next;
    //! Previous node in the list of this node's siblings
    struct violation_node_t *prev;

    //! The number of children this node has
    int32_t rank;

    //! Pointer to a piece of client data
    item_type item;
    //! Key for the item
    key_type key;
} violation_node;

/**
 * A mutable, meldable, violation heap.  Maintains a forest of trees indexed by
 * rank.  At most two trees of each rank remain after a @ref <pq_delete> or @ref
 * <pq_delete_min> operation.
 */
typedef struct violation_heap_t
{
    //! The number of items held in the heap
    uint32_t size;
    //! Pointer to the minimum node in the heap
    violation_node* minimum;
    //! An array of roots of the heap, indexed by rank
    violation_node* roots[MAXRANK][2];
    //! Current largest rank in heap
    uint32_t largest_rank;
} violation_heap;

typedef violation_heap* pq_ptr;
typedef violation_node it_type;

//==============================================================================
// PUBLIC DECLARATIONS
//==============================================================================

/**
 * Creates a new, empty heap.
 *
 * @param capacity  Maximum number of nodes the heap is expected to hold
 * @return          Pointer to the new heap
 */
violation_heap* pq_create( uint32_t capacity );

/**
 * Frees all the memory used by the heap.
 *
 * @param heap  Heap to destroy
 */
void pq_destroy( violation_heap *heap );

/**
 * Repeatedly deletes nodes associated with the heap until it is empty.
 *
 * @param heap  Heap to clear
 */
void pq_clear( violation_heap *heap );

/**
 * Returns the key associated with the queried node.
 *
 * @param heap  Heap to which node belongs
 * @param node  Node to query
 * @return      Node's key
 */
key_type pq_get_key( violation_heap *heap, violation_node *node );

/**
 * Returns the item associated with the queried node.
 *
 * @param heap  Heap to which node belongs
 * @param node  Node to query
 * @return      Node's item
 */
item_type* pq_get_item( violation_heap *heap, violation_node *node );

/**
 * Returns the current size of the heap.
 *
 * @param heap  Heap to query
 * @return      Size of heap
 */
uint32_t pq_get_size( violation_heap *heap );

/**
 * Takes an item-key pair to insert into the heap and creates a new
 * corresponding node.  Makes the new node a root.
 *
 * @param heap  Heap to insert into
 * @param item  Item to insert
 * @param key   Key to use for node priority
 * @return      Pointer to corresponding node
 */
violation_node* pq_insert( violation_heap *heap, item_type item, key_type key );

/**
 * Returns the minimum item from the heap.
 *
 * @param heap  Heap to query
 * @return      Node with minimum key
 */
violation_node* pq_find_min( violation_heap *heap );

/**
 * Removes the minimum item from the heap and returns it.  Relies on
 * @ref <pq_delete> to remove the minimum item.
 *
 * @param heap  Heap to query
 * @return      Minimum key, corresponding to item deleted
 */
key_type pq_delete_min( violation_heap *heap );

/**
 * Removes an arbitrary item from the heap and modifies heap structure
 * to preserve heap properties.  Requires that the location of the
 * item's corresponding node is known.  Merges the node's children with
 * the root list.  Merges roots such that no more than two roots have
 * the same rank.
 *
 * @param heap  Heap in which the node resides
 * @param node  Pointer to node corresponding to the item to remove
 * @return      Key of item removed
 */
key_type pq_delete( violation_heap *heap, violation_node *node );

/**
 * If the item in the heap is modified in such a way to decrease the
 * key, then this function will update the heap to preserve heap
 * properties given a pointer to the corresponding node.  Replaces
 * the subtree rooted at the given node with its active subtree of
 * larger rank and then relocates the rest of the tree as a new root.
 *
 * @param heap      Heap in which the node resides
 * @param node      Node to change
 * @param new_key   New key to use for the given node
 */
void pq_decrease_key( violation_heap *heap, violation_node *node,
    key_type new_key );

/**
 * Determines whether the heap is empty, or if it holds some items.
 *
 * @param heap  Heap to query
 * @return      True if heap holds nothing, false otherwise
 */
bool pq_empty( violation_heap *heap );

#endif
