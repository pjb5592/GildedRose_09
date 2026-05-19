add_test([=[GildedRoseTest.Foo]=]  D:/DEV/GildedRose_Prompting_09/build/gilded_rose_test.exe [==[--gtest_filter=GildedRoseTest.Foo]==] --gtest_also_run_disabled_tests)
set_tests_properties([=[GildedRoseTest.Foo]=]  PROPERTIES DEF_SOURCE_LINE D:/DEV/GildedRose_Prompting_09/cpp/test/GildedRoseTest.cpp:4 WORKING_DIRECTORY D:/DEV/GildedRose_Prompting_09/build SKIP_REGULAR_EXPRESSION [==[\[  SKIPPED \]]==])
set(  gilded_rose_test_TESTS GildedRoseTest.Foo)
