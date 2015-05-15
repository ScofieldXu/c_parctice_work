#include <stdbool.h>

/* B+��������(order) ������ÿ���ڵ��������capacity��
*  ��Ҷ�ӽڵ���ӽڵ���Ŀ��ΧΪ [order/2����ȡ��, order]
*  ��Ҷ�ӽڵ��root�ڵ�����������������2���ӽڵ�
*  ��Ҷ�ӽڵ��key����ĿΪ�ӽڵ���Ŀ-1
*  Ҷ�ӽڵ���ӽڵ�ָ��ʵ�ʴ洢ֵ����ΧΪ [order/2����ȡ��, order - 1]
*  Ҷ�ӽڵ��root�ڵ�����������������1���ӽڵ�洢ֵ
*  Ҷ�ӽڵ��key��Ŀ�����ӽڵ���Ŀ
*  ��B+�����������ʱ��ֻ��һ���ڵ㣬������root�ڵ㣬����Ҷ�ӽڵ㣬������ [1, order - 1]���ӽڵ�
*  ------------------------------------------------------------------------------------------------------------------
*  | Node Type        | Children Type                | Min Children   | Max Children | Min Keys           | Max Keys  |
*  ------------------------------------------------------------------------------------------------------------------
*  | Root Node (only) | Records                      | 1              | order - 1    | 1                  | order - 1 |
*  ------------------------------------------------------------------------------------------------------------------
*  | Root Node        | Internal Nodes or Leaf Nodes | 2              | order        | 1                  | order - 1 |
*  ------------------------------------------------------------------------------------------------------------------
*  | Internal Node    | Internal Nodes or Leaf Nodes | order/2����ȡ�� | order        | order/2����ȡ�� - 1 | order - 1 |
*  ------------------------------------------------------------------------------------------------------------------
*  | Leaf Node        | Records                      | order/2����ȡ�� | order - 1    | order/2����ȡ��     | order - 1 |
*  ------------------------------------------------------------------------------------------------------------------*/
#define ORDER 4

/* B+���ڵ� */
typedef struct node {
	void **pointers;		//�������ָ�����飬��Ҷ�ӽڵ�ָ���ӽڵ㣬Ҷ�ӽڵ�ָ��Record
	int *keys;				//�����Ԫ��Key���飬key��Ϊ��ָ��ķָ���ʶ������ָ������1��
	struct node *parent;	//ָ�򸸽ڵ��ָ��
	int num_of_keys;		//Ԫ����Ŀ���ô�ֵ����keys��pointers�ĳ���
	bool is_leaf;			//�Ƿ�Ҷ�ӽڵ�
	struct node *prev;		//˫��ָ�룬���ڿ�������
	struct node *next;
} Node;

/* B+�� */
typedef struct btree {
	Node *root;				//ָ��root�ڵ��ָ��
	Node *first_node;		//ָ���һ��key�ڵ��ָ��
} BTree;

/* B+��key��Ӧʵ�ʴ洢��ֵ */
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