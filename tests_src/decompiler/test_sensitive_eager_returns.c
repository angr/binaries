/* This testcase tests the eager_returns optimization in angr's decompiler. Normally, when we 
 * see an edge that goes to a return region (which can be a block or small graph), we duplicate
 * that entire region. In the foo function below, the `goto cleanup` will almost always be 
 * eliminated in the decompilation. To combat the goto being eliminated, we've added a check
 * to see how many calls are at the return site. Currently, which may have changed in the code,
 * the max number of calls aloud is 2. Any more should result in the goto remaining in the decomp.
 * 
 * The goto in bar should be eliminated when we decompile. The goto in foo should remain when 
 * decompiled. 
 *
 * Compile:
 * gcc test_sensitive_eager_returns.c -o test_sensitive_eager_returns 
 */ 


int bar(int a, int b, int c) {
    int val = a;
    if (a) {
        if (a + b == 1337) 
            goto cleanup;

        sleep(1);
    }
    
    sleep(2); 
    if (b && c) 
        val = b + c;
    sleep(3); 

cleanup:
    sleep(4);
    sleep(5);
    return val; 
}

int foo(int a, int b, int c) {
    int val = a;
    if (a) {
        if (a + b == 1337) 
            goto cleanup;

        sleep(1);
    }
    
    sleep(2); 
    if (b && c) 
        val = b + c;
    sleep(3); 

cleanup:
    sleep(4);
    sleep(5);
    sleep(6); 
    return val;
}

int main(int argc, char** argv) {
    return foo(argv[0][0], argv[0][1], argv[0][2]);
}
