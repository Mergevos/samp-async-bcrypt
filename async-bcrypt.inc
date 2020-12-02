/*
	* Library: Samp Async Bcryot (async-bcrypt.inc)
	* Author: Mergevos
	* Date: 02. Decembre 2020.
	* Copyright: (C) 2020
	* Credits: 
        IllidanS4 - PawnPlus
        Graber - Snippets from pp-mysql.inc. 
*/

/**
 * <library name="async-bcrypt">
 * <section>
 *  Version
 * </section>
 *  1.0.0
 * </library>
 */

#if defined _inc_async_bcrypt
    #undef _inc_async_bcrypt
#endif

#if defined _Async_BCrypt_included_
    #endinput
#endif
#define _Async_BCrypt_included_

#define _ASYNC_BCRYPT_VERSION "1.0.0"


#include <a_samp>
#include <PawnPlus> 

// Currently supported only Lassir's plugin.
// SyS' plugin support will be released as soon as possible.

forward _pp@on_init@PPAsync_BCrypt();
public _pp@on_init@PPAsync_BCrypt() {
    printf("[%s] Async-Bcrypt by Mergevos loaded!", _ASYNC_BCRYPT_VERSION);
}

forward _pp@on_exit@PPAsync_BCrypt();
public _pp@on_exit@PPAsync_BCrypt() {
    printf("[%s] Async-Bcrypt by Mergevos unloaded!", _ASYNC_BCRYPT_VERSION);
}



#if defined bcrypt_included
    
    /** 
    * <summary> Hashes a key but as AmxString. </summary>
    * <param name="key"> AmxString of key to hash.</param>
    * <param name="cost"> Bcrypt cost to use. </param>
    * <param name="callback_name"> Callback to call after the execution.</param>
    * <param name="callback_format"> Callback specifiers. </param>
    * <param name="..."> Arguments. </param>
    * <returns> 1 on success, 0 on fail.</returns>
    */
    native bcrypt_hash_s(ConstAmxString: key, cost = 12, const callback_name[], const callback_format[] = "", {AmxString, Float, _}:...) = bcrypt_hash;
    /** 
    * <summary> Checks if a password is same as hash but as AmxString. </summary>
    * <param name="password"> AmxString of password to check.</param>
    * <param name="hash"> AmxString of hash to check password with.</param>
    * <param name="callback_name"> Callback to call after the execution.</param>
    * <param name="callback_format"> Callback specifiers. </param>
    * <param name="..."> Arguments. </param>
    * <returns> 1 on success, 0 on fail.</returns>
    */
    native bcrypt_check_s(ConstAmxString: password, ConstAmxString: hash, const callback_name[], const callback_format[] = "", {AmxString, Float, _}:...) = bcrypt_check;
    /** 
    * <summary> Checks if a hash needs rehash, but as AmxString. </summary>
    * <param name="hash"> AmxString of hash to check if rehashing needs.</param>
    * <param name="cost"> Bcrypt cost to use. </param>
    * <returns> True if needs, false otherwise.</returns>
    */
    native bool:bcrypt_needs_rehash_s(ConstAmxString: hash, cost) = bcrypt_needs_rehash;

    static const 
        bool:ACTION_HASH = false,
        bool:ACTION_CHECK = true;

    enum e_BCRYPT_INFO {
        E_BCRYPT_Hash[61],
        bool: E_BCRYPT_Equal
    };

    /** 
    * <summary> Hashes a key but asynchronously. </summary>
    * <param name="key"> Key to hash.</param>
    * <param name="cost"> Bcrypt cost to use. </param>
    * <returns> Task, task results.</returns>
    */

    stock Task: bcrypt_ahash(const key[], cost) {
        new 
            Task:bcrypt_task = task_new(),
            Handle: bcrypt_handle = handle_new(bcrypt_task, .weak = true); // thanks to Graber.
        
        new const ret = bcrypt_hash(key, cost, "__AsyncBCryptOn", "dd", _:bcrypt_handle, ACTION_HASH);
    
        if(ret) {
            handle_acquire(bcrypt_handle);
        }
        else {
            task_set_error_ms(bcrypt_task, amx_err_exit, 0);
        }
        return bcrypt_task;
    
    }

    /** 
    * <summary> Hashes a key but asynchronously as AmxString. </summary>
    * <param name="key"> Key to hash.</param>
    * <param name="cost"> Bcrypt cost to use. </param>
    * <returns> Task, task results.</returns>
    */

    stock Task: bcrypt_ahash_s(ConstStringTag: key, cost) {
        new 
            Task:bcrypt_task = task_new(),
            Handle: bcrypt_handle = handle_new(bcrypt_task, .weak = true); // thanks to Graber.
        
        new const ret = bcrypt_hash_s(str_addr(String: key), cost, "__AsyncBCryptOn", "dd", _:bcrypt_handle, ACTION_HASH);
    
        if(ret) {
            handle_acquire(bcrypt_handle);
        }
        else {
            task_set_error_ms(bcrypt_task, amx_err_exit, 0);
        }
        return bcrypt_task;
    
    }

    /** 
    * <summary> Checks if a password is same as hash but asynchronously. </summary>
    * <param name="password"> Password to check.</param>
    * <param name="hash"> Hash to check password with.</param>
    * <returns> Task, task results.</returns>
    */

    stock Task: bcrypt_acheck(const password[], const hash[]) {
        new 
            Task:bcrypt_task = task_new(),
            Handle: bcrypt_handle = handle_new(bcrypt_task, .weak = true); // thanks to Graber.
            
        new const ret = bcrypt_check(password, hash, "__AsyncBCryptOn", "dd", _:bcrypt_handle, ACTION_CHECK);
    
        if(ret) {
            handle_acquire(bcrypt_handle);
        }
        else {
            task_set_error_ms(bcrypt_task, amx_err_exit, 0);
        }
        return bcrypt_task;
    }

    /** 
    * <summary> Checks if a password is same as hash but asynchronously as AmxString. </summary>
    * <param name="password"> AmxString of password to check.</param>
    * <param name="hash"> AmxString of hash to check password with.</param>
    * <returns> Task, task results.</returns>
    */

    stock Task: bcrypt_acheck_s(ConstStringTag: password, ConstStringTag: hash) {
        new 
            Task:bcrypt_task = task_new(),
            Handle: bcrypt_handle = handle_new(bcrypt_task, .weak = true); // thanks to Graber.
            
        new const ret = bcrypt_check_s(str_addr(String: password), str_addr(String: hash), "__AsyncBCryptOn", "dd", _:bcrypt_handle, ACTION_CHECK);
    
        if(ret) {
            handle_acquire(bcrypt_handle);
        }
        else {
            task_set_error_ms(bcrypt_task, amx_err_exit, 0);
        }
        return bcrypt_task;
    }

    //------------------------------------------------------

    forward __AsyncBCryptOn(Handle: bc_handle, bool: action);
    public __AsyncBCryptOn(Handle: bc_handle, bool: action) {
        if(handle_linked(bc_handle)) {
            new 
                bc_response[e_BCRYPT_INFO],
                Task:local_bcrypt = Task:handle_get(bc_handle);

            if(action == ACTION_HASH) {
                bcrypt_get_hash(bc_response[E_BCRYPT_Hash]);
            } else {
                bc_response[E_BCRYPT_Equal] = bcrypt_is_equal();
            }
        
            task_set_result_arr(local_bcrypt, bc_response);
        }
        handle_release(bc_handle);
    }

#endif

