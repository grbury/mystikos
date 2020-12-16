# Getting started with a TEE-aware program

Please see [README](../README.md) for how to install Open LibOS or build
it from source code.

## The problem statement

Why would an application want to be aware of the TEE it's running within? Two
possible requirements are:

* The application should behave differently based on whether it's running
inside a TEE or not, and if yes, which types of TEE;
* The application wants to get help from the TEE, and gain trust and obtain
secrets from an external party.

## Open LibOS solution

To the first requirement, Open LibOS provides an environment variable
`LIBOS_TARGET`. Applications can query the variable to find out whether it
is running outside a TEE, or if it's running in SGX or other TEE platforms.

To the second requirement, Open LibOS provides a system call, and C/C++
applications can use it directly to request the TEE to generate
a self-signed certificates which includes:

* A public key for subsequent encrypted communications, and
* An attestation report containing:
    * The prove that the application is running in a specific TEE;
    * The application's identity (for SGX, that's
    **MRENCLAVE**/**MRSIGNER**/etc.); and
    * The hash of the communication public key.

When the TEE-aware application interacts with another application running
in another TEE, it switches the role and becomes a party that wants to verify
an self-signed certificate. For that, Open LibOS provides another system call to
verify self-signed certificate generated by TEEs. It does so by allowing the
application to plug in an identity verifier that dictates which peer
applications are expected to be accepted or rejected. Again, for SGX enclaves,
the application identity is tied to **MRENCLAVE**/**MRSIGNER**/etc.

## Write a program that behaves differently for TEE and non-TEE

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    const char* target = getenv("LIBOS_TARGET");
    if (target && strcmp(target, "linux") == 0)
    {
        printf("I am in non-TEE\n");
    }
    else if (target && strcmp(target, "sgx") == 0)
    {
        printf("I am in SGX\n");
    }
    else
        printf("I am in unknown environment\n");

    return 0;
}
```

Save this to `tee.c`. You can build and run the program on Ubuntu with the
following commands:

```bash
gcc tee.c -o tee && ./tee
```
The output should be: `I am in unknown environment`.

Now build and run the program in SGX with the following commands:
```bash
mkdir appdir; libos-gcc a.c -o appdir/tee
libos mkcpio appdir rootfs && libos exec rootfs /tee
```

The output should be: `I am in SGX`.

## Write a programs that generates/verifies self-signed certificates

```c
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <stdint.h>
#include <libos/tee.h>

int _verifier(libos_tee_identity_t* identity, void* ptr)
{
    // Expected Product ID: {1}
    const uint8_t PRODID[LIBOS_PRODUCT_ID_SIZE] = {1};
    // Expected security version: 1
    const SVN = 1;

    // Returning 0 means pass.
    // We can easily expand this to more sophicated checks
    // based on unique_id, signer_id, etc.
    return memcmp(identity->product_id, PRODID, LIBOS_PRODUCT_ID_SIZE) ||
           identity->security_version != SVN;
}

int main()
{
    long ret;
    /* Extended syscalls */
    const long SYS_libos_gen_creds = 1009;
    const long SYS_libos_free_creds = 1010;
    const long SYS_libos_verify_cert = 1011;
    void* cert = NULL;
    void* pkey = NULL;
    size_t cert_size = 0, pkey_size = 0;

    ret = syscall(SYS_libos_gen_creds, &cert, &cert_size, &pkey, &pkey_size);
    assert(ret == 0);
    printf("Generated self-signed certificate and private key\n");

    ret = syscall(SYS_libos_verify_cert, cert, cert_size, _verifier, NULL);
    assert(ret == 0);
    printf("Verified self-signed certificate\n");

    ret = syscall(SYS_libos_free_creds, cert, cert_size, pkey, pkey_size);
    assert(ret == 0);

    return 0;
}
```

Save the file as `tee2.c`. Now build and run the program in SGX with the
following commands:
```bash
libos-gcc tee2.c -I/opt/openlibos/include -o appdir/tee2
libos mkcpio appdir rootfs && libos exec rootfs /tee2
```
And the output should be:
```
Generated self-signed certificate and private key
Verified self-signed certificate
```

The application is free to provide `NULL` for the third parameter of
`SYS_libos_verify_cert`. In that case, Open LibOS would accept any
application running inside a TEE with no regard to the application identity.

If the application does want to reject/approve based on app identity, it
needs include `libos/tee.h`, defines a function similar to `_verifier`,
and passes it to syscall `SYS_libos_verify_cert`.

For application written in high level languages which can not make direct
syscalls, such as C# or Python, we need to write a C/C++ library that does
certificate generation and/or verification, and use FFI to call into the
library from the application.