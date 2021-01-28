// Copyright (c) Microsoft Corporation.
// Licensed under the MIT License.

#include <assert.h>
#include <errno.h>
#include <limits.h>
#include <spawn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

static void _run_tests(const char* tests[], size_t ntests)
{
    for (size_t i = 0; i < ntests; i++)
    {
        int r;
        pid_t pid;
        int wstatus;

        printf("=== start test: %s\n", tests[i]);

        char* const args[] = {(char*)tests[i], NULL};
        char* const envp[] = {"VALUE=1", NULL};

        r = posix_spawn(&pid, tests[i], NULL, NULL, args, envp);
        assert(r == 0);
        assert(pid >= 0);

        assert(waitpid(pid, &wstatus, WNOHANG) == 0);
        assert(waitpid(pid, &wstatus, 0) == pid);
        assert(WIFEXITED(wstatus));
        assert(WEXITSTATUS(wstatus) == 0);

        printf("=== passed test (%s)\n", tests[i]);
    }
}

int main(int argc, const char* argv[])
{
    static const char* tests[] = {
        "/src/functional/tls_local_exec.exe",
        "/src/functional/argv.exe",
        "/src/functional/basename.exe",
        "/src/functional/clocale_mbfuncs.exe",
        "/src/functional/clock_gettime.exe",
        "/src/functional/crypt.exe",
        "/src/functional/dirname.exe",
        "/src/functional/dlopen.exe",
        "/src/functional/fdopen.exe",
        "/src/functional/fnmatch.exe",
        "/src/functional/fscanf.exe",
        "/src/functional/fwscanf.exe",
        "/src/functional/iconv_open.exe",
        "/src/functional/inet_pton.exe",
        "/src/functional/mbc.exe",
        "/src/functional/memstream.exe",
        "/src/regression/pthread_cancel-sem_wait.exe",
        "/src/functional/pthread_cond.exe",
        "/src/functional/pthread_tsd.exe",
        "/src/functional/qsort.exe",
        "/src/functional/random.exe",
        "/src/functional/search_hsearch.exe",
        "/src/functional/search_insque.exe",
        "/src/functional/search_lsearch.exe",
        "/src/functional/search_tsearch.exe",
        "/src/functional/sem_init.exe",
        "/src/functional/setjmp.exe",
        "/src/functional/snprintf.exe",
        "/src/functional/socket.exe",
        "/src/functional/sscanf.exe",
        "/src/functional/strftime.exe",
        "/src/functional/string.exe",
        "/src/functional/string_memcpy.exe",
        "/src/functional/string_memmem.exe",
        "/src/functional/string_memset.exe",
        "/src/functional/string_strchr.exe",
        "/src/functional/string_strcspn.exe",
        "/src/functional/string_strstr.exe",
        "/src/functional/strtod.exe",
        "/src/functional/strtod_long.exe",
        "/src/functional/strtod_simple.exe",
        "/src/functional/strtof.exe",
        "/src/functional/strtold.exe",
        "/src/functional/strtol.exe",
        "/src/functional/swprintf.exe",
        "/src/functional/tgmath.exe",
        "/src/functional/time.exe",
        "/src/functional/tls_align_dlopen.exe",
        "/src/functional/tls_align.exe",
        "/src/functional/tls_init_dlopen.exe",
        "/src/functional/tls_init.exe",
        "/src/functional/udiv.exe",
        "/src/functional/ungetc.exe",
        "/src/functional/wcsstr.exe",
        "/src/functional/wcstol.exe",
        "/src/math/acos.exe",
        "/src/math/acosf.exe",
        "/src/math/acosh.exe",
        "/src/math/acoshf.exe",
        "/src/math/acoshl.exe",
        "/src/math/acosl.exe",
        "/src/math/asin.exe",
        "/src/math/asinf.exe",
        "/src/math/asinh.exe",
        "/src/math/asinhf.exe",
        "/src/math/asinhl.exe",
        "/src/math/asinl.exe",
        "/src/math/atan2.exe",
        "/src/math/atan2f.exe",
        "/src/math/atan2l.exe",
        "/src/math/atan.exe",
        "/src/math/atanf.exe",
        "/src/math/atanh.exe",
        "/src/math/atanhf.exe",
        "/src/math/atanhl.exe",
        "/src/math/atanl.exe",
        "/src/math/cbrt.exe",
        "/src/math/cbrtf.exe",
        "/src/math/cbrtl.exe",
        "/src/math/ceil.exe",
        "/src/math/ceilf.exe",
        "/src/math/ceill.exe",
        "/src/math/copysign.exe",
        "/src/math/copysignf.exe",
        "/src/math/copysignl.exe",
        "/src/math/cos.exe",
        "/src/math/cosf.exe",
        "/src/math/cosh.exe",
        "/src/math/coshf.exe",
        "/src/math/coshl.exe",
        "/src/math/cosl.exe",
        "/src/math/drem.exe",
        "/src/math/dremf.exe",
        "/src/math/erfc.exe",
        "/src/math/erfcf.exe",
        "/src/math/erfcl.exe",
        "/src/math/erf.exe",
        "/src/math/erff.exe",
        "/src/math/erfl.exe",
        "/src/math/exp10.exe",
        "/src/math/exp10f.exe",
        "/src/math/exp10l.exe",
        "/src/math/exp2.exe",
        "/src/math/exp2f.exe",
        "/src/math/exp2l.exe",
        "/src/math/exp.exe",
        "/src/math/expf.exe",
        "/src/math/expl.exe",
        "/src/math/expm1.exe",
        "/src/math/expm1f.exe",
        "/src/math/expm1l.exe",
        "/src/math/fabs.exe",
        "/src/math/fabsf.exe",
        "/src/math/fabsl.exe",
        "/src/math/fdim.exe",
        "/src/math/fdimf.exe",
        "/src/math/fdiml.exe",
        "/src/math/fenv.exe",
        "/src/math/floor.exe",
        "/src/math/floorf.exe",
        "/src/math/floorl.exe",
        "/src/math/fma.exe",
        "/src/math/fmaf.exe",
        "/src/math/fmax.exe",
        "/src/math/fmaxf.exe",
        "/src/math/fmaxl.exe",
        "/src/math/fmin.exe",
        "/src/math/fminf.exe",
        "/src/math/fminl.exe",
        "/src/math/fmod.exe",
        "/src/math/fmodf.exe",
        "/src/math/fmodl.exe",
        "/src/math/fpclassify.exe",
        "/src/math/frexp.exe",
        "/src/math/frexpf.exe",
        "/src/math/frexpl.exe",
        "/src/math/hypot.exe",
        "/src/math/hypotf.exe",
        "/src/math/hypotl.exe",
        "/src/math/ilogb.exe",
        "/src/math/ilogbf.exe",
        "/src/math/ilogbl.exe",
        "/src/math/isless.exe",
        "/src/math/j0.exe",
        "/src/math/j0f.exe",
        "/src/math/j1.exe",
        "/src/math/j1f.exe",
        "/src/math/jn.exe",
        "/src/math/jnf.exe",
        "/src/math/ldexp.exe",
        "/src/math/ldexpf.exe",
        "/src/math/ldexpl.exe",
        "/src/math/lgammaf_r.exe",
        "/src/math/lgammal_r.exe",
        "/src/math/lgamma_r.exe",
        "/src/math/llrint.exe",
        "/src/math/llrintf.exe",
        "/src/math/llrintl.exe",
        "/src/math/llround.exe",
        "/src/math/llroundf.exe",
        "/src/math/llroundl.exe",
        "/src/math/log10.exe",
        "/src/math/log10f.exe",
        "/src/math/log10l.exe",
        "/src/math/log1p.exe",
        "/src/math/log1pf.exe",
        "/src/math/log1pl.exe",
        "/src/math/log2.exe",
        "/src/math/log2l.exe",
        "/src/math/logb.exe",
        "/src/math/logbf.exe",
        "/src/math/logbl.exe",
        "/src/math/log.exe",
        "/src/math/logf.exe",
        "/src/math/logl.exe",
        "/src/math/lrint.exe",
        "/src/math/lrintf.exe",
        "/src/math/lrintl.exe",
        "/src/math/lround.exe",
        "/src/math/lroundf.exe",
        "/src/math/lroundl.exe",
        "/src/math/modf.exe",
        "/src/math/modff.exe",
        "/src/math/modfl.exe",
        "/src/math/nearbyint.exe",
        "/src/math/nearbyintf.exe",
        "/src/math/nearbyintl.exe",
        "/src/math/nextafter.exe",
        "/src/math/nextafterf.exe",
        "/src/math/nextafterl.exe",
        "/src/math/nexttoward.exe",
        "/src/math/nexttowardf.exe",
        "/src/math/nexttowardl.exe",
        "/src/math/pow10.exe",
        "/src/math/pow10f.exe",
        "/src/math/pow10l.exe",
        "/src/math/pow.exe",
        "/src/math/powl.exe",
        "/src/math/remainder.exe",
        "/src/math/remainderf.exe",
        "/src/math/remainderl.exe",
        "/src/math/remquo.exe",
        "/src/math/remquof.exe",
        "/src/math/remquol.exe",
        "/src/math/rint.exe",
        "/src/math/rintf.exe",
        "/src/math/rintl.exe",
        "/src/math/round.exe",
        "/src/math/roundf.exe",
        "/src/math/roundl.exe",
        "/src/math/scalb.exe",
        "/src/math/scalbf.exe",
        "/src/math/scalbln.exe",
        "/src/math/scalblnf.exe",
        "/src/math/scalblnl.exe",
        "/src/math/scalbn.exe",
        "/src/math/scalbnf.exe",
        "/src/math/scalbnl.exe",
        "/src/math/sincos.exe",
        "/src/math/sincosf.exe",
        "/src/math/sincosl.exe",
        "/src/math/sin.exe",
        "/src/math/sinf.exe",
        "/src/math/sinh.exe",
        "/src/math/sinhf.exe",
        "/src/math/sinhl.exe",
        "/src/math/sinl.exe",
        "/src/math/sqrt.exe",
        "/src/math/sqrtf.exe",
        "/src/math/sqrtl.exe",
        "/src/math/tan.exe",
        "/src/math/tanf.exe",
        "/src/math/tanh.exe",
        "/src/math/tanhf.exe",
        "/src/math/tanhl.exe",
        "/src/math/tanl.exe",
        "/src/math/tgamma.exe",
        "/src/math/tgammaf.exe",
        "/src/math/tgammal.exe",
        "/src/math/trunc.exe",
        "/src/math/truncf.exe",
        "/src/math/truncl.exe",
        "/src/math/y0.exe",
        "/src/math/y0f.exe",
        "/src/math/y1.exe",
        "/src/math/y1f.exe",
        "/src/math/yn.exe",
        "/src/math/ynf.exe",
        "/src/regression/dn_expand-empty.exe",
        "/src/regression/dn_expand-ptr-0.exe",
        "/src/regression/fgets-eof.exe",
        "/src/regression/flockfile-list.exe",
        "/src/regression/fpclassify-invalid-ld80.exe",
        "/src/regression/ftello-unflushed-append.exe",
        "/src/regression/getpwnam_r-crash.exe",
        "/src/regression/getpwnam_r-errno.exe",
        "/src/regression/iconv-roundtrips.exe",
        "/src/regression/inet_ntop-v4mapped.exe",
        "/src/regression/inet_pton-empty-last-field.exe",
        "/src/regression/iswspace-null.exe",
        "/src/regression/lrand48-signextend.exe",
        "/src/regression/malloc-0.exe",
        "/src/regression/mbsrtowcs-overflow.exe",
        "/src/regression/memmem-oob.exe",
        "/src/regression/memmem-oob-read.exe",
        "/src/regression/mkdtemp-failure.exe",
        "/src/regression/mkstemp-failure.exe",
        "/src/regression/printf-1e9-oob.exe",
        "/src/regression/printf-fmt-g-round.exe",
        "/src/regression/printf-fmt-g-zeros.exe",
        "/src/regression/printf-fmt-n.exe",
        "/src/regression/pthread_condattr_setclock.exe",
        "/src/regression/pthread_cond-smasher.exe",
        "/src/regression/pthread_exit-cancel.exe",
        "/src/regression/pthread_once-deadlock.exe",
        "/src/regression/pthread_rwlock-ebusy.exe",
        "/src/regression/putenv-doublefree.exe",
        "/src/regression/regex-backref-0.exe",
        "/src/regression/regex-bracket-icase.exe",
        "/src/regression/regexec-nosub.exe",
        "/src/regression/regex-ere-backref.exe",
        "/src/regression/regex-escaped-high-byte.exe",
        "/src/regression/regex-negated-range.exe",
        "/src/regression/rewind-clear-error.exe",
        "/src/regression/scanf-bytes-consumed.exe",
        "/src/regression/scanf-match-literal-eof.exe",
        "/src/regression/scanf-nullbyte-char.exe",
        "/src/regression/setvbuf-unget.exe",
        "/src/regression/sigaltstack.exe",
        "/src/regression/sigprocmask-internal.exe",
        "/src/regression/sscanf-eof.exe",
        "/src/regression/strverscmp.exe",
        "/src/regression/uselocale-0.exe",
        "/src/regression/wcsncpy-read-overflow.exe",
        "/src/regression/wcsstr-false-negative.exe",
    };
    size_t ntests = sizeof(tests) / sizeof(tests[0]);

    _run_tests(tests, ntests);

    printf("=== passed all tests: %s\n", argv[0]);

    return 0;
}
