#include <stdbool.h>

/* B+树的序数(order) 代表了每个节点的容量（capacity）
*  非叶子节点的子节点数目范围为 [order/2向上取整, order]
*  非叶子节点的root节点特例，可以有至少2个子节点
*  非叶子节点的key的数目为子节点数目-1
*  叶子节点的子节点指向实际存储值，范围为 [order/2向上取整, order - 1]
*  叶子节点的root节点特例，可以有至少1个子节点存储值
*  叶子节点的key数目等于子节点数目
*  当B+树最初建立的时候，只有一个节点，它既是root节点，又是叶子节点，可以有 [1, order - 1]个子节点
*  ------------------------------------------------------------------------------------------------------------------
*  | Node Type        | Children Type                | Min Children   | Max Children | Min Keys           | Max Keys  |
*  ------------------------------------------------------------------------------------------------------------------
*  | Root Node (only) | Records                      | 1              | order - 1    | 1                  | order - 1 |
*  ------------------------------------------------------------------------------------------------------------------
*  | Root Node        | Internal Nodes or Leaf Nodes | 2              | order        | 1                  | order - 1 |
*  ------------------------------------------------------------------------------------------------------------------
*  | Internal Node    | Internal Nodes or Leaf Nodes | order/2向上取整 | order        | order/2向上取整 - 1 | order - 1 |
*  ------------------------------------------------------------------------------------------------------------------
*  | Leaf Node        | Records                      | order/2向上取整 | order - 1    | order/2向上取整     | order - 1 |
*  ------------------------------------------------------------------------------------------------------------------*/
#define ORDER 4

/* B+树节点 */
typedef struct node {
	void **pointers;		//有序的子指针数组，非叶子节点指向子节点，叶子节点指向Record
	int *keys;				//有序的元素Key数组，key作为子指针的分隔标识，比子指针数少1个
	struct node *parent;	//指向父节点的指针
	int num_of_keys;		//元素数目，用此值控制keys和pointers的长度
	bool is_leaf;			//是否叶子节点
	struct node *prev;		//双向指针，用于快速搜索
	struct node *next;
} Node;

/* B+树 */
typedef struct btree {
	Node *root;				//指向root节点的指针
	Node *first_node;		//指向第一个key节点的指针
} BTree;

/* B+树key对应实际存储的值 */
typedef struct record {
	char *value;
} Record;

typedef struct queue {
	int capacity;
	int front;
	int rear;
	int size;
	Node **items;
} queue;