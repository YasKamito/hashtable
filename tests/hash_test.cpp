/*
 * hash_test.cpp
 *
 *  Created on: 2014/07/24
 *      Author: kamitokusari
 */

extern "C"
{
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include "hash.h"
}

#include "gtest/gtest.h" /** GoogleTest インクルード */


int mock_hash(char *pchStr){
    return 5;
}

nodeptr hashtable[HASHSIZE];
/*******************************************************************************
* テストクラスの定義
*******************************************************************************/
class HashTest : public ::testing::Test
{

	int (*saved_hash)(char *);

  virtual void SetUp()
  {

	  saved_hash = hash;

  }

  virtual void TearDown()
  {

	  hash = saved_hash;

  }
};

/*******************************************************************************
* テストケース１
*
* 条件：
*
*******************************************************************************/
TEST_F(HashTest, CASE001)
{
	int i;

    EXPECT_EQ(0, hash_init(hashtable, 0) );

	for(i = 0; i < HASHSIZE; i++){
		EXPECT_TRUE((hashtable[i])->left == 0x00);
		EXPECT_TRUE((hashtable[i])->right == 0x00);
		EXPECT_TRUE((hashtable[i])->key != 0x00);
		EXPECT_EQ(0, (hashtable[i])->val );
	}
}


/*******************************************************************************
* テストケース１
*
* 条件：
*
*******************************************************************************/
TEST_F(HashTest, CASE002)
{
	char key[10], *pkey;
	pkey = key;

	struct node outnode;
	nodeptr pout;
	pout = &outnode;

    EXPECT_EQ(0, hash_init(hashtable, 0) );

	memset(pkey,0x00,sizeof(key));
	strcpy(key, "aaaa");
    EXPECT_EQ(1, hash_insert(hashtable,pkey,1, &pout) );
    EXPECT_EQ(1, (pout)->val );

	memset(pkey,0x00,sizeof(key));
	strcpy(key, "aaaa");
    EXPECT_EQ(0, hash_insert(hashtable,pkey,1, &pout) );
    EXPECT_EQ(1, (pout)->val );

    EXPECT_EQ(0, hash_terminate(hashtable) );
}


/*******************************************************************************
* テストケース１
*
* 条件：
*
*******************************************************************************/
TEST_F(HashTest, CASE003)
{
	char key[10], *pkey;
	pkey = key;

	struct node outnode;
	nodeptr pout;
	pout = &outnode;

	memset(pkey,0x00,sizeof(key));
	strcpy(key, "bbbb");
    EXPECT_EQ(1, hash_insert(hashtable,pkey,2, &pout) );

	memset(pkey,0x00,sizeof(key));
	strcpy(key, "cccc");
    EXPECT_EQ(1, hash_insert(hashtable,pkey,3, &pout) );

	memset(pkey,0x00,sizeof(key));
	strcpy(key, "dddd");
    EXPECT_EQ(1, hash_insert(hashtable,pkey,4, &pout) );

	memset(pkey,0x00,sizeof(key));
	strcpy(key, "dddd");
    EXPECT_EQ(0, hash_insert(hashtable,pkey,4, &pout) );

    EXPECT_EQ(0, hash_terminate(hashtable) );
}
