:: delete the old files for the coverage
:: forse da spostare prima??? 
::DEL /S /Q *.gcda
::DEL /S /Q *.gcno
::DEL /S /Q *.gcov
:: compile the test
::g++ --coverage -lgcov -fprofile-arcs -ftest-coverage -I"..\src" -I"..\src-gen" -I"..\googletest" -I"..\googletest\include" -I"..\src-gen-test" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src-gen-test/test.d" -MT"src-gen-test/test.o" -o "src-gen-test/test.o" "../src-gen-test/test.cpp"
g++ -I"..\src" -I"..\src-gen" -I"..\googletest" -I"..\googletest\include" -I"..\src-gen-test" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src-gen-test/test.d" -MT"src-gen-test/test.o" -o "src-gen-test/test.o" "../src-gen-test/test.cpp"
:: link all together
:: g++  -o "mvm_controller_yakindu.exe" ./src-gen-test/Pcv.o ./src-gen-test/Psv.o ./src-gen-test/sc_runner_timed.o ./src-gen-test/test.o  ./src-gen/MVMStateMachineCore.o ./src-gen/MVMStateMachineStopVar.o  ./googletest/src/gtest-death-test.o ./googletest/src/gtest-filepath.o ./googletest/src/gtest-matchers.o ./googletest/src/gtest-port.o ./googletest/src/gtest-printers.o ./googletest/src/gtest-test-part.o ./googletest/src/gtest-typed-test.o ./googletest/src/gtest.o ./googletest/src/gtest_main.o
g++ --coverage -lgcov -fprofile-arcs -ftest-coverage -o "mvm_controller_yakindu.exe" ./src-gen-test/sc_runner_timed.o ./src-gen-test/test.o  ./src-gen/MVMStateMachineCore.o ./googletest/src/gtest-death-test.o ./googletest/src/gtest-filepath.o ./googletest/src/gtest-matchers.o ./googletest/src/gtest-port.o ./googletest/src/gtest-printers.o ./googletest/src/gtest-test-part.o ./googletest/src/gtest-typed-test.o ./googletest/src/gtest.o ./googletest/src/gtest_main.o
.\mvm_controller_yakindu.exe  
:: gcovr -b -v -k -r ..\src-gen .\src-gen
:: lcov --capture --directory . --output-file coverage.info
:: genhtml coverage.info --output-directory out
gcov .\src-gen\MVMStateMachineCore.gcda --branch-counts --branch-probabilities