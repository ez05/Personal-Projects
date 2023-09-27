#! /bin/sh
testFiles=$(ls $2)
echo testFiles

echo "Expected result is no output for each test." > test_results.txt
echo >> test_results.txt

umFiles=$(ls | grep .um -)
inputFiles=$(ls | grep .0 -)
outputFiles=$(ls | grep .1 -)


for testFile in $testFiles ; do
        # ... some stuff in the loop body
done