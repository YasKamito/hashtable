/***************************************************************************//**
 *
 * @file 		hash.c
 *
 * @brief		HashTable
 *
 * @date 		Created on: 2015/06/18
 *
 * @version		v0.0.1
 *
 * @par change log:
 * | ModID  | Date       | Name           | Description           | TicketID  |
 * | :----: | :--------: | :--------------| :-------------------- | :-------: |
 * |        | 2015-06-20 | Y.Kamitokusari | Initial release       |           |
 *
 ******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>

#include "hash.h"

char initword[HASHSIZE][MAXWORDLEN + 1];		/**< �n�b�V���m�[�h�Ԑl�pKey(�z��) */
struct node initrootnode[HASHSIZE] = {NULL, NULL, NULL, 0};	/**< �n�b�V���m�[�h�Ԑl(�z��) */

long g_maxvalue;

/***************************************************************************//**
 * @fn		void hashaddrdump(nodeptr *p)
 * �f�o�b�O�p�������_���v
 * �֐��W���G���[�o�͂Ƀn�b�V���e�[�u���̓��e���o�͂���
 *
 * @param[in]	nodeptr *p		�o�͂���n�b�V���e�[�u���̃A�h���X
 * @retval	�Ȃ�
 *
 * ### �������e
 *
 ******************************************************************************/
void hashaddrdump(nodeptr *p){

	fprintf(stderr, "    ++--------------------------++\n");
	fprintf(stderr, "    hashtable addr       =%x\n", p);
	if (*p == 0x00){
		fprintf(stderr, "    ++--------------------------++\n");
		return;
	}
	fprintf(stderr, "    key                  =%s\n", (*p)->key);
	fprintf(stderr, "    left                 =%x\n", (*p)->left);
	fprintf(stderr, "    right                =%x\n", (*p)->right);
	fprintf(stderr, "    val                  =%d\n", (*p)->val);
	fprintf(stderr, "    ++--------------------------++\n");
	return;
}

/***************************************************************************//**
 * @fn		int hash_impl(char *pchStr)
 * �����̕�����̃n�b�V���l��Ԃ�
 *
 * @param[in]	char *pchStr	������ւ̃|�C���^
 * @retval	int	�n�b�V���l
 *
 * ### �������e
 *
 ******************************************************************************/
int hash_impl(char *pchStr){

	unsigned v,x;

	for(v=0;*pchStr != '\0'; pchStr++){
		x = v << CHAR_BIT;
		v = ((v << CHAR_BIT) + *pchStr) % HASHSIZE;
	}

	return (int)v;
}
int (*hash)(char *) = hash_impl;

/***************************************************************************//**
 * @fn		int hash_init_impl(nodeptr *hashtable, ...)
 * �n�b�V���e�[�u��������������
 *
 * @param[in]	nodeptr *hashtable	�n�b�V���e�[�u���̃A�h���X
 * @param[in]	long	maxvalue	�n�b�V���e�[�u����value�l�̍ő�l
 * @retval	0	����
 *
 * ### �������e
 *
 ******************************************************************************/
int hash_init_impl(nodeptr *hashtable, long maxvalue){

	int i;

	/**
	 * �������o�����[�ő�l���擾
	 * �w�肪�Ȃ���΃f�t�H���g��999999���Z�b�g
	 */
	g_maxvalue = maxvalue;
	if(g_maxvalue == 0){
		g_maxvalue = 999999;
	}

	/**
	 * HASHSIZE�̕�����nodeptr *hashtable������������
	 * �������ɂ́Ainitrootnode���g�p����
	 * �܂��Ahashtable->key�ɂ�initword�̃A�h���X���Z�b�g����
	 */
	for(i = 0; i < HASHSIZE; i++){
		hashtable[i] = &initrootnode[i];
		hashtable[i]->key = initword[i];
	}

	return 0;

}
int (*hash_init)(nodeptr *, long) = hash_init_impl;

/***************************************************************************//**
 * @fn		nodeptr hash_bin_search_impl(nodeptr *p, nodeptr *root, char *pKey)
 * �Q���T���؊֐�
 *
 * @param[in]	nodeptr *p			�n�b�V���e�[�u���̃A�h���X
 * @param[in]	nodeptr *root		���[�g�m�[�h�̃A�h���X
 * @param[in]	char *pKey			Key�l�̕�����ւ̃|�C���^
 * @retval	��������
 *
 * ### �������e
 *
 ******************************************************************************/
nodeptr * hash_bin_search_impl(nodeptr *currnode, nodeptr *initroot, char *pkey){

	int cmp;

	/**
	 * strcmp�֐��ɂ�镶�����r�ɂ��A�Q���T���؂��s���B
	 */
	while ((cmp = strcmp(pkey, (*currnode)->key)) != 0){
		if (cmp < 0){
#ifdef DEBUG
			fprintf(stderr, "  ->left \n");
			fprintf(stderr, "  ->pKey                 =%s\n", pkey);
			fprintf(stderr, "  ->strcmp return code   =%d\n", cmp);
			hashaddrdump(currnode);
#endif
			currnode = &((*currnode)->left );
			if (*currnode == *initroot || *currnode == 0x00) break;
		}else{
#ifdef DEBUG
			fprintf(stderr, "  ->right \n");
			fprintf(stderr, "  ->pKey                 =%s\n", pkey);
			fprintf(stderr, "  ->strcmp return code   =%d\n", cmp);
			hashaddrdump(currnode);
#endif
			currnode = &((*currnode)->right);
			if (*currnode == *initroot || *currnode == 0x00) break;
		}
	}

#ifdef DEBUG
	fprintf(stderr, "  ->srch exit. strcmp rc =%d\n", cmp);
	fprintf(stderr, "  ->pKey                 =%s\n", pkey);
	hashaddrdump(currnode);
#endif

	return currnode;
}
nodeptr *(*hash_bin_search)(nodeptr *, nodeptr *, char *) = hash_bin_search_impl;

/***************************************************************************//**
 * @fn		nodeptr * hash_malloc_impl(nodeptr *currnode, nodeptr *initroot, char *pKeyStr, int iValue)
 * �Q���T����malloc�֐�
 *
 * @param[in]	nodeptr *p			�n�b�V���e�[�u���̃A�h���X
 * @param[in]	nodeptr *root		���[�g�m�[�h�̃A�h���X
 * @param[in]	char *pKey			Key�l�̕�����ւ̃|�C���^
 * @param[in]	int iValue			Value�l
 * @retval	��������
 *
 * ### �������e
 *
 ******************************************************************************/
nodeptr * hash_malloc_impl(nodeptr *currnode, nodeptr *initroot, char *pkey, long lvalue){

	nodeptr newnode;

	/**
	 * �m�[�h�̃��������m�ۂ���
	 */
	if ((newnode = fp_malloc(sizeof *newnode)) == NULL
	  || (newnode->key = fp_malloc(strlen(pkey) + 1)) == NULL) {
		fprintf(stderr, "malloc error. errno=%d: errmsg=%s\n",errno,strerror(errno));
		return (nodeptr *)-1;
	}

	strcpy(newnode->key, pkey);
	newnode->val = lvalue;
	newnode->left = *initroot;
	newnode->right = *currnode;
	*currnode = newnode;

	return currnode;

}
nodeptr *(*hash_malloc)(nodeptr *, nodeptr *, char *, long ) = hash_malloc_impl;
void *(*fp_malloc)(size_t) = malloc;

/***************************************************************************//**
 * @fn		int hash_get_impl(nodeptr *hashtable, char *pKey, nodeptr *out)
 * �n�b�V���e�[�u�����擾����
 *
 * @param[in]	nodeptr *hashtable	�n�b�V���e�[�u���̃A�h���X
 * @param[in]	char *pKey			Key�l�̕�����ւ̃|�C���^
 * @param[out]	nodeptr *out		�ǉ��Ώۃm�[�h�ւ̃|�C���^
 * @retval	1	����	Key�l�����݂���
 * @retval	0	����	Key�l�����݂��Ȃ�
 * @retval	-1	�ُ�	�������ɃG���[����������
 *
 * ### �������e
 *
 ******************************************************************************/
int hash_get_impl(nodeptr *hashtable, char *pkey, nodeptr *out){

	nodeptr *p;
	nodeptr *root;

	/**
	 * ���� *pKey�����hash(char *)�֐��ɂ��n�b�V���l�����߁A���̃n�b�V���l��
	 * �z��̃C���f�b�N�X�Ƃ���n�b�V���e�[�u�����擾����
	 * �܂��A�f�[�^���[(����і؍\���̐�[)�𔻒f���邽�߂ɁAinitrootnode�̃A�h���X
	 * ���擾���Ă����B
	 */

	p = &hashtable[hash(pkey)];
	root = (nodeptr *)&initrootnode[hash(pkey)];

#ifdef DEBUG
	fprintf(stderr, "**************************************\n");
	fprintf(stderr, "hash get start\n");
	fprintf(stderr, "  insert Key             =%s\n", pkey);
	fprintf(stderr, "  hash                   =%d\n", hash(pkey));
	fprintf(stderr, "  rootnode addr          =%x\n", root);
#endif

	/**
	 * hash_bin_search(nodeptr *, nodeptr *, char *)�֐��ɂ��Q���؂���������B
	 */
	p = hash_bin_search(p,root,pkey);

	/**
	 * ���łɓo�^����Ă���ꍇ�͏o�͒lnodeptr *out�Ƀm�[�h�̃A�h���X��
	 * �Z�b�g���ď����𔲂���
	 */
	if (*p != *root || *p != 0x00) {
#ifdef DEBUG
		fprintf(stderr, "already exists. key=%s: val=%d\n", (*p)->key, (*p)->val);
		fprintf(stderr, "**************************************\n");
#endif
		*out = *p;
		return 1;
	}


#ifdef DEBUG
	fprintf(stderr, "not exists\n");
	fprintf(stderr, "unregistered node. key=%s\n", pkey);
	fprintf(stderr, "**************************************\n");
#endif
	*out = 0x00;
	return 0;

}
int (*hash_get)(nodeptr *,char *, nodeptr *) = hash_get_impl;


/***************************************************************************//**
 * @fn		int hash_insert_impl(nodeptr *hashtable, char *pKey, long lVal, nodeptr *out)
 * �n�b�V���e�[�u����}������
 *
 * @param[in]	nodeptr *hashtable	�n�b�V���e�[�u���̃A�h���X
 * @param[in]	char *pKey			Key�l�̕�����ւ̃|�C���^
 * @param[in]	long lVal			Value�l�ɑ}������f�[�^
 * @param[out]	nodeptr *out		�ǉ��Ώۃm�[�h�ւ̃|�C���^
 * @retval	1	����	�}������
 * @retval	0	����	����Key�����݂���
 * @retval	-1	�ُ�	�������ɃG���[����������
 *
 * ### �������e
 *
 ******************************************************************************/
int hash_insert_impl(nodeptr *hashtable, char *pkey, long lval, nodeptr *out){

	nodeptr *p;
	nodeptr *root;

	/**
	 * ���� *pKey�����hash(char *)�֐��ɂ��n�b�V���l�����߁A���̃n�b�V���l��
	 * �z��̃C���f�b�N�X�Ƃ���n�b�V���e�[�u�����擾����
	 * �܂��A�f�[�^���[(����і؍\���̐�[)�𔻒f���邽�߂ɁAinitrootnode�̃A�h���X
	 * ���擾���Ă����B
	 */

	p = &hashtable[hash(pkey)];
	root = (nodeptr *)&initrootnode[hash(pkey)];

#ifdef DEBUG
	fprintf(stderr, "**************************************\n");
	fprintf(stderr, "hash insert start\n");
	fprintf(stderr, "  insert Key             =%s\n", pkey);
	fprintf(stderr, "  hash                   =%d\n", hash(pkey));
	fprintf(stderr, "  rootnode addr          =%x\n", root);
#endif

	/**
	 * hash_bin_search(nodeptr *, nodeptr *, char *)�֐��ɂ��Q���؂���������B
	 */
	p = hash_bin_search(p,root,pkey);

	/**
	 * ���łɓo�^����Ă���ꍇ�͏����𔲂���
	 * value�l�͍X�V���Ȃ�
	 */
	if (*p != *root || *p != 0x00) {
#ifdef DEBUG
		fprintf(stderr, "already exists. key=%s: val=%d\n", (*p)->key, (*p)->val);
		fprintf(stderr, "**************************************\n");
#endif
		*out = *p;
		return 0;
	}


	/**
	 * �܂��o�^����Ă��Ȃ�������̏ꍇ�A�m�[�h�ɒǉ�����
	 */
	p = hash_malloc(p, root, pkey, lval);
	if (p == (nodeptr *)-1) {
		*out = 0x00;
		return -1;
	}

#ifdef DEBUG
	fprintf(stderr, "not exists\n");
	fprintf(stderr, "insert node. key=%s: val=%d\n", (*p)->key, (*p)->val);
	fprintf(stderr, "**************************************\n");
#endif
	*out = *p;
	return 1;

}
int (*hash_insert)(nodeptr *,char *,long, nodeptr *) = hash_insert_impl;

/***************************************************************************//**
 * @fn		int hash_update_impl(nodeptr *hashtable, char *pKey, long lVal, nodeptr *out)
 * �n�b�V���e�[�u�����X�V����
 *
 * @param[in]	nodeptr *hashtable	�n�b�V���e�[�u���̃A�h���X
 * @param[in]	char *pKey			Key�l�̕�����ւ̃|�C���^
 * @param[in]	long lVal			Value�l�ɑ}������f�[�^
 * @param[out]	nodeptr *out		�ǉ��Ώۃm�[�h�ւ̃|�C���^
 * @retval	1	����	�X�V����
 * @retval	0	����	Key�l�����݂��Ȃ�
 * @retval	-1	�ُ�	�������ɃG���[����������
 *
 * ### �������e
 *
 ******************************************************************************/
int hash_update_impl(nodeptr *hashtable, char *pkey, long lval, nodeptr *out){

	nodeptr *p;
	nodeptr *root;

	/**
	 * ���� *pKey�����hash(char *)�֐��ɂ��n�b�V���l�����߁A���̃n�b�V���l��
	 * �z��̃C���f�b�N�X�Ƃ���n�b�V���e�[�u�����擾����
	 * �܂��A�f�[�^���[(����і؍\���̐�[)�𔻒f���邽�߂ɁAinitrootnode�̃A�h���X
	 * ���擾���Ă����B
	 */

	p = &hashtable[hash(pkey)];
	root = (nodeptr *)&initrootnode[hash(pkey)];

#ifdef DEBUG
	fprintf(stderr, "**************************************\n");
	fprintf(stderr, "hash update start\n");
	fprintf(stderr, "  insert Key             =%s\n", pkey);
	fprintf(stderr, "  hash                   =%d\n", hash(pkey));
	fprintf(stderr, "  rootnode addr          =%x\n", root);
#endif

	/**
	 * hash_bin_search(nodeptr *, nodeptr *, char *)�֐��ɂ��Q���؂���������B
	 */
	p = hash_bin_search(p,root,pkey);

	/**
	 * ���łɓo�^����Ă���ꍇ�͏����𔲂���
	 */
	if (*p != *root || *p != 0x00) {
#ifdef DEBUG
		fprintf(stderr, "already exists. key=%s: val=%d\n", (*p)->key, (*p)->val);
		fprintf(stderr, "**************************************\n");
#endif
		(*p)->val = lval;
		*out = *p;
		return 1;
	}


#ifdef DEBUG
	fprintf(stderr, "not exists\n");
	fprintf(stderr, "unregistered node. key=%s\n", pkey);
	fprintf(stderr, "**************************************\n");
#endif
	*out = 0x00;
	return 0;

}
int (*hash_update)(nodeptr *,char *,long, nodeptr *) = hash_update_impl;


/***************************************************************************//**
 * @fn		int hash_upsert_impl(nodeptr *hashtable, char *pKey, long lVal, nodeptr *out)
 * �n�b�V���e�[�u���ɁA�L�[�����݂��Ȃ����insert���A�L�[�����݂����update����
 *
 * @param[in]	nodeptr *hashtable	�n�b�V���e�[�u���̃A�h���X
 * @param[in]	char *pKey			Key�l�̕�����ւ̃|�C���^
 * @param[in]	long lVal			Value�l�ɑ}������f�[�^
 * @param[out]	nodeptr *out		�ǉ��Ώۃm�[�h�ւ̃|�C���^
 * @retval	1	����	�}������
 * @retval	0	����	����Key�����݂���
 * @retval	-1	�ُ�	�������ɃG���[����������
 *
 * ### �������e
 *
 ******************************************************************************/
int hash_upsert_impl(nodeptr *hashtable, char *pkey, long lval, nodeptr *out){

	nodeptr *p;
	nodeptr *root;

	/**
	 * ���� *pKey�����hash(char *)�֐��ɂ��n�b�V���l�����߁A���̃n�b�V���l��
	 * �z��̃C���f�b�N�X�Ƃ���n�b�V���e�[�u�����擾����
	 * �܂��A�f�[�^���[(����і؍\���̐�[)�𔻒f���邽�߂ɁAinitrootnode�̃A�h���X
	 * ���擾���Ă����B
	 */

	p = &hashtable[hash(pkey)];
	root = (nodeptr *)&initrootnode[hash(pkey)];

#ifdef DEBUG
	fprintf(stderr, "**************************************\n");
	fprintf(stderr, "hash upsert start\n");
	fprintf(stderr, "  insert Key             =%s\n", pkey);
	fprintf(stderr, "  hash                   =%d\n", hash(pkey));
	fprintf(stderr, "  rootnode addr          =%x\n", root);
#endif

	/**
	 * hash_bin_search(nodeptr *, nodeptr *, char *)�֐��ɂ��Q���؂���������B
	 */
	p = hash_bin_search(p,root,pkey);

	/**
	 * ���łɓo�^����Ă���ꍇ�͏����𔲂���
	 */
	if (*p != *root || *p != 0x00) {
		(*p)->val = lval;
#ifdef DEBUG
		fprintf(stderr, "already exists. key=%s: val=%d\n", (*p)->key, (*p)->val);
		fprintf(stderr, "**************************************\n");
#endif
		*out = *p;
		return 0;
	}


	/**
	 * �܂��o�^����Ă��Ȃ�������̏ꍇ�A�m�[�h�ɒǉ�����
	 */
	p = hash_malloc(p, root, pkey, lval);
	if (p == (nodeptr *)-1) {
		*out = 0x00;
		return -1;
	}

#ifdef DEBUG
	fprintf(stderr, "not exists\n");
	fprintf(stderr, "insert node. key=%s: val=%d\n", (*p)->key, (*p)->val);
	fprintf(stderr, "**************************************\n");
#endif
	*out = *p;
	return 1;

}
int (*hash_upsert)(nodeptr *,char *,long, nodeptr *) = hash_upsert_impl;


/***************************************************************************//**
 * @fn		int hash_update_plusone_impl(nodeptr *hashtable, char *pKey, nodeptr *out)
 * �n�b�V���e�[�u���̃o�����[�l���{�P����
 *
 * @param[in]	nodeptr *hashtable	�n�b�V���e�[�u���̃A�h���X
 * @param[in]	char *pKey			Key�l�̕�����ւ̃|�C���^
 * @param[out]	nodeptr *out		�ǉ��Ώۃm�[�h�ւ̃|�C���^
 * @retval	1	����	�X�V����
 * @retval	0	����	Key�l�����݂��Ȃ�
 *
 * ### �������e
 *
 ******************************************************************************/
int hash_update_plusone_impl(nodeptr *hashtable, char *pkey, nodeptr *out){

	nodeptr *p;
	nodeptr *root;

	/**
	 * ���� *pKey�����hash(char *)�֐��ɂ��n�b�V���l�����߁A���̃n�b�V���l��
	 * �z��̃C���f�b�N�X�Ƃ���n�b�V���e�[�u�����擾����
	 * �܂��A�f�[�^���[(����і؍\���̐�[)�𔻒f���邽�߂ɁAinitrootnode�̃A�h���X
	 * ���擾���Ă����B
	 */

	p = &hashtable[hash(pkey)];
	root = (nodeptr *)&initrootnode[hash(pkey)];


#ifdef DEBUG
	fprintf(stderr, "**************************************\n");
	fprintf(stderr, "hash update_plusone start\n");
	fprintf(stderr, "  insert Key             =%s\n", pkey);
	fprintf(stderr, "  hash                   =%d\n", hash(pkey));
	fprintf(stderr, "  rootnode addr          =%x\n", root);
#endif

	/**
	 * hash_bin_search(nodeptr *, nodeptr *, char *)�֐��ɂ��Q���؂���������B
	 */
	p = hash_bin_search(p,root,pkey);

	/**
	 * ���łɓo�^����Ă���ꍇ�͏����𔲂���
	 */
	if (*p != *root || *p != 0x00) {
		(*p)->val++;
		if((*p)->val > g_maxvalue){
			(*p)->val = 1;
		}
#ifdef DEBUG
		fprintf(stderr, "already exists. key=%s: val=%d\n", (*p)->key, (*p)->val);
		fprintf(stderr, "**************************************\n");
#endif
		*out = *p;
		return 1;
	}


#ifdef DEBUG
	fprintf(stderr, "not exists\n");
	fprintf(stderr, "unregistered node. key=%s\n", pkey);
	fprintf(stderr, "**************************************\n");
#endif
	*out = 0x00;
	return 0;

}
int (*hash_update_plusone)(nodeptr *,char *, nodeptr *) = hash_update_plusone_impl;


/***************************************************************************//**
 * @fn		int hash_upsert_plusone_impl(nodeptr *hashtable, char *pKey, nodeptr *out)
 * �n�b�V���e�[�u���ɁA�L�[�����݂��Ȃ���΃o�����[�l���P��insert���A
 * �L�[�����݂���΃o�����[�l���{�P����
 *
 * @param[in]	nodeptr *hashtable	�n�b�V���e�[�u���̃A�h���X
 * @param[in]	char *pKey			Key�l�̕�����ւ̃|�C���^
 * @param[out]	nodeptr *out		�ǉ��Ώۃm�[�h�ւ̃|�C���^
 * @retval	1	����	�}������
 * @retval	0	����	����Key�����݂���
 * @retval	-1	�ُ�	�������ɃG���[����������
 *
 * ### �������e
 *
 ******************************************************************************/
int hash_upsert_plusone_impl(nodeptr *hashtable, char *pkey, nodeptr *out){

	nodeptr *p;
	nodeptr *root;

	/**
	 * ���� *pKey�����hash(char *)�֐��ɂ��n�b�V���l�����߁A���̃n�b�V���l��
	 * �z��̃C���f�b�N�X�Ƃ���n�b�V���e�[�u�����擾����
	 * �܂��A�f�[�^���[(����і؍\���̐�[)�𔻒f���邽�߂ɁAinitrootnode�̃A�h���X
	 * ���擾���Ă����B
	 */

	p = &hashtable[hash(pkey)];
	root = (nodeptr *)&initrootnode[hash(pkey)];


#ifdef DEBUG
	fprintf(stderr, "**************************************\n");
	fprintf(stderr, "hash upsert_plusone start\n");
	fprintf(stderr, "  insert Key             =%s\n", pkey);
	fprintf(stderr, "  hash                   =%d\n", hash(pkey));
	fprintf(stderr, "  rootnode addr          =%x\n", root);
#endif

	/**
	 * hash_bin_search(nodeptr *, nodeptr *, char *)�֐��ɂ��Q���؂���������B
	 */
	p = hash_bin_search(p,root,pkey);

	/**
	 * ���łɓo�^����Ă���ꍇ�͏����𔲂���
	 */
	if (*p != *root || *p != 0x00) {
		(*p)->val++;
		if((*p)->val > g_maxvalue){
			(*p)->val = 1;
		}
#ifdef DEBUG
		fprintf(stderr, "already exists. key=%s: val=%d\n", (*p)->key, (*p)->val);
		fprintf(stderr, "**************************************\n");
#endif
		*out = *p;
		return 0;
	}


	/**
	 * �܂��o�^����Ă��Ȃ�������̏ꍇ�A�m�[�h�ɒǉ�����
	 */
	p = hash_malloc(p, root, pkey, 1);
	if (p == (nodeptr *)-1) {
		*out = 0x00;
		return -1;
	}

#ifdef DEBUG
	fprintf(stderr, "not exists\n");
	fprintf(stderr, "insert node. key=%s: val=%d\n", (*p)->key, (*p)->val);
	fprintf(stderr, "**************************************\n");
#endif
	*out = *p;
	return 1;

}
int (*hash_upsert_plusone)(nodeptr *,char *, nodeptr *) = hash_upsert_plusone_impl;

/***************************************************************************//**
 * @fn		int hash_terminate_impl(nodeptr *hashtable)
 * �n�b�V���e�[�u���S���������
 *
 * @param[in]	nodeptr *hashtable	�n�b�V���e�[�u���̃A�h���X
 * @retval	0	����	�}������
 *
 * ### �������e
 *
 ******************************************************************************/
int hash_terminate_impl(nodeptr *hashtable){

	int i;

	nodeptr p;
/**
 * �n�b�V���e�[�u�����T�C�Y������������������s��
 */
	for(i = 0; i < HASHSIZE; i++){
#ifdef DEBUG2
	fprintf(stderr, "**************************************\n");
	fprintf(stderr, "hash terminate start\n");
	fprintf(stderr, "  table index            =%d\n", i);
	fprintf(stderr, "  current hashtable addr =%x\n", &hashtable[i]);
	hashaddrdump(&hashtable[i]);
#endif
		p = hashtable[i];
		hash_free(&p, (nodeptr *)&initrootnode[i]);
	}

	/*hash_init(hashtable);*/

#ifdef DEBUG2
	fprintf(stderr, "hash terminate ended\n");
	fprintf(stderr, "**************************************\n");
#endif
	return 0;

}
int (*hash_terminate)(nodeptr *) = hash_terminate_impl;

/***************************************************************************//**
 * @fn		int hash_free_impl(nodeptr *hashtable, nodeptr *initroot)
 * �n�b�V���e�[�u���̃m�[�h�̃����������
 *
 * @param[in]	nodeptr *hashtable	�n�b�V���e�[�u���̃A�h���X
 * @param[in]	nodeptr *initroot	�擪�m�[�h(initrootnode)�̃A�h���X
 * @retval	0	����	�}������
 *
 * ### �������e
 *
 ******************************************************************************/
int hash_free_impl(nodeptr *hashtable, nodeptr *initroot){

	nodeptr *p;

	/**
	 * left�Ƀm�[�h�����邩�ǂ������ׂ�
	 * �m�[�h����������ċA�I��hash_free���R�[�����A�m�[�h���[�܂ł����̂ڂ�
	 * �m�[�h��������ꂽ��left��null���Z�b�g����
	 */
	p = hashtable;
	p = &((*p)->left );

#ifdef DEBUG2
	fprintf(stderr, "  --------------------------------\n");
	fprintf(stderr, "  hash free start\n");
	fprintf(stderr, "  left \n");
	hashaddrdump(p);
#endif

	if ((char *)*p != (char *)initroot && *p != 0x00){
		hash_free(p, initroot);
	}

	p = hashtable;
	(*p)->left = 0x00;


	/**
	 * right�Ƀm�[�h�����邩�ǂ������ׂ�
	 * �m�[�h����������ċA�I��hash_free���R�[�����A�m�[�h���[�܂ł����̂ڂ�
	 * �m�[�h��������ꂽ��right��null���Z�b�g����
	 */
	p = &((*p)->right );

#ifdef DEBUG2
	fprintf(stderr, "  right \n");
	hashaddrdump(p);
#endif

	if ((char *)*p != (char *)initroot && *p != 0x00){
		hash_free(p, initroot);
	}

	p = hashtable;
	(*p)->right = 0x00;


	/**
	 * left�Aright�Ƃ��Ƀ�������������ꂽ��A���m�[�h���������
	 * ���������[�g�m�[�h�̏ꍇ�͉�����Ȃ�
	 */
#ifdef DEBUG2
	fprintf(stderr, "  center \n");
	hashaddrdump(p);
#endif

	if ((char *)*p != (char *)initroot && *p != 0x00){
		free((*p)->key);
		free(*p);
		p = initroot;
	}

#ifdef DEBUG2
	fprintf(stderr, "  hash free ended\n");
	fprintf(stderr, "  --------------------------------\n");
#endif
	return 0;

}
int (*hash_free)(nodeptr *, nodeptr *) = hash_free_impl;

