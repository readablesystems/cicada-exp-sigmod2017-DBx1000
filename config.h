#ifndef _CONFIG_H_
#define _CONFIG_H_

/***********************************************/
// Simulation + Hardware
/***********************************************/
#define THREAD_CNT 64
#define PART_CNT 64
// each transaction only accesses 1 virtual partition. But the lock/ts manager and index are not aware of such partitioning. VIRTUAL_PART_CNT describes the request distribution and is only used to generate queries. For HSTORE, VIRTUAL_PART_CNT should be the same as PART_CNT.
#define VIRTUAL_PART_CNT			1
#define PAGE_SIZE					4096
#define CL_SIZE						64

// # of transactions to run for warmup
#define WARMUP 66666
// YCSB or TPCC or TATP
#define WORKLOAD TPCC
// print the transaction latency distribution
#define PRT_LAT_DISTR				false
#define STATS_ENABLE				true
#define TIME_ENABLE					false
#define PROF_ENABLE					false

#define MEM_ALLIGN					8

// [THREAD_ALLOC]
#define THREAD_ALLOC				false
#define THREAD_ARENA_SIZE			(1UL << 22)
#define MEM_PAD 					true

// [PART_ALLOC]
#define PART_ALLOC 					false
#define MEM_SIZE					(1UL << 30)
#define NO_FREE						false

// [RCU_ALLOC]
#define RCU_ALLOC true
#define RCU_ALLOC_SIZE (32*(1UL << 30))

/***********************************************/
// Concurrency Control
/***********************************************/
// WAIT_DIE, NO_WAIT, DL_DETECT, TIMESTAMP, MVCC, HEKATON, HSTORE, OCC, VLL, TICTOC, SILO
// TODO TIMESTAMP does not work at this moment
#define CC_ALG MICA
#define ISOLATION_LEVEL SERIALIZABLE

// all transactions acquire tuples according to the primary key order.
#define KEY_ORDER					false
// transaction roll back changes after abort
#define ROLL_BACK					true
// per-row lock/ts management or central lock/ts management
#define CENTRAL_MAN					false
#define BUCKET_CNT					31
#define ABORT_PENALTY 				100000
#define ABORT_BUFFER_SIZE			10
#define ABORT_BUFFER_ENABLE			true
// [ INDEX ]
#define ENABLE_LATCH				false
#define CENTRAL_INDEX				false
#define CENTRAL_MANAGER 			false
#define INDEX_STRUCT IDX_MICA
#define BTREE_ORDER 				16

// [DL_DETECT]
#define DL_LOOP_DETECT				1000 	// 100 us
#define DL_LOOP_TRIAL				100	// 1 us
#define NO_DL						KEY_ORDER
#define TIMEOUT						1000000 // 1ms
// [TIMESTAMP]
#define TS_TWR						false
#define TS_ALLOC					TS_CAS
#define TS_BATCH_ALLOC				false
#define TS_BATCH_NUM				1
// [MVCC]
// when read/write history is longer than HIS_RECYCLE_LEN
// the history should be recycled.
//#define HIS_RECYCLE_LEN				10
//#define MAX_PRE_REQ					1024
//#define MAX_READ_REQ				1024
#define MIN_TS_INTVL				5000000 //5 ms. In nanoseconds
// [OCC]
#define MAX_WRITE_SET				10
#define PER_ROW_VALID				true
// [TICTOC]
#define WRITE_COPY_FORM				"data" // ptr or data
#define TICTOC_MV					false
#define WR_VALIDATION_SEPARATE		true
#define WRITE_PERMISSION_LOCK		false
#define ATOMIC_TIMESTAMP			"false"
// [TICTOC, SILO]
#define VALIDATION_LOCK "no-wait"
#define PRE_ABORT "true"
#define ATOMIC_WORD					true
// [HSTORE]
// when set to true, hstore will not access the global timestamp.
// This is fine for single partition transactions.
#define HSTORE_LOCAL_TS				false
// [VLL]
#define TXN_QUEUE_SIZE_LIMIT		THREAD_CNT

/***********************************************/
// Logging
/***********************************************/
#define LOG_COMMAND					false
#define LOG_REDO					false
#define LOG_BATCH_TIME				10 // in ms

/***********************************************/
// Benchmark
/***********************************************/
// max number of rows touched per transaction
#define MAX_ROW_PER_TXN				1024
#define QUERY_INTVL 				1UL
#define MAX_TXN_PER_PART            2000000
#define MAX_WARMUP_DURATION         10.0
#define MAX_TXN_DURATION            10.0
#define FIRST_PART_LOCAL 			true
#define MAX_TUPLE_SIZE 704
// ==== [YCSB] ====
#define INIT_PARALLELISM 4
#define SYNTH_TABLE_SIZE 			(1024 * 40)
#define ZIPF_THETA 					0.6
#define READ_PERC 					0.95
#define WRITE_PERC 					0.05
#define SCAN_PERC 					0
#define SCAN_LEN					20
#define PART_PER_TXN 				1
#define PERC_MULTI_PART				1
#define REQ_PER_QUERY				16
#define FIELD_PER_TUPLE				1
// ==== [TPCC] ====
// For large warehouse count, the tables do not fit in memory
// small tpcc schemas shrink the table size.
#define TPCC_SMALL					false
// Some of the transactions read the data but never use them.
// If TPCC_ACCESS_ALL == fales, then these parts of the transactions
// are not modeled.
#define TPCC_ACCESS_ALL 			true
#define WH_UPDATE					true
#define NUM_WH 64
//
#define TPCC_INSERT_ROWS true
#define TPCC_DELETE_ROWS true
#define TPCC_INSERT_INDEX true
#define TPCC_DELETE_INDEX true
// TPCC_FULL requires TPCC_INSERT_ROWS and TPCC_UPDATE_INDEX to fully function
#define TPCC_FULL true
#define TPCC_CF		  false
#define TPCC_SPLIT_DELIVERY false
#define TPCC_VALIDATE_GAP false
#define TPCC_VALIDATE_NODE true
#define SIMPLE_INDEX_UPDATE false
//
enum TPCCTxnType {TPCC_ALL,
				TPCC_PAYMENT,
				TPCC_NEW_ORDER,
				TPCC_ORDER_STATUS,
				TPCC_DELIVERY,
				TPCC_STOCK_LEVEL};
extern TPCCTxnType 					g_tpcc_txn_type;

//#define TXN_TYPE					TPCC_ALL
#define PERC_PAYMENT 				0.5
#define FIRSTNAME_MINLEN 			8
#define FIRSTNAME_LEN 				16
#define LASTNAME_LEN 				16

#define DIST_PER_WARE				10

// ==== [TATP] ====
#define TATP_DEFAULT_NUM_SUBSCRIBERS 100000
#define TATP_SUB_NBR_PADDING_SIZE 15
#define TATP_FREQUENCY_DELETE_CALL_FORWARDING    2
#define TATP_FREQUENCY_GET_ACCESS_DATA           35
#define TATP_FREQUENCY_GET_NEW_DESTINATION       10
#define TATP_FREQUENCY_GET_SUBSCRIBER_DATA       35
#define TATP_FREQUENCY_INSERT_CALL_FORWARDING    2
#define TATP_FREQUENCY_UPDATE_LOCATION           14
#define TATP_FREQUENCY_UPDATE_SUBSCRIBER_DATA    2

#define TATP_SCALE_FACTOR 1
#define TATP_SUB_SIZE (TATP_DEFAULT_NUM_SUBSCRIBERS * TATP_SCALE_FACTOR)

/***********************************************/
// TODO centralized CC management.
/***********************************************/
#define MAX_LOCK_CNT				(20 * THREAD_CNT)
#define TSTAB_SIZE                  50 * THREAD_CNT
#define TSTAB_FREE                  TSTAB_SIZE
#define TSREQ_FREE                  4 * TSTAB_FREE
#define MVHIS_FREE                  4 * TSTAB_FREE
#define SPIN                        false

/***********************************************/
// Test cases
/***********************************************/
#define TEST_ALL					true
enum TestCases {
	READ_WRITE,
	CONFLICT
};
extern TestCases					g_test_case;
/***********************************************/
// DEBUG info
/***********************************************/
#define WL_VERB						true
#define IDX_VERB					false
#define VERB_ALLOC					true

#define DEBUG_LOCK					false
#define DEBUG_TIMESTAMP				false
#define DEBUG_SYNTH					false
#define DEBUG_ASSERT				false
#define DEBUG_CC					false //true

/***********************************************/
// Constant
/***********************************************/
// INDEX_STRUCT
#define IDX_HASH 					1
#define IDX_BTREE					2
#define IDX_MICA					3
// WORKLOAD
#define YCSB						1
#define TPCC						2
#define TATP						3
#define TEST						4
// Concurrency Control Algorithm
#define NO_WAIT						1
#define WAIT_DIE					2
#define DL_DETECT					3
#define TIMESTAMP					4
#define MVCC						5
#define HSTORE						6
#define OCC							7
#define TICTOC						8
#define SILO						9
#define VLL							10
#define HEKATON 					11
#define MICA 					12
//Isolation Levels
#define SERIALIZABLE				1
#define SNAPSHOT					2
#define REPEATABLE_READ				3
// TIMESTAMP allocation method.
#define TS_MUTEX					1
#define TS_CAS						2
#define TS_HW						3
#define TS_CLOCK					4

#define MICA_FULLINDEX true

#define MICA_NO_TSC false
#define MICA_NO_PRE_VALIDATION false
#define MICA_NO_INSERT_NEWEST_VERSION_ONLY false
#define MICA_NO_SORT_WRITE_SET_BY_CONTENTION false
#define MICA_NO_STRAGGLER_AVOIDANCE false
#define MICA_NO_WAIT_FOR_PENDING false
#define MICA_NO_INLINING false
#define MICA_NO_BACKOFF false

#define MICA_USE_FIXED_BACKOFF false
#define MICA_FIXED_BACKOFF 0.

#define MICA_USE_SLOW_GC false
#define MICA_SLOW_GC 10

#define PRINT_LAT_DIST false

#endif