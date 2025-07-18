_task(reducer_avg OFF "mapreduce")

set_target_properties(reducer_avg PROPERTIES OUTPUT_NAME "reducer")

install(TARGETS reducer_avg RUNTIME DESTINATION /usr/lib/mapreduce/average COMPONENT "mapreduce")