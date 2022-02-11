# Multithread-Matrix-Multiplication-in-C

Multiplication of matrix does take time surely. Time complexity of matrix multiplication is O(n^3) using normal matrix multiplication. And Strassen algorithm improves it and its time complexity is O(n^(2.8074)).

But, Is there any way to improve the performance of matrix multiplication using the normal method. 

Multi-threading can be done to improve it. In multi-threading, instead of utilizing a single core of your processor, we utilizes all or more core to solve the problem.
We create different threads, each thread evaluating some part of matrix multiplication. 

Depending upon the number of cores your processor has, you can create the number of threads required. Although you can create as many threads as you need, a better way is to create each thread for one core.

In second approach,we create a separate thread for each element in resultant matrix. Using pthread_exit() we return computed value from each thread which is collected by pthread_join(). This approach does not make use of any global variables.

![image](https://user-images.githubusercontent.com/15093808/153595020-f942aa50-f764-4877-8b74-546b9cf31261.png)

Examples: 
```
Input : 
Matrix A
 1 0 0
 0 1 0
 0 0 1

Matrix B
 2 3 2
 4 5 1
 7 8 6

Output : Multiplication of A and B
2 3 2
4 5 1
7 8 6
