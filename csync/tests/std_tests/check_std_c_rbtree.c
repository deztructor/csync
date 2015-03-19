#include <errno.h>
#include <time.h>

#include "torture.h"

#include "std/c_alloc.h"
#include "std/c_rbtree.h"

typedef struct test_s {
    int key;
    int number;
} test_t;

static int data_cmp(const void *key, const void *data) {
    test_t *a, *b;

    a = (test_t *) key;
    b = (test_t *) data;

    if (a->key < b->key) {
        return -1;
    } else if (a->key > b->key) {
        return 1;
    }

    return 0;
}

static int key_cmp(const void *key, const void *data) {
    int a;
    test_t *b;

    a = POINTER_TO_INT(key);
    b = (test_t *) data;

    if (a < b->key) {
        return -1;
    } else if (a > b->key) {
        return 1;
    }

    return 0;
}

static int visitor(void *obj, void *data) {
    test_t *a;
    test_t *b;

    a = (test_t *) obj;
    b = (test_t *) data;

    if (a->key == b->key) {
        a->number = 42;
    }

    return 0;
}

static void destructor(void *data) {
    test_t *freedata = NULL;

    freedata = (test_t *) data;
    SAFE_FREE(freedata);
}

static void setup(void **state) {
    c_rbtree_t *tree = NULL;
    int rc;

    rc = c_rbtree_create(&tree, key_cmp, data_cmp);
    assert_int_equal(rc, 0);

    *state = tree;
}

static void setup_complete_tree(void **state) {
    c_rbtree_t *tree = NULL;
    int i = 0;
    int rc;

    rc = c_rbtree_create(&tree, key_cmp, data_cmp);
    assert_int_equal(rc, 0);

    for (i = 0; i < 100; i++) {
        test_t *testdata = NULL;

        testdata = c_malloc(sizeof(test_t));
        assert_non_null(testdata);

        testdata->key = i;

        rc = c_rbtree_insert(tree, (void *) testdata);
        assert_int_equal(rc, 0);
    }

    *state = tree;
}

static void teardown(void **state) {
    c_rbtree_t *tree = *state;

    c_rbtree_destroy(tree, destructor);
    c_rbtree_free(tree);

    *state = NULL;
}

static void check_c_rbtree_create_free(void **state)
{
    c_rbtree_t *tree = NULL;
    int rc;

    (void) state; /* unused */

    rc = c_rbtree_create(&tree, key_cmp, data_cmp);
    assert_int_equal(rc, 0);
    assert_int_equal(tree->size, 0);

    rc = c_rbtree_free(tree);
    assert_int_equal(rc, 0);
}

static void check_c_rbtree_create_null(void **state)
{
    c_rbtree_t *tree = NULL;
    int rc;

    (void) state; /* unused */

    rc = c_rbtree_create(NULL, key_cmp, data_cmp);
    assert_int_equal(rc, -1);

    rc = c_rbtree_create(&tree, NULL, data_cmp);
    assert_int_equal(rc, -1);

    rc = c_rbtree_create(&tree, key_cmp, NULL);
    assert_int_equal(rc, -1);
}

static void check_c_rbtree_free_null(void **state)
{
    int rc;

    (void) state; /* unused */

    rc = c_rbtree_free(NULL);
    assert_int_equal(rc, -1);
}

static void check_c_rbtree_insert_delete(void **state)
{
    c_rbtree_t *tree = NULL;
    c_rbnode_t *node = NULL;
    test_t *testdata = NULL;
    int rc;

    (void) state; /* unused */

    rc = c_rbtree_create(&tree, key_cmp, data_cmp);
    assert_int_equal(rc, 0);

    testdata = malloc(sizeof(test_t));
    testdata->key = 42;

    rc = c_rbtree_insert(tree, (void *) testdata);
    assert_int_equal(rc, 0);

    node = c_rbtree_head(tree);
    assert_non_null(node);

    testdata = c_rbtree_node_data(node);
    SAFE_FREE(testdata);
    rc = c_rbtree_node_delete(node);
    assert_int_equal(rc, 0);

    c_rbtree_free(tree);
}

static void check_c_rbtree_insert_random(void **state)
{
    c_rbtree_t *tree = *state;
    int i = 0, rc;

    for (i = 0; i < 100; i++) {
        test_t *testdata = NULL;

        testdata = malloc(sizeof(test_t));
        assert_non_null(testdata);

        testdata->key = i;

        rc = c_rbtree_insert(tree, testdata);
        assert_int_equal(rc, 0);

    }
    rc = c_rbtree_check_sanity(tree);
    assert_int_equal(rc, 0);
}

static void check_c_rbtree_insert_duplicate(void **state)
{
    c_rbtree_t *tree = *state;
    test_t *testdata;
    int rc;

    testdata = malloc(sizeof(test_t));
    assert_non_null(testdata);

    testdata->key = 42;

    rc = c_rbtree_insert(tree, (void *) testdata);
    assert_int_equal(rc, 0);

    /* add again */
    testdata = malloc(sizeof(test_t));
    assert_non_null(testdata);

    testdata->key = 42;

    /* check for duplicate */
    rc = c_rbtree_insert(tree, (void *) testdata);
    assert_int_equal(rc, 1);

    free(testdata);
}

static void check_c_rbtree_find(void **state)
{
    c_rbtree_t *tree = *state;
    int rc, i = 42;
    c_rbnode_t *node;
    test_t *testdata;

    rc = c_rbtree_check_sanity(tree);
    assert_int_equal(rc, 0);

    /* find the node with the key 42 */
    node = c_rbtree_find(tree, (void *) &i);
    assert_non_null(node);

    testdata = (test_t *) c_rbtree_node_data(node);
    assert_int_equal(testdata->key, 42);
}

static void check_c_rbtree_delete(void **state)
{
    c_rbtree_t *tree = *state;
    int rc, i = 42;
    c_rbnode_t *node = NULL;
    test_t *freedata = NULL;

    rc = c_rbtree_check_sanity(tree);
    assert_int_equal(rc, 0);

    node = c_rbtree_find(tree, (void *) &i);
    assert_non_null(node);

    freedata = (test_t *) c_rbtree_node_data(node);
    free(freedata);
    rc = c_rbtree_node_delete(node);
    assert_int_equal(rc, 0);

    rc = c_rbtree_check_sanity(tree);
    assert_int_equal(rc, 0);
}

static void check_c_rbtree_walk(void **state)
{
    c_rbtree_t *tree = *state;
    int rc, i = 42;
    test_t *testdata;
    c_rbnode_t *node;

    rc = c_rbtree_check_sanity(tree);
    assert_int_equal(rc, 0);

    testdata = (test_t *) c_malloc(sizeof(test_t));
    testdata->key = 42;

    rc = c_rbtree_walk(tree, testdata, visitor);
    assert_int_equal(rc, 0);

    /* find the node with the key 42 */
    node = c_rbtree_find(tree, (void *) &i);
    assert_non_null(node);
    free(testdata);

    testdata = (test_t *) c_rbtree_node_data(node);
    assert_int_equal(testdata->number, 42);
}

static void check_c_rbtree_walk_null(void **state)
{
    c_rbtree_t *tree = *state;
    int rc, i = 42;
    test_t *testdata;
    c_rbnode_t *node;

    rc = c_rbtree_check_sanity(tree);
    assert_int_equal(rc, 0);

    testdata = (test_t *) malloc(sizeof(test_t));
    testdata->key = 42;

    rc = c_rbtree_walk(NULL, testdata, visitor);
    assert_int_equal(rc, -1);
    assert_int_equal(errno, EINVAL);

    rc = c_rbtree_walk(tree, NULL, visitor);
    assert_int_equal(rc, -1);
    assert_int_equal(errno, EINVAL);

    rc = c_rbtree_walk(tree, testdata, NULL);
    assert_int_equal(rc, -1);
    assert_int_equal(errno, EINVAL);

    /* find the node with the key 42 */
    node = c_rbtree_find(tree, (void *) &i);
    assert_non_null(node);

    free(testdata);
}

static void check_c_rbtree_dup(void **state)
{
    c_rbtree_t *tree = *state;
    c_rbtree_t *duptree = NULL;
    int rc = -1;

    duptree = c_rbtree_dup(tree);
    assert_non_null(duptree);

    rc = c_rbtree_check_sanity(duptree);
    assert_int_equal(rc, 0);

    c_rbtree_free(duptree);
}

#if 0
static void check_c_rbtree_x)
{
  int rc = -1;

  rc = c_rbtree_check_sanity(tree);
  fail_unless(rc == 0, "c_rbtree_check_sanity failed with return code %d", rc);
}
#endif

int torture_run_tests(void)
{
  const UnitTest tests[] = {
      unit_test(check_c_rbtree_create_free),
      unit_test(check_c_rbtree_create_null),
      unit_test(check_c_rbtree_free_null),
      unit_test(check_c_rbtree_insert_delete),
      unit_test_setup_teardown(check_c_rbtree_insert_random, setup, teardown),
      unit_test_setup_teardown(check_c_rbtree_insert_duplicate, setup, teardown),
      unit_test_setup_teardown(check_c_rbtree_find, setup_complete_tree, teardown),
      unit_test_setup_teardown(check_c_rbtree_delete, setup_complete_tree, teardown),
      unit_test_setup_teardown(check_c_rbtree_walk, setup_complete_tree, teardown),
      unit_test_setup_teardown(check_c_rbtree_walk_null, setup_complete_tree, teardown),
      unit_test_setup_teardown(check_c_rbtree_dup, setup_complete_tree, teardown),
  };

  return run_tests(tests);
}

