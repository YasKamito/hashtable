/***************************************************************************//**
 *
 * @file 		hash.h
 *
 * @brief		HashTable
 *
 * @date 		Created on: 2015/06/20
 *
 * @version		v0.0.1
 *
 * @par 機能追加:
 * | ModID  | Date       | Name           | Description           | TicketID  |
 * | :----: | :--------: | :--------------| :-------------------- | :-------: |
 * | \@f000 | 2015-06-20 | Y.Kamitokusari | Initial release       |           |
 *
 * @par 不具合修正:
 * | ModID  | Date       | Name           | Description           | TicketID  |
 * | :----: | :--------: | :--------------| :-------------------- | :-------: |
 * |        |            |                |                       |           |
 *
 * @par 例外修正:
 * | ModID  | Date       | Name           | Description           | TicketID  |
 * | :----: | :--------: | :--------------| :-------------------- | :-------: |
 * |        |            |                |                       |           |
 *
 ******************************************************************************/

#ifndef SAMPLE_TEST_H_
#define SAMPLE_TEST_H_

#define HASHSIZE		32768 + 3			/**< ハッシュサイズ(素数) */
#define MAXWORDLEN		128					/**< Key値Maxサイズ */

/**
 * ハッシュノード構造体
 */
typedef struct node {
	struct node 	*left, *right;			/**< ２分探索木ポインタ */
	char			*key;					/**< ハッシュKey */
	long 			val;					/**< ハッシュValue */
} *nodeptr;

extern int (*hash)(char *);
extern int (*hash_init)(nodeptr *, long);
extern nodeptr *(*hash_find)(nodeptr *, nodeptr *, char *);
extern nodeptr *(*hash_malloc)(nodeptr *, nodeptr *, char *, long );
extern int (*hash_get)(nodeptr *,char *, nodeptr *);
extern int (*hash_insert)(nodeptr *,char *, long, nodeptr *);
extern int (*hash_update)(nodeptr *,char *, long, nodeptr *);
extern int (*hash_upsert)(nodeptr *,char *, long, nodeptr *);
extern int (*hash_update_plusone)(nodeptr *,char *, nodeptr *);
extern int (*hash_upsert_plusone)(nodeptr *,char *, nodeptr *);
extern int (*hash_terminate)(nodeptr *);
extern int (*hash_free)(nodeptr *, nodeptr *);
extern void *(*fp_malloc)(size_t);

#endif /* SAMPLE_TEST_H_ */
