/*********************************************************
 * Copyright (C) 1998-2016 VMware, Inc. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation version 2.1 and no later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the Lesser GNU General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA.
 *
 *********************************************************/

/*
 * dynbuf.c --
 *
 *    Dynamic buffers --hpreg
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vmware.h"
#include "dynbuf.h"


/*
 *-----------------------------------------------------------------------------
 *
 * DynBuf_Init --
 *
 *      Dynamic buffer constructor --hpreg
 *
 * Results:
 *      None
 *
 * Side effects:
 *      None
 *
 *-----------------------------------------------------------------------------
 */

void
DynBuf_Init(DynBuf *b)  // OUT:
{
   ASSERT(b);

   b->data = NULL;
   b->size = 0;
   b->allocated = 0;
}


/*
 *-----------------------------------------------------------------------------
 *
 * DynBuf_Destroy --
 *
 *      Dynamic buffer destructor --hpreg
 *
 * Results:
 *      None
 *
 * Side effects:
 *      None
 *
 *-----------------------------------------------------------------------------
 */

void
DynBuf_Destroy(DynBuf *b)  // IN/OUT:
{
   ASSERT(b);

   free(b->data);
   DynBuf_Init(b);
}


/*
 *-----------------------------------------------------------------------------
 *
 * DynBuf_AllocGet --
 *
 *      Retrieve a pointer to the data contained in a dynamic buffer.  Return
 *      a copy of that data.
 *
 * Results:
 *      The pointer to the data.  NULL on out of memory failure.
 *
 * Side effects:
 *      Allocates memory.
 *
 *-----------------------------------------------------------------------------
 */

void *
DynBuf_AllocGet(DynBuf const *b)  // IN:
{
   void *new_data;
   ASSERT(b);

   new_data = malloc(b->size);
   if (new_data) {
      memcpy(new_data, b->data, b->size);
   }

   return new_data;
}


/*
 *-----------------------------------------------------------------------------
 *
 * DynBuf_Attach --
 *
 *      Grants ownership of the specified buffer to the DynBuf
 *      object. If there is an existing buffer, it is freed.
 *
 * Results:
 *      None.
 *
 * Side effects:
 *      None
 *
 *-----------------------------------------------------------------------------
 */

void
DynBuf_Attach(DynBuf *b,    // IN/OUT:
              size_t size,  // IN:
              void *data)   // IN:
{
   ASSERT(b);
   ASSERT((size == 0) == (data == NULL));

   free(b->data);
   b->data = data;
   b->size = b->allocated = size;
}


/*
 *-----------------------------------------------------------------------------
 *
 * DynBuf_Detach --
 *
 *      Releases ownership of the buffer stored in the DynBuf object,
 *      and returns a pointer to it.
 *
 * Results:
 *      The pointer to the data.
 *
 * Side effects:
 *      None
 *
 *-----------------------------------------------------------------------------
 */

void *
DynBuf_Detach(DynBuf *b)  // IN/OUT:
{
   void *data;

   ASSERT(b);

   data = b->data;
   b->data = NULL;
   b->allocated = 0;

   return data;
}


/*
 *-----------------------------------------------------------------------------
 *
 * DynBufRealloc --
 *
 *      Reallocate a dynamic buffer --hpreg
 *
 * Results:
 *      TRUE on success
 *      FALSE on failure (not enough memory)
 *
 * Side effects:
 *      None
 *
 *-----------------------------------------------------------------------------
 */

static Bool
DynBufRealloc(DynBuf *b,            // IN/OUT:
              size_t newAllocated)  // IN:
{
   void *new_data;

   ASSERT(b);

   new_data = realloc(b->data, newAllocated);
   if (new_data == NULL && newAllocated) {
      /* Not enough memory */
      return FALSE;
   }

   b->data = new_data;
   b->allocated = newAllocated;

   return TRUE;
}


/*
 *-----------------------------------------------------------------------------
 *
 * DynBuf_Enlarge --
 *
 *      Enlarge a dynamic buffer. The resulting dynamic buffer is guaranteed to
 *      be larger than the one you passed, and at least 'minSize' bytes
 *      large --hpreg
 *
 * Results:
 *      TRUE on success
 *      FALSE on failure (not enough memory)
 *
 * Side effects:
 *      None
 *
 *-----------------------------------------------------------------------------
 */

Bool
DynBuf_Enlarge(DynBuf *b,       // IN/OUT:
               size_t minSize)  // IN:
{
   size_t newAllocated;

   ASSERT(b);

   newAllocated = b->allocated
                      ?
#if defined(DYNBUF_DEBUG)
                        b->allocated + 1
#else
                        /*
                         * Double the previously allocated size if it is less
                         * than 256KB; otherwise grow it linearly by 256KB
                         */
                        (b->allocated < 256 * 1024 ? b->allocated * 2
                                                   : b->allocated + 256 * 1024)
#endif
                      :
#if defined(DYNBUF_DEBUG)
                        1
#else
                        /*
                         * Initial size: 1 KB. Most buffers are smaller than
                         * that --hpreg
                         */
                        1 << 10
#endif
                      ;

   if (minSize > newAllocated) {
      newAllocated = minSize;
   }

   /*
    * Prevent integer overflow. We can use this form of checking specifically
    * because a multiple by 2 is used (in the worst case). This type of
    * checking does not work in the general case.
    */

   if (newAllocated < b->allocated) {
      return FALSE;
   }

   return DynBufRealloc(b, newAllocated);
}


/*
 *-----------------------------------------------------------------------------
 *
 * DynBuf_Append --
 *
 *      Append data at the end of a dynamic buffer. 'size' is the size of the
 *      data. If it is <= 0, no operation is performed --hpreg
 *
 * Results:
 *      TRUE on success
 *      FALSE on failure (not enough memory)
 *
 * Side effects:
 *      None
 *
 *-----------------------------------------------------------------------------
 */

Bool
DynBuf_Append(DynBuf *b,        // IN/OUT:
              void const *data, // IN:
              size_t size)      // IN:
{
   size_t new_size;

   ASSERT(b);

   if (size <= 0) {
      return TRUE;
   }

   ASSERT(data);

   new_size = b->size + size;

   if (new_size < b->size) {  // Prevent integer overflow
      return FALSE;
   }

   if (new_size > b->allocated) {
      /* Not enough room */
      if (DynBuf_Enlarge(b, new_size) == FALSE) {
         return FALSE;
      }
   }

   memcpy(b->data + b->size, data, size);
   b->size = new_size;

   return TRUE;
}


/*
 *-----------------------------------------------------------------------------
 *
 * DynBuf_SafeInternalAppend --
 *
 *      Append data at the end of a dynamic buffer. Memory allocation failure
 *      are handled the same way as Util_SafeMalloc, that is to say, with a
 *      Panic.
 *
 * Results:
 *      None
 *
 * Side effects:
 *      None
 *
 *-----------------------------------------------------------------------------
 */

void
DynBuf_SafeInternalAppend(DynBuf *b,            // IN/OUT:
                          void const *data,     // IN:
                          size_t size,          // IN:
                          char const *file,     // IN:
                          unsigned int lineno)  // IN:
{
   if (!DynBuf_Append(b, data, size)) {
      Panic("Unrecoverable memory allocation failure at %s:%u\n",
            file, lineno);
   }
}


/*
 *-----------------------------------------------------------------------------
 *
 * DynBuf_Trim --
 *
 *      Reallocate a dynamic buffer to the exact size it occupies --hpreg
 *
 * Results:
 *      TRUE on success
 *      FALSE on failure (not enough memory)
 *
 * Side effects:
 *      None
 *
 *-----------------------------------------------------------------------------
 */

Bool
DynBuf_Trim(DynBuf *b)  // IN/OUT:
{
   ASSERT(b);

   return DynBufRealloc(b, b->size);
}


/*
 *-----------------------------------------------------------------------------
 *
 * DynBuf_Copy --
 *
 *      Copies all data and metadata from src dynbuf to dest dynbuf.
 *
 *      Dest should be an initialized DynBuf of alloced length zero
 *      to prevent memory leaks.
 *
 * Results:
 *      TRUE on success, FALSE on failure.
 *
 * Side effects:
 *      None
 *
 *-----------------------------------------------------------------------------
 */

Bool
DynBuf_Copy(DynBuf *src,   // IN:
            DynBuf *dest)  // OUT:
{
   ASSERT(src);
   ASSERT(dest);
   ASSERT(!dest->data);

   dest->data = malloc(src->allocated);

   if (dest->data == NULL) {
      return FALSE;
   }

   dest->size = src->size;
   dest->allocated = src->allocated;

   memcpy(dest->data, src->data, src->size);

   return TRUE;
}
