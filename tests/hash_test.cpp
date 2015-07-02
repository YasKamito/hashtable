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

#include "gtest/gtest.h" /** GoogleTest �C���N���[�h */


int mock_getRandam(int a, int b){
    return 5;
}
nodeptr hashtable[HASHSIZE];

/*******************************************************************************
* �e�X�g�N���X�̒�`
*******************************************************************************/
class HashTest : public ::testing::Test
{

  virtual void SetUp()
  {


  }

  virtual void TearDown()
  {

  }
};

/*******************************************************************************
* �e�X�g�P�[�X�P
*
* �����F
*
*******************************************************************************/
TEST_F(HashTest, CASE001)
{
    EXPECT_EQ(0, hash_init(hashtable) );
}


/*******************************************************************************
* �e�X�g�P�[�X�P
*
* �����F
*
*******************************************************************************/
TEST_F(HashTest, CASE002)
{
	char key[10], *pkey;
	pkey = key;
	memset(pkey,0x00,sizeof(key));
	strcpy(key, "aaaa");
    EXPECT_EQ(1, hash_insert(hashtable,pkey,1) );

	memset(pkey,0x00,sizeof(key));
	strcpy(key, "aaaa");
    EXPECT_EQ(0, hash_insert(hashtable,pkey,1) );
}


/*******************************************************************************
* �e�X�g�P�[�X�P
*
* �����F
*
*******************************************************************************/
TEST_F(HashTest, CASE003)
{
	char key[10], *pkey;
	pkey = key;
	memset(pkey,0x00,sizeof(key));
	strcpy(key, "bbbb");
    EXPECT_EQ(1, hash_insert(hashtable,pkey,2) );

	memset(pkey,0x00,sizeof(key));
	strcpy(key, "cccc");
    EXPECT_EQ(1, hash_insert(hashtable,pkey,3) );

	memset(pkey,0x00,sizeof(key));
	strcpy(key, "dddd");
    EXPECT_EQ(1, hash_insert(hashtable,pkey,4) );

	memset(pkey,0x00,sizeof(key));
	strcpy(key, "dddd");
    EXPECT_EQ(0, hash_insert(hashtable,pkey,4) );
}

/*******************************************************************************
* �e�X�g�P�[�X�P
*
* �����F
*
*******************************************************************************/
TEST_F(HashTest, CASE004)
{
    EXPECT_EQ(0, hash_terminate(hashtable) );

	char key[10], *pkey;
	pkey = key;
	memset(pkey,0x00,sizeof(key));
	strcpy(key, "dddd");
    EXPECT_EQ(1, hash_insert(hashtable,pkey,2) );

	memset(pkey,0x00,sizeof(key));
	strcpy(key, "cccc");
    EXPECT_EQ(1, hash_insert(hashtable,pkey,2) );

    EXPECT_EQ(0, hash_terminate(hashtable) );
}


/*******************************************************************************
* �e�X�g�P�[�X�P
*
* �����F
*
*******************************************************************************/
TEST_F(HashTest, CASE005)
{
	char key[10], *pkey;
	pkey = key;

	memset(pkey,0x00,sizeof(key));
	strcpy(key, "dddd");
    EXPECT_EQ(1, hash_insert(hashtable,pkey,4) );

	memset(pkey,0x00,sizeof(key));
	strcpy(key, "cccc");
    EXPECT_EQ(1, hash_insert(hashtable,pkey,3) );

	memset(pkey,0x00,sizeof(key));
	strcpy(key, "bbbb");
    EXPECT_EQ(1, hash_insert(hashtable,pkey,2) );

	memset(pkey,0x00,sizeof(key));
	strcpy(key, "dddd");
    EXPECT_EQ(0, hash_insert(hashtable,pkey,4) );

	memset(pkey,0x00,sizeof(key));
	strcpy(key, "cccc");
    EXPECT_EQ(0, hash_insert(hashtable,pkey,4) );

	memset(pkey,0x00,sizeof(key));
	strcpy(key, "aaaa");
    EXPECT_EQ(1, hash_insert(hashtable,pkey,1) );

	memset(pkey,0x00,sizeof(key));
	strcpy(key, "eeee");
    EXPECT_EQ(1, hash_insert(hashtable,pkey,2) );



}
