# samp-async-bcrypt

[![Mergevos](https://img.shields.io/badge/Mergevos-samp--async--bcrypt-2f2f2f.svg?style=for-the-badge)](https://github.com/Mergevos/samp-async-bcrypt)

Asynchronous SA:MP BCrypt plugin library.


## Installation

Simply install to your project:

```bash
sampctl package install Mergevos/samp-async-bcrypt
```

Include in your code and begin using the library:

```pawn
#include <async-bcrypt>
```

## Usage

There're few functions converted to adapt PawnPlus dynamic strings, they're default functions from last lassir's BCrypt plugin version.
Those're:
```c
bcrypt_hash_s(ConstAmxString: key, cost = 12, const callback_name[], const callback_format[] = "", {AmxString, Float, _}:...)
bcrypt_check_s(ConstAmxString: password, ConstAmxString: hash, const callback_name[], const callback_format[] = "", {AmxString, Float, _}:...)
bool:bcrypt_needs_rehash_s(ConstAmxString: hash, cost)
```

I'm pretty sure that you know what these arguments mean, but in case you don't, there're: 
```
bcrypt_hash_s: 
key - AmxString of key to hash.
cost - Bcrypt cost to use. 
callback_name - Callback to call after the execution.
callback_format - Callback specifiers. 
... - Arguments. 
Returns 1 on success, 0 on fail.

bcrypt_check_s: 

password - AmxString of password to check.
hash - AmxString of hash to check password with.
callback_name - Callback to call after the execution.
callback_format - Callback specifiers. 
... - Arguments. 
Returns 1 on success, 0 on fail.

bcrypt_needs_rehash_s: 

hash - AmxString of hash to check if rehashing needs.
cost - Bcrypt cost to use.
Returns true if needs, false otherwise.

```

Those were converted functions, I assume you know how to use PawnPlus dynamic strings, if not, check PawnPlus wiki [here](https://github.com/IllidanS4/PawnPlus/wiki)

Now, the new functions which came into game are: 
```c
Task: bcrypt_ahash(const key[], cost)
Task: bcrypt_ahash_s(ConstStringTag: key, cost)
Task: bcrypt_acheck(const password[], const hash[])
Task: bcrypt_acheck_s(ConstStringTag: password, ConstStringTag: hash)
```
Now the callback name and specifiers are gone. You don't need it, the correct way of use would be like this one [here](https://github.com/Mergevos/samp-async-bcrypt/blob/master/async-bcrypt.inc).
By reading the source, you'll be explained what functions do, but this is the basic syntax:
```c
new res[e_BCRYPT_INFO],
await_arr(res) bcrypt_ahash("Test", 12);
// now res[E_BCRYPT_Hash] contains the hash of the "Test" password.
await_arr(res) bcrypt_acheck("Testl", res[E_BCRYPT_Hash]);
// res[E_BCRYPT_Equal] is boolean holding the value of bcrypt_acheck, simply it is check boolean, it's true if password 
// and hash matches, otherwise it's not. In this case, we were checking "Test1" with the hash of "Test", should be false
```


## Testing

To test, simply run the package:

```bash
sampctl package run
```
