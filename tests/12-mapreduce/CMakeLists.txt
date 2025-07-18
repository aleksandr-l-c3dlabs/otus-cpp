set(target test_mapreduce)

_test(test_mapreduce test_mapreduce.cpp mapper_lib reducer_avg_lib reducer_var_lib)
