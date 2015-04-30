
/*single link node*/
typedef struct singleLinkNode{
	int id;
	struct singleLinkNode *next;
} SingleLinkNode;

/*single link*/
typedef struct singleLink{
	struct singleLinkNode *root;
	struct singleLinkNode *last;
} SingleLink;

/*double link node*/
typedef struct doubleLinkNode{
	int id;
	struct doubleLinkNode *prev;
	struct doubleLinkNode *next;
} DoubleLinkNode;