add_bond_test (event event.bond event.cpp COMM)

target_use_cxx11 (event)
target_compile_definitions (event PRIVATE -D_WIN32_WINNT=0x0600)
