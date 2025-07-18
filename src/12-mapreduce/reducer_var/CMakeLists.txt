_task(reducer_var OFF "mapreduce")

set_target_properties(reducer_var PROPERTIES OUTPUT_NAME "reducer")

install(TARGETS reducer_var RUNTIME DESTINATION /usr/lib/mapreduce/variance COMPONENT "mapreduce")