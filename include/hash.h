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
 * @par �@�\�ǉ�:
 * | ModID  | Date       | Name           | Description           | TicketID  |
 * | :----: | :--------: | :--------------| :-------------------- | :-------: |
 * | \@f000 | 2015-06-20 | Y.Kamitokusari | Initial release       |           |
 *
 * @par �s��C��:
 * | ModID  | Date       | Name           | Description           | TicketID  |
 * | :----: | :--------: | :--------------| :-------------------- | :-------: |
 * |        |            |                |                       |           |
 *
 * @par ��O�C��:
 * | ModID  | Date       | Name           | Description           | TicketID  |
 * | :----: | :--------: | :--------------| :-------------------- | :-------: |
 * |        |            |                |                       |           |
 *
 ******************************************************************************/

#ifndef SAMPLE_TEST_H_
#define SAMPLE_TEST_H_

#define HASHSIZE		32768 + 3			/**< �n�b�V���T�C�Y(�f��) */
#define MAXWORDLEN		128					/**< Key�lMax�T�C�Y */

/**
 * �n�b�V���m�[�h�\����
 */
typedef struct node {
	struct node 	*left, *right;			/**< �Q���T���؃|�C���^ */
	char			*key;					/**< �n�b�V��Key */
	long 			val;					/**< �n�b�V��Value */
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
