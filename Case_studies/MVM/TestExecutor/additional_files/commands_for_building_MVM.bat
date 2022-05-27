make all 
:: g++ -I"..\src" -I"..\src-gen" -I"..\googletest" -I"..\googletest\include" -I"..\src-gen-test" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src-gen-test/Pcv.d" -MT"src-gen-test/Pcv.o" -o "src-gen-test/Pcv.o" "../src-gen-test/Pcv.cc"
:: g++ -I"..\src" -I"..\src-gen" -I"..\googletest" -I"..\googletest\include" -I"..\src-gen-test" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src-gen-test/Psv.d" -MT"src-gen-test/Psv.o" -o "src-gen-test/Psv.o" "../src-gen-test/Psv.cc"
g++ -I"..\src" -I"..\src-gen" -I"..\googletest" -I"..\googletest\include" -I"..\src-gen-test" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src-gen-test/sc_runner_timed.d" -MT"src-gen-test/sc_runner_timed.o" -o "src-gen-test/sc_runner_timed.o" "../src-gen-test/sc_runner_timed.cpp"
g++ -I"..\src" -I"..\src-gen" -I"..\googletest" -I"..\googletest\include" -I"..\src-gen-test" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src-gen-test/test.d" -MT"src-gen-test/test.o" -o "src-gen-test/test.o" "../src-gen-test/test.cpp"
g++ -I"..\src" -I"..\src-gen" -I"..\googletest" -I"..\googletest\include" -I"..\src-gen-test" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src-gen/MVMStateMachineCore.d" -MT"src-gen/MVMStateMachineCore.o" -o "src-gen/MVMStateMachineCore.o" "../src-gen/MVMStateMachineCore.cpp"
g++ -I"..\src" -I"..\src-gen" -I"..\googletest" -I"..\googletest\include" -I"..\src-gen-test" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"src-gen/MVMStateMachineStopVar.d" -MT"src-gen/MVMStateMachineStopVar.o" -o "src-gen/MVMStateMachineStopVar.o" "../src-gen/MVMStateMachineStopVar.cpp"
g++ -I"..\src" -I"..\src-gen" -I"..\googletest" -I"..\googletest\include" -I"..\src-gen-test" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"googletest/src/gtest-death-test.d" -MT"googletest/src/gtest-death-test.o" -o "googletest/src/gtest-death-test.o" "../googletest/src/gtest-death-test.cc"
g++ -I"..\src" -I"..\src-gen" -I"..\googletest" -I"..\googletest\include" -I"..\src-gen-test" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"googletest/src/gtest-filepath.d" -MT"googletest/src/gtest-filepath.o" -o "googletest/src/gtest-filepath.o" "../googletest/src/gtest-filepath.cc"
g++ -I"..\src" -I"..\src-gen" -I"..\googletest" -I"..\googletest\include" -I"..\src-gen-test" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"googletest/src/gtest-matchers.d" -MT"googletest/src/gtest-matchers.o" -o "googletest/src/gtest-matchers.o" "../googletest/src/gtest-matchers.cc"
g++ -I"..\src" -I"..\src-gen" -I"..\googletest" -I"..\googletest\include" -I"..\src-gen-test" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"googletest/src/gtest-port.d" -MT"googletest/src/gtest-port.o" -o "googletest/src/gtest-port.o" "../googletest/src/gtest-port.cc"
g++ -I"..\src" -I"..\src-gen" -I"..\googletest" -I"..\googletest\include" -I"..\src-gen-test" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"googletest/src/gtest-printers.d" -MT"googletest/src/gtest-printers.o" -o "googletest/src/gtest-printers.o" "../googletest/src/gtest-printers.cc"
g++ -I"..\src" -I"..\src-gen" -I"..\googletest" -I"..\googletest\include" -I"..\src-gen-test" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"googletest/src/gtest-test-part.d" -MT"googletest/src/gtest-test-part.o" -o "googletest/src/gtest-test-part.o" "../googletest/src/gtest-test-part.cc"
g++ -I"..\src" -I"..\src-gen" -I"..\googletest" -I"..\googletest\include" -I"..\src-gen-test" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"googletest/src/gtest-typed-test.d" -MT"googletest/src/gtest-typed-test.o" -o "googletest/src/gtest-typed-test.o" "../googletest/src/gtest-typed-test.cc"
g++ -I"..\src" -I"..\src-gen" -I"..\googletest" -I"..\googletest\include" -I"..\src-gen-test" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"googletest/src/gtest.d" -MT"googletest/src/gtest.o" -o "googletest/src/gtest.o" "../googletest/src/gtest.cc"
g++ -I"..\src" -I"..\src-gen" -I"..\googletest" -I"..\googletest\include" -I"..\src-gen-test" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"googletest/src/gtest_main.d" -MT"googletest/src/gtest_main.o" -o "googletest/src/gtest_main.o" "../googletest/src/gtest_main.cc"
:: g++  -o "mvm_controller_yakindu.exe" ./src-gen-test/Pcv.o ./src-gen-test/Psv.o ./src-gen-test/sc_runner_timed.o ./src-gen-test/test.o  ./src-gen/MVMStateMachineCore.o ./src-gen/MVMStateMachineStopVar.o  ./googletest/src/gtest-death-test.o ./googletest/src/gtest-filepath.o ./googletest/src/gtest-matchers.o ./googletest/src/gtest-port.o ./googletest/src/gtest-printers.o ./googletest/src/gtest-test-part.o ./googletest/src/gtest-typed-test.o ./googletest/src/gtest.o ./googletest/src/gtest_main.o
g++  -o "mvm_controller_yakindu.exe" ./src-gen-test/sc_runner_timed.o ./src-gen-test/test.o  ./src-gen/MVMStateMachineCore.o ./src-gen/MVMStateMachineStopVar.o  ./googletest/src/gtest-death-test.o ./googletest/src/gtest-filepath.o ./googletest/src/gtest-matchers.o ./googletest/src/gtest-port.o ./googletest/src/gtest-printers.o ./googletest/src/gtest-test-part.o ./googletest/src/gtest-typed-test.o ./googletest/src/gtest.o ./googletest/src/gtest_main.o
.\mvm_controller_yakindu.exe   