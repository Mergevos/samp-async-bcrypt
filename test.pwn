#include <a_samp> 
#include <logging>
#include <bcrypt>

#define PP_SYNTAX_AWAIT
#include "async-bcrypt.inc"

native TestPrint_s(ConstAmxString: str) = print;
main()
{
    new res[e_BCRYPT_INFO],
        another_res[e_BCRYPT_INFO];

    Log_ToggleHandle("TEST", true);

    // Here we await async bcrypt_hash aka bcrypt_ahash to finish its job. 
    // It returns a result into an array, this time it is `res`. 
    // Then it outputs the hash via logging library.
    

    Log_Debug("TEST", "Awaiting hash...");
    await_arr(res) bcrypt_ahash("Test", 12);
    Log_Info("TEST", "Hashed. ", Log_Field_s("Hash", res[E_BCRYPT_Hash]));

    // Here we await async bcrypt_check aka bcrypt_acheck to do its job.
    // It also returns the result into the array. 
    // Then it outputs the check, actually, the match of hash and password. Hash should be loaded from somewhere and then used to check 
    // it along with password. 
    // It's expected to throw false because we have hashed "Test" password before, and now we're checking "Test1"

    Log_Debug("TEST", "Awaiting check...");
    await_arr(res) bcrypt_acheck("Testl", res[E_BCRYPT_Hash]);
    Log_Info("TEST", "Checked. ", Log_Field_s("Match", res[E_BCRYPT_Equal] ? "true" : "false"));
    
    // These below works same way as these above, but they use pawnplus dynamic strings.
    // Currently the hash retrieving isn't supported yet, it should be used like it is on line 40.

    Log_Debug("TEST", "Awaiting amxstr hash...");
    await_arr(another_res) bcrypt_ahash_s(str_new("Test"), 12);
    TestPrint_s(str_format("AMX String hashed: %S", str_new(another_res[E_BCRYPT_Hash]))); // This is actually bcrypt_get_hash

    Log_Debug("TEST", "Awaiting amxstr check...");
    await_arr(another_res) bcrypt_acheck_s(str_new("Test"), str_new(another_res[E_BCRYPT_Hash]));
    Log_Info("TEST", "Awaited", Log_Field_s("Match", another_res[E_BCRYPT_Equal] ? "true" : "false"));
}

