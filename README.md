# OperatingSystemsA1
HOW TO RUN:
    {executable} -{method} {thread_amount} {minimum_allocation} {maximum_allocation} {minimum_allocation_size} {maximum_allocation_size} {seed}
        
        {method} = f/b/w (first/best/worst)
        
        {thread_amount} = number of threads to test
        
        {minimum_allocation} {maximum_allocation} = THe min and max ammount of allocations per thread

        {minimum_allocation_size} {maximum_allocation_size} = the min and max size of any allocation

        {seed} = seed for pseudo random numbers

        Example: ./allocator -f 4 50 200 1 20 5

DESCRIPTION:
    Code creates pseudo random list of integers, passing one of these lists to each thread to allocate

    Allocator is first filled with an initiliser, which fills memory based on the same input as the tests before tests are run