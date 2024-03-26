test_storage = executable('test-storage', ['TestStorage.cpp'], include_directories: incdir)
test_distributions_array= executable('test-distributions-array', ['TestDistributionsArray.cpp'], include_directories: incdir)
test_distributions_lru = executable('test-distributions-lru', ['TestDistributionsLRU.cpp'], include_directories: incdir)
test_jena = executable('test-jena', ['TestJena.cpp'], include_directories: incdir)

test('storage', test_storage, timeout: 240)
test('distributions_array', test_distributions_array, timeout: 240)
test('distributions_lru', test_distributions_lru, timeout: 240)
test('test_jena', test_jena, timeout: 240)
