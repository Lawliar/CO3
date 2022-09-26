# at shift's side
＞　AFL_NO_FORKSRV=1 $PWD/afl-fuzz -I $PWD/test/out/symcc -c /dev/ttyACM3 -w 10000000 -t 5000 -i ./test/in -o ./test/out -M master 2> err.txt

# at fuzzing helper size:
> ./target/debug/symcc_fuzzing_helper -a master -v -n symcc -o ~/github/spear/shift/AFL/test/out > output.txt -- /home/lcm/github/spear/spear-code/sym_backend/build_release/qsym_backend/orchestrator -i /home/lcm/github/spear/spear-code/firmware/STMfirmware/<Your firmware>/intermediate_results -s /dev/ttyACM1 -b 10000000