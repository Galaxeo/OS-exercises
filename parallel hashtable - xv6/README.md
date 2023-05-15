## Step 1
Keys are lost because there is no lock during the multithreading.

We do not have to modify retrieve in order to not lose the keys because retrieval is done all at once, and thus does not have the same issues that insert does.

We do have to modify insert to ensure that locks or other issues do not occur when we attempt to utilize multiple threads.

## Step 2
If insertes and retrives are being done on different buckets in parallel, then a lock is technically not needed because they are not accessing the same memory space, which does not cause for a lock to be needed. However, if they do end up accessing the same bucket, then a lock will be needed


## Step 3
With spinlocks, it took 23.174 seconds for insertion and 2.41 seconds for retrieval, whereas with mutex it took 3.1175 seconds for insertion, and 2.58 seconds for retrieval. Here, it shows that although retrieval does not significantly change, insertion does due to spinlocks wasting CPU cycles while waiting.
