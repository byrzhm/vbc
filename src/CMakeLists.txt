set(
    SRCS
    main.c
    lexer.c
    parser.c
)

add_executable(vbc ${SRCS})

target_link_libraries(vbc PRIVATE readline)

