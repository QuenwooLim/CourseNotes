--------------------------------------------------------------------------------
--
--  Murphi description of the
--
--            Scalable Coherent Interface (SCI), IEEE Std 1596-1992
--
--------------------------------------------------------------------------------
-- 
--  version:      1.0
--                typical set coherence protocol
--         
--  written by:   Ulrich Stern
--  date:         Dec 1994
--  affiliation:  Stanford University (visiting scholar),
--                Technical University Munich (PhD student)
--
--------------------------------------------------------------------------------



-- =============================================================================
--
--  possible changes:
--  - exec.tStat only used in CodeFault/CodeSetTStat
--  - shrink: MemoryCacheCommands, MemoryCacheStates, TransactStatus
--
--  changed:
--  - sStat stripped (memory behavior 1x, cache behavior 2x)  1-9-95
--
-- =============================================================================



--------------------------------------------------------------------------------
-- constant declarations
--------------------------------------------------------------------------------

const
  NumProcs:      3;         -- number of processors (execution unit and cache)
  CacheSize:     1;         -- number of lines in each cache
  NumMemories:   1;         -- number of memories
  MemorySize:    1;         -- number of lines in each memory
  NumDataValues: 1;         -- number of different data values

  EN_LOAD:   true;          -- enable instructions
  EN_DELETE: false;
  EN_STORE:  true;
  EN_FLUSH:  false;

  FO_FETCH:  false;          -- enable fetch options (for load instruction)
  FO_LOAD:   true;
  FO_STORE:  false;

  -- cache options
  COP_DIRTY:  true;         -- create dirty lists (if true)
  COP_FRESH:  true;         -- maintain fresh lists
  COP_MODS:   true;         -- write to fresh lists

  -- processor options
  POP_DIRTY:  COP_DIRTY;    -- see cache options
  POP_FRESH:  COP_FRESH;
  POP_MODS:   COP_MODS;

  -- memory options
  MOP_FRESH:  true;         -- memory supports MS_HOME, MS_GONE and MS_FRESH
                            -- don't change - only option implemented

  -- don't enable the following options - they are not fully implemented
  COP_CLEAN:    false;
  POP_CLEAN:    false;
  POP_PAIR:     false;
  POP_QOLB:     false;
  POP_WASH:     false;
  POP_CLEANSE:  false;
  POP_ROBUST:   false;

--------------------------------------------------------------------------------
-- type declarations
--------------------------------------------------------------------------------

type
  ProcId:          scalarset (NumProcs);               -- identifiers
  MemoryId:        scalarset (NumMemories);
  NodeId:          union {ProcId, MemoryId};

  MemoryLineId:    scalarset (MemorySize);             -- addresses
  CacheLineId:     scalarset (CacheSize);

  Data:            scalarset (NumDataValues);


--------------------------------------------------------------------------------
-- original SCI definitions

  -- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  -- general data structures

  AccessCommandsReq : enum {               -- AccessCommands (pg.18)
    SC_MREAD,       /* Coherent read00/64 */
    SC_MWRITE64,    /* Coherent write64 */
    SC_CREAD        /* Updating read00/read64 */
  };

  AccessCommandsRes : enum {
    SC_RESP00,      /* Response, status and no data */
    SC_RESP64       /* Response, status and data[64] */
  };
      
  MemoryCacheCommands : enum {
    -- memory commands                     -- MemoryCommands (pg.26)
    MC_PASS_HEAD,             /* Sharing list head transfer */
    MC_CACHE_FRESH,           /* Fetch read-only copy */
    MC_CACHE_CLEAN,           /* Fetch modifiable copy */
    MC_CACHE_DIRTY,           /* Fetch copy for immediate modification */
    MC_LOCK_CLEAR,            /* Clear lock, after fault-recovery */
    MC_LOCK_SET,              /* Set lock, for fault-recovery */
    MC_ATTACH_TO_GONE,        /* Attach to clean&dirty sharing lists */
    MC_ATTACH_TO_LIST,        /* Attach to any sharing list */
    MC_LIST_TO_GONE,          /* Convert from fresh to gone state */
    MC_FRESH_TO_HOME,         /* Change fresh to home, join clean/dirty */
    MC_LIST_TO_HOME,          /* Return list to home (no sharing list) */
    MC_LIST_TO_FRESH,         /* Convert DIRTY->FRESH, if still owned */
    MC_WASH_TO_FRESH,         /* Convert WASH->FRESH, as second rinse */

    -- cache commands                      -- CacheReadCommands (pg.31) and
                                           --   CacheWriteCommands (pg.32)
    CC_LOCK_CLEAR,            /* Clear cache-line lock */
    CC_PEND_VALID,            /* Prepend, data not copied */
    CC_FRESH_INVALID,         /* Fresh purge: new head purges list */
    CC_NEXT_SSAVE,            /* Forw deletion from SAVE_STALE */
    CC_PREV_VMID,             /* Back deletion from MID_VALID */
    CC_PREV_VTAIL,            /* Back deletion from TAIL_VALID */
    CC_VALID_INVALID,         /* Valid purge: HX_INVAL_OX purges */
    CC_TAIL_INVALID,          /* Purging: pairwise TAIL entry */
    CC_NEXT_VMID,             /* Forw deletion from MID_VALID */
    CC_NEXT_FHEAD,            /* Forw deletion from HEAD_FRESH */
    CC_NEXT_CHEAD,            /* Forw deletion from HEAD_CLEAN */
    CC_NEXT_DHEAD,            /* Forw deletion from HEAD_DIRTY */
    CC_HEADD_TO_STALE,        /* Change HEAD_DIRTY->HEAD_STALE */
    CC_HEADV_TO_STALE,        /* Change HEAD_VALID->HEAD_STALE */
    CC_TAILD_TO_STALE,        /* Change TAIL_DIRTY->TAIL_STALE */
    CC_TAILV_TO_STALE,        /* Change TAIL_VALID->TAIL_STALE */
    CC_PREV_IMID,             /* Back deletion from MID_IDLE */
    CC_PREV_DTAIL,            /* Back deletion from TAIL_DIRTY */
    CC_PREV_STAIL,            /* Back deletion from TAIL_STALE */
    CC_PREV_ITAIL,            /* Back deletion from TAIL_IDLE */
    CC_NEXT_IMID,             /* Forw deletion from MID_IDLE */
    CC_NEXT_VHEAD,            /* Forw deletion from HEAD_VALID */
    CC_NEXT_SHEAD,            /* Forw deletion from HEAD_STALE */
    CC_NEXT_IHEAD,            /* Forw deletion from HEAD_IDLE */
    CC_LOCK_SET,              /* Set cache-line lock */
    CC_COPY_VALID,            /* Prepend, data is copied */
    CC_COPY_STALE,            /* Prepend, data copied, made stale */
    CC_COPY_QOLB,             /* Prepend, QOLB queuing applied */
    CC_HEADE_TO_DIRTY,        /* Change HEAD_EXCL->HEAD_DIRTY */
    CC_TAILE_TO_DIRTY,        /* Change TAIL_EXCL->TAIL_DIRTY */
    CC_HEADE_TO_STALE0,       /* Change HEAD_EXCL->HEAD_STALE0 */
    CC_HEADE_TO_STALE1,       /* Change HEAD_EXCL->HEAD_STALE1 */
    CC_TAILE_TO_STALE0,       /* Change TAIL_EXCL->TAIL_STALE0 */
    CC_TAILE_TO_STALE1,       /* Change TAIL_EXCL->TAIL_STALE1 */
    CC_HEADU_TO_NEED0,        /* QOLB fetch from HEAD_EXCL/USED */
    CC_HEADU_TO_NEED1,        /* QOLB fetch from HEAD_EXCL/USED */
    CC_TAILU_TO_NEED0,        /* QOLB fetch from TAIL_EXCL/USED */
    CC_TAILU_TO_NEED1,        /* QOLB fetch from TAIL_EXCL/USED */
    CC_NEXT_EHEAD,            /* Forw deletion from HEAD_EXCL */
    CC_NEXT_NHEAD,            /* Forw deletion from HEAD_NEED */
    CC_PREV_ETAIL,            /* Back deletion from TAIL_EXCL */
    CC_PREV_NTAIL,            /* Back deletion from TAIL_NEED */
    CC_PREVI_TO_USED,         /* QOLB release: from TAIL_NEED */
    CC_PREVI_TO_NEED,         /* QOLB cycling: from TAIL_NEED */
    CC_TAILI_TO_USED,         /* QOLB release: from HEAD_NEED */
    CC_TAILI_TO_NEED          /* QOLB cycling: from HEAD_NEED */
  };

  SStatValues: enum {                      -- SStatValues (pg.18)
    RESP_NORMAL,                           --   not needed
    RESP_ADVICE,
    RESP_GONE,
    RESP_LOCKED,
    RESP_CONFLICT,
    RESP_DATA,
    RESP_TYPE,
    RESP_ADDRESS
  };

  MemoryCacheStates: enum {
    -- memory states                       -- MemoryStates (pg.25)
    MS_HOME,                        /* No sharing list */
    MS_FRESH,                       /* The same data are in memory and list */
    MS_GONE,                        /* The sharing-list data may differ */
    MS_WASH,                        /* Sharing-list transition (dirty->fresh) */

    -- cache states                        -- CacheStates (pg.31)
    /* CT_ONLY_DIRTY=0000XXX */
    CS_INVALID,                     /* # Invalid: cache-entry empty */
    CS_OD_RETN_IN,                  /* Busy ONLY_XXXX line, used in deletions */
    CS_OD_spin_IN,                  /* # Return ONLY_DIRTY, wait for prepend */
    CS_QUEUED_FRESH,                /* Prepending: (fresh list)->HEAD_FRESH */
    CS_ONLY_DIRTY,                  /* # Only-exclusive, read&modify, dirty */
    CS_QUEUED_DIRTY,                /* Prepend purge:(fresh list)->ONLY_DIRTY */

    /* CT_HEAD_DIRTY=0001XXX */
    CS_HEAD_DIRTY,                  /* # Head of sharing list, dirty */
    CS_QUEUED_CLEAN,                /* Prepending: ->HEAD_CLEAN,HEAD_DIRTY */
    CS_QUEUED_MODS,                 /* Modified QUEUED_CLEAN, ->ONLY_DIRTY */

    /* CT_MID_VALID=0010XXX, also CT_ONLYQ_DIRTY alias */
    CS_PENDING,                     /* Pending: waiting for memory response */
    CS_QUEUED_JUNK,                 /* Attaching to list_clean or list_dirty */
    CS_MV_forw_MV,                  /* # Forw deletion: from MID_VALID */
    CS_MV_back_IN,                  /* # Back deletion: from MID_VALID */
    CS_MID_VALID,                   /* # Middle of list, dirty valid copy */
    CS_MID_COPY,                    /* # Middle of list, clean valid copy */
    CS_ONLYQ_DIRTY,                 /* # Pairwise capable ONLY_DIRTY */
    CS_HD_INVAL_OD,                 /* Purging: HEAD_DIRTY->ONLY_DIRTY/USED */

    /* CT_HEAD_IDLE=0011XXX */
    CS_MI_forw_MI,                  /* # Forw deletion: from MID_IDLE */
    CS_MI_back_IN,                  /* # Back deletion: from MID_IDLE */
    CS_HEAD_IDLE,                   /* # Head-stale copy, queued for QOLB */
    CS_MID_IDLE,                    /* # Mid-stale copy, queued for QOLB */
    CS_TAIL_VALID,                  /* # Tail-entry copy: read-only & dirty */
    CS_TAIL_COPY,                   /* # Tail-entry copy: read-only & clean */

    /* CT_ONLY_FRESH=0100XXX */
    CS_OF_retn_IN,                  /* # List deletion: from ONLY_FRESH */
    CS_QUEUED_HOME,                 /* Purging: (fresh list)->INVALID */
    CS_HX_FORW_HX,                  /* Forw deletion: from HEAD_DIRTY/CLEAN */
    CS_HX_FORW_OX,                  /* Forw deletion: list also collapses */
    CS_ONLY_FRESH,                  /* # One list entry: read-only copy */
    CS_OF_MODS_OD,                  /* Converting: ONLY_FRESH->ONLY_DIRTY */

    /* CT_HEAD_FRESH=0101XXX */
    CS_HX_INVAL_OX,                 /* Purging others: list also collapses */
    CS_HEAD_FRESH,                  /* # Head shared/clean/read-only entry */
    CS_HF_MODS_HD,                  /* Converting: HEAD_FRESH->HEAD_DIRTY */

    /* CT_ONLYP_DIRTY=0110XXX */
    CS_TO_INVALID,                  /* Purged, while in transient state */
    CS_TV_back_IN,                  /* Back deletion: from TAIL_VALID */
    CS_QF_FLUSH_IN,                 /* Purging: (fresh list)->FLUSHED */
    CS_LOCAL_CLEAN,                 /* # Noncoherent copy, clean */
    CS_HW_WASH_HF,                  /* Converting: HEAD_WASH->HEAD_FRESH */
    CS_ONLYP_DIRTY,                 /* # Pairwise capable ONLY_DIRTY */

    /* CT_HEAD_RETN=0111XXX */
    CS_HEAD_STALE0,                 /* # Head of pairwise list, stale */
    CS_HEAD_STALE1,                 /* # Head of pairwise list, stale */
    CS_HX_retn_IN,                  /* # Back deletion: from HEAD_XXXX */
    CS_HEAD_VALID,                  /* # Head of pairwise list, valid */
    CS_LOCAL_DIRTY,                 /* # Noncoherent copy, dirty */

    /* CT_HEAD_CLEAN=1000XXX */
    CS_H0_PEND_HX,                  /* Head0 transition, concurrent prepend */
    CS_H1_PEND_HX,                  /* Head1 transition, concurrent prepend */
    CS_HX_PEND_HX,                  /* Head transition, concurrent prepend */
    CS_OC_retn_IN,                  /* # Back deletion: from ONLY_CLEAN */
    CS_HEAD_CLEAN,                  /* # Head of shared copies, clean */
    CS_ONLY_CLEAN,                  /* # Only modifiable copy, clean */
    CS_HX_XXXX_OD,                  /* Transient conflict: ->ONLY_DIRTY */

    /* CT_HEAD_WASH=1001XXX */
    CS_TS0_Move_TE,                 /* Converting: TAIL_STALE0->TAIL_EXCL */
    CS_TS1_Move_TE,                 /* Converting: TAIL_STALE1->TAIL_EXCL */
    CS_TS_copy_TV,                  /* Converting: TAIL_STALE->TAIL_VALID */
    CS_TV_mark_TS,                  /* Like TS_copy_TV, head conflict */
    CS_HEAD_WASH,                   /* # Like HEAD_CLEAN, wash conflict */
    CS_TX_XXXX_OD,                  /* Transient conflict: ->ONLY_DIRTY */

    /* CT_TAIL_EXCL=1010XXX */
    CS_TV_mark_TE,                  /* # Converting: TAIL_VALID->TAIL_EXCL */
    CS_TD_back_IN,                  /* # Back deletion: from TAIL_DIRTY */
    CS_TS_back_IN,                  /* Back deletion: from TAIL_STALE */
    CS_TE_back_IN,                  /* # Back deletion: from TAIL_EXCL */
    CS_TAIL_DIRTY,                  /* # Pairwise shared, owned copy */
    CS_TAIL_EXCL,                   /* # Pairwise shared, exclusive tail */
    CS_TD_mark_TE,                  /* # Converting: TAIL_DIRTY->TAIL_EXCL */

    /* CT_HEAD_EXCL=1011XXX */
    CS_TAIL_IDLE,                   /* Like TAIL_STALE, awaiting QOLB copy */
    CS_TI_back_IN,                  /* Back deletion: from TAIL_IDLE */
    CS_TAIL_STALE0,                 /* Pairwise exclusive, stale copy */
    CS_TAIL_STALE1,                 /* Pairwise exclusive, stale copy */
    CS_HV_MARK_HE,                  /* Converting: HEAD_VALID->HEAD_EXCL */
    CS_HEAD_EXCL,                   /* # Pairwise exclusive, owned copy */
    CS_HD_MARK_HE,                  /* Converting: HEAD_DIRTY->HEAD_EXCL */

    /* CT_ONLY_USED=1100XXX */
    CS_TN_back_IN,                  /* # Back deletion: from TAIL_NEED */
    CS_TN_send_TS,                  /* # QOLB release: TAIL_NEED->TAIL_STALE */
    CS_TN_send_TI,                  /* # QOLB release: TAIL_NEED->TAIL_IDLE */
    CS_OD_CLEAN_OC,                 /* Converting: ONLY_DIRTY->ONLY_CLEAN */
    CS_ONLY_USED,                   /* # Like ONLY_DIRTY, QOLB owned */
    CS_TAIL_NEED,                   /* # Like TAIL_USED, QOLB requested */

    /* CT_HEAD_NEED=1101XXX */
    CS_HS0_MOVE_HE,                 /* Converting: HEAD_STALE0->HEAD_EXCL */
    CS_HS1_MOVE_HE,                 /* Converting: HEAD_STALE1->HEAD_EXCL */
    CS_HS_COPY_HV,                  /* Converting: HEAD_STALE->HEAD_VALID */
    CS_HV_MARK_HS,                  /* Converting: HEAD_VALID->HEAD_STALE */
    CS_HD_CLEAN_HC,                 /* Converting: HEAD_DIRTY-HEAD_CLEAN */
    CS_HEAD_NEED,                   /* # Like HEAD_USED, QOLB requested */
  
    /* CT_TAIL_USED=1110XXX */
    CS_SAVE_STALE,                  /* Being purged, was HEAD_VALID */
    CS_SS_next_IN,                  /* Forw deletion: from SAVE_STALE */
    CS_HN_SEND_HS,                  /* Converting: HEAD_NEED->HEAD_STALE */
    CS_HN_SEND_HI,                  /* Converting: HEAD_NEED->HEAD_IDLE */
    CS_HD_WASH_HF,                  /* Converting: HEAD_DIRTY->HEAD_FRESH */
    CS_TAIL_USED,                   /* # Like TAIL_EXCL, QOLB owned */

    /* CT_HEAD_USED=1111XXX */
    CS_HS0_MOVE_HI,                 /* Converting: HEAD_STALE0->HEAD_IDLE */
    CS_HS1_MOVE_HI,                 /* Converting: HEAD_STALE1->HEAD_IDLE */
    CS_TS0_Move_TI,                 /* Converting: TAIL_STALE0->TAIL_IDLE */
    CS_TS1_Move_TI,                 /* Converting: TAIL_STALE1->TAIL_IDLE */
    CS_HEAD_USED,                   /* # Like HEAD_EXCL, QOLB owned */

    /* Internal intermediate vendor-dependent codes */
    CI_HEAD_DIRTY,                  /* Like HEAD_DIRTY state */
    CI_HEAD_CLEAN,                  /* Like HEAD_CLEAN state */
    CI_HEAD_WASH,                   /* Like HEAD_WASH state */
    CI_HEAD_MODS,                   /* Like HEAD_DIRTY, ->ONLY_DIRTY */
    CI_HEADQ_DIRTY,                 /* Like HEAD_DIRTY, ->ONLYQ_DIRTY */
    CI_HEADQ_EXCL,                  /* Like HEAD_EXCL, ->ONLYQ_DIRTY */
    CI_HD_WASH_HF,                  /* Like HEAD_DIRTY, ->HEAD_FRESH */
    CI_HW_WASH_HF,                  /* Like HEAD_WASH, ->HEAD_FRESH */

    CI_QUEUED_CLEAN,                /* Like QUEUED_CLEAN, ->HEAD_CLEAN */
    CI_QUEUED_FRESH,                /* Like QUEUED_FRESH, ->HEAD_FRESH */
    CI_QUEUED_DIRTY,                /* Like QUEUED_DIRTY, ->ONLY_DIRTY */
    CI_ONLY_EXCL,                   /* Like ONLY_DIRTY */
    CI_HEAD_EXCL,                   /* Like HEAD_EXCL */
    CI_TAIL_EXCL,                   /* Like TAIL_EXCL */
    CI_QD_MODS_OU,                  /* Like QUEUED_DIRTY, ->ONLY_USED */
    CI_HD_MODS_OU,                  /* Like HEAD_DIRTY, ->ONLY_USED */

    CI_HD_FLUSH_IN,                 /* Like HEAD_DIRTY, ->flushed */
    CI_HC_FLUSH_IN,                 /* Like HEAD_CLEAN, ->flushed */
    CI_LD_FLUSH_IN,                 /* Like LOCAL_DIRTY, ->flushed */
    CI_OD_PURGE_IN,                 /* Like ONLY_DIRTY, ->purged */
    CI_HN_PURGE_IN,                 /* Like HEAD_NEED, ->purged */
    CI_HE_PURGE_IN,                 /* Like HEAD_EXCL, ->purged */
    CI_HD_PURGE_IN,                 /* Like HEAD_DIRTY, ->purged */
    CI_LD_CLEAN_LC,                 /* Like LOCAL_DIRTY, ->LOCAL_CLEAN */

    CI_OD_CLEAN_OC,                 /* Like ONLY_DIRTY, ->ONLY_CLEAN */
    CI_HD_CLEAN_HC,                 /* Like HEAD_DIRTY, ->HEAD_CLEAN */
    CI_HN_CLEAN_OC,                 /* Like HEAD_NEED, ->ONLY_CLEAN */
    CI_HE_CLEAN_OC,                 /* Like HEAD_EXCL, ->ONLY_CLEAN */
    CI_HD_MODS_HE,                  /* Like HEAD_DIRTY, ->HEAD_EXCL */
    CI_HD_MODS_HU,                  /* Like HEAD_DIRTY, ->HEAD_USED */
    CI_TD_MODS_TE,                  /* Like TAIL_DIRTY, ->TAIL_EXCL */
    CI_TD_MODS_TU,                  /* Like TAIL_DIRTY, ->TAIL_USED */

    CI_HN_DEQOLB_HS,                /* Like HEAD_NEED, ->HEAD_STALE */
    CI_TN_DEQOLB_TS,                /* Like TAIL_NEED, ->TAIL_STALE */
    CI_HN_REQOLB_HI,                /* Like HEAD_NEED, ->HEAD_IDLE */
    CI_TN_REQOLB_TI,                /* Like TAIL_NEED, ->TAIL_IDLE */
    CI_HI_DELETE_IN,                /* Like HEAD_IDLE, ->deleted */
    CI_MI_DELETE_IN,                /* Like MID_IDLE, ->deleted */
    CI_TI_DELETE_IN,                /* Like TAIL_IDLE, ->deleted */
    CI_CHECK_HOME,                  /* Coherent check, is in memory */

    CI_HF_FLUSH_IN,                 /* Like HEAD_FRESH, ->flushing */
    CI_QF_FLUSH_IN,                 /* Like QUEUED_FRESH, ->flushing */
    CI_QD_FLUSH_IN,                 /* Like QUEUED_DIRTY, ->flushing */
    CI_OD_FLUSH_IN,                 /* Like ONLY_DIRTY, ->flushing */
    CI_OC_FLUSH_IN,                 /* Like ONLY_CLEAN, ->flushing */
    CI_OF_FLUSH_IN,                 /* Like ONLY_FRESH, ->flushing */
    CI_HN_FLUSH_IN,                 /* Like HEAD_NEED, ->flushing */
    CI_HE_FLUSH_IN,                 /* Like HEAD_EXCL, ->flushing */

    CI_WRITE_CHECK,                 /* Coherent write into memory */
    CI_WC_DONE_IN,                  /* Successful WRITE_CHECK, ->invalid */
    CI_READ_FRESH,                  /* Coherent read, -> memory */
    CI_ONLY_CLEAN,                  /* Like ONLY_CLEAN */
    CI_ONLY_DIRTY                   /* Like ONLY_DIRTY */
  };

  -- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  -- data structures for memory behavior

  DataFlags : enum {                       -- DataFlags (pg.26)
    NORM_NONE,                      /* Complete no data return */
    NORM_DATA,                      /* Complete data returned */
    NORM_NULL,                      /* Nullify, no data returned */
    NORM_SKIP,                      /* Nullify, data returned */
    FULL_NONE,                      /* Optional complete, no data return */
    FULL_DATA,                      /* Optional complete, data returned */
    FULL_NULL,                      /* Optional nullify, no data return */
    HUGE_TAGS,                      /* Request Ids bits exceed tag-storage */
    LOCK_DATA,                      /* Set lock-error status, but return data */
    MISS_MORE,                      /* Recognized state, continue checking */
    MISS_NONE                       /* Unrecognized state, continue checking */
  };

  -- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  -- data structures for cache behavior

  AccessTypes : enum {                     -- AccessTypes (pg.29)
    CT_ONLY_DIRTY,                  /* One dirty (gone) entry */
    CT_HEAD_DIRTY,                  /* Head of dirty list */
    CT_MID_VALID,                   /* Like MID_VALID (purge continues) */
    CT_HEAD_IDLE,                   /* Idle list prepend */
    CT_ONLY_FRESH,                  /* One fresh entry (invalidate stops) */
    CT_HEAD_FRESH,                  /* Head of fresh list */
    CT_ONLYP_DIRTY,                 /* Pairwise capable ONLY_DIRTY */
    CT_HEAD_RETN,                   /* HX_retn_IN (strip from front) */

    CT_HEAD_CLEAN,                  /* Head of clean list */
    CT_HEAD_WASH,                   /* Head of wash (dirty->clean) list */
    CT_TAIL_EXCL,                   /* Exclusive, preceeded by HEAD_STALE */
    CT_HEAD_EXCL,                   /* Exclusive, followed by TAIL_STALE */
    CT_ONLY_USED,                   /* Only entry, qolb-lock set */
    CT_HEAD_NEED,                   /* Qolb-owned, other copy requested */
    CT_TAIL_USED,                   /* Qolb-owned, preceded by HEAD_STALE */
    CT_HEAD_USED                    /* Qolb-owned, followed by TAIL_STALE */
  };

  -- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  -- data structures for execution unit behavior

  FetchOptions : enum {                    -- FetchOptions (pg.28)
    CO_FETCH,                       /* Instruction-cache fetch */
    CO_LOAD,                        /* Read-only data access */
    CO_STORE                        /* Write-likely data, pairwise unlikely */
  };

  TransactStatus : enum {                  -- TransactStatus (pg.26)
    TS_NORM_NULL,                   /* Normal access, null response code */
    TS_NORM_CODE,                   /* Normal access, good response code */
    TS_RESP_FAKE,                   /* Abnormal access, fake response code */
    TS_RESP_CODE,                   /* Abnormal access, with response code */
    TS_CORE_JUNK,                   /* Illegal sourceId=coreId value */
    TS_RESP_SIZE,                   /* Unexpected response-packet size */
    TS_RESP_DIFF,                   /* Inaccurate response-packet size */
    TS_LINK_DEAD,                   /* Transfer cloud in unusable state */
    TS_LOST_TIDS,                   /* Discard, waiting for tid assignment */
    TS_TIME_TIDS,                   /* Timeout, waiting for tid assignment */
    TS_SEND_TYPE,                   /* Type error, while sending request */
    TS_SEND_ADDR,                   /* Address error, while sending request */
    TS_TIME_BUSY,                   /* Timeout, waiting for busy-retry */
    TS_TIME_RESP,                   /* Timeout, waiting for response */
    TS_RESP_IDS,                    /* Response packet: unexpected targetId */
    TS_RESP_BAD,                    /* Response packet: bad CRC check */
    TS_LOCK_BEFORE,                 /* Cache-tag cLock set before access */
    TS_LOCK_DURING,                 /* Cache-tag cLock set during access */
    TS_MTAG_STATE,                  /* Memory-tag inconsistency */
    TS_CTAG_STATE,                  /* Cache-tag inconsistency */
    TS_CODE_NULL                    /* Error code which is never used */
  };

  FindFlags : enum {                       -- FindFlags (pg.28)
    FF_FIND,                        /* Find stabilized cache state or invalid */
    FF_WAIT,                        /* Fetch stabilized cache states */
    FF_LOAD,                        /* Load access of transient cache states */
    FF_STORE,                       /* Store access of transient cache states */
    FF_LOCK,                        /* Lock access of transient cache states */
    FF_ENQOLB,                      /* Wait for qolb-owned line */
    FF_INDEX                        /* Cache line address is its index */
  };

  BypassOptions : enum {                   -- BypassOptions (pg.26)
    CU_BYPASS,                      /* Bypass cache, trap on conflicts */
    CU_LOCAL,                       /* Local caching, no coherence */
    CU_GLOBAL                       /* Global caching, full coherence */
  };

  StableGroups : enum {                    -- StableGroups (pg.31)
    SG_NONE,                        /* Transient cache-line state */
    SG_INVALID,                     /* Invalid (CS_INVALID) state */
    SG_STALE,                       /* A stale or idle state */
    SG_GLOBAL,                      /* A readable global state */
    SG_LOCAL                        /* A readable local state */
  };

  AttachStatus: record                     -- model AttachStatus (pg.40)
    cState:  MemoryCacheStates;
    backId:  NodeId;
  end;


--------------------------------------------------------------------------------
-- packets

  -- request packet
  ReqPacket: record
    targetId:  NodeId;                 -- target for the packet
    cmd:       AccessCommandsReq;      -- transaction command 
    sourceId:  NodeId;                 -- originator of packet
    offset:    MemoryLineId;           -- address offset
    mopCop:    MemoryCacheCommands;    -- memory and cache command
    s:         Boolean;                -- size
    eh:        Boolean;                -- extended header
    data:      Data;                   -- data
    newId:     NodeId;                 -- in extended header
    memId:     NodeId;
  end;

  -- response packet
  ResPacket: record
    cmd:       AccessCommandsRes;
    cn:        Boolean;                -- command-nullified bit
    co:        Boolean;                -- coherence-option bit
    mCState:   MemoryCacheStates;      -- old memory and cache line states
    data:      Data;
    forwId:    NodeId;                 -- not in extended header
    backId:    NodeId;
  end;


--------------------------------------------------------------------------------
-- memory

  MemoryLine: record
    mState:  MemoryCacheStates;
    forwId:  NodeId;
    data:    Data;
  end;

  Memory: record
    line:      array[MemoryLineId] of MemoryLine;
  end;

--------------------------------------------------------------------------------
-- processor

  -- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  -- cache

  CacheLine: record
    cState:         MemoryCacheStates;
    forwId:         NodeId;
    backId:         NodeId;
    memId:          NodeId;
    addrOffset:     MemoryLineId;
    data:           Data;
  end;

  Cache: record
    line:      array[CacheLineId] of CacheLine;
  end;

  -- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  -- execution unit

  InstructionType : enum {            -- typical set instructions
    I_NONE,
    I_LOAD,
    I_STORE,
    I_FLUSH,
    I_DELETE,
    I_LOCK
  };

  InstructionPhase : enum {           -- instruction phases
    I_START,                          --   execution phase cannot initiate
    I_ALLOCATE,                       --   transaction, therefore left out
    I_SETUP,
    I_CLEANUP
  };

  LB: record
    cState:        MemoryCacheStates;
    firstTo:       Boolean;
    firstEntrySub: Boolean;
    memId:         MemoryId;
    addrOffset:    MemoryLineId;
  end;

  LC: record
    cState:        MemoryCacheStates;
  end;

  SubState: record                    -- state of instruction subroutines
    lB: LB;                           --   levelB
    lC: LC;                           --   levelC
  end;

  L0: record
    secondEntry:   Boolean;
    mState:        MemoryCacheStates;
    co:            Boolean;
  end;

  L1: record
    firstEntrySub: Boolean;
    firstLoop:     Boolean;
    cState:        MemoryCacheStates;
  end;

  L2: record
    firstEntrySub: Boolean;
    firstLoop:     Boolean;
    cState:        MemoryCacheStates;
    nextId:        ProcId;
  end;

  ToState: record                     -- state of ...To... routines
    l0: L0;                           --   level0
    l1: L1;                           --   level1
    l2: L2;                           --   level2
  end;

  ExecState: record
    insType:       InstructionType;   -- type of instruction in progress
    insPhase:      InstructionPhase;  -- phase of instruction in progress
    cTPtr:         CacheLineId;       -- currently accessed cache line
    tStat:         TransactStatus;    -- internal transaction-status
    fetchOption:   FetchOptions;      -- fetch option for load instruction
    data:          Data;              -- data for store instruction
    subState:      SubState;          -- state of instruction subroutines
    toState:       ToState;           -- state of ...To... routines
  end;

  ExecUnit: record
    state:         ExecState;
    reqPacket:     ReqPacket;
    resPacket:     ResPacket;
  end;

  -- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  -- processor

  Proc: record
    exec:   ExecUnit;
    cache:  Cache;
  end;


 
--------------------------------------------------------------------------------
-- variable declarations
--------------------------------------------------------------------------------

var
  cacheUsage: BypassOptions;                 -- cache usage
  proc:    array[ProcId] of Proc;            -- processors
  memory:  array[MemoryId] of Memory;        -- memories



--------------------------------------------------------------------------------
-- procedures and functions
--------------------------------------------------------------------------------


--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
-- general procedures and functions


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- miscellanies

procedure UndefineUnusedValues();
begin
  -- state of cache
  for i: ProcId do
    for j: CacheLineId do
      alias line: proc[i].cache.line[j] do
        switch line.cState
        case CS_INVALID:
          undefine line;
          line.cState := CS_INVALID;
        case CS_ONLY_FRESH, CS_ONLY_CLEAN, CS_ONLY_DIRTY:
          undefine line.forwId;
          undefine line.backId;
        case CS_HEAD_FRESH, CS_HEAD_CLEAN, CS_HEAD_DIRTY:
          undefine line.backId;
        case CS_TAIL_VALID, CS_TAIL_COPY:
          undefine line.forwId;
        end;
      end;
    end;
  end;

  -- state of memory
  for i: MemoryId do
    for j: MemoryLineId do
      if memory[i].line[j].mState=MS_HOME then
        undefine memory[i].line[j].forwId;
      end;
    end;
  end;
end;


procedure UndefineUnusedRes(var p: ResPacket);
begin
  if p.cn then               -- nullified packet
--    undefine p.cmd;
    undefine p.co;
    undefine p.data;
    undefine p.forwId;
    undefine p.backId;
  else                       -- non-nullified packet
    if p.cmd=SC_RESP00 then
      undefine p.data;
    end;
  end;
end;


procedure UndefineUnusedReq(var p: ReqPacket);
begin
  if p.cmd != SC_MWRITE64 then
    undefine p.data;
  end;
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- packet handling

-- send response packet p to processor i
procedure SendResPacket (p: ResPacket; i: ProcId);
begin
  alias
    resP: proc[i].exec.resPacket
  do
    resP := p;
    UndefineUnusedRes(resP);
  end;
end;


-- set request packet p by processor i (everything but the extended header)
procedure SetReqPacket (i: ProcId; var p: ReqPacket;
                        target: NodeId;
                        cmd: AccessCommandsReq;
                        mopCop: MemoryCacheCommands; s: Boolean;
                        eh: Boolean);
begin
  alias
    line: proc[i].cache.line[proc[i].exec.state.cTPtr];
  do
    undefine p;
    p.targetId := target;
    p.cmd      := cmd;
    p.sourceId := i;
    if ! isundefined(line.addrOffset) then
      p.offset   := line.addrOffset;
    end;
    p.mopCop   := mopCop;
    p.s        := s;
    p.eh       := eh;
    if ! isundefined(line.data) then
      p.data     := line.data;
    end;
  end;  -- alias
end;


-- send request packet p from node i to its target
procedure SendReqPacket (i: ProcId; p: ReqPacket);
begin
  alias
    reqP: proc[i].exec.reqPacket;
  do
    reqP := p;
    UndefineUnusedReq(reqP);
  end;
end;


-- remove request packet p
procedure RemoveReqPacket (var p: ReqPacket);
begin
  undefine p;
end;


-- remove response packet p
procedure RemoveResPacket (var p: ResPacket);
begin
  undefine p;
end;


--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
-- memory behavior


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine MemoryTagLockUpdate (pg.147)
function MemoryTagLockUpdate (var line: MemoryLine;
                              code: MemoryCacheCommands): DataFlags;
begin
  switch code
  case MC_LOCK_SET:
    error "MemoryTagLockUpdate: MC_LOCK_SET not implemented";
  case MC_LOCK_CLEAR:
    error "MemoryTagLockUpdate: MC_LOCK_CLEAR not implemented";
  else
    -- break
  end;
  return MISS_MORE;
end;
 

-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine MemoryTagFreshUpdate (pg.148)
function MemoryTagFreshUpdate (var line: MemoryLine; 
                               code: MemoryCacheCommands;
                               sourceId: NodeId; nextId: NodeId): DataFlags;
var
  eq: Boolean;
begin
  switch line.mState
  case MS_HOME, MS_GONE, MS_FRESH:
    -- allowed states
  else
    error "MemoryTagFreshUpdate: memory state not allowed";
  end;
  
  alias
    s: line.mState
  do
    if ! isundefined(line.forwId) then
      eq := (sourceId = line.forwId);
    end;  -- otherwise eq is undefined
      
    switch code

    case MC_ATTACH_TO_LIST:
      if s=MS_HOME then
        return FULL_NONE;
      elsif !eq & s=MS_FRESH then
        line.forwId := nextId;
      elsif !eq & s=MS_GONE then
        line.forwId := nextId;
        return FULL_NONE;
      else
        return FULL_NULL;
      end;

    case MC_ATTACH_TO_GONE:
      if s=MS_HOME |
         !eq & s=MS_FRESH then 
        -- break
      elsif !eq & s=MS_GONE then
        line.forwId := nextId;
        return FULL_NONE;
      else
        return FULL_NULL;
      end;

    case MC_FRESH_TO_HOME:
      if s=MS_HOME then
        -- break
      elsif !eq & s=MS_FRESH then
        line.mState := MS_HOME;
      elsif !eq & s=MS_GONE then
        line.forwId := nextId;
        return FULL_NONE;
      else
        return FULL_NULL;
      end;

    case MC_LIST_TO_GONE:
      if s=MS_HOME |
         !eq & s=MS_FRESH |
         !eq & s=MS_GONE then
        return FULL_NULL;
      elsif eq & s=MS_FRESH then
        line.mState := MS_GONE;
      else
        return FULL_NONE;
      end;

    case MC_LIST_TO_HOME:
      if s=MS_HOME |
         !eq & s=MS_FRESH |
         !eq & s=MS_GONE then
        return FULL_NULL;
      elsif eq & s=MS_FRESH then
        line.mState := MS_HOME;
      else
        line.mState := MS_HOME;
        return FULL_NONE;
      end;

    case MC_CACHE_FRESH:
      if s=MS_HOME then
        line.mState := MS_FRESH;
        line.forwId := nextId;
        return FULL_DATA;
      elsif !eq & s=MS_FRESH then
        line.forwId := nextId;
      elsif !eq & s=MS_GONE then
        line.forwId := nextId;
        return FULL_NONE;
      else
        return FULL_NULL;
      end;

    case MC_CACHE_CLEAN, MC_CACHE_DIRTY:
      if s=MS_HOME |
         !eq & s=MS_FRESH then
        line.mState := MS_GONE;
        line.forwId := nextId;
      elsif !eq & s=MS_GONE then
        line.forwId := nextId;
        return FULL_NONE;
      else
        return FULL_NULL;
      end;

    case MC_PASS_HEAD:
      if s=MS_HOME |
         !eq & s=MS_FRESH then
        return FULL_NULL;
      elsif !eq & s=MS_GONE then
        return FULL_NULL;
      elsif eq & s=MS_FRESH then
        line.forwId := nextId;
      else
        line.forwId := nextId;
        return FULL_NONE;
      end;

    case MC_LIST_TO_FRESH:
      if s=MS_HOME |
         s=MS_FRESH |
         !eq & s=MS_GONE then
        return FULL_NULL;
      else
        line.mState := MS_FRESH;
      end;

    case MC_WASH_TO_FRESH:
      return FULL_NULL;

    else
      return NORM_NULL;
    end;  -- switch code
  end;  -- alias

  return FULL_DATA;
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine MemoryTagUpdate (pg.147)
function MemoryTagUpdate (var line: MemoryLine; inP: ReqPacket): DataFlags;
var
  update: DataFlags;
  nextId: NodeId;
begin
  update := MemoryTagLockUpdate (line, inP.mopCop);
  if update != MISS_MORE then
    return update;
  end;
  nextId := inP.eh ? inP.newId : inP.sourceId;
  if MOP_FRESH then
    return MemoryTagFreshUpdate (line, inP.mopCop, inP.sourceId, nextId);
  end;
  error "MemoryTagUpdate: only MOP_FRESH implemented";
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine MemoryAccessBasic (pg.145)
function MemoryAccessBasic (var line: MemoryLine; inP: ReqPacket;
                            var outP: ResPacket) : SStatValues;
begin
  switch inP.cmd
  case SC_MREAD:
    if ! inP.s then
      outP.cmd := SC_RESP00;
    else
      outP.data := line.data;
      outP.cmd  := SC_RESP64;
    end;
  case SC_MWRITE64:
    line.data := inP.data;
    outP.cmd  := SC_RESP00;
  else
    error "MemoryAccessBasic: requested memory command not implemented";
  end;
  return RESP_NORMAL;
end;


--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
-- cache behavior


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI macro CSTATE_TYPES (pg.28)
function CSTATE_TYPES (s: MemoryCacheStates): AccessTypes;
begin
  switch s
  case CS_INVALID, CS_OD_RETN_IN, CS_OD_spin_IN, CS_QUEUED_FRESH,
       CS_ONLY_DIRTY, CS_QUEUED_DIRTY:
    return CT_ONLY_DIRTY;
  case CS_HEAD_DIRTY, CS_QUEUED_CLEAN, CS_QUEUED_MODS:
    return CT_HEAD_DIRTY;
  case CS_PENDING, CS_QUEUED_JUNK, CS_MV_forw_MV, CS_MV_back_IN,
       CS_MID_VALID, CS_MID_COPY, CS_ONLYQ_DIRTY, CS_HD_INVAL_OD:
    return CT_MID_VALID;
  case CS_MI_forw_MI, CS_MI_back_IN, CS_HEAD_IDLE, CS_MID_IDLE,
       CS_TAIL_VALID, CS_TAIL_COPY:
    return CT_HEAD_IDLE;
  case CS_OF_retn_IN, CS_QUEUED_HOME, CS_HX_FORW_HX, CS_HX_FORW_OX,
       CS_ONLY_FRESH, CS_OF_MODS_OD:
    return CT_ONLY_FRESH;
  case CS_HX_INVAL_OX, CS_HEAD_FRESH, CS_HF_MODS_HD:
    return CT_HEAD_FRESH;
  case CS_TO_INVALID, CS_TV_back_IN, CS_QF_FLUSH_IN, CS_LOCAL_CLEAN,
       CS_HW_WASH_HF, CS_ONLYP_DIRTY:
    return CT_ONLYP_DIRTY;
  case CS_HEAD_STALE0, CS_HEAD_STALE1, CS_HX_retn_IN, CS_HEAD_VALID,
       CS_LOCAL_DIRTY:
    return CT_HEAD_RETN;
  case CS_H0_PEND_HX, CS_H1_PEND_HX, CS_HX_PEND_HX, CS_OC_retn_IN,
       CS_HEAD_CLEAN, CS_ONLY_CLEAN, CS_HX_XXXX_OD:
    return CT_HEAD_CLEAN;
  case CS_TS0_Move_TE, CS_TS1_Move_TE, CS_TS_copy_TV, CS_TV_mark_TS,
       CS_HEAD_WASH, CS_TX_XXXX_OD:
    return CT_HEAD_WASH;
  case CS_TV_mark_TE, CS_TD_back_IN, CS_TS_back_IN, CS_TE_back_IN,
       CS_TAIL_DIRTY, CS_TAIL_EXCL, CS_TD_mark_TE:
    return CT_TAIL_EXCL;
  case CS_TAIL_IDLE, CS_TI_back_IN, CS_TAIL_STALE0, CS_TAIL_STALE1,
       CS_HV_MARK_HE, CS_HEAD_EXCL, CS_HD_MARK_HE:
    return CT_HEAD_EXCL;
  case CS_TN_back_IN, CS_TN_send_TS, CS_TN_send_TI, CS_OD_CLEAN_OC,
       CS_ONLY_USED, CS_TAIL_NEED:
    return CT_ONLY_USED;
  case CS_HS0_MOVE_HE, CS_HS1_MOVE_HE, CS_HS_COPY_HV, CS_HV_MARK_HS,
       CS_HD_CLEAN_HC, CS_HEAD_NEED:
    return CT_HEAD_NEED;
  case CS_SAVE_STALE, CS_SS_next_IN, CS_HN_SEND_HS, CS_HN_SEND_HI,
       CS_HD_WASH_HF, CS_TAIL_USED:
    return CT_TAIL_USED;
  case CS_HS0_MOVE_HI, CS_HS1_MOVE_HI, CS_TS0_Move_TI, CS_TS1_Move_TI,
       CS_HEAD_USED:
    return CT_HEAD_USED;
  else
    error "CSTATE_TYPES: cache state not allowed";
  end;
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine CacheCheckSample (pg.179)
-- only normal cache access modeled, routine had to be modeled by the condition
--   part of the rules in which this routine is called
function CacheCheckSample (i: ProcId;
                           offset: MemoryLineId; mem: NodeId;
                           k: CacheLineId): CacheLineId;
begin
  return k;
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine CacheTagLockUpdate (pg.246)
function CacheTagLockUpdate (var line: CacheLine;
                             code: MemoryCacheCommands): DataFlags;
begin
  switch code
  case CC_LOCK_SET:
    error "CacheTagLockUpdate: CC_LOCK_SET not implemented";
  case CC_LOCK_CLEAR:
    error "CacheTagLockUpdate: CC_LOCK_CLEAR not implemented";
  else
    -- break
  end;
  return MISS_MORE;
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine CacheTagBasicUpdate (pg.247)
function CacheTagBasicUpdate (var line: CacheLine;
                              code: MemoryCacheCommands;
                              sourceId: NodeId; newId: NodeId): DataFlags;
var 
  goodData: DataFlags;
begin
  alias
    s: line.cState;
  do
    goodData := (code=CC_PEND_VALID) ? NORM_NONE : NORM_DATA;
    switch s
    case CS_ONLY_FRESH, CS_ONLY_DIRTY, CS_TAIL_VALID, CS_OD_spin_IN,
         CS_OF_retn_IN, CS_TV_back_IN:
      -- break
    case CS_INVALID, CS_PENDING, CS_QUEUED_JUNK, CS_QUEUED_DIRTY,
         CS_HD_INVAL_OD, CS_HX_INVAL_OX, CS_OD_RETN_IN, CS_TO_INVALID:
           -- CS_HX_INVAL_OX added
      return MISS_MORE;
    else
      return MISS_NONE;
    end;

    switch code

    case CC_PEND_VALID, CC_COPY_VALID, CC_COPY_STALE, CC_COPY_QOLB:
      switch s
      case CS_ONLY_FRESH, CS_ONLY_DIRTY:
        if s=CS_ONLY_FRESH & COP_CLEAN then
          -- break
        else
          line.backId := sourceId;
          s           := CS_TAIL_VALID;
          return goodData;
        end;
      case CS_OF_retn_IN, CS_OD_spin_IN:
        line.backId := sourceId;
        s           := CS_TV_back_IN;
        return goodData;
      else
        -- break
      end;

    case CC_FRESH_INVALID:
      switch s
      case CS_ONLY_FRESH:
        line.cState := CS_INVALID;
        return NORM_NONE;
      case CS_OF_retn_IN:
        s := CS_TO_INVALID;
        return NORM_NONE;
      else
        -- break
      end;

    case CC_VALID_INVALID:
      switch s
      case CS_TAIL_VALID:
        line.cState := CS_INVALID;
        return NORM_NONE;
      case CS_TV_back_IN:
        s := CS_TO_INVALID;
        return NORM_NONE;
      else
        -- break
      end;

    case CC_NEXT_FHEAD:
      switch s
      case CS_TAIL_VALID:
        if line.backId!=sourceId then
          -- break
        else
          s := CS_ONLY_FRESH;
          return NORM_NONE;
        end;
      else
        -- break
      end;

    case CC_NEXT_CHEAD, CC_NEXT_DHEAD:
      if code=CC_NEXT_CHEAD & COP_CLEAN then
        return MISS_MORE;
      end;
      switch s
      case CS_TAIL_VALID:
        if sourceId!=line.backId then
          -- break
        else
          s := CS_ONLY_DIRTY;  -- no pairwise sharing
          return NORM_NONE;
        end;
      else
        -- break
      end;

    case CC_NEXT_VMID:
      switch s
      case CS_TAIL_VALID:
        if sourceId!=line.backId then
          -- break
        else
          line.backId := newId;
          return NORM_NONE;
        end;
      else
        -- break
      end;
  
    end;  -- switch code

    return MISS_MORE;

  end;  -- alias
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine CacheTagDirtyUpdate (pg.247)
function CacheTagDirtyUpdate (var line: CacheLine;
                              code: MemoryCacheCommands;
                              sourceId: NodeId; newId: NodeId): DataFlags;
var 
  goodData: DataFlags;
begin
  alias
    s: line.cState;
  do
    goodData := (code=CC_PEND_VALID) ? NORM_NONE : NORM_DATA;
    switch s
    case CS_HX_FORW_HX, CS_HEAD_FRESH, CS_HEAD_DIRTY, CS_HX_retn_IN,
         CS_MID_VALID, CS_MV_forw_MV, CS_MV_back_IN:
      -- break
    case CS_QUEUED_CLEAN, CS_QUEUED_MODS, CS_HX_FORW_OX:
      return MISS_MORE;
    else
      return MISS_NONE;
    end;

    switch code

    case CC_PEND_VALID, CC_COPY_VALID, CC_COPY_STALE, CC_COPY_QOLB:
      switch s
      case CS_HEAD_FRESH, CS_HEAD_DIRTY:
        if s=CS_HEAD_FRESH & COP_CLEAN then
          -- break
        else
          line.backId := sourceId;
          s           := CS_MID_VALID;
          return goodData;
        end;
      case CS_HX_retn_IN:
        s := CS_TO_INVALID;
        return NORM_NONE;
      else
        -- break
      end;

    case CC_FRESH_INVALID:
      switch s
      case CS_HEAD_FRESH:
        line.cState := CS_INVALID;
        return NORM_NONE;
      case CS_HX_retn_IN:
        s := CS_TO_INVALID;
        return NORM_NONE;
      else
        -- break
      end;

    case CC_VALID_INVALID:
      switch s
      case CS_MID_VALID:
        line.cState := CS_INVALID;
        return NORM_NONE;
      case CS_MV_forw_MV, CS_MV_back_IN:
        s := CS_TO_INVALID;
        return NORM_NONE;
      else
        -- break
      end;

    case CC_NEXT_FHEAD:
      switch s
      case CS_MID_VALID:
        if sourceId!=line.backId then
          -- break
        else
          s := CS_HEAD_FRESH;
          return NORM_NONE;
        end;
      else
        -- break
      end;

    case CC_NEXT_CHEAD, CC_NEXT_DHEAD:
      if code=CC_NEXT_CHEAD & COP_CLEAN then
        return MISS_MORE;
      end;
      switch s
      case CS_MID_VALID:
        if sourceId!=line.backId then
          -- break
        else
          s := CS_HEAD_DIRTY;
          return NORM_NONE;
        end;
      else
        -- break
      end;

    case CC_NEXT_VMID:
      switch s
      case CS_MID_VALID:
        if sourceId!=line.backId then
          -- break
        else
          line.backId := newId;
          return NORM_NONE;
        end;
      else
        -- break
      end;

    case CC_PREV_VMID:
      switch s
      case CS_HEAD_FRESH, CS_HEAD_DIRTY, CS_HX_FORW_HX, CS_MID_VALID,
           CS_MV_forw_MV:
        if sourceId!=line.forwId then
          -- break
        else
          line.forwId := newId;
          return NORM_NONE;
        end;
      else
        -- break
      end;

    case CC_PREV_VTAIL:
      switch s
      case CS_HEAD_FRESH:
        if sourceId!=line.forwId then
          -- break
        else
          s := CS_ONLY_FRESH;
          return NORM_NONE;
        end;
      case CS_HEAD_DIRTY:
        if sourceId!=line.forwId then
          -- break
        else
          s := CS_ONLY_DIRTY;  -- no pairwise sharing
          return NORM_NONE;
        end;
      case CS_HX_FORW_HX:
        if sourceId!=line.forwId then
          -- break
        else
          s := CS_HX_FORW_OX;
          return NORM_NONE;
        end;
      case CS_MID_VALID:
        if sourceId!=line.forwId then
          -- break
        else
          s := CS_TAIL_VALID;
          return NORM_NONE;
        end;
      case CS_MV_forw_MV:
        if sourceId!=line.forwId then
          -- break
        else
          s := CS_TV_back_IN;
          return NORM_NONE;
        end;
      else
        -- break
      end;

    end;  -- switch code

    return MISS_MORE;

  end;  -- alias
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine CacheTagFreshUpdate (pg.249)
function CacheTagFreshUpdate (var line: CacheLine;
                              code: MemoryCacheCommands;
                              sourceId: NodeId; newId: NodeId): DataFlags;
begin
  switch line.cState
  case CS_QUEUED_FRESH:
    -- break
  else
    return MISS_NONE;
  end;

  return MISS_MORE;
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine CacheTagModsUpdate (pg.249)
function CacheTagModsUpdate (var line: CacheLine;
                             code: MemoryCacheCommands;
                             sourceId: NodeId; newId: NodeId): DataFlags;
begin
  alias
    s: line.cState;
  do
    switch s
    case CS_HF_MODS_HD:
      -- break
    case CS_OF_MODS_OD:
      return MISS_MORE;
    else
      return MISS_NONE;
    end;

    switch code

    case CC_PREV_VMID:
      switch s
      case CS_HF_MODS_HD:
        if line.forwId!=sourceId then
          -- break
        else
          line.forwId := newId;
          return NORM_NONE;
        end;
      else
        -- break
      end;

    case CC_PREV_VTAIL:
      switch s
      case CS_HF_MODS_HD:
        if line.forwId!=sourceId then
          -- break
        else
          s := CS_OF_MODS_OD;
          return NORM_NONE;
        end;
      else
        -- break
      end;

    end;  -- switch code

    return MISS_MORE;

  end;  -- alias
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine CacheTagUpdate (pg.178)
function CacheTagUpdate (var line: CacheLine; inP: ReqPacket): DataFlags;
var
  status: DataFlags;
  legal:  Boolean;
begin
  status := CacheTagLockUpdate(line, inP.mopCop);
  if status!=MISS_MORE then
    return status;
  end;

  status := CacheTagBasicUpdate(line, inP.mopCop, inP.sourceId, inP.newId);
  if status!=MISS_MORE & status!=MISS_NONE then 
    return status;
  end;
  legal := (status!=MISS_NONE);

  if COP_DIRTY then
    status := CacheTagDirtyUpdate(line, inP.mopCop, inP.sourceId, inP.newId);
    if status!=MISS_MORE & status!=MISS_NONE then
      return status;
    end;
    legal := legal | status!=MISS_NONE;
  end;

  if COP_FRESH then
    status := CacheTagFreshUpdate(line, inP.mopCop, inP.sourceId, inP.newId);
    if status!=MISS_MORE & status!=MISS_NONE then
      return status;
    end;
    legal := legal | status!=MISS_NONE;
  end;

  if COP_MODS then
    status := CacheTagModsUpdate(line, inP.mopCop, inP.sourceId, inP.newId);
    if status!=MISS_MORE & status!=MISS_NONE then
      return status;
    end;
    legal := legal | status!=MISS_NONE;
  end;

  assert legal "CacheTagUpdate: legal not true";
  return NORM_NULL;
end;


--------------------------------------------------------------------------------
--------------------------------------------------------------------------------
-- execution unit behavior


--------------------------------------------------------------------------------
-- general procedures and functions


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine CodeFault (pg.165)
function CodeFault (i: ProcId): Boolean;
begin
  switch proc[i].exec.state.tStat
  case TS_NORM_NULL, TS_NORM_CODE:
    -- break
  else
    return true;
  end;
  return false;
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine CodeSetTStat (pg.165)
procedure CodeSetTStat (i: ProcId; code: TransactStatus);
begin
  proc[i].exec.state.tStat := code;
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine TypicalStableStates (pg.185)
function TypicalStableStates (s: MemoryCacheStates): StableGroups;
begin
  switch s
  case CS_INVALID:
    return SG_INVALID;
  case CS_ONLY_DIRTY, CS_ONLY_FRESH, CS_HEAD_DIRTY, CS_HEAD_FRESH,
       CS_MID_VALID, CS_TAIL_VALID:
    return SG_GLOBAL;
  case CS_LOCAL_CLEAN, CS_LOCAL_DIRTY:
    return SG_LOCAL;
  else
    return SG_NONE;
  end;
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine CacheReadSrc (pg.209)
procedure CacheReadSrc (i: ProcId; cmd: AccessCommandsReq;
                        mopCop: MemoryCacheCommands; s: Boolean;
                        target: ProcId);
var
  outP: ReqPacket;
begin
  alias
    line: proc[i].cache.line[proc[i].exec.state.cTPtr];
  do
    SetReqPacket(i, outP, target, cmd, mopCop, s, true);
    if ! isundefined(line.memId) then
      outP.memId := line.memId;
    end;
    SendReqPacket(i, outP);
  end;
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine CacheReadNew (pg.209)
procedure CacheReadNew (i: ProcId; cmd: AccessCommandsReq;
                        mopCop: MemoryCacheCommands; s: Boolean;
                        target: ProcId; newId: NodeId);
var
  outP: ReqPacket;
begin
  alias
    line: proc[i].cache.line[proc[i].exec.state.cTPtr];
  do
    SetReqPacket(i, outP, target, cmd, mopCop, s, true);
    outP.newId := newId;
    if ! isundefined(line.memId) then
      outP.memId := line.memId;
    end;
    SendReqPacket(i, outP);
  end;
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine MemoryUpdateSrc (pg.209)
procedure MemoryUpdateSrc (i: ProcId; cmd: AccessCommandsReq;
                           mopCop: MemoryCacheCommands; s: Boolean);
var
  outP: ReqPacket;
begin
  alias
    line: proc[i].cache.line[proc[i].exec.state.cTPtr];
  do
    SetReqPacket(i, outP, line.memId, cmd, mopCop, s, false);
    assert ! isundefined(line.memId)
      "MemoryUpdateSrc: target undefined";
    SendReqPacket(i, outP);
  end;
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine MemoryUpdateNew (pg.209)
procedure MemoryUpdateNew (i: ProcId; cmd: AccessCommandsReq;
                           mopCop: MemoryCacheCommands; s: Boolean;
                           newId: NodeId);
var
  outP: ReqPacket;
begin
  alias
    line: proc[i].cache.line[proc[i].exec.state.cTPtr];
  do
    SetReqPacket(i, outP, line.memId, cmd, mopCop, s, true);
    outP.newId := newId;
    assert ! isundefined(line.memId)
      "MemoryUpdateNew: target undefined";
    SendReqPacket(i, outP);
  end;
end;


--------------------------------------------------------------------------------
-- level 3 routines


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine TailValidDoInvalid (pg.215)               -- level3 routine
procedure TailValidDoInvalid (i: ProcId;
                              inP: ResPacket;
                              firstEntry: Boolean; var exit: Boolean);
begin
  alias
    line: proc[i].cache.line[proc[i].exec.state.cTPtr];
  do
    if firstEntry then
      assert line.cState=CS_TV_back_IN
        "TailValidDoInvalid: cache line state not CS_TV_back_IN";

      -- start loop
      CacheReadSrc(i, SC_CREAD, CC_PREV_VTAIL, false, line.backId);
      exit := true;
      return;
    end;

    if CodeFault(i) then
      return;
    end;

    switch line.cState
    case CS_TO_INVALID:
      line.cState := CS_INVALID;
    case CS_TV_back_IN:
      if inP.cn then

        -- continue loop
        CacheReadSrc(i, SC_CREAD, CC_PREV_VTAIL, false, line.backId);
        exit := true;
        return;
      end;
      line.cState := CS_INVALID;
    else
      error "TailValidDoInvalid: cache line state not allowed";
    end;

    assert line.cState=CS_INVALID
      "TailValidDoInvalid: cache line state not CS_INVALID";

  end;  -- alias
end;


--------------------------------------------------------------------------------
-- level 2 routines


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine AttachLists (pg.210)                      -- level2 routine
procedure AttachLists (i: ProcId;
                       command: MemoryCacheCommands; size: Boolean;
                       inP: ResPacket;
                       firstEntry: Boolean; var exit: Boolean;
                       var aStatus: AttachStatus);
begin
  alias
    line:     proc[i].cache.line[proc[i].exec.state.cTPtr];
    oldState: proc[i].exec.state.toState.l2.cState;
  do
    undefine aStatus;  -- this record is not saved (and not really needed)

    if firstEntry then
      if command=CC_PEND_VALID & !size |
         command=CC_COPY_VALID & size  |
         command=CC_COPY_STALE & size  |
         command=CC_COPY_QOLB  & size then
        -- legal values
      else
        error "AttachLists: illegal cache command";
      end;
      
      switch line.cState
      case CS_QUEUED_JUNK, CS_QUEUED_CLEAN, CS_QUEUED_FRESH:
        -- legal values
      else
        error "AttachLists: illegal cache line state";
      end;

      oldState := line.cState;

      -- start loop
      CacheReadSrc(i, SC_CREAD, command, size, line.forwId);
      exit := true;
      return;
    end;  -- if firstEntry

    if CodeFault(i) then
      return;
    end;

    if line.cState != oldState then
      error "AttachLists: POP_WEAK not implemented";
    end;

    if inP.cn then

      -- continue loop
      CacheReadSrc(i, SC_CREAD, command, size, line.forwId);
      exit := true;
      return;
    end;

    aStatus.cState := inP.mCState;
    if ! isundefined(inP.backId) then
      aStatus.backId := inP.backId;
    end;

    switch CSTATE_TYPES(inP.mCState)
    case CT_ONLY_DIRTY, CT_ONLYP_DIRTY, CT_MID_VALID, CT_HEAD_CLEAN,
         CT_ONLY_FRESH, CT_HEAD_FRESH, CT_HEAD_DIRTY, CT_HEAD_WASH,
         CT_ONLY_USED:
      -- break

    case CT_HEAD_EXCL, CT_HEAD_USED, CT_TAIL_EXCL, CT_TAIL_USED,
         CT_HEAD_NEED:
      error "AttachLists: cache line state not implemented";

    case CT_HEAD_IDLE, CT_HEAD_RETN:
      if CSTATE_TYPES(inP.mCState)=CT_HEAD_IDLE &
         command=CC_COPY_QOLB & size then
        undefine oldState;
        return;
      end;
      line.forwId := inP.forwId;

      -- continue loop
      CacheReadSrc(i, SC_CREAD, command, size, line.forwId);
      exit := true;
      return;

    else
      error "AttachLists: cache line state not allowed";
    end;  -- switch CSTATE_TYPES(inP.mCState)

    undefine oldState;

  end;  -- alias
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine OnlyFreshDoInvalid (pg.213)               -- level2 routine
procedure OnlyFreshDoInvalid (i: ProcId;
                              inP: ResPacket;
                              firstEntry: Boolean; var exit: Boolean);
begin
  alias
    line: proc[i].cache.line[proc[i].exec.state.cTPtr];
    s:    proc[i].exec.state.toState.l2;
  do
    if firstEntry then
      assert line.cState=CS_OF_retn_IN
        "OnlyFreshDoInvalid: cache line state not CS_OF_retn_IN";

      -- start loop
      MemoryUpdateSrc(i, SC_MREAD, MC_LIST_TO_HOME, false);
      s.firstEntrySub := true;
      exit := true;
      return;
    end;  -- if firstEntry

    if s.firstEntrySub then
      if CodeFault(i) then
        return;
      end;
      s.cState := line.cState;
    end;

    switch s.cState
    case CS_TO_INVALID:
      if ! inP.cn then
        CodeSetTStat(i, TS_MTAG_STATE);
        return;
      end;
      line.cState := CS_INVALID;

    case CS_TV_back_IN:
      if s.firstEntrySub then
        if ! inP.cn then
          CodeSetTStat(i, TS_MTAG_STATE);
          return;
        end;
      end;
      TailValidDoInvalid(i, inP, s.firstEntrySub, exit);
      s.firstEntrySub := false;
      if exit then
        return;
      end;
      if CodeFault(i) then
        return;
      end;

    case CS_OF_retn_IN:
      if inP.cn then

        -- continue loop
        MemoryUpdateSrc(i, SC_MREAD, MC_LIST_TO_HOME, false);
        exit := true;
        return;
      end;
      if inP.mCState!=MS_FRESH then
        CodeSetTStat(i, TS_MTAG_STATE);
        return;
      end;
      line.cState := CS_INVALID;

    else
      error "OnlyFreshDoInvalid: cache line state not allowed";
    end;

    assert line.cState=CS_INVALID
      "OnlyFreshDoInvalid: cache line state not CS_INVALID";

    undefine s;

  end;  -- alias
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine OnlyDirtyDoReturn (pg.214)                -- level2 routine
procedure OnlyDirtyDoReturn (i: ProcId;
                             save: Boolean;
                             inP: ResPacket;
                             firstEntry: Boolean; var exit: Boolean);
begin
  alias
    line: proc[i].cache.line[proc[i].exec.state.cTPtr];
    s:    proc[i].exec.state.toState.l2;
  do
    if firstEntry then
      assert line.cState=CS_OD_RETN_IN
        "OnlyDirtyDoReturn: cache line state not CS_OD_RETN_IN";

      -- start first loop
      MemoryUpdateSrc(i, save ? SC_MWRITE64 : SC_MREAD, MC_LIST_TO_HOME, false);
      s.firstLoop := true;
      exit := true;
      return;
    end;

    if s.firstLoop then
      if CodeFault(i) then
        return;
      end;
      assert line.cState=CS_OD_RETN_IN
        "OnlyDirtyDoReturn: cache line state not CS_OD_RETN_IN";
      switch inP.mCState
      case MS_HOME, MS_FRESH:
        CodeSetTStat(i, TS_MTAG_STATE);
        return;
      else
        -- break
      end;
      if ! inP.cn then
        line.cState := CS_INVALID;
        undefine s;
        return;
      end;
      line.cState := CS_OD_spin_IN;

      -- start second loop
      MemoryUpdateSrc(i, SC_MREAD, MC_LIST_TO_GONE, false);
      s.firstLoop     := false;
      s.firstEntrySub := true;
      exit := true;
      return;
    end;  -- if s.firstLoop

    if s.firstEntrySub then
      if CodeFault(i) then
        return;
      end;
      s.cState := line.cState;
    end;

    switch s.cState
    case CS_TO_INVALID:
      line.cState := CS_INVALID;
      undefine s;
      return;

    case CS_TV_back_IN:
      TailValidDoInvalid(i, inP, s.firstEntrySub, exit);
      s.firstEntrySub := false;
      if exit then
        return;
      end;
      if CodeFault(i) then
        assert line.cState=CS_INVALID
          "OnlyDirtyDoReturn: cache line state not CS_INVALID";
      end;
      undefine s;
      return;

    case CS_OD_spin_IN:
      switch inP.mCState
      case MS_HOME, MS_FRESH:
        CodeSetTStat(i, TS_MTAG_STATE);
        return;
      else
        -- break
      end;
      if inP.cn then

        -- continue second loop
        MemoryUpdateSrc(i, SC_MREAD, MC_LIST_TO_GONE, false);
        exit := true;
        return;
      end;

    else    
      error "OnlyDirtyDoReturn: cache line state not allowed";
    
    end;  -- switch s.cState
    -- end of second loop

    line.cState := CS_OD_RETN_IN;

    -- continue first loop
    MemoryUpdateSrc(i, save ? SC_MWRITE64 : SC_MREAD, MC_LIST_TO_HOME, false);
    s.firstLoop := true;
    exit := true;
    return;

  end;  -- alias
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine HalfGoneDoInvalid (pg.218)                -- level2 routine
procedure HalfGoneDoInvalid (i: ProcId;
                             inP: ResPacket;
                             firstEntry: Boolean; var exit: Boolean);
begin
  alias
    line: proc[i].cache.line[proc[i].exec.state.cTPtr];
  do
    if firstEntry then
      assert POP_DIRTY "HalfGoneDoInvalid: POP_DIRTY false";
      assert line.cState=CS_HX_retn_IN
        "HalfGoneDoInvalid: cache line state not CS_HX_retn_IN";

      -- start loop
      MemoryUpdateNew(i, SC_MREAD, MC_PASS_HEAD, false, line.forwId);
      exit := true;
      return;
    end;

    if CodeFault(i) then
      return;
    end;

    switch line.cState
    case CS_TO_INVALID:
      line.cState := CS_INVALID;

    case CS_HX_retn_IN:
      if inP.cn then

        -- continue loop
        MemoryUpdateNew(i, SC_MREAD, MC_PASS_HEAD, false, line.forwId);
        exit := true;
        return;
      end;
      switch inP.mCState
      case MS_HOME, MS_FRESH:
        CodeSetTStat(i, TS_MTAG_STATE);
        return;
      else
        -- break
      end;
      line.cState := CS_INVALID;

    else
      error "HalfGoneDoInvalid: cache line state not allowed";
    end;  -- switch line.cState

    assert line.cState=CS_INVALID
      "HalfGoneDoInvalid: cache line state not CS_INVALID";

  end;  -- alias
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine ListValidDoInvalid (pg.212)               -- level2 routine
-- routine uses input parameter nextId in level2 state
procedure ListValidDoInvalid (i: ProcId;
                              inP: ResPacket;
                              firstEntry: Boolean; var exit: Boolean);
begin
  alias
    line: proc[i].cache.line[proc[i].exec.state.cTPtr];
    s:    proc[i].exec.state.toState.l2;
  do
    if firstEntry then
      switch line.cState
      case CS_HD_INVAL_OD, CS_HX_INVAL_OX, CS_QUEUED_HOME, CS_QUEUED_DIRTY,
           CS_QF_FLUSH_IN:
        -- break
      else
        error "ListValidDoInvalid: cache line state not allowed";
      end;

      -- start loop
      CacheReadSrc(i, SC_CREAD, CC_VALID_INVALID, false, s.nextId);
      exit := true;
      return;
    end;

    if CodeFault(i) then
      return;
    end;

    if inP.cn then

      -- continue
      CacheReadSrc(i, SC_CREAD, CC_VALID_INVALID, false, s.nextId);
      exit := true;
      return;
    end;

    switch CSTATE_TYPES(inP.mCState)
    case CT_MID_VALID:
      s.nextId := inP.forwId;

      -- continue
      CacheReadSrc(i, SC_CREAD, CC_VALID_INVALID, false, s.nextId);
      exit := true;
      return;
    else
      undefine s;
      return;
    end;

  end;  -- alias
end;


--------------------------------------------------------------------------------
-- level 1 routines


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine HeadFreshDoInvalid (pg.217)               -- level1 routine
procedure HeadFreshDoInvalid (i: ProcId;
                              inP: ResPacket;
                              firstEntry: Boolean; var exit: Boolean);
begin
  alias
    line: proc[i].cache.line[proc[i].exec.state.cTPtr];
    s:    proc[i].exec.state.toState.l1;
  do
    if firstEntry then
      assert POP_DIRTY "HeadFreshDoInvalid: POP_DIRTY false";
      assert line.cState=CS_HX_FORW_HX
        "HeadFreshDoInvalid: cache line state not CS_HX_FORW_HX";

      -- start first loop
      CacheReadSrc(i, SC_CREAD, CC_NEXT_FHEAD, false, line.forwId);
      s.firstLoop     := true;
      s.firstEntrySub := true;
      exit := true;
      return;
    end;  -- if firstEntry

    if s.firstLoop then

      if s.firstEntrySub then
        if CodeFault(i) then
          return;
        end;
        s.cState := line.cState;
      end;

      switch s.cState
      case CS_HX_FORW_OX:
        if s.firstEntrySub then
          line.cState := CS_OF_retn_IN;
        end;
        OnlyFreshDoInvalid(i, inP, s.firstEntrySub, exit);
        s.firstEntrySub := false;
        if exit then
          return;
        end;
        if CodeFault(i) then
          return;
        end;
        assert line.cState=CS_INVALID
          "HeadFreshDoInvalid: cache line state not CS_INVALID";
        undefine s;
        return;

      case CS_HX_FORW_HX:
        if inP.cn then

          -- continue first loop
          CacheReadSrc(i, SC_CREAD, CC_NEXT_FHEAD, false, line.forwId);
          exit := true;
          return;
        end;
        line.cState := CS_HX_retn_IN;

      else
        error "HeadFreshDoInvalid: cache line state not allowed";
      end;

      -- start second loop
      MemoryUpdateNew(i, SC_MREAD, MC_PASS_HEAD, false, line.forwId);
      s.firstLoop := false;
      exit := true;
      return;

    end;  -- if s.firstLoop then

    if CodeFault(i) then
      return;
    end;

    switch line.cState
    case CS_TO_INVALID:
      line.cState := CS_INVALID;
    case CS_HX_retn_IN:
      if inP.cn then

        -- continue second loop
        MemoryUpdateNew(i, SC_MREAD, MC_PASS_HEAD, false, line.forwId);
        exit := true;
        return;
      end;
      if inP.mCState!=MS_FRESH then
        CodeSetTStat(i, TS_MTAG_STATE);
        return;
      end;
      line.cState := CS_INVALID;
    else
      error "HeadFreshDoInvalid: cache line state not allowed";
    end;

    assert line.cState=CS_INVALID
      "HeadFreshDoInvalid: cache line state not CS_INVALID";

    undefine s;

  end;  -- alias
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine MidValidToInvalid (pg.219)                -- level1 routine
procedure MidValidToInvalid (i: ProcId;
                             inP: ResPacket;
                             firstEntry: Boolean; var exit: Boolean);
begin
  alias
    line: proc[i].cache.line[proc[i].exec.state.cTPtr];
    s:    proc[i].exec.state.toState.l1;
  do
    if firstEntry then
      switch line.cState
      case CS_MID_VALID:
        assert POP_DIRTY "MidValidToInvalid: POP_DIRTY false";
      case CS_MID_COPY:
        assert POP_CLEAN "MidValidToInvalid: POP_CLEAN false";
      else
        error "MidValidToInvalid: cache line state not allowed";
      end;
      line.cState := CS_MV_forw_MV;      

      -- start first loop
      CacheReadNew(i, SC_CREAD, CC_NEXT_VMID, false, line.forwId, line.backId);
      s.firstLoop     := true;
      s.firstEntrySub := true;
      exit := true;
      return;
    end;  -- if firstEntry

    if s.firstLoop then

      if s.firstEntrySub then
        if CodeFault(i) then
          return;
        end;
        s.cState := line.cState;
      end;

      switch s.cState
      case CS_TO_INVALID:
        line.cState := CS_INVALID;
        undefine s;
        return;

      case CS_TV_back_IN:
        TailValidDoInvalid(i, inP, s.firstEntrySub, exit);
        s.firstEntrySub := false;
        if exit then
          return;
        end;
        if CodeFault(i) then
          return;
        end;
        assert line.cState=CS_INVALID
          "MidValidToInvalid: cache line state not CS_INVALID";
        undefine s;
        return;

      case CS_MV_forw_MV:
        if inP.cn then

          -- continue first loop
          CacheReadNew(i, SC_CREAD, CC_NEXT_VMID, false, line.forwId, 
                       line.backId);
          exit := true;
          return;
        end;

      else
        error "MidValidToInvalid: cache line state not allowed";
      end;

      assert line.cState=CS_MV_forw_MV
        "MidValidToInvalid: cache line state not CS_MV_forw_MV";
      line.cState := CS_MV_back_IN;

      -- start second loop
      CacheReadNew(i, SC_CREAD, CC_PREV_VMID, false, line.backId, line.forwId);
      s.firstLoop := false;
      exit := true;
      return;

    end;  -- if s.firstLoop then

    if CodeFault(i) then
      return;
    end;

    switch line.cState
    case CS_TO_INVALID:
      -- break
    case CS_MV_back_IN:
      if inP.cn then

        -- continue second loop
        CacheReadNew(i, SC_CREAD, CC_PREV_VMID, false, line.backId,
                     line.forwId);
        exit := true;
        return;
      end;
    else
      error "MidValidToInvalid: cache line state not allowed";
    end;

    line.cState := CS_INVALID;

    undefine s;

  end;  -- alias
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine InvalidDoHeadDirty (pg.216)               -- level1 routine
-- routine returns dCodes in level0 state variable mState
procedure InvalidDoHeadDirty (i: ProcId;
                              command: MemoryCacheCommands; size: Boolean;
                              inP: ResPacket;
                              firstEntry: Boolean; var exit: Boolean);
var
  aStatus: AttachStatus;
  temp:    MemoryCacheStates;

begin
  alias
    line: proc[i].cache.line[proc[i].exec.state.cTPtr];
    s:    proc[i].exec.state.toState.l0;  -- routine uses level0 state !
  do
    if firstEntry then
      assert isundefined(s.secondEntry)&isundefined(s.mState)
        "InvalidDoHeadDirty: routine uses level0 state !";
      assert POP_DIRTY "InvalidDoHeadDirty: POP_DIRTY false";
      assert line.cState=CS_INVALID
        "InvalidDoHeadDirty: cache line state not CS_INVALID";
      line.cState := CS_PENDING;

      MemoryUpdateSrc(i, SC_MREAD, command, size);
      s.secondEntry := true;
      exit := true;
      return;
    end;  -- if firstEntry

    if s.secondEntry then
      if CodeFault(i) then
        return;
      end;
      s.mState := inP.mCState;  -- dCodes
      assert line.cState=CS_PENDING
        "InvalidDoHeadDirty: cache line state not CS_PENDING";
      if inP.cn then
        CodeSetTStat(i, TS_MTAG_STATE);
        return;
      end;
    end;

    switch s.mState
    case MS_HOME:
      temp := s.mState;
      undefine s;
      s.mState := temp;
      return;
    case MS_FRESH:
      line.forwId := inP.forwId;
      temp := s.mState;
      undefine s;
      s.mState := temp;
      return;
    case MS_GONE:
      if s.secondEntry then
        line.forwId := inP.forwId;
        line.cState := CS_QUEUED_JUNK;
      end;
      AttachLists(i, CC_COPY_VALID, true, inP, s.secondEntry, exit, aStatus);
      s.secondEntry := false;
      if exit then
        return;
      end;
      if CodeFault(i) then
        return;
      end;
      switch CSTATE_TYPES(aStatus.cState)
      case CT_ONLY_DIRTY, CT_ONLYP_DIRTY, CT_MID_VALID, CT_HEAD_DIRTY,
           CT_HEAD_EXCL, CT_TAIL_EXCL:
        line.cState := CI_HEAD_DIRTY;
      case CT_HEAD_WASH, CT_HEAD_CLEAN:
        line.cState := POP_CLEAN ? CI_HEAD_CLEAN : CI_HEAD_DIRTY;
      case CT_ONLY_USED, CT_HEAD_NEED, CT_HEAD_USED, CT_TAIL_USED:
        line.cState := POP_QOLB ? CI_HEADQ_DIRTY : CI_HEAD_DIRTY;
      else
        error "InvalidDoHeadDirty: attach status not allowed";
      end;
    else
      error
        "InvalidDoHeadDirty: memory line state not allowed or not implemented";
    end;

    switch line.cState
    case CI_HEAD_CLEAN, CI_HEAD_DIRTY, CI_HEADQ_DIRTY:
      -- allowed states
    else
      error "InvalidDoHeadDirty: cache line state not allowed";
    end;

    temp := s.mState;
    undefine s;
    s.mState := temp;

  end;  -- alias
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine OnlyDirtyDoInvalid (pg.214)               -- level1 routine
procedure OnlyDirtyDoInvalid (i: ProcId;
                              inP: ResPacket;
                              firstEntry: Boolean; var exit: Boolean);
begin
  alias
    line:   proc[i].cache.line[proc[i].exec.state.cTPtr];
  do
    if firstEntry then
      switch line.cState
      case CS_ONLY_DIRTY:
        assert !POP_PAIR "OnlyDirtyDoInvalid: POP_PAIR true";
      case CI_OD_FLUSH_IN, CS_HX_INVAL_OX, CS_HD_INVAL_OD, CS_QUEUED_DIRTY,
           CS_HX_FORW_OX:
        -- break
      case CS_ONLYP_DIRTY:
        assert POP_PAIR "OnlyDirtyDoInvalid: POP_PAIR false";
      case CS_ONLYQ_DIRTY, CS_ONLY_USED:
        assert POP_QOLB "OnlyDirtyDoInvalid: POP_QOLB false";
      case CS_OD_CLEAN_OC:
        assert POP_CLEANSE "OnlyDirtyDoInvalid: POP_CLEANSE false";
      else
        error "OnlyDirtyDoInvalid: cache line state not allowed";
      end;
      if POP_ROBUST then
        error "OnlyDirtyDoInvalid: POP_ROBUST not implemented";
      end;
      line.cState := CS_OD_RETN_IN;
    end;  -- if firstEntry

    OnlyDirtyDoReturn(i, true, inP, firstEntry, exit);
    if exit then
      return;
    end;
    if CodeFault(i) then
      return;
    end;

    assert line.cState=CS_INVALID
      "OnlyDirtyDoInvalid: cache line state not CS_INVALID";

  end;  -- alias
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine HeadGoneDoInvalid (pg.218)                -- level1 routine
procedure HeadGoneDoInvalid (i: ProcId;
                             aCmd: AccessCommandsReq;
                             cmd: MemoryCacheCommands; size: Boolean;
                             inP: ResPacket;
                             firstEntry: Boolean; var exit: Boolean);
begin
  alias
    line: proc[i].cache.line[proc[i].exec.state.cTPtr];
    s:    proc[i].exec.state.toState.l1;
  do
    if firstEntry then
      assert POP_DIRTY "HeadGoneDoInvalid: POP_DIRTY false";
      switch aCmd
      case SC_CREAD:
        if cmd=CC_NEXT_DHEAD & !size then
          -- break
        elsif cmd=CC_NEXT_VHEAD & !size |
              cmd=CC_NEXT_SHEAD & !size then
          assert POP_PAIR "HeadGoneDoInvalid: POP_PAIR false";
        elsif cmd=CC_NEXT_IHEAD & !size then
          assert POP_QOLB "HeadGoneDoInvalid: POP_QOLB false";
        else
          error 
            "HeadGoneDoInvalid: cache command not allowed or not implemented";
        end;
      else
        error
        "HeadGoneDoInvalid: transaction command not allowed or not implemented";
      end;
      assert line.cState=CS_HX_FORW_HX
        "HeadGoneDoInvalid: cache line state not CS_HX_FORW_HX";

      -- start loop
      if aCmd=SC_CREAD then
        CacheReadSrc(i, aCmd, cmd, size, line.forwId);
      else
        error "HeadGoneDoInvalid: transaction command not implemented";
      end;
      s.firstEntrySub := true;
      exit := true;
      return;
    end;  -- if firstEntry

    if s.firstEntrySub then
      if CodeFault(i) then
        return;
      end;
      s.cState := line.cState;
    end;

    switch s.cState
    case CS_HX_FORW_OX:
      OnlyDirtyDoInvalid(i, inP, s.firstEntrySub, exit);
      s.firstEntrySub := false;
      if exit then
        return;
      end;

    case CS_HX_FORW_HX:
      if s.firstEntrySub then
        if inP.cn then

          -- continue loop
          if aCmd=SC_CREAD then
            CacheReadSrc(i, aCmd, cmd, size, line.forwId);
          else
            error "HeadGoneDoInvalid: transaction command not implemented";
          end;
          exit := true;
          return;
        end;
        line.cState := CS_HX_retn_IN;
      end;
      HalfGoneDoInvalid(i, inP, s.firstEntrySub, exit);
      s.firstEntrySub := false;
      if exit then
        return;
      end;

    else
      error "HeadGoneDoInvalid: cache line state not allowed";
    end;

    if CodeFault(i) then
      return;
    end;

    assert line.cState=CS_INVALID
      "HeadGoneDoInvalid: cache line state not CS_INVALID";

    undefine s;

  end;  -- alias
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine ListFreshDoInvalid (pg.212)               -- level1 routine
procedure ListFreshDoInvalid (i: ProcId;
                              inP: ResPacket;
                              firstEntry: Boolean; var exit: Boolean);
begin
  alias
    line:   proc[i].cache.line[proc[i].exec.state.cTPtr];
    s:      proc[i].exec.state.toState.l1;
    nextId: proc[i].exec.state.toState.l2.nextId;  -- for ListValidDoInvalid
  do
    if firstEntry then
      switch line.cState
      case CS_QUEUED_DIRTY:
        -- break
      case CS_QUEUED_HOME:
        error "ListFreshDoInvalid: POP_WRITE not implemented";
      case CS_QF_FLUSH_IN:
        error "ListFreshDoInvalid: POP_FLUSH not implemented";
      else
        error "ListFreshDoInvalid: cache line state not allowed";
      end;

      -- start loop
      CacheReadSrc(i, SC_CREAD, CC_FRESH_INVALID, false, line.forwId);
      s.firstEntrySub := true;
      exit := true;
      return;
    end;  -- if firstEntry

    if s.firstEntrySub then
      if CodeFault(i) then
        return;
      end;
      if inP.cn then

        -- continue loop
        CacheReadSrc(i, SC_CREAD, CC_FRESH_INVALID, false, line.forwId);
        exit := true;
        return;
      end;

      s.cState := inP.mCState;
    end;

    switch CSTATE_TYPES(s.cState)
    case CT_ONLY_FRESH:
      -- break

    case CT_HEAD_FRESH:
      if s.firstEntrySub then
        line.forwId := inP.forwId;
        nextId := line.forwId;
      end;
      ListValidDoInvalid(i, inP, s.firstEntrySub, exit);
      s.firstEntrySub := false;
      if exit then
        return;
      end;
      if CodeFault(i) then
        return;
      end;

    case CT_HEAD_RETN:
      line.forwId := inP.forwId;

      -- continue
      CacheReadSrc(i, SC_CREAD, CC_FRESH_INVALID, false, line.forwId);
      exit := true;
      return;

    else
      error "ListFreshDoInvalid: cache line state not allowed";
    end;

    undefine s;

  end;  -- alias
end;


--------------------------------------------------------------------------------
-- level 0 routines


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine InvalidToHeadFresh (pg.220)               -- level0 routine
procedure InvalidToHeadFresh (i: ProcId;
                              inP: ResPacket;
                              firstEntry: Boolean; var exit: Boolean);
var
  aStatus: AttachStatus;

begin
  alias
    line: proc[i].cache.line[proc[i].exec.state.cTPtr];
    s:    proc[i].exec.state.toState.l0; 
  do
    if firstEntry then
      assert POP_FRESH "InvalidToHeadFresh: POP_FRESH false";
      assert line.cState=CS_INVALID
        "InvalidToHeadFresh: cache line state not CS_INVALID";
      line.cState := CS_PENDING;

      MemoryUpdateSrc(i, SC_MREAD, MC_CACHE_FRESH, true);
      s.secondEntry := true;
      exit := true;
      return;
    end;  -- if firstEntry

    if s.secondEntry then
      if CodeFault(i) then
        return;
      end;
      assert line.cState=CS_PENDING
        "InvalidToHeadFresh: cache line state not CS_PENDING";
      if inP.cn then
        CodeSetTStat(i, TS_MTAG_STATE);
        return;
      end;

      s.mState := inP.mCState;
      s.co     := inP.co;
    end;
    
    switch s.mState
    case MS_HOME:
      line.cState := inP.co ? CS_ONLY_FRESH : CS_ONLY_DIRTY;
        -- no pairwise sharing

    case MS_FRESH:
      line.forwId := inP.forwId;
      line.cState := CI_QUEUED_FRESH;

    case MS_GONE:
      if s.secondEntry then
        line.cState := CS_QUEUED_JUNK;
        line.forwId := inP.forwId;
      end;

      AttachLists(i, CC_COPY_VALID, true, inP, s.secondEntry, exit, aStatus);
      s.secondEntry := false;
      if exit then
        return;
      end;
      if CodeFault(i) then
        return;
      end;

      switch CSTATE_TYPES(aStatus.cState)
      case CT_HEAD_DIRTY:
        line.cState := CI_HEAD_DIRTY;
          -- POP_WASH false
      case CT_HEAD_CLEAN:
        if ! s.co then
          CodeSetTStat(i, TS_MTAG_STATE);
          return;
        end;
        if POP_WASH then
          error "InvalidToHeadFresh: POP_WASH not implemented";
        else
          line.cState := POP_CLEAN ? CI_HEAD_CLEAN : CI_HEAD_DIRTY;
        end;
      case CT_ONLY_DIRTY, CT_ONLYP_DIRTY, CT_MID_VALID, CT_HEAD_EXCL,
           CT_TAIL_EXCL:
        line.cState := CI_HEAD_DIRTY;
      case CT_HEAD_WASH:
        if ! s.co then
          CodeSetTStat(i, TS_MTAG_STATE);
          return;
        end;
        line.backId := aStatus.backId;
        if POP_WASH then
          error "InvalidToHeadFresh: POP_WASH not implemented";
        else
          line.cState := POP_CLEAN ? CI_HEAD_CLEAN : CI_HEAD_DIRTY;
        end;
      case CT_ONLY_FRESH, CT_HEAD_FRESH:
        if s.mState!=MS_FRESH then
          CodeSetTStat(i, TS_MTAG_STATE);
          return;
        end;
        line.cState := CS_HEAD_FRESH;
      case CT_ONLY_USED, CT_HEAD_NEED, CT_HEAD_USED, CT_TAIL_USED:
        line.cState := POP_QOLB ? CI_HEADQ_DIRTY : CI_HEAD_DIRTY;
      else
        error "InvalidToHeadFresh: attach status not allowed";
      end;  -- switch CSTATE_TYPES(aStatus.cState)

    else
      error
        "InvalidToHeadFresh: memory state not allowed or not yet implemented";
    end;  -- switch s.mState

    switch line.cState
    case CS_ONLY_FRESH, CS_ONLY_DIRTY, CS_ONLYP_DIRTY, CI_QUEUED_FRESH,
         CI_HD_WASH_HF, CI_HW_WASH_HF, CI_HEAD_CLEAN, CI_HEAD_DIRTY,
         CI_HEADQ_DIRTY, CS_HEAD_WASH, CS_HEAD_FRESH:
      -- break
    else
      error "InvalidToHeadFresh: cache line state not allowed";
    end;

    undefine s;

  end;  -- alias
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine QueuedFreshToHeadFresh (pg.221)           -- level0 routine
procedure QueuedFreshToHeadFresh (i: ProcId;
                                  inP: ResPacket;
                                  firstEntry: Boolean; var exit: Boolean);
var
  aStatus: AttachStatus;

begin
  alias
    line: proc[i].cache.line[proc[i].exec.state.cTPtr];
  do
    if firstEntry then
      assert line.cState=CI_QUEUED_FRESH
        "QueuedFreshToHeadFresh: cache line state not allowed";
      line.cState := CS_QUEUED_FRESH;
    end;
    AttachLists(i, CC_PEND_VALID, false, inP, firstEntry, exit, aStatus);
    if exit then
      return;
    end;
    if CodeFault(i) then
      return;
    end;

    switch CSTATE_TYPES(aStatus.cState)
    case CT_ONLY_FRESH, CT_HEAD_FRESH:
      -- break
    else
      CodeSetTStat(i, TS_CTAG_STATE);
      return;
    end;
    line.cState := CS_HEAD_FRESH;
  
  end;  -- alias
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine OnlyFreshToInvalid (pg.213)               -- level0 routine
procedure OnlyFreshToInvalid (i: ProcId;
                              inP: ResPacket;
                              firstEntry: Boolean; var exit: Boolean);
begin
  alias
    line: proc[i].cache.line[proc[i].exec.state.cTPtr];
  do
    if firstEntry then
      assert line.cState=CS_ONLY_FRESH | line.cState=CI_OF_FLUSH_IN
        "OnlyFreshToInvalid: cache line state not allowed";
      line.cState := CS_OF_retn_IN;
    end;
    OnlyFreshDoInvalid(i, inP, firstEntry, exit);
    if exit then
      return;
    end;
    if CodeFault(i) then
      return;
    end;
    assert line.cState=CS_INVALID
      "OnlyFreshToInvalid: cache line state not CS_INVALID";
  end;  -- alias
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine HeadFreshToInvalid (pg.217)               -- level0 routine
procedure HeadFreshToInvalid (i: ProcId;
                              inP: ResPacket;
                              firstEntry: Boolean; var exit: Boolean);
begin
  alias
    line: proc[i].cache.line[proc[i].exec.state.cTPtr];
  do
    if firstEntry then
      assert POP_DIRTY "HeadFreshToInvalid: POP_DIRTY false";
      assert line.cState=CS_HEAD_FRESH
        "HeadFreshToInvalid: cache line state not CS_HEAD_FRESH";
      line.cState := CS_HX_FORW_HX;
    end;
    HeadFreshDoInvalid(i, inP, firstEntry, exit);
    if exit then
      return;
    end;
    if CodeFault(i) then
      return;
    end;
    assert line.cState=CS_INVALID
      "HeadFreshToInvalid: cache line state not CS_INVALID";
  end;  -- alias
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine TailValidToInvalid (pg.215)               -- level0 routine
procedure TailValidToInvalid (i: ProcId;
                              inP: ResPacket;
                              firstEntry: Boolean; var exit: Boolean);
begin
  alias
    line: proc[i].cache.line[proc[i].exec.state.cTPtr];
  do
    if firstEntry then
      switch line.cState
      case CS_TAIL_VALID:
        -- break
      case CS_TAIL_COPY:
        assert POP_CLEAN "TailValidToInvalid: POP_CLEAN false";
      else
        error "TailValidToInvalid: cache line state not allowed";
      end;
      line.cState := CS_TV_back_IN;
    end;

    TailValidDoInvalid(i, inP, firstEntry, exit);

  end;  -- alias
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine InvalidToHeadDirty (pg.215)               -- level0 routine
procedure InvalidToHeadDirty (i: ProcId;
                              inP: ResPacket;
                              firstEntry: Boolean; var exit: Boolean);
begin
  alias
    line:   proc[i].cache.line[proc[i].exec.state.cTPtr];
    mState: proc[i].exec.state.toState.l0.mState;  -- dCodes
  do
    if firstEntry then
      assert POP_DIRTY "InvalidToHeadDirty: POP_DIRTY false";
      assert line.cState=CS_INVALID
        "InvalidToHeadDirty: cache line state not CS_INVALID";
    end;

    InvalidDoHeadDirty(i, MC_CACHE_CLEAN, true, inP, firstEntry, exit);
    if exit then
      return;
    end;
    if CodeFault(i) then
      return;
    end;

    switch mState
    case MS_HOME:
      assert !POP_CLEAN "InvalidToHeadDirty: POP_CLEAN true";
      line.cState := CI_ONLY_EXCL;
    case MS_FRESH:
      line.cState := CI_QUEUED_CLEAN;
    case MS_GONE:
      -- break
    else
      error
        "InvalidToHeadDirty: memory line state not allowed or not implemented";
    end;

    switch line.cState
    case CI_ONLY_CLEAN, CI_ONLY_EXCL, CI_HEAD_CLEAN, CI_HEAD_DIRTY,
         CI_HEADQ_DIRTY, CI_QUEUED_CLEAN:
      -- allowed values
    else
      error "InvalidToHeadDirty: cache line state not allowed";
    end;

    undefine mState;

  end;  -- alias
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine OnlyDirtyToInvalid (pg.213)               -- level0 routine
procedure OnlyDirtyToInvalid (i: ProcId;
                              inP: ResPacket;
                              firstEntry: Boolean; var exit: Boolean);
begin
  alias
    line:   proc[i].cache.line[proc[i].exec.state.cTPtr];
  do
    if firstEntry then
      switch line.cState
      case CS_ONLY_DIRTY:
        assert !POP_PAIR "OnlyDirtyToInvalid: POP_PAIR true";
      case CI_OD_FLUSH_IN:
        -- break
      case CS_ONLYP_DIRTY:
        assert POP_PAIR "OnlyDirtyToInvalid: POP_PAIR false";
      case CS_ONLYQ_DIRTY, CS_ONLY_USED:
        assert POP_QOLB "OnlyDirtyToInvalid: POP_QOLB false";
      else
        error "OnlyDirtyToInvalid: cache line state not allowed";
      end;
    end;
    OnlyDirtyDoInvalid(i, inP, firstEntry, exit);
    if exit then
      return;
    end;
    if CodeFault(i) then
      return;
    end;
    assert line.cState=CS_INVALID
      "OnlyDirtyToInvalid: cache line state not CS_INVALID";
  end;
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine HeadDirtyToInvalid (pg.218)               -- level0 routine
procedure HeadDirtyToInvalid (i: ProcId;
                              inP: ResPacket;
                              firstEntry: Boolean; var exit: Boolean);
begin
  alias
    line:   proc[i].cache.line[proc[i].exec.state.cTPtr];
  do
    if firstEntry then
      assert POP_DIRTY "HeadDirtyToInvalid: POP_DIRTY false";
      assert line.cState=CS_HEAD_DIRTY
        "HeadDirtyToInvalid: cache line state not CS_HEAD_DIRTY";
      line.cState := CS_HX_FORW_HX;
    end;
    HeadGoneDoInvalid(i, SC_CREAD, CC_NEXT_DHEAD, false, inP, firstEntry, exit);
    if exit then
      return;
    end;
    if CodeFault(i) then
      return;
    end;
    assert line.cState=CS_INVALID
      "HeadDirtyToInvalid: cache line state not CS_INVALID";
  end;
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine QueuedCleanToHeadDirty (pg.216)           -- level0 routine
procedure QueuedCleanToHeadDirty (i: ProcId;
                                  inP: ResPacket;
                                  firstEntry: Boolean; var exit: Boolean);
var
  aStatus: AttachStatus;

begin
  alias
    line:   proc[i].cache.line[proc[i].exec.state.cTPtr];
  do
    if firstEntry then
      assert POP_DIRTY "QueuedCleanToHeadDirty: POP_DIRTY false";
      assert line.cState=CI_QUEUED_CLEAN
        "QueuedCleanToHeadDirty: cache line state not CI_QUEUED_CLEAN";
      line.cState := CS_QUEUED_CLEAN;
    end;

    AttachLists(i, CC_PEND_VALID, false, inP, firstEntry, exit, aStatus);
    if exit then
      return;
    end;
    if CodeFault(i) then
      return;
    end;
    switch CSTATE_TYPES(aStatus.cState)
    case CT_ONLY_FRESH, CT_HEAD_FRESH:
      -- break
    else
      CodeSetTStat(i, TS_CTAG_STATE);
      return;
    end;

    switch line.cState
    case CS_QUEUED_CLEAN:
      line.cState := POP_CLEAN ? CS_HEAD_CLEAN : CS_HEAD_DIRTY;
    case CS_QUEUED_MODS:
      error "QueuedCleanToHeadDirty: POP_WEAK not implemented";
    else
      error "QueuedCleanToHeadDirty: cache line state not allowed";
    end;

    switch line.cState
    case CS_ONLYP_DIRTY:
      assert POP_PAIR "QueuedCleanToHeadDirty: POP_PAIR false";
    case CS_ONLY_DIRTY, CS_HEAD_CLEAN, CS_HEAD_DIRTY:
      -- allowed values
    else
      error "QueuedCleanToHeadDirty: cache line state not allowed";
    end;

  end;  -- alias
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine OnlyFreshToOnlyDirty (pg.221)             -- level0 routine
procedure OnlyFreshToOnlyDirty (i: ProcId;
                                inP: ResPacket;
                                firstEntry: Boolean; var exit: Boolean);
begin
  alias
    line: proc[i].cache.line[proc[i].exec.state.cTPtr];
    s:    proc[i].exec.state.toState.l0;
    cn:   proc[i].exec.state.toState.l0.co;  -- cn bit has to be saved
  do
    if firstEntry then
      assert POP_MODS "OnlyFreshToOnlyDirty: POP_MODS false";
      assert line.cState=CS_ONLY_FRESH
        "OnlyFreshToOnlyDirty: cache line state not CS_ONLY_FRESH";
      line.cState := CS_OF_MODS_OD;

      MemoryUpdateSrc(i, SC_MREAD, MC_LIST_TO_GONE, false);
      s.secondEntry := true;
      exit := true;
      return;
    end;  -- if firstEntry

    if s.secondEntry then
      if CodeFault(i) then
        return;
      end;
      assert line.cState=CS_OF_MODS_OD
        "OnlyFreshToOnlyDirty: cache line state not CS_OF_MODS_OD";

      cn := inP.cn;
    end;

    if cn then
      if s.secondEntry then
        line.cState := CS_OF_retn_IN;
      end;
      OnlyFreshDoInvalid(i, inP, s.secondEntry, exit);
      s.secondEntry := false;
      if exit then
        return;
      end;
      if CodeFault(i) then
        return;
      end;
      assert line.cState=CS_INVALID
        "OnlyFreshToOnlyDirty: cache line state not CS_INVALID";
      undefine s;
      return;
    end;

    if inP.mCState != MS_FRESH then
      CodeSetTStat(i, TS_MTAG_STATE);
      return;
    end;

    line.cState := POP_CLEAN ? CI_ONLY_CLEAN : CI_ONLY_EXCL;

    undefine s;

  end;  -- alias
end;         


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine HeadFreshToHeadDirty (pg.221)             -- level0 routine
procedure HeadFreshToHeadDirty (i: ProcId;
                                inP: ResPacket;
                                firstEntry: Boolean; var exit: Boolean);
begin
  alias
    line:   proc[i].cache.line[proc[i].exec.state.cTPtr];
    s:      proc[i].exec.state.toState.l0;
    cState: proc[i].exec.state.toState.l0.mState;  -- cState has to be saved
    flag:   proc[i].exec.state.toState.l0.co;
  do
    if firstEntry then
      assert POP_MODS "HeadFreshToHeadDirty: POP_MODS false";
      assert line.cState=CS_HEAD_FRESH
        "HeadFreshToHeadDirty: cache line state not CS_HEAD_FRESH";
      line.cState := CS_HF_MODS_HD;

      MemoryUpdateSrc(i, SC_MREAD, MC_LIST_TO_GONE, false);
      s.secondEntry := true;
      exit := true;
      return;
    end;  -- if firstEntry

    if s.secondEntry then
      if CodeFault(i) then
        return;
      end;

      cState := line.cState;
    end;

    switch cState
    case CS_HF_MODS_HD:
      if s.secondEntry then
        if inP.cn then
          line.cState := CS_HX_FORW_HX;
          flag := true;
        else
          line.cState := CI_HEAD_MODS;
          flag := false;
        end;
      end;
      if flag then
        HeadFreshDoInvalid(i, inP, s.secondEntry, exit);
        s.secondEntry := false;
        if exit then
          return;
        end;
        if CodeFault(i) then
          return;
        end;
      end;

    case CS_OF_MODS_OD:
      if s.secondEntry then
        if inP.cn then
          line.cState := CS_OF_retn_IN;
          flag := true;
        else
          line.cState := POP_CLEAN ? CI_ONLY_CLEAN : CI_ONLY_EXCL;
          flag := false;
        end;
      end;
      if flag then
        OnlyFreshDoInvalid(i, inP, s.secondEntry, exit);
        s.secondEntry := false;
        if exit then
          return;
        end;
        if CodeFault(i) then
          return;
        end;
      end;

    else
      error "HeadFreshToHeadDirty: cache line state not allowed";
    end;  -- switch cState

    switch line.cState
    case CI_HEAD_MODS, CI_ONLY_EXCL, CI_ONLY_CLEAN, CS_INVALID:
      -- allowed states
    else
      error "HeadFreshToHeadDirty: cache line state not allowed";
    end;

    undefine s;

  end;  -- alias
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine HeadDirtyToOnlyDirty (pg.213)             -- level0 routine
procedure HeadDirtyToOnlyDirty (i: ProcId;
                                inP: ResPacket;
                                firstEntry: Boolean; var exit: Boolean);
begin
  alias
    line:   proc[i].cache.line[proc[i].exec.state.cTPtr];
    nextId: proc[i].exec.state.toState.l2.nextId;  -- for ListValidDoInvalid
  do
    if firstEntry then
      assert line.cState=CI_HEAD_MODS
        "HeadDirtyToOnlyDirty: cache line state not CI_HEAD_MODS";
      line.cState := CS_HD_INVAL_OD;
      nextId := line.forwId;
    end;
    ListValidDoInvalid(i, inP, firstEntry, exit);
    if exit then
      return;
    end;
    assert line.cState=CS_HD_INVAL_OD
      "HeadDirtyToOnlyDirty: cache line state not CS_HD_INVAL_OD";
    if CodeFault(i) then
      return;
    end;
    line.cState := CS_ONLY_DIRTY;  -- no pairwise sharing

    undefine nextId;

  end;  -- alias
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine InvalidToOnlyDirty (pg.210)               -- level0 routine
procedure InvalidToOnlyDirty (i: ProcId;
                              inP: ResPacket;
                              firstEntry: Boolean; var exit: Boolean);
var
  aStatus: AttachStatus;

begin
  alias
    line: proc[i].cache.line[proc[i].exec.state.cTPtr];
    s:    proc[i].exec.state.toState.l0;
  do
    if firstEntry then
      assert line.cState=CS_INVALID
        "InvalidToOnlyDirty: cache line state not CS_INVALID";
      line.cState := CS_PENDING;

      MemoryUpdateSrc(i, SC_MREAD, MC_CACHE_DIRTY, true);
      s.secondEntry := true;
      exit := true;
      return;
    end;  -- if firstEntry

    if s.secondEntry then
      if CodeFault(i) then
        return;
      end;
      assert line.cState=CS_PENDING
        "InvalidToOnlyDirty: cache line state not CS_PENDING";
      if inP.cn then
        CodeSetTStat(i, TS_MTAG_STATE);
        return;
      end;

      s.mState := inP.mCState;
    end;

    switch s.mState
    case MS_HOME:
      line.cState := CS_ONLY_DIRTY;  -- no pairwise sharing

    case MS_FRESH:
      line.forwId := inP.forwId;
      line.cState := CI_QUEUED_DIRTY;

    case MS_GONE:
      if s.secondEntry then
        line.forwId := inP.forwId;
        line.cState := CS_QUEUED_JUNK;
      end;

      AttachLists(i, CC_COPY_VALID, true, inP, s.secondEntry, exit, aStatus);
      s.secondEntry := false;
      if exit then
        return;
      end;
      if CodeFault(i) then
        return;
      end;

      switch CSTATE_TYPES(aStatus.cState)
      case CT_ONLY_DIRTY, CT_ONLYP_DIRTY, CT_MID_VALID, CT_HEAD_DIRTY,
           CT_HEAD_EXCL, CT_TAIL_EXCL, CT_HEAD_WASH, CT_HEAD_CLEAN:
        line.cState := CI_HEAD_MODS;
      case CT_ONLY_USED, CT_HEAD_NEED, CT_HEAD_USED, CT_TAIL_USED:
        line.cState := POP_QOLB ? CI_HEADQ_DIRTY : CI_HEAD_MODS;
      else
        error "InvalidToOnlyDirty: attach status not allowed";
      end;

    else
      error
        "InvalidToOnlyDirty: memory state not allowed or not yet implemented";
    end;  -- switch s.mState

    switch line.cState
    case CS_ONLY_DIRTY, CS_ONLYP_DIRTY, CI_QUEUED_DIRTY, CI_HEAD_MODS,
         CI_HEADQ_DIRTY:
      -- break
    else
      error "InvalidToOnlyDirty: cache line state not allowed";
    end;

    undefine s;

  end;  -- alias
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine QueuedDirtyToOnlyDirty (pg.211)           -- level0 routine
procedure QueuedDirtyToOnlyDirty (i: ProcId;
                                  inP: ResPacket;
                                  firstEntry: Boolean; var exit: Boolean);
begin
  alias
    line: proc[i].cache.line[proc[i].exec.state.cTPtr];
  do
    if firstEntry then
      assert line.cState=CI_QUEUED_DIRTY
        "QueuedDirtyToOnlyDirty: cache line state not CI_QUEUED_DIRTY";
      line.cState := CS_QUEUED_DIRTY;
    end;
    ListFreshDoInvalid(i, inP, firstEntry, exit);
    if exit then
      return;
    end;
    assert line.cState=CS_QUEUED_DIRTY
      "QueuedDirtyToOnlyDirty: cache line state not CS_QUEUED_DIRTY";
    if ! CodeFault(i) then
      line.cState := CS_ONLY_DIRTY;  -- no pairwise sharing
    end;
  end;  -- alias
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine QueuedDirtyToFlushed (pg.213)             -- level0 routine
procedure QueuedDirtyToFlushed (i: ProcId;
                                inP: ResPacket;
                                firstEntry: Boolean; var exit: Boolean);
begin
  alias
    line:          proc[i].cache.line[proc[i].exec.state.cTPtr];
    s:             proc[i].exec.state.toState.l0;
    firstDo:       proc[i].exec.state.toState.l0.secondEntry;
    firstEntrySub: proc[i].exec.state.toState.l0.co;
  do
    if firstEntry then
      assert line.cState=CI_QD_FLUSH_IN
        "QueuedDirtyToFlushed: cache line state not CI_QD_FLUSH_IN";
      line.cState := CS_QUEUED_DIRTY;
      firstDo := true;
    end;

    if firstDo then
      ListFreshDoInvalid(i, inP, firstEntry, exit);
      if exit then
        return;
      end;
      assert line.cState=CS_QUEUED_DIRTY
        "QueuedDirtyToFlushed: cache line state not CS_QUEUED_DIRTY";
      if CodeFault(i) then
        return;
      end;
      firstDo       := false;
      firstEntrySub := true;
    end;

    OnlyDirtyDoInvalid(i, inP, firstEntrySub, exit);
    firstEntrySub := false;
    if exit then
      return;
    end;
    if CodeFault(i) then
      return;
    end;
    assert line.cState=CS_INVALID
      "QueuedDirtyToFlushed: cache line state not CS_INVALID";

    undefine s;

  end;  -- alias
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine HeadDirtyToFlushed (pg.219)               -- level0 routine
procedure HeadDirtyToFlushed (i: ProcId;
                              inP: ResPacket;
                              firstEntry: Boolean; var exit: Boolean);
begin
  alias
    line:          proc[i].cache.line[proc[i].exec.state.cTPtr];
    s:             proc[i].exec.state.toState.l0;
    firstDo:       proc[i].exec.state.toState.l0.secondEntry;
    firstEntrySub: proc[i].exec.state.toState.l0.co;
    nextId:        proc[i].exec.state.toState.l2.nextId; 
                     -- for ListValidDoInvalid
  do
    if firstEntry then
      assert POP_DIRTY "HeadDirtyToFlushed: POP_DIRTY false";
      assert line.cState=CI_HD_FLUSH_IN
        "HeadDirtyToFlushed: cache line state not CI_HD_FLUSH_IN";
      line.cState := CS_HX_INVAL_OX;
      firstDo := true;
      nextId  := line.forwId;
    end;

    if firstDo then
      ListValidDoInvalid(i, inP, firstEntry, exit);
      if exit then
        return;
      end;
      assert line.cState=CS_HX_INVAL_OX
        "HeadDirtyToFlushed: cache line state not CS_HX_INVAL_OX";
      if CodeFault(i) then
        return;
      end;
      firstDo       := false;
      firstEntrySub := true;
    end;

    OnlyDirtyDoInvalid(i, inP, firstEntrySub, exit);
    firstEntrySub := false;
    if exit then
      return;
    end;
    if CodeFault(i) then
      return;
    end;
    assert line.cState=CS_INVALID
      "HeadDirtyToFlushed: cache line state not CS_INVALID";

    undefine s;
    undefine nextId;

  end;  -- alias
end;


--------------------------------------------------------------------------------
-- load instruction


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine TypicalRolloutEntry (pg.185)              -- levelC routine
procedure TypicalRolloutEntry (i: ProcId;
                               inP: ResPacket;
                               firstEntry: Boolean; var exit: Boolean);
begin
  alias
    line: proc[i].cache.line[proc[i].exec.state.cTPtr];
    s:    proc[i].exec.state.subState.lC;
  do
    if firstEntry then
      s.cState := line.cState;
    end;

    switch s.cState
    case CS_ONLY_DIRTY:
      OnlyDirtyToInvalid(i, inP, firstEntry, exit);
    case CS_ONLY_FRESH:
      OnlyFreshToInvalid(i, inP, firstEntry, exit);
    case CS_HEAD_DIRTY:
      HeadDirtyToInvalid(i, inP, firstEntry, exit);
    case CS_HEAD_FRESH:
      HeadFreshToInvalid(i, inP, firstEntry, exit);
    case CS_MID_VALID:
      MidValidToInvalid(i, inP, firstEntry, exit);
    case CS_TAIL_VALID:
      TailValidToInvalid(i, inP, firstEntry, exit);
    else
      error
        "TypicalRolloutEntry: cache line state not allowed or not implemented";
    end;
    if exit then
      return;
    end;

    undefine s;

  end;  -- alias
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine TypicalFindLine (pg.184)                  -- levelB routine
procedure TypicalFindLine (i: ProcId; flag: FindFlags;
                           inP: ResPacket;
                           memId: MemoryId; addrOffset: MemoryLineId;
                           firstEntry: Boolean; var exit: Boolean);
begin
  alias
    cache:     proc[i].cache;
    cTPtr:     proc[i].exec.state.cTPtr;
    s:         proc[i].exec.state.subState.lB;
    cacheMiss: proc[i].exec.state.subState.lB.firstTo;
  do
    if firstEntry then
      cTPtr := CacheCheckSample(i, addrOffset, memId, cTPtr);
     
      -- lock not implemented
 
      if cache.line[cTPtr].cState=CS_INVALID & flag=FF_FIND then
        undefine s;
        return;
      end;

      s.cState  := cache.line[cTPtr].cState;
      cacheMiss := isundefined(cache.line[cTPtr].memId) |
                   ! (cache.line[cTPtr].memId = memId &
                      cache.line[cTPtr].addrOffset = addrOffset);
      s.memId      := memId;
      s.addrOffset := addrOffset;
    end;  -- if firstEntry

    if cacheMiss then
      if s.cState != CS_INVALID then
        TypicalRolloutEntry(i, inP, firstEntry, exit);
        if exit then
          return;
        end;
        if CodeFault(i) then
          return;
        end;
      end;
      cache.line[cTPtr].memId      := s.memId;
      cache.line[cTPtr].addrOffset := s.addrOffset;
    end;

    undefine s;

  end;  -- alias
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine TypicalLoadSetup (pg.183)                 -- levelB routine
procedure TypicalLoadSetup (i: ProcId;
                           inP: ResPacket;
                           firstEntry: Boolean; var exit: Boolean);
begin
  alias
    line: proc[i].cache.line[proc[i].exec.state.cTPtr];
  do
    if firstEntry then
      switch line.cState
      case CS_LOCAL_CLEAN, CS_LOCAL_DIRTY:
        error "TypicalLoadSetup: LOCAL cache line states not implemented";
      case CS_INVALID:
        -- break
      else
        if cacheUsage = CU_LOCAL then
          error "TypicalLoadSetup: only CU_GLOBAL cache usage implemented";
        end;
        switch line.cState
        case CS_ONLY_DIRTY, CS_ONLY_FRESH, CS_HEAD_DIRTY, CS_HEAD_FRESH,
             CS_MID_VALID, CS_TAIL_VALID:
          -- break
        else
          error "TypicalLoadSetup: cache line state not supported";
        end;
      end;
      if line.cState!=CS_INVALID | CodeFault(i) then
        return;
      end;
    end;  -- if firstEntry

    if cacheUsage = CU_LOCAL then
      error "TypicalLoadSetup: only CU_GLOBAL cache usage implemented";
    else
      switch proc[i].exec.state.fetchOption
      case CO_FETCH:
        if POP_FRESH then  -- POP_FRESH check added
          InvalidToHeadFresh(i, inP, firstEntry, exit);
        else
          InvalidToOnlyDirty(i, inP, firstEntry, exit);
        end;
      case CO_LOAD:
        if POP_DIRTY then  -- POP_DIRTY check added
          InvalidToHeadDirty(i, inP, firstEntry, exit);
        else
          InvalidToOnlyDirty(i, inP, firstEntry, exit);
        end;
      case CO_STORE:
        InvalidToOnlyDirty(i, inP, firstEntry, exit);
      else
        error "TypicalLoadSetup: fetch option not yet implemented";
      end;
    end;

    -- no "if exit then ..." needed (return in any case)

  end;  -- alias
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine TypicalLoad (pg.184)                      -- levelB routine
procedure TypicalLoad (i: ProcId);
begin
  alias
    line: proc[i].cache.line[proc[i].exec.state.cTPtr];
  do
    switch line.cState
    case CS_LOCAL_CLEAN, CS_LOCAL_DIRTY:
      error "TypicalLoad: LOCAL cache line states not implemented";
    else
      -- break
    end;

    switch line.cState
    case CI_READ_FRESH:
      line.cState := CS_INVALID;
    case CI_HEAD_DIRTY:
      line.cState := CS_HEAD_DIRTY;
    case CI_ONLY_EXCL:              -- added
      line.cState := CS_ONLY_DIRTY;
        -- no pairwise sharing
    case CS_ONLY_DIRTY, CS_HEAD_DIRTY, CS_ONLY_FRESH, CS_HEAD_FRESH,
         CS_MID_VALID, CS_TAIL_VALID, CI_QUEUED_DIRTY, CI_QUEUED_CLEAN,
         CI_QUEUED_FRESH, CI_HEAD_MODS, CS_LOCAL_CLEAN, CS_LOCAL_DIRTY:
      -- break
    else
      error "TypicalLoad: cache line state not allowed";
    end;

  end;  -- alias
end;    
     

-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine TypicalCleanup (pg.184)                   -- levelB routine
procedure TypicalCleanup (i: ProcId;
                          inP: ResPacket;
                          firstEntry: Boolean; var exit: Boolean);
begin
  alias
    line: proc[i].cache.line[proc[i].exec.state.cTPtr];
    s:    proc[i].exec.state.subState.lB;
  do
    if firstEntry then
      switch line.cState
      case CS_INVALID:
        -- break
      case CI_LD_FLUSH_IN, CS_LOCAL_CLEAN, CS_LOCAL_DIRTY:
        error "TypicalCleanup: LOCAL cache line states not implemented";
      else
        assert cacheUsage=CU_GLOBAL
          "TypicalCleanup: only CU_GLOBAL cache usage implemented";
      end;

      s.cState := line.cState;
    end;  -- if firstEntry

    switch s.cState   -- saved state has to be used for re-entry !
    case CI_HEAD_MODS:
      HeadDirtyToOnlyDirty(i, inP, firstEntry, exit);
    case CI_QUEUED_DIRTY:
      QueuedDirtyToOnlyDirty(i, inP, firstEntry, exit);
    case CI_QUEUED_CLEAN:
      QueuedCleanToHeadDirty(i, inP, firstEntry, exit);
    case CI_QUEUED_FRESH:
      QueuedFreshToHeadFresh(i, inP, firstEntry, exit);
    case CI_QD_FLUSH_IN:
      QueuedDirtyToFlushed(i, inP, firstEntry, exit);
    case CI_OD_FLUSH_IN:
      OnlyDirtyToInvalid(i, inP, firstEntry, exit);
    case CI_OF_FLUSH_IN:                             -- added
      OnlyFreshToInvalid(i, inP, firstEntry, exit);
    case CI_HD_FLUSH_IN:
      HeadDirtyToFlushed(i, inP, firstEntry, exit);
    case CS_HEAD_DIRTY, CS_HEAD_FRESH, CS_MID_VALID, CS_TAIL_VALID,
         CS_ONLY_DIRTY, CS_ONLY_FRESH, CS_LOCAL_CLEAN, CS_LOCAL_DIRTY,
         CS_INVALID:
      -- break
    else
      error
        "TypicalCleanup: cache line state not allowed or not yet implemented";
    end;
    if exit then
      return;
    end;
    if CodeFault(i) then
      return;
    end;

    switch TypicalStableStates(line.cState)
    case SG_GLOBAL, SG_INVALID:
      -- break
    else
      error
        "TypicalCleanup: cache line state not allowed or not yet implemented";
    end;

    undefine s;

  end;  -- alias
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine TypicalExecuteLoad (pg.183)               -- levelA routine
procedure TypicalExecuteLoad (i: ProcId;
                              inP: ResPacket;
                              memId: MemoryId; addrOffset: MemoryLineId);
var
  firstEntry: Boolean;  -- indicate to subroutine it is entered the first time
  exit:       Boolean;  -- set by subroutine if interruption (i.e. request
                        --   packet sent)

begin
  alias
    insPhase: proc[i].exec.state.insPhase;
    state:    proc[i].exec.state;
  do
    firstEntry := false;
    exit       := false;

    if insPhase = I_START then
      insPhase   := I_ALLOCATE;
      firstEntry := true;
    end;

    if insPhase = I_ALLOCATE then
      TypicalFindLine(i, FF_WAIT, inP, memId, addrOffset, firstEntry, exit);
      if exit then
        return;
      end;
      if CodeFault(i) then
        error "TypicalExecuteLoad: CodeFault";
      end;
      insPhase   := I_SETUP;
      firstEntry := true;
    end;

    if insPhase = I_SETUP then
      TypicalLoadSetup(i, inP, firstEntry, exit);
      if exit then
        return;
      end;
      if CodeFault(i) then
        error "TypicalExecuteLoad: CodeFault";
      end;

      TypicalLoad(i);

      insPhase   := I_CLEANUP;
      firstEntry := true;
    end;

    if insPhase = I_CLEANUP then
      TypicalCleanup(i, inP, firstEntry, exit);
      if exit then
        return;
      end;
      if CodeFault(i) then
        error "TypicalExecuteLoad: CodeFault";
      end;
    end;

    undefine state;
    state.insType := I_NONE;

  end;  -- alias

end;


--------------------------------------------------------------------------------
-- delete instruction


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine TypicalExecuteDelete (pg.188)             -- levelA routine
procedure TypicalExecuteDelete (i: ProcId;
                                inP: ResPacket;
                                memId: MemoryId; addrOffset: MemoryLineId);
var
  firstEntry: Boolean;  -- indicate to subroutine it is entered the first time
  exit:       Boolean;  -- set by subroutine if interruption (i.e. request
                        --   packet sent)

begin
  alias
    insPhase: proc[i].exec.state.insPhase;
    state:    proc[i].exec.state;
    line:     proc[i].cache.line;
  do
    firstEntry := false;
    exit       := false; 

    if insPhase = I_START then
      insPhase   := I_ALLOCATE;
      firstEntry := true;
    end;

    if insPhase = I_ALLOCATE then
      TypicalFindLine(i, FF_FIND, inP, memId, addrOffset, firstEntry, exit);
      if exit then
        return;
      end;
      if CodeFault(i) then
        error "TypicalExecuteDelete: CodeFault";
      end;
      if line[state.cTPtr].cState=CS_INVALID then
        -- instruction already completed
        insPhase := I_CLEANUP;
      else
        insPhase   := I_SETUP;
        firstEntry := true;
      end;
    end;

    if insPhase = I_SETUP then
      if firstEntry then
        assert line[state.cTPtr].cState!=CS_INVALID
          "TypicalExecuteDelete: cache line state is CS_INVALID";
      end;
      TypicalRolloutEntry(i, inP, firstEntry, exit);
      if exit then
        return;
      end;
      if CodeFault(i) then
        error "TypicalExecuteDelete: CodeFault";
      end;
    end;

    assert line[state.cTPtr].cState=CS_INVALID
      "TypicalExecuteDelete: cache line state not CS_INVALID";

    undefine state;
    state.insType := I_NONE;

  end;  -- alias

end;


--------------------------------------------------------------------------------
-- store instruction


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine TypicalStoreSetup (pg.186)                -- levelB routine
procedure TypicalStoreSetup (i: ProcId;
                             inP: ResPacket;
                             firstEntry: Boolean; var exit: Boolean);
begin
  alias
    line:  proc[i].cache.line[proc[i].exec.state.cTPtr];
    s:     proc[i].exec.state.subState.lB;
  do
    if firstEntry then
      switch line.cState
      case CS_LOCAL_CLEAN, CS_LOCAL_DIRTY:
        error "TypicalStoreSetup: LOCAL cache line states not implemented";
      case CS_INVALID:
        -- break
      else
        if cacheUsage = CU_LOCAL then
          error "TypicalStoreSetup: only CU_GLOBAL cache usage implemented";
        end;
      end;

      s.cState  := line.cState;
      s.firstTo := true;
    end;  -- if firstEntry

    if s.firstTo then
      switch s.cState
      case CS_MID_VALID, CS_TAIL_VALID:
        TypicalRolloutEntry(i, inP, firstEntry, exit);
      case CS_ONLY_FRESH:
        if POP_MODS then  -- POP_MODS check added
          OnlyFreshToOnlyDirty(i, inP, firstEntry, exit);
        else
          TypicalRolloutEntry(i, inP, firstEntry, exit);
        end;
      case CS_HEAD_FRESH:
        if POP_MODS then  -- POP_MODS check added
          HeadFreshToHeadDirty(i, inP, firstEntry, exit);
        else
          TypicalRolloutEntry(i, inP, firstEntry, exit);
        end;
      case CS_ONLY_DIRTY, CS_HEAD_DIRTY:
        -- break
      case CS_INVALID:
        -- legal as well
      else
        error "TypicalStoreSetup: cache line state not allowed";
      end;
      if exit then
        return;
      end;
      if line.cState!=CS_INVALID | CodeFault(i) then
        undefine s;
        return;
      end;

      s.firstTo := false;
      s.firstEntrySub := true;
    end;  -- if s.firstTo

    if cacheUsage = CU_LOCAL then
      error "TypicalStoreSetup: only CU_GLOBAL cache usage implemented";
    else
      -- CO_CHECK not implemented
      InvalidToOnlyDirty(i, inP, s.firstEntrySub, exit);
      s.firstEntrySub := false;
      if exit then
        return;
      end;
    end;

    undefine s;

  end;  -- alias
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine TypicalStore (pg.186)                     -- levelB routine
procedure TypicalStore (i: ProcId);
begin
  alias
    line: proc[i].cache.line[proc[i].exec.state.cTPtr];
  do
    switch line.cState
    case CS_LOCAL_CLEAN, CS_LOCAL_DIRTY:
      error "TypicalStore: LOCAL cache line states not implemented";
    else
      -- break
    end;

    switch line.cState
    case CI_ONLY_CLEAN:
      line.cState := CS_ONLY_DIRTY;    -- added (assumption: enabled store)
    case CI_ONLY_DIRTY, CI_ONLY_EXCL:  -- CI_ONLY_EXCL added
      line.cState := CS_ONLY_DIRTY;
    case CS_HEAD_DIRTY:
      line.cState := CI_HEAD_MODS;
    case CS_ONLY_DIRTY, CI_QUEUED_DIRTY, CI_HEAD_MODS, CI_WRITE_CHECK:
      -- break
    else
      error "TypicalStore: cache line state not allowed";
    end;

  end;  -- alias
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine TypicalExecuteStore (pg.185)              -- levelA routine
procedure TypicalExecuteStore (i: ProcId;
                               inP: ResPacket;
                               memId: MemoryId; addrOffset: MemoryLineId);
var
  firstEntry: Boolean;  -- indicate to subroutine it is entered the first time
  exit:       Boolean;  -- set by subroutine if interruption (i.e. request
                        --   packet sent)

begin
  alias
    insPhase: proc[i].exec.state.insPhase;
    state:    proc[i].exec.state;
    line:     proc[i].cache.line;
  do
    firstEntry := false;
    exit       := false;

    if insPhase = I_START then
      insPhase   := I_ALLOCATE;
      firstEntry := true;
    end;

    if insPhase = I_ALLOCATE then
      TypicalFindLine(i, FF_WAIT, inP, memId, addrOffset, firstEntry, exit);
      if exit then
        return;
      end;
      if CodeFault(i) then
        error "TypicalExecuteStore: CodeFault";
      end;
      insPhase   := I_SETUP;
      firstEntry := true;
    end;

    if insPhase = I_SETUP then
      TypicalStoreSetup(i, inP, firstEntry, exit);
      if exit then
        return;
      end;
      if CodeFault(i) then
        error "TypicalExecuteStore: CodeFault";
      end;

      if line[state.cTPtr].cState=CI_WRITE_CHECK then
        error "TypicalExecuteStore: CI_WRITE_CHECK not implemented";
      else
        TypicalStore(i);
        line[state.cTPtr].data := state.data;
      end;

      insPhase   := I_CLEANUP;
      firstEntry := true;
    end;

    if insPhase = I_CLEANUP then
      TypicalCleanup(i, inP, firstEntry, exit);
      if exit then
        return;
      end;
      if CodeFault(i) then
        error "TypicalExecuteStore: CodeFault";
      end;
    end;

    undefine state;
    state.insType := I_NONE;

  end;  -- alias

end;


--------------------------------------------------------------------------------
-- flush instruction


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine TypicalFlushSetup (pg.187)                -- levelB routine
procedure TypicalFlushSetup (i: ProcId;
                             inP: ResPacket;
                             firstEntry: Boolean; var exit: Boolean);
begin
  alias
    line:  proc[i].cache.line[proc[i].exec.state.cTPtr];
    s:     proc[i].exec.state.subState.lB;
  do
    if firstEntry then
      switch line.cState
      case CS_LOCAL_CLEAN, CS_LOCAL_DIRTY:
        error "TypicalFlushSetup: LOCAL cache line states not implemented";
      else  -- includes CS_INVALID
        if cacheUsage = CU_LOCAL then
          error "TypicalFlushSetup: only CU_GLOBAL cache usage implemented";
        end;
      end;

      s.cState  := line.cState;
      s.firstTo := true;
    end;  -- if firstEntry

    if s.firstTo then
      switch s.cState
      case CS_HEAD_DIRTY, CS_ONLY_DIRTY, CS_ONLY_FRESH:
        -- break
      case CS_MID_VALID, CS_TAIL_VALID:
        TypicalRolloutEntry(i, inP, firstEntry, exit);
      case CS_HEAD_FRESH:
        TypicalRolloutEntry(i, inP, firstEntry, exit);  -- added
          -- original SCI code: HeadFreshToHeadDirty();
      case CS_INVALID:
        -- legal as well
      else
        error "TypicalFlushSetup: cache line state not allowed";
      end;
      if exit then
        return;
      end;
      if line.cState!=CS_INVALID | CodeFault(i) then
        undefine s;
        return;
      end;

      s.firstTo := false;
      s.firstEntrySub := true;
    end;  -- if s.firstTo

    InvalidToOnlyDirty(i, inP, s.firstEntrySub, exit);
    s.firstEntrySub := false;
    if exit then
      return;
    end;

    undefine s;

  end;  -- alias
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine TypicalFlush (pg.187)                     -- levelB routine
procedure TypicalFlush (i: ProcId);
begin
  alias
    line: proc[i].cache.line[proc[i].exec.state.cTPtr];
  do
    switch line.cState
    case CS_INVALID, CS_LOCAL_CLEAN, CS_LOCAL_DIRTY:
      error "TypicalFlush: CU_LOCAL not implemented";
    else
      -- break
    end;

    switch line.cState
    case CI_QUEUED_DIRTY:
      line.cState := CI_QD_FLUSH_IN;
    case CS_ONLY_DIRTY, CI_ONLY_EXCL:  --  CI_ONLY_EXCL added
      line.cState := CI_OD_FLUSH_IN;
    case CS_ONLY_FRESH:
      line.cState := CI_OF_FLUSH_IN;
    case CS_HEAD_DIRTY, CI_HEAD_DIRTY, CI_HEAD_MODS:  -- CI_HEAD_MODS added
      line.cState := CI_HD_FLUSH_IN;
    case CS_INVALID:
      -- break
    else
      error "TypicalFlush: cache line state not allowed or not implemented";
    end;

  end;  -- alias
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- model SCI routine TypicalExecuteFlush (pg.187)              -- levelA routine
procedure TypicalExecuteFlush (i: ProcId;
                               inP: ResPacket;
                               memId: MemoryId; addrOffset: MemoryLineId);
var
  firstEntry: Boolean;  -- indicate to subroutine it is entered the first time
  exit:       Boolean;  -- set by subroutine if interruption (i.e. request
                        --   packet sent)

begin
  alias
    insPhase: proc[i].exec.state.insPhase;
    state:    proc[i].exec.state;
  do
    firstEntry := false;
    exit       := false;

    if insPhase = I_START then
      insPhase   := I_ALLOCATE;
      firstEntry := true;
    end;

    if insPhase = I_ALLOCATE then
      if firstEntry then
        assert cacheUsage=CU_GLOBAL
          "TypicalExecuteFlush: only CU_GLOBAL cache usage implemented";
      end;
      TypicalFindLine(i, FF_WAIT, inP, memId, addrOffset, firstEntry, exit);
      if exit then
        return;
      end;
      if CodeFault(i) then
        error "TypicalExecuteFlush: CodeFault";
      end;
      insPhase   := I_SETUP;
      firstEntry := true;
    end;

    if insPhase = I_SETUP then
      TypicalFlushSetup(i, inP, firstEntry, exit);
      if exit then
        return;
      end;
      if CodeFault(i) then
        error "TypicalExecuteFlush: CodeFault";
      end;

      TypicalFlush(i);

      insPhase   := I_CLEANUP;
      firstEntry := true;
    end;

    if insPhase = I_CLEANUP then
      TypicalCleanup(i, inP, firstEntry, exit);
      if exit then
        return;
      end;
      if CodeFault(i) then
        error "TypicalExecuteFlush: CodeFault";
      end;
    end;

    undefine state;
    state.insType := I_NONE;

  end;  -- alias

end;



--------------------------------------------------------------------------------
-- rules
--------------------------------------------------------------------------------


--------------------------------------------------------------------------------
-- memory behavior                           -- models SCI routine
                                             --   MemoryAccessCoherent (pg.146)

ruleset i: MemoryId do                       -- one rule for each memory
  ruleset j: ProcId do                       -- arbitrary choice of requesting
                                             --   processor
    rule "memory behavior"

      ! isundefined( proc[j].exec.reqPacket.targetId ) &
      ismember( proc[j].exec.reqPacket.targetId, MemoryId ) &
      proc[j].exec.reqPacket.targetId = i

    ==>

    var
      outP:    ResPacket;                    -- response packet (outgoing)
      updates: DataFlags; 
      status:  SStatValues;

    begin
      alias
        inP:  proc[j].exec.reqPacket;  -- request packet (incoming)
        line: memory[i].line[proc[j].exec.reqPacket.offset];
      do
        undefine outP;
        outP.cn      := false;               -- added
        outP.co      := false;
        outP.mCState := line.mState;
        if line.mState!=MS_HOME then
          outP.forwId := line.forwId;
        end;

        switch inP.cmd
        case SC_MREAD, SC_MWRITE64:
          -- allowed commands
        else
          error "memory behavior: transaction command not implemented";
        end;

        if ! MOP_FRESH then
          error "memory behavior: only MOP_FRESH implemented";
        end;
        updates := MemoryTagUpdate(line, inP);
        status  := MemoryAccessBasic(line, inP, outP);

        switch updates
        case NORM_DATA:
          -- break;
        case NORM_NONE:
          outP.cmd := SC_RESP00;
        case NORM_NULL:
          outP.cmd := SC_RESP00;
          outP.cn  := true;
        case NORM_SKIP:
          outP.cn  := true;
        case FULL_DATA:
          outP.co  := true;
        case FULL_NONE:
          outP.cmd := SC_RESP00;
          outP.co  := true;
        case FULL_NULL:
          outP.cmd := SC_RESP00;
          outP.cn  := true;
          outP.co  := true;
        else
          error "memory behavior: MemoryTagUpdate return value not implemented";
        end;

        SendResPacket(outP, inP.sourceId);
        RemoveReqPacket(inP);

        UndefineUnusedValues();
 
      end;  -- alias

    end;  -- rule "memory behavior"

  end;
end;


--------------------------------------------------------------------------------
-- cache behavior                            -- models SCI routine
                                             --   CacheRamAccess (pg.177)

ruleset i: ProcId do               -- one rule for each cache
  ruleset j: ProcId do             -- arbitrary choice of requesting processor
    ruleset k: CacheLineId do      -- random selection of cache line for cache
                                   --   misses
      rule "cache behavior"

        ! isundefined( proc[j].exec.reqPacket.targetId ) &
        ismember( proc[j].exec.reqPacket.targetId, ProcId ) &
        proc[j].exec.reqPacket.targetId = i &

        ( -- is the memory line in any cache line?  -- model SCI routine
          exists l: CacheLineId do                  --   CacheCheckSample
            proc[i].cache.line[l].cState != CS_INVALID
            & proc[i].cache.line[l].addrOffset = proc[j].exec.reqPacket.offset
            & proc[i].cache.line[l].memId      = proc[j].exec.reqPacket.memId
          end
          ->  
          -- is the random k the right k?
          ( proc[i].cache.line[k].cState != CS_INVALID
            & proc[i].cache.line[k].addrOffset = proc[j].exec.reqPacket.offset
            & proc[i].cache.line[k].memId      = proc[j].exec.reqPacket.memId )
        )

      ==>

      var
        outP:    ResPacket;                       -- response packet (outgoing)
        c:       CacheLineId;
        updates: DataFlags;
                                            
      begin
        alias
          inP: proc[j].exec.reqPacket;            -- request packet (incoming)
        do
          undefine outP;

          switch inP.cmd
          case SC_CREAD:
            -- allowed command
          else
            error "cache behavior: transaction command not implemented";
          end;

          c := CacheCheckSample(i, inP.offset, inP.memId, k);
          alias
            line: proc[i].cache.line[c];
          do
            outP.mCState := line.cState;
            outP.cmd     := SC_RESP00;            -- added
            if CSTATE_TYPES(line.cState)=CT_TAIL_USED then
              if ! isundefined(line.backId) then
                outP.forwId := line.backId;
              end;
            else
              if ! isundefined(line.forwId) then
                outP.forwId := line.forwId;
              end;
            end;
              -- no pairwise sharing
            if ! isundefined(line.backId) then
              outP.backId  := line.backId;
            end;

            if line.cState=CS_INVALID |                -- cache miss
               ! (line.memId=inP.memId &
                  line.addrOffset=inP.offset) then
              outP.cn      := true;
              outP.mCState := CS_INVALID;
            else
              updates := CacheTagUpdate(line, inP);
              
              -- SCI routine ReservationCheck not modeled

              switch inP.cmd
              case SC_CREAD:
                if updates = NORM_DATA then
                  if ! isundefined(line.data) then
                    outP.data := line.data;
                  end;
                  outP.cmd := SC_RESP64;
                end;
              else
                error "cache behavior: transaction command not implemented";
              end;
              
              switch updates
              case NORM_DATA:
                outP.cn  := false;
                outP.cmd := SC_RESP64;
              case NORM_NONE:
                outP.cn  := false;
                outP.cmd := SC_RESP00;
              case NORM_NULL:
                outP.cn  := true;
                outP.cmd := SC_RESP00;
              case NORM_SKIP:
                outP.cn  := true;
              else
                error
                  "cache behavior: CacheTagUpdate return value not implemented";
              end;

            end;  -- if cache miss

            SendResPacket(outP, inP.sourceId);
            RemoveReqPacket(inP);

            UndefineUnusedValues();

          end;  -- aliases
        end;

      end;  -- rule "cache behavior"

    end;
  end;
end;


--------------------------------------------------------------------------------
-- execution unit behavior


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- load instruction                        -- models first entry of SCI routine
                                           --   TypicalExecuteLoad (pg.183)

ruleset i: ProcId do            -- one rule for each execution unit
 ruleset m: MemoryId do         -- arbitrary choice of 64-bit address
  ruleset o: MemoryLineId do
   ruleset k: CacheLineId do    -- random selection of cache line for cache
                                --   misses
    ruleset fo: FetchOptions do -- arbitrary choice of fetch option

     rule "load instruction"

       EN_LOAD &
       (fo=CO_FETCH -> FO_FETCH) &
       (fo=CO_LOAD  -> FO_LOAD)  &
       (fo=CO_STORE -> FO_STORE) &
       proc[i].exec.state.insType = I_NONE &

       ( -- is the memory line in any cache line?  -- model SCI routine
         exists l: CacheLineId do                  --   CacheCheckSample
           proc[i].cache.line[l].cState != CS_INVALID
           & proc[i].cache.line[l].addrOffset = o
           & proc[i].cache.line[l].memId      = m
         end
         ->
         -- is the random k the right k?
         ( proc[i].cache.line[k].cState != CS_INVALID
           & proc[i].cache.line[k].addrOffset = o
           & proc[i].cache.line[k].memId      = m )
       )

     ==>

     var
       p: ResPacket;  -- dummy
                                            
     begin
       alias
         state: proc[i].exec.state;
       do
         undefine p;

         undefine state;  -- should be already undefined
         state.insType     := I_LOAD;
         state.insPhase    := I_START;
         state.cTPtr       := k;
         state.tStat       := TS_NORM_CODE;
         state.fetchOption := fo;

         TypicalExecuteLoad(i, p, m, o);

         UndefineUnusedValues();

       end;  -- alias

     end;  -- rule "load instruction"

    end;
   end;
  end;
 end;
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- delete instruction                      -- models first entry of SCI routine
                                           --   TypicalExecuteDelete (pg.188)

ruleset i: ProcId do            -- one rule for each execution unit
 ruleset m: MemoryId do         -- arbitrary choice of 64-bit address
  ruleset o: MemoryLineId do
   ruleset k: CacheLineId do    -- random selection of cache line for cache
                                --   misses

     rule "delete instruction"

       EN_DELETE &
       proc[i].exec.state.insType = I_NONE &

       ( -- is the memory line in any cache line?  -- model SCI routine
         exists l: CacheLineId do                  --   CacheCheckSample
           proc[i].cache.line[l].cState != CS_INVALID
           & proc[i].cache.line[l].addrOffset = o
           & proc[i].cache.line[l].memId      = m
         end
         ->
         -- is the random k the right k?
         ( proc[i].cache.line[k].cState != CS_INVALID
           & proc[i].cache.line[k].addrOffset = o
           & proc[i].cache.line[k].memId      = m )
       )

     ==>

     var
       p: ResPacket;  -- dummy

     begin
       alias
         state: proc[i].exec.state;
       do
         undefine p;

         undefine state;  -- should be already undefined
         state.insType     := I_DELETE;
         state.insPhase    := I_START;
         state.cTPtr       := k;
         state.tStat       := TS_NORM_CODE;

         TypicalExecuteDelete(i, p, m, o);

         UndefineUnusedValues();

       end;  -- alias

     end;  -- rule "delete instruction"

   end;
  end;
 end;
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- store instruction                       -- models first entry of SCI routine
                                           --   TypicalExecuteStore (pg.185)

ruleset i: ProcId do            -- one rule for each execution unit
 ruleset m: MemoryId do         -- arbitrary choice of 64-bit address
  ruleset o: MemoryLineId do
   ruleset k: CacheLineId do    -- random selection of cache line for cache
                                --   misses
    ruleset d: Data do          -- arbitrary choice of data value

     rule "store instruction"

       EN_STORE &
       proc[i].exec.state.insType = I_NONE &

       ( -- is the memory line in any cache line?  -- model SCI routine
         exists l: CacheLineId do                  --   CacheCheckSample
           proc[i].cache.line[l].cState != CS_INVALID
           & proc[i].cache.line[l].addrOffset = o
           & proc[i].cache.line[l].memId      = m
         end
         ->
         -- is the random k the right k?
         ( proc[i].cache.line[k].cState != CS_INVALID
           & proc[i].cache.line[k].addrOffset = o
           & proc[i].cache.line[k].memId      = m )
       )

     ==>

     var
       p: ResPacket;  -- dummy
                                            
     begin
       alias
         state: proc[i].exec.state;
       do
         undefine p;

         undefine state;  -- should be already undefined
         state.insType     := I_STORE;
         state.insPhase    := I_START;
         state.cTPtr       := k;
         state.tStat       := TS_NORM_CODE;
         state.data        := d;

         TypicalExecuteStore(i, p, m, o);

         UndefineUnusedValues();

       end;  -- alias

     end;  -- rule "store instruction"

    end;
   end;
  end;
 end;
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- flush instruction                       -- models first entry of SCI routine
                                           --   TypicalExecuteFlush (pg.187)

ruleset i: ProcId do            -- one rule for each execution unit
 ruleset m: MemoryId do         -- arbitrary choice of 64-bit address
  ruleset o: MemoryLineId do
   ruleset k: CacheLineId do    -- random selection of cache line for cache
                                --   misses

     rule "flush instruction"

       EN_FLUSH &
       proc[i].exec.state.insType = I_NONE &

       ( -- is the memory line in any cache line?  -- model SCI routine
         exists l: CacheLineId do                  --   CacheCheckSample
           proc[i].cache.line[l].cState != CS_INVALID
           & proc[i].cache.line[l].addrOffset = o
           & proc[i].cache.line[l].memId      = m
         end
         ->
         -- is the random k the right k?
         ( proc[i].cache.line[k].cState != CS_INVALID
           & proc[i].cache.line[k].addrOffset = o
           & proc[i].cache.line[k].memId      = m )
       )

     ==>

     var
       p: ResPacket;  -- dummy

     begin
       alias
         state: proc[i].exec.state;
       do
         undefine p;

         undefine state;  -- should be already undefined
         state.insType     := I_FLUSH;
         state.insPhase    := I_START;
         state.cTPtr       := k;
         state.tStat       := TS_NORM_CODE;

         TypicalExecuteFlush(i, p, m, o);

         UndefineUnusedValues();

       end;  -- alias

     end;  -- rule "flush instruction"

   end;
  end;
 end;
end;


-- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
-- continue instruction execution          -- models re-entry of SCI routines
                                           --   TypicalExecute... after inter-
                                           --   ruption due to transaction

ruleset i: ProcId do            -- one rule for each execution unit

   rule "continue instructions"

     proc[i].exec.state.insType != I_NONE &
     ! isundefined( proc[i].exec.resPacket.cn )  -- cn bit defined in packet

   ==>

   var
     m: MemoryId;      -- dummies
     o: MemoryLineId;

   begin
     alias
       inP:  proc[i].exec.resPacket;
       line: proc[i].cache.line[proc[i].exec.state.cTPtr];
     do
       undefine m;
       undefine o;

       -- saving the data is done in SCI routine CommonTransaction (pg.159)
       if inP.cmd=SC_RESP64 then
         line.data := inP.data;
       end;

       switch proc[i].exec.state.insType
       case I_LOAD:
         TypicalExecuteLoad(i, inP, m, o);
       case I_DELETE:
         TypicalExecuteDelete(i, inP, m, o);
       case I_STORE:
         TypicalExecuteStore(i, inP, m, o);
       case I_FLUSH:
         TypicalExecuteFlush(i, inP, m, o);
       else
         error "continue instructions: instruction not yet implemented";
       end;

       RemoveResPacket(inP);

       UndefineUnusedValues();

     end;  -- alias
   end;  -- rule "continue instructions"

end;



--------------------------------------------------------------------------------
-- startstate
--------------------------------------------------------------------------------

ruleset d: Data do                                 -- "random" values

startstate
  -- initialize general variables
  cacheUsage := CU_GLOBAL;

  -- initialize processor
  for i: ProcId do
    undefine proc[i];                              -- undefine unused values

    -- execution unit
    proc[i].exec.state.insType := I_NONE;          -- no instruction currently
                                                   --   in progress
    -- cache
    for j: CacheLineId do
      proc[i].cache.line[j].cState := CS_INVALID;  -- cache lines are invalid
    end;
  end;

  -- initialize memory
  for i: MemoryId do
    undefine memory[i];                            -- undefine unused values
    for j: MemoryLineId do
      memory[i].line[j].mState := MS_HOME;         -- memory state is MS_HOME
      memory[i].line[j].data   := d;               -- "random" data
    end;
  end;

end;

end;



--------------------------------------------------------------------------------
-- invariants
--------------------------------------------------------------------------------

-- includes all the stable cache line states of the typical set protocol,
-- although some of them cannot be reached with the options implemented
-- (except CS_ONLYP_DIRTY, CS_HEAD_WASH)

invariant "unmodified"                   -- check the "unmodified" states
  forall i: ProcId do
    forall j: CacheLineId do
      proc[i].cache.line[j].cState = CS_ONLY_CLEAN |
      proc[i].cache.line[j].cState = CS_ONLY_FRESH |
      proc[i].cache.line[j].cState = CS_HEAD_CLEAN |
      proc[i].cache.line[j].cState = CS_HEAD_FRESH |
      proc[i].cache.line[j].cState = CS_MID_COPY   |
      proc[i].cache.line[j].cState = CS_TAIL_COPY
      ->
      memory[proc[i].cache.line[j].memId]. 
        line[proc[i].cache.line[j].addrOffset].data =
        proc[i].cache.line[j].data
    end
  end;

invariant "head element successor"       -- check successors of head elements
  forall i: ProcId do
    forall j: CacheLineId do
      proc[i].cache.line[j].cState = CS_HEAD_DIRTY |
      proc[i].cache.line[j].cState = CS_HEAD_CLEAN |
      proc[i].cache.line[j].cState = CS_HEAD_FRESH
      ->
      exists k: CacheLineId do
        proc[proc[i].cache.line[j].forwId].cache.line[k].cState != CS_INVALID
        & !isundefined(proc[proc[i].cache.line[j].forwId].cache.line[k].backId)
        & proc[proc[i].cache.line[j].forwId].cache.line[k].backId = i
        & proc[proc[i].cache.line[j].forwId].cache.line[k].memId =
          proc[i].cache.line[j].memId
        & proc[proc[i].cache.line[j].forwId].cache.line[k].addrOffset =
          proc[i].cache.line[j].addrOffset
/*      & (
        proc[proc[i].cache.line[j].forwId].cache.line[k].cState=CS_MID_VALID  |
        proc[proc[i].cache.line[j].forwId].cache.line[k].cState=CS_MID_COPY   |
        proc[proc[i].cache.line[j].forwId].cache.line[k].cState=CS_TAIL_VALID |
        proc[proc[i].cache.line[j].forwId].cache.line[k].cState=CS_TAIL_COPY
        )*/
      end
    end
  end;

invariant "mid element neighbors"        -- check neighbors of mid elements
  forall i: ProcId do
    forall j: CacheLineId do
      proc[i].cache.line[j].cState = CS_MID_VALID |
      proc[i].cache.line[j].cState = CS_MID_COPY
      ->
      exists k: CacheLineId do  -- successor
        proc[proc[i].cache.line[j].forwId].cache.line[k].cState != CS_INVALID
        & !isundefined(proc[proc[i].cache.line[j].forwId].cache.line[k].backId)
        & proc[proc[i].cache.line[j].forwId].cache.line[k].backId = i
        & proc[proc[i].cache.line[j].forwId].cache.line[k].memId =
          proc[i].cache.line[j].memId
        & proc[proc[i].cache.line[j].forwId].cache.line[k].addrOffset =
          proc[i].cache.line[j].addrOffset
/*      & (
        proc[proc[i].cache.line[j].forwId].cache.line[k].cState=CS_MID_VALID  |
        proc[proc[i].cache.line[j].forwId].cache.line[k].cState=CS_MID_COPY   |
        proc[proc[i].cache.line[j].forwId].cache.line[k].cState=CS_TAIL_VALID |
        proc[proc[i].cache.line[j].forwId].cache.line[k].cState=CS_TAIL_COPY
        )*/
      end
      &
      forall k: CacheLineId do  -- predecessor (may have been deleted)
        isundefined(proc[proc[i].cache.line[j].backId].cache.line[k].forwId)
        | proc[proc[i].cache.line[j].backId].cache.line[k].forwId != i
        | proc[proc[i].cache.line[j].backId].cache.line[k].forwId = i
          & proc[proc[i].cache.line[j].backId].cache.line[k].memId =
            proc[i].cache.line[j].memId
          & proc[proc[i].cache.line[j].backId].cache.line[k].addrOffset =
            proc[i].cache.line[j].addrOffset
/*        & (
        proc[proc[i].cache.line[j].backId].cache.line[k].cState=CS_HEAD_DIRTY |
        proc[proc[i].cache.line[j].backId].cache.line[k].cState=CS_HEAD_CLEAN |
        proc[proc[i].cache.line[j].backId].cache.line[k].cState=CS_HEAD_FRESH |
        proc[proc[i].cache.line[j].backId].cache.line[k].cState=CS_MID_VALID  |
        proc[proc[i].cache.line[j].backId].cache.line[k].cState=CS_MID_COPY
          )*/
      end
    end
  end;
  -- >>>>> additional checks needed (e.g. for a list of two mid-elements)

/*invariant "tail element predecessor"     -- check predecessors of tail elements
  forall i: ProcId do
    forall j: CacheLineId do
      proc[i].cache.line[j].cState = CS_TAIL_VALID |
      proc[i].cache.line[j].cState = CS_TAIL_COPY
      ->
      forall k: CacheLineId do  -- predecessor (may have been deleted)
        isundefined(proc[proc[i].cache.line[j].backId].cache.line[k].forwId)
        | proc[proc[i].cache.line[j].backId].cache.line[k].forwId != i
        | proc[proc[i].cache.line[j].backId].cache.line[k].forwId = i
          & proc[proc[i].cache.line[j].backId].cache.line[k].memId =
            proc[i].cache.line[j].memId
          & proc[proc[i].cache.line[j].backId].cache.line[k].addrOffset =
            proc[i].cache.line[j].addrOffset
          & (
        proc[proc[i].cache.line[j].backId].cache.line[k].cState=CS_HEAD_DIRTY |
        proc[proc[i].cache.line[j].backId].cache.line[k].cState=CS_HEAD_CLEAN |
        proc[proc[i].cache.line[j].backId].cache.line[k].cState=CS_HEAD_FRESH |
        proc[proc[i].cache.line[j].backId].cache.line[k].cState=CS_MID_VALID  |
        proc[proc[i].cache.line[j].backId].cache.line[k].cState=CS_MID_COPY   |
        proc[proc[i].cache.line[j].backId].cache.line[k].cState=CS_QUEUED_JUNK|
        proc[proc[i].cache.line[j].backId].cache.line[k].cState=CS_HD_INVAL_OD
          )
      end
    end
  end;*/

invariant "sharing list head"            -- check sharing list head
  forall i: MemoryId do
    forall j: MemoryLineId do
      memory[i].line[j].mState = MS_FRESH |
      memory[i].line[j].mState = MS_GONE
      ->
      exists k: CacheLineId do
        proc[memory[i].line[j].forwId].cache.line[k].cState != CS_INVALID
        & proc[memory[i].line[j].forwId].cache.line[k].memId = i
        & proc[memory[i].line[j].forwId].cache.line[k].addrOffset = j
      end
    end
  end;
