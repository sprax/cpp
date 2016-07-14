/*

Copyright (c) 2005, Simon Howard
All rights reserved.

Redistribution and use in source and binary forms, with or without 
modification, are permitted provided that the following conditions 
are met:

 * Redistributions of source code must retain the above copyright 
   notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright 
   notice, this list of conditions and the following disclaimer in 
   the documentation and/or other materials provided with the 
   distribution.
 * Neither the name of the C Algorithms project nor the names of its 
   contributors may be used to endorse or promote products derived 
   from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; 
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER 
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN 
ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
POSSIBILITY OF SUCH DAMAGE.

*/

/**
 * @file trie.h
 *
 * @brief Fast string lookups
 *
 * A trie is a data structure which provides fast mappings from strings
 * to values.
 *
 * To create a new trie, use @ref trie_new.  To destroy a trie,
 * use @ref trie_free.
 *
 * To insert a value into a trie, use @ref trie_insert. To remove a value
 * from a trie, use @ref trie_remove.  
 *
 * To look up a value from its key, use @ref trie_lookup.
 *
 * To find the number of entries in a trie, use @ref trie_num_entries.
 */

#ifndef TRIE_HPP
#define TRIE_HPP

typedef struct _Trie Trie;

/**
 * Create a new trie.
 *
 * @return                   Pointer to a new trie structure.
 */

Trie *trie_new(void);

/** 
 * Destroy a trie.
 *
 * @param trie               The trie to destroy.
 */

void trie_free(Trie *trie);

/**
 * Insert a new key-value pair into a trie.
 *
 * @param trie               The trie.
 * @param key                The key to access the new value.
 * @param value              The value.
 */

void trie_insert(Trie *trie, char *key, void *value);

/**
 * Look up a value from its key in a trie.
 *
 * @param trie               The trie.
 * @param key                The key.
 * @return                   The value associated with the key, or NULL if
 *                           not found in the trie.
 */

void *trie_lookup(Trie *trie, char *key);

/**
 * Remove an entry from a trie.
 *
 * @param trie               The trie.
 * @param key                The key of the entry to remove.
 */

void trie_remove(Trie *trie, char *key);

/** 
 * Find the number of entries in a trie.
 *
 * @param trie               The trie.
 * @return                   Count of the number of entries in the trie.
 */

int trie_num_entries(Trie *trie);

#endif // TRIE_HPP

