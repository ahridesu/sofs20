set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_ROOT_DIR}/lib/grp)

add_library(grp_freedatablocks STATIC
    grp_alloc_data_block.cpp
    grp_free_data_block.cpp
    grp_replenish_retrieval_cache.cpp
    grp_deplete_insertion_cache.cpp
)

