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

char initword[HASHSIZE][MAXWORDLEN + 1];		/**< ハッシュノード番人用Key(配列) */
struct node initrootnode[HASHSIZE] = {NULL, NULL, NULL, 0};	/**< ハッシュノード番人(配列) */

long g_maxvalue;

/***************************************************************************//**
 * @fn		void hashaddrdump(nodeptr *p)
 * デバッグ用メモリダンプ
 * 関数標準エラー出力にハッシュテーブルの内容を出力する
 *
 * @param[in]	nodeptr *p		出力するハッシュテーブルのアドレス
 * @retval	なし
 *
 * ### 処理内容
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
 * 引数の文字列のハッシュ値を返す
 *
 * @param[in]	char *pchStr	文字列へのポインタ
 * @retval	int	ハッシュ値
 *
 * ### 処理内容
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
 * ハッシュテーブルを初期化する
 *
 * @param[in]	nodeptr *hashtable	ハッシュテーブルのアドレス
 * @param[in]	long	maxvalue	ハッシュテーブル内value値の最大値
 * @retval	0	正常
 *
 * ### 処理内容
 *
 ******************************************************************************/
int hash_init_impl(nodeptr *hashtable, long maxvalue){

	int i;

	/**
	 * 引数よりバリュー最大値を取得
	 * 指定がなければデフォルトで999999をセット
	 */
	g_maxvalue = maxvalue;
	if(g_maxvalue == 0){
		g_maxvalue = 999999;
	}

	/**
	 * HASHSIZEの分だけnodeptr *hashtableを初期化する
	 * 初期化には、initrootnodeを使用する
	 * また、hashtable->keyにはinitwordのアドレスをセットする
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
 * ２分探索木関数
 *
 * @param[in]	nodeptr *p			ハッシュテーブルのアドレス
 * @param[in]	nodeptr *root		ルートノードのアドレス
 * @param[in]	char *pKey			Key値の文字列へのポインタ
 * @retval	検索結果
 *
 * ### 処理内容
 *
 ******************************************************************************/
nodeptr * hash_bin_search_impl(nodeptr *currnode, nodeptr *initroot, char *pkey){

	int cmp;

	/**
	 * strcmp関数による文字列比較により、２分探索木を行う。
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
 * ２分探索木malloc関数
 *
 * @param[in]	nodeptr *p			ハッシュテーブルのアドレス
 * @param[in]	nodeptr *root		ルートノードのアドレス
 * @param[in]	char *pKey			Key値の文字列へのポインタ
 * @param[in]	int iValue			Value値
 * @retval	検索結果
 *
 * ### 処理内容
 *
 ******************************************************************************/
nodeptr * hash_malloc_impl(nodeptr *currnode, nodeptr *initroot, char *pkey, long lvalue){

	nodeptr newnode;

	/**
	 * ノードのメモリを確保する
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
 * ハッシュテーブルを取得する
 *
 * @param[in]	nodeptr *hashtable	ハッシュテーブルのアドレス
 * @param[in]	char *pKey			Key値の文字列へのポインタ
 * @param[out]	nodeptr *out		追加対象ノードへのポインタ
 * @retval	1	正常	Key値が存在する
 * @retval	0	正常	Key値が存在しない
 * @retval	-1	異常	処理中にエラーが発生した
 *
 * ### 処理内容
 *
 ******************************************************************************/
int hash_get_impl(nodeptr *hashtable, char *pkey, nodeptr *out){

	nodeptr *p;
	nodeptr *root;

	/**
	 * 引数 *pKeyを基にhash(char *)関数によりハッシュ値を求め、そのハッシュ値を
	 * 配列のインデックスとするハッシュテーブルを取得する
	 * また、データ末端(および木構造の先端)を判断するために、initrootnodeのアドレス
	 * も取得しておく。
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
	 * hash_bin_search(nodeptr *, nodeptr *, char *)関数により２分木を検索する。
	 */
	p = hash_bin_search(p,root,pkey);

	/**
	 * すでに登録されている場合は出力値nodeptr *outにノードのアドレスを
	 * セットして処理を抜ける
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
 * ハッシュテーブルを挿入する
 *
 * @param[in]	nodeptr *hashtable	ハッシュテーブルのアドレス
 * @param[in]	char *pKey			Key値の文字列へのポインタ
 * @param[in]	long lVal			Value値に挿入するデータ
 * @param[out]	nodeptr *out		追加対象ノードへのポインタ
 * @retval	1	正常	挿入成功
 * @retval	0	正常	既にKeyが存在する
 * @retval	-1	異常	処理中にエラーが発生した
 *
 * ### 処理内容
 *
 ******************************************************************************/
int hash_insert_impl(nodeptr *hashtable, char *pkey, long lval, nodeptr *out){

	nodeptr *p;
	nodeptr *root;

	/**
	 * 引数 *pKeyを基にhash(char *)関数によりハッシュ値を求め、そのハッシュ値を
	 * 配列のインデックスとするハッシュテーブルを取得する
	 * また、データ末端(および木構造の先端)を判断するために、initrootnodeのアドレス
	 * も取得しておく。
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
	 * hash_bin_search(nodeptr *, nodeptr *, char *)関数により２分木を検索する。
	 */
	p = hash_bin_search(p,root,pkey);

	/**
	 * すでに登録されている場合は処理を抜ける
	 * value値は更新しない
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
	 * まだ登録されていない文字列の場合、ノードに追加する
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
 * ハッシュテーブルを更新する
 *
 * @param[in]	nodeptr *hashtable	ハッシュテーブルのアドレス
 * @param[in]	char *pKey			Key値の文字列へのポインタ
 * @param[in]	long lVal			Value値に挿入するデータ
 * @param[out]	nodeptr *out		追加対象ノードへのポインタ
 * @retval	1	正常	更新成功
 * @retval	0	正常	Key値が存在しない
 * @retval	-1	異常	処理中にエラーが発生した
 *
 * ### 処理内容
 *
 ******************************************************************************/
int hash_update_impl(nodeptr *hashtable, char *pkey, long lval, nodeptr *out){

	nodeptr *p;
	nodeptr *root;

	/**
	 * 引数 *pKeyを基にhash(char *)関数によりハッシュ値を求め、そのハッシュ値を
	 * 配列のインデックスとするハッシュテーブルを取得する
	 * また、データ末端(および木構造の先端)を判断するために、initrootnodeのアドレス
	 * も取得しておく。
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
	 * hash_bin_search(nodeptr *, nodeptr *, char *)関数により２分木を検索する。
	 */
	p = hash_bin_search(p,root,pkey);

	/**
	 * すでに登録されている場合は処理を抜ける
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
 * ハッシュテーブルに、キーが存在しなければinsertし、キーが存在すればupdateする
 *
 * @param[in]	nodeptr *hashtable	ハッシュテーブルのアドレス
 * @param[in]	char *pKey			Key値の文字列へのポインタ
 * @param[in]	long lVal			Value値に挿入するデータ
 * @param[out]	nodeptr *out		追加対象ノードへのポインタ
 * @retval	1	正常	挿入成功
 * @retval	0	正常	既にKeyが存在する
 * @retval	-1	異常	処理中にエラーが発生した
 *
 * ### 処理内容
 *
 ******************************************************************************/
int hash_upsert_impl(nodeptr *hashtable, char *pkey, long lval, nodeptr *out){

	nodeptr *p;
	nodeptr *root;

	/**
	 * 引数 *pKeyを基にhash(char *)関数によりハッシュ値を求め、そのハッシュ値を
	 * 配列のインデックスとするハッシュテーブルを取得する
	 * また、データ末端(および木構造の先端)を判断するために、initrootnodeのアドレス
	 * も取得しておく。
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
	 * hash_bin_search(nodeptr *, nodeptr *, char *)関数により２分木を検索する。
	 */
	p = hash_bin_search(p,root,pkey);

	/**
	 * すでに登録されている場合は処理を抜ける
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
	 * まだ登録されていない文字列の場合、ノードに追加する
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
 * ハッシュテーブルのバリュー値を＋１する
 *
 * @param[in]	nodeptr *hashtable	ハッシュテーブルのアドレス
 * @param[in]	char *pKey			Key値の文字列へのポインタ
 * @param[out]	nodeptr *out		追加対象ノードへのポインタ
 * @retval	1	正常	更新成功
 * @retval	0	正常	Key値が存在しない
 *
 * ### 処理内容
 *
 ******************************************************************************/
int hash_update_plusone_impl(nodeptr *hashtable, char *pkey, nodeptr *out){

	nodeptr *p;
	nodeptr *root;

	/**
	 * 引数 *pKeyを基にhash(char *)関数によりハッシュ値を求め、そのハッシュ値を
	 * 配列のインデックスとするハッシュテーブルを取得する
	 * また、データ末端(および木構造の先端)を判断するために、initrootnodeのアドレス
	 * も取得しておく。
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
	 * hash_bin_search(nodeptr *, nodeptr *, char *)関数により２分木を検索する。
	 */
	p = hash_bin_search(p,root,pkey);

	/**
	 * すでに登録されている場合は処理を抜ける
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
 * ハッシュテーブルに、キーが存在しなければバリュー値を１でinsertし、
 * キーが存在すればバリュー値を＋１する
 *
 * @param[in]	nodeptr *hashtable	ハッシュテーブルのアドレス
 * @param[in]	char *pKey			Key値の文字列へのポインタ
 * @param[out]	nodeptr *out		追加対象ノードへのポインタ
 * @retval	1	正常	挿入成功
 * @retval	0	正常	既にKeyが存在する
 * @retval	-1	異常	処理中にエラーが発生した
 *
 * ### 処理内容
 *
 ******************************************************************************/
int hash_upsert_plusone_impl(nodeptr *hashtable, char *pkey, nodeptr *out){

	nodeptr *p;
	nodeptr *root;

	/**
	 * 引数 *pKeyを基にhash(char *)関数によりハッシュ値を求め、そのハッシュ値を
	 * 配列のインデックスとするハッシュテーブルを取得する
	 * また、データ末端(および木構造の先端)を判断するために、initrootnodeのアドレス
	 * も取得しておく。
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
	 * hash_bin_search(nodeptr *, nodeptr *, char *)関数により２分木を検索する。
	 */
	p = hash_bin_search(p,root,pkey);

	/**
	 * すでに登録されている場合は処理を抜ける
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
	 * まだ登録されていない文字列の場合、ノードに追加する
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
 * ハッシュテーブル全メモリ解放
 *
 * @param[in]	nodeptr *hashtable	ハッシュテーブルのアドレス
 * @retval	0	正常	挿入完了
 *
 * ### 処理内容
 *
 ******************************************************************************/
int hash_terminate_impl(nodeptr *hashtable){

	int i;

	nodeptr p;
/**
 * ハッシュテーブルをサイズ分メモリ解放処理を行う
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
 * ハッシュテーブルのノードのメモリを解放
 *
 * @param[in]	nodeptr *hashtable	ハッシュテーブルのアドレス
 * @param[in]	nodeptr *initroot	先頭ノード(initrootnode)のアドレス
 * @retval	0	正常	挿入完了
 *
 * ### 処理内容
 *
 ******************************************************************************/
int hash_free_impl(nodeptr *hashtable, nodeptr *initroot){

	nodeptr *p;

	/**
	 * leftにノードがあるかどうか調べる
	 * ノードがあったら再帰的にhash_freeをコールし、ノード末端までさかのぼる
	 * ノードが解放されたらleftにnullをセットする
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
	 * rightにノードがあるかどうか調べる
	 * ノードがあったら再帰的にhash_freeをコールし、ノード末端までさかのぼる
	 * ノードが解放されたらrightにnullをセットする
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
	 * left、rightともにメモリが解放されたら、自ノードを解放する
	 * ただしルートノードの場合は解放しない
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

