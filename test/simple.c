/*
    simple.c -- cmocka simple unittest
    Copyright (C) 2015 Rafal Lesniak

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with this program; if not, write to the Free Software Foundation, Inc.,
    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
*/

/* Use the unit test allocators */
#define UNIT_TESTING 1

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include <stdbool.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>

//#include "config.h"
#include "system.h"

#include "splay_tree.h"
#include "conf.h"
#include "edge.h"
#include "xalloc.h"

/*
   edge tests
*/
static int edge_setup(void **state) {
  splay_tree_t *edge_tree;
  edge_tree = new_edge_tree();
  *state = edge_tree;
  return 0;
}

static int edge_teardown(void **state) {
  splay_tree_t *edge_tree = *state;
  splay_delete_tree(edge_tree);
  return 0;
}


static void test_edge_init(void **state) {
  splay_tree_t *edge_tree = *state;
  edge_t *e1, *e2;

  assert_non_null(edge_tree);
  e1 = new_edge();
  assert_non_null(e1);
  //TODO: make edge_add work on any edge_tree
  //edge_add(edge_tree, e1);

  e2 = clone_edge(e1);
  assert_non_null(e2);

  //edge_add(edge_tree, e2);

  assert_int_equal(edge_tree->count, 2);

  //TODO: make edge_del work on any edge_tree
  //edge_del(edge_tree, e2);
  assert_int_equal(edge_tree->count, 1);

}


/*
  splay tree tests
*/

typedef struct mock_item {
  int value;
} mock_item_t;

int splay_mock_compare(const mock_item_t *a, const mock_item_t *b);
int splay_mock_compare(const mock_item_t *a, const mock_item_t *b) {
  if (a->value > b->value)
    return 1;
  return 0;
}

void splay_mock_delete(mock_item_t *);
void splay_mock_delete(mock_item_t *a) {
  test_free(a);
}

static int splay_tree_setup(void **state) {
  splay_tree_t *my_tree;
  my_tree = splay_alloc_tree((splay_compare_t) splay_mock_compare,
                             (splay_action_t) splay_mock_delete);
  *state = my_tree;
  return 0;
}

static void test_splay_tree_init(void **state) {
  splay_tree_t *my_tree = *state;

  assert_non_null(my_tree);
  assert_null(my_tree->head);
  assert_null(my_tree->tail);
  assert_null(my_tree->root);
  assert_non_null(my_tree->compare);
  assert_non_null(my_tree->delete);
  assert_int_equal(my_tree->count, 0);

  splay_delete_tree(my_tree);

}

void test_splay_tree_basic(void **);
void test_splay_tree_basic(void **state) {
  splay_tree_t *my_tree = *state;
  mock_item_t *i, *b;
  int r;
  assert_non_null(my_tree);

  i = NULL;
  b = NULL;

  for(int z=0; z < 10; z++) {
    i = xzalloc(sizeof(mock_item_t));
    assert_non_null(i);
    i->value = z;
    assert_int_equal(i->value, z);
    splay_insert(my_tree, i);
  }
  assert_int_equal(my_tree->count, 10);

  i = xzalloc(sizeof(mock_item_t));
  i->value = 7;
  b = splay_search_closest(my_tree, i, &r);
  assert_non_null(b);
  // this is a reference test for current state of splay_search algorithm
  assert_int_equal(b->value, 9);
  // cleanup local alloc
  test_free(i);

  splay_delete_tree(my_tree);

}

/*
  Config tests
*/

static int config_setup(void **state) {
  splay_tree_t *config_tree;
  init_configuration(&config_tree);
  assert_non_null(config_tree);
  *state = config_tree;

  return 0;
}

static int config_teardown(void **state) {
  splay_tree_t *config_tree = *state;
  exit_configuration(&config_tree);
  assert_null(config_tree);

  return 0;
}

static void test_read_config_file(void **state) {
  char *priority = NULL;
  splay_tree_t *config_tree = *state;

  read_config_file(config_tree, "./conf1");

  get_config_string(lookup_config(config_tree, "Ed25519PublicKey111111"), &priority);
  assert_null(priority);

  //this one triggers a memory leak
  get_config_string(lookup_config(config_tree, "Ed25519PublicKey"), &priority);
  assert_non_null(priority);
}

static void test_config_add_item(void **state) {
  splay_tree_t *config_tree = *state;
  config_t *item = NULL;
  config_t *item2;
  char *value = NULL;

  item = new_config();
  assert_non_null(item);

  item->variable = xstrdup("testVar1");
  item->value = xstrdup("barbara");
  item->file = xstrdup("none");;
  item->line = 1;

  config_add(config_tree, item);

  value = NULL;
  get_config_string(lookup_config(config_tree, "testVar1"), &value);
  assert_non_null(value);

  if (!value) {
    item2 = NULL;
    for splay_each(config_t, item2, config_tree) {
        printf("%s %s %s %d\n", item2->variable, item2->value, item2->file, item2->line);
      }
  }
}

static void test_config_add_item_no_filename(void **state) {
  splay_tree_t *config_tree = *state;
  config_t *item = NULL;
  config_t *item2;
  char *value = NULL;

  item = new_config();
  assert_non_null(item);

  item->variable = xstrdup("testVar2");
  item->value = xstrdup("barbara");
  item->file = NULL;
  item->line = -1;

  config_add(config_tree, item);

  value = NULL;
  get_config_string(lookup_config(config_tree, "testVar2"), &value);
  assert_non_null(value);
  if (!value) {
    item2 = NULL;
    for splay_each(config_t, item2, config_tree) {
        printf("%s %s %s %d\n", item2->variable, item2->value, item2->file, item2->line);
      }
  }
}

int main(void) {
  const struct CMUnitTest tests[] = {
    // edge tests
    cmocka_unit_test_setup_teardown(test_edge_init, edge_setup, edge_teardown),
    // config tests
    cmocka_unit_test_setup_teardown(test_read_config_file, config_setup, config_teardown),
    cmocka_unit_test_setup_teardown(test_config_add_item, config_setup, config_teardown),
    cmocka_unit_test_setup_teardown(test_config_add_item_no_filename, config_setup, config_teardown),
    // splay_tree tests
    cmocka_unit_test_setup(test_splay_tree_init, splay_tree_setup),
    cmocka_unit_test_setup(test_splay_tree_basic, splay_tree_setup)
  };

  return cmocka_run_group_tests(tests, NULL, NULL);
}
