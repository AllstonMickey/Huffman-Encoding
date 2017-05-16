/* 
 * bf.h - Bloom Filter
 * Interface Author: Galdrafoor
 */

# ifndef NIL
# define NIL (void *) 0
# endif

# ifndef _BF_H
# define _BF_H

# ifndef SETBIT
# define SETBIT(vec, bit) (vec[bit >> 3] |= (0x1 << (bit % 8)))
# endif // end of SETBIT

# ifndef CLRBIT
# define CLRBIT(vec, bit) (vec[bit >> 3] &= ~(0x1 << (bit % 8)))
# endif // end of CLRBIT

# include <stdint.h>
# include <stdlib.h>
# include <stdio.h>
# include <string.h>
# include "hash.h"

typedef struct bloomF
{
	uint8_t *v; // Vector
	uint32_t l; // Length
	uint32_t s[4]; // Salt
} bloomF;

// Each function has its own has function, determined by the salt.

uint32_t hashBF(bloomF *, char *);

/*
 * Create a new Bloom Filter
 *
 * @param len     Length, number of bits to allocate to the new BF
 * @param hashes  Hash functions
 *
 * @return bloomF The new BF
 */
static inline bloomF *newBF(uint32_t len, uint32_t hashes[])
{
	bloomF *bf = (bloomF *) malloc(sizeof(bloomF));
	if (bf != NIL)
	{
		bf->v = (uint8_t *) calloc((len / 8) + 1, sizeof(uint8_t));
		if (bf->v != NIL)
		{
			bf->l    = len;
			bf->s[0] = hashes[0];
			bf->s[1] = hashes[1];
			bf->s[2] = hashes[2];
			bf->s[3] = hashes[3];
		}
		else
		{
			perror("calloc error [bf.h:XX]: bf->v == NIL\n");
		}
	}
	else
	{
		perror("malloc error [bf.h:XX]: bf == NIL\n");
	}
	return bf;
}

/*
 * Deletes a Bloom Filter.
 * If there is something to memory to free (not NIL), free it and set to NIL.
 * 
 * @param bf Bloom Filter to delete
 * @return void
 */
static inline void delBF(bloomF *bf)
{
	if (bf != NIL)
	{
		if (bf->v != NIL)
		{
			bf->s[0] = 0;
			bf->s[1] = 0;
			bf->s[2] = 0;
			bf->s[3] = 0;
			bf->l    = 0;
			
			free(bf->v);
			if (bf->v == NIL)
			{
				perror("free error [bf.h:XX]: bf->v == NIL\n");
			}
			bf->v = NIL;
		}

		free(bf);
		if (bf == NIL)
		{
			perror("free error [bf.h:XX]: bf == NIL\n");
		}
		bf = NIL;
	}
}

/*
 * Returns the value of the kth bit in the Bloom Filter
 *
 * @param bf Bloom Filter to check
 * @param k  kth bit position, 0 to (bf->l - 1) inclusive.
 *
 * @return 0: bit is turned off
 *         1: bit is turned on
 */
static inline uint32_t valBF(bloomF *bf, uint32_t k)
{
	return ((bf->v)[k >> 3] & (0x1 << (k % 8))) >> (k % 8);
}

/*
 * Returns the length of the Bloom Filter
 *
 * @param bf Bloom Filter to check
 * 
 * @return Length, number of total bits allocated to the BF
 */
static inline uint32_t lenBF(bloomF *bf)
{
	return bf->l;
}

/*
 * Count bits in the Bloom Filter.
 * For each bit, if it's turned on (1), count it.
 *
 * @param bf Bloom Filter to check
 *
 * @return Number of set bits in BF
 */
static inline uint32_t countBF(bloomF *bf)
{
	uint32_t count = 0;
	for (uint32_t i = 0; i < bf->l; i += 1)
	{
		if (valBF(bf, i))
		{
			count += 1;
		}
	}
	return count;
}

/*
 * Hashes a key string to a 2 byte unsigned integer.
 * Sets the BF vector's bit to 1 in the index of the hashed value.
 *
 * @param bf  Bloom Filter to set bit in
 * @param key String to hash into an index for the BF vector
 *
 * @return void
 */
static inline void setBF(bloomF *bf, const char *key)
{
	SETBIT(bf->v, hash(bf->s, key) % 16);
}


/*
 * Hashes a key string to a 2 byte unsigned integer.
 * Clears the BF vector's bit to 0 in the index of the hased value.
 *
 * @param bf  Bloom Filter to clear bit in
 * @param key String to hash into an index for the BF vector
 * 
 * @return void
 */
static inline void clrBF(bloomF *bf, const char *key)
{
	CLRBIT(bf->v, hash(bf->s, key) % 16);
}

/*
// Check membership in the Bloom filter

static inline uint32_t memBF(bloomF *bf, char *key)
{
	// Code
}

*/

static inline void printBF(bloomF *bf)
{
	for (uint32_t i = 0; i < bf->l; i += 1)
	{
		printf("%u", valBF(bf, i));
		if ((i + 1) % 25 == 0)
		{
			printf("\n");
		}
	}
}

# endif // end of _BF_H
