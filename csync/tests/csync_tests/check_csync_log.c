#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "torture.h"

#include "csync.h"
#include "csync_log.c"
#include "c_private.h"

static void setup(void **state) {
    CSYNC *csync;
    int rc;

    rc = system("mkdir -p /tmp/check_csync1");
    assert_int_equal(rc, 0);

    rc = system("mkdir -p /tmp/check_csync2");
    assert_int_equal(rc, 0);

    rc = csync_create(&csync, "/tmp/check_csync1", "/tmp/check_csync2");
    assert_int_equal(rc, 0);

    rc = csync_set_config_dir(csync, "/tmp/check_csync");
    assert_int_equal(rc, 0);

    *state = csync;
}

static void teardown(void **state) {
    CSYNC *csync = *state;
    int rc;

    rc = csync_destroy(csync);

    rc = system("rm -rf /tmp/check_csync");
    assert_int_equal(rc, 0);

    rc = system("rm -rf /tmp/check_csync1");
    assert_int_equal(rc, 0);

    rc = system("rm -rf /tmp/check_csync2");
    assert_int_equal(rc, 0);

    *state = NULL;
}

static void check_log_callback(int verbosity,
                               const char *function,
                               const char *buffer,
                               void *userdata)
{
    int rc;

    assert_true(verbosity >= 0);
    assert_non_null(function);
    assert_false(function[0] == '\0');
    assert_non_null(buffer);
    assert_false(buffer[0] == '\0');

    (void) userdata; /* unused */

    rc = system("touch /tmp/check_csync1/cb_called");
    assert_int_equal(rc, 0);
}

static void check_set_log_level(void **state)
{
    int rc;

    (void) state;

    rc = csync_set_log_level(-5);
    assert_int_equal(rc, -1);

    rc = csync_set_log_level(5);
    assert_int_equal(rc, 0);

    rc = csync_get_log_level();
    assert_int_equal(rc, 5);
}

static void check_set_auth_callback(void **state)
{
    csync_log_callback log_fn;
    int rc;

    (void) state;

    rc = csync_set_log_callback(NULL);
    assert_int_equal(rc, -1);

    rc = csync_set_log_callback(check_log_callback);
    assert_int_equal(rc, 0);

    log_fn = csync_get_log_callback();
    assert_non_null(log_fn);
    assert_true(log_fn == &check_log_callback);
}

static void check_logging(void **state)
{
    int rc;
    csync_stat_t sb;
    mbchar_t *path;
    path = c_utf8_to_locale("/tmp/check_csync1/cb_called");

    (void) state; /* unused */

    assert_non_null(path);

    rc = csync_set_log_level(1);
    assert_int_equal(rc, 0);

    rc = csync_set_log_callback(check_log_callback);
    assert_int_equal(rc, 0);

    csync_log(1, __FUNCTION__, "rc = %d", rc);

    rc = _tstat(path, &sb);

    c_free_locale_string(path);

    assert_int_equal(rc, 0);
}

int torture_run_tests(void)
{
    const UnitTest tests[] = {
        unit_test(check_set_log_level),
        unit_test(check_set_auth_callback),
        unit_test_setup_teardown(check_logging, setup, teardown),
    };

    return run_tests(tests);
}
