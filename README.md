# Project
The repository consists of an executable that implements Elias-Fano encoding and three types of Range Minimum Datastructures. See the project description in the PDF.
## Compile && Execution
Compile the programm using the Makefile (call `make Release`). You can then call the programm by the following way:
```
./ads_programm [pq|rmq] INPUT_FILE OUTPUT_FILE
```
E.g., Predecessor:
```
>> ./ads_programm pd Experiments/predecessor_examples/predecessor_example_1.txt Experiments/pred_test1.txt
RESULT algo=pd name=patrick_steil time=2909 space=376000160
```
or Range Minimum Queries:
```
>> ./ads_programm rmq Experiments/rmq_examples/rmq_example_1.txt Experiments/rmq_test1.txt
RESULT algo=rmq name=patrick_steil time=2266 space=32000000
```
