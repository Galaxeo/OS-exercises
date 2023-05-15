## Part 1
Why are some keys lost?
Keys are lost because there is no lock during the multithreading.
Do we need to modify insert in order to not lose the keys?
We do have to modify insert to ensure that locks or other issues do not occur when we attempt to utilize multiple threads.
Do we need to modify retrieve in order to not lose the keys?
We do not have to modify retrieve in order to not lose the keys because retrieval is done all at once, and thus does not have the same issues that insert does.


## Step 2
If two inserts/retrieves are being done on different buckets in parallel, is a lock still needed at the moment, respectively?
If inserts and retrives are being done on different buckets in parallel, then a lock is technically not needed because they are not accessing the same memory space, which does not cause for a lock to be needed. However, if they do end up accessing the same bucket, then a lock will be needed


## Step 3
Does insertion/retrieval time performance significantly change comparing spinlocks and mutexs?
(Utilizng 200 threads) With spinlocks, it took 23.174 seconds for insertion and 2.41 seconds for retrieval, whereas with mutex it took 3.1175 seconds for insertion, and 2.58 seconds for retrieval. Here, it shows that although retrieval does not significantly change, insertion does due to spinlocks wasting CPU cycles while waiting.
