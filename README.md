# OperatingSystemsA1
HOW TO RUN:
    {executable} -{method} {test_file_01} {test_file_02}... 
        
        {method} = f/b/w (first/best/worst)
        
        {test_file} = files of strings to load into allocator minimum of 1 file but can include many
                      test files must be in 'test' folder
        
        Example: ./allocator -f experiment_input_01

DESCRIPTION:
    Code will load in any file of strings seperated by line you give it and allocate the memory needed to store
    each string, set that memory to be the string as a character array, then it will deallocate each string.

    Loading in multiple files will allow you to run test on an allocator with the memort chunks created from
    the previous tests.

    First file loaded in will always need to create new memory chunks for each string as none would have been 
    made before.

    Strings will always be one byte longer than the length of the string to include the \0