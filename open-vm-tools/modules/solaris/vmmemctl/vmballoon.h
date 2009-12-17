/*********************************************************
 * Copyright (C) 2000 VMware, Inc. All rights reserved.
 *
 * The contents of this file are subject to the terms of the Common
 * Development and Distribution License (the "License") version 1.0
 * and no later version.  You may not use this file except in
 * compliance with the License.
 *
 * You can obtain a copy of the License at
 *         http://www.opensource.org/licenses/cddl1.php
 *
 * See the License for the specific language governing permissions
 * and limitations under the License.
 *
 *********************************************************/

/* 
 * vmballoon.h: Definitions and macros for vmballoon driver.
 */

#ifndef	VMBALLOON_H
#define	VMBALLOON_H

#include "vm_basic_types.h"

/*
 * Page allocation flags
 */
typedef enum BalloonPageAllocType {
   BALLOON_PAGE_ALLOC_NOSLEEP = 0,
   BALLOON_PAGE_ALLOC_CANSLEEP = 1,
   BALLOON_PAGE_ALLOC_TYPES_NR,	// total number of alloc types
} BalloonPageAllocType;

/*
 * Types
 */

typedef struct {
   /* current status */
   uint32 nPages;
   uint32 nPagesTarget;

   /* adjustment rates */
   uint32 rateAlloc;
   uint32 rateFree;

   /* high-level operations */
   uint32 timer;

   /* primitives */
   uint32 primAlloc[BALLOON_PAGE_ALLOC_TYPES_NR];
   uint32 primAllocFail[BALLOON_PAGE_ALLOC_TYPES_NR];
   uint32 primFree;
   uint32 primErrorPageAlloc;
   uint32 primErrorPageFree;

   /* monitor operations */
   uint32 lock;
   uint32 lockFail;
   uint32 unlock;
   uint32 unlockFail;
   uint32 target;
   uint32 targetFail;
   uint32 start;
   uint32 startFail;
   uint32 guestType;
   uint32 guestTypeFail;
} BalloonStats;

/*
 * Operations
 */

extern void BalloonGetStats(BalloonStats *stats);
extern int  init_module(void);
extern void cleanup_module(void);

#endif	/* VMBALLOON_H */