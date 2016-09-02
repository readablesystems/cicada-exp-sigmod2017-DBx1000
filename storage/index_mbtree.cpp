#define CONFIG_H "silo/config/config-perf.h"

#define NDB_MASSTREE 1
#include "silo/masstree/config.h"
#include "silo/masstree_btree.h"

#ifdef NDEBUG
#undef NDEBUG
#endif

#include "index_mbtree.h"
#include "helper.h"
#include "storage/row.h"
#include "system/mem_alloc.h"

// #ifdef DEBUG
// #undef NDEBUG
// #endif

#if CC_ALG == MICA
struct mbtree_value_type {
  row_t* row;
  int refcount;
};
#endif

struct mbtree_params : public Masstree::nodeparams<> {
#if CC_ALG != MICA
  typedef row_t* value_type;
#else
  typedef mbtree_value_type value_type;
#endif
  typedef Masstree::value_print<value_type> value_print_type;
  typedef simple_threadinfo threadinfo_type;
  enum { RcuRespCaller = true };
};

typedef mbtree<mbtree_params> concurrent_mbtree;

RC IndexMBTree::init(uint64_t part_cnt, table_t* table) {
  return init(part_cnt, table, 0);
}

RC IndexMBTree::init(uint64_t part_cnt, table_t* table, uint64_t bucket_cnt) {
  (void)bucket_cnt;

  this->table = table;

  for (uint64_t part_id = 0; part_id < part_cnt; part_id++) {
    mem_allocator.register_thread(part_id % g_thread_cnt);

    auto t = (concurrent_mbtree*)mem_allocator.alloc(sizeof(concurrent_mbtree),
                                                     part_id);
    new (t) concurrent_mbtree;

    btree_idx.push_back(t);
  }

  return RCOK;
}

RC IndexMBTree::index_insert(idx_key_t key, row_t* row, int part_id) {
  auto idx = reinterpret_cast<concurrent_mbtree*>(btree_idx[part_id]);

  u64_varkey mbtree_key(key);

#if CC_ALG != MICA
  if (!idx->insert_if_absent(mbtree_key, row)) return ERROR;
#else
  mbtree_value_type v{row, 1};
  if (!idx->insert_refcount(mbtree_key, v)) return ERROR;
#endif

  return RCOK;
}

RC IndexMBTree::index_read(idx_key_t key, row_t** row, int part_id) {
  auto idx = reinterpret_cast<concurrent_mbtree*>(btree_idx[part_id]);

  u64_varkey mbtree_key(key);

#if CC_ALG != MICA
  if (!idx->search(mbtree_key, *row)) return ERROR;
#else
  mbtree_value_type v;
  if (!idx->search(mbtree_key, v)) return ERROR;
  *row = v.row;
#endif

  return RCOK;
}

RC IndexMBTree::index_read_multiple(idx_key_t key, row_t** rows,
                                    uint64_t& count, int part_id) {
  // Duplicate keys are currently not supported in IndexMBTree.
  assert(false);
  (void)key;
  (void)rows;
  (void)count;
  (void)part_id;
  return ERROR;
}

RC IndexMBTree::index_read_range(idx_key_t min_key, idx_key_t max_key,
                                 row_t** rows, uint64_t& count, int part_id) {
  if (count == 0) return RCOK;

  auto idx = reinterpret_cast<concurrent_mbtree*>(btree_idx[part_id]);

  u64_varkey mbtree_key_min(min_key);

  // mbtree's range is right-open.
  max_key++;
  assert(max_key != 0);
  u64_varkey mbtree_key_max(max_key);

  uint64_t i = 0;
  auto f = [&i, rows, count](auto& k, auto& v) {
#if CC_ALG != MICA
    rows[i++] = v;
#else
    rows[i++] = v.row;
#endif
    return i < count;
  };

  idx->search_range(mbtree_key_min, &mbtree_key_max, f);

  count = i;

  return RCOK;
}

RC IndexMBTree::index_read_range_rev(idx_key_t min_key, idx_key_t max_key,
                                     row_t** rows, uint64_t& count,
                                     int part_id) {
  if (count == 0) return RCOK;

  auto idx = reinterpret_cast<concurrent_mbtree*>(btree_idx[part_id]);

  // mbtree's range is left-open.
  assert(min_key != 0);
  min_key--;
  u64_varkey mbtree_key_min(min_key);

  u64_varkey mbtree_key_max(max_key);

  uint64_t i = 0;
  auto f = [&i, rows, count](auto& k, auto& v) {
#if CC_ALG != MICA
    rows[i++] = v;
#else
    rows[i++] = v.row;
#endif
    return i < count;
  };

  idx->rsearch_range(mbtree_key_max, &mbtree_key_min, f);

  count = i;

  return RCOK;
}

RC IndexMBTree::index_remove(idx_key_t key, row_t*, int part_id) {
  auto idx = reinterpret_cast<concurrent_mbtree*>(btree_idx[part_id]);

  u64_varkey mbtree_key(key);

#if CC_ALG != MICA
  if (!idx->remove(mbtree_key, NULL)) return ERROR;
#else
  if (!idx->remove_refcount(mbtree_key, NULL)) return ERROR;
#endif

  return RCOK;
}
