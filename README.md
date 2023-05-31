# Project

The repository consists of an executable that implements EliaFano coding and three types of Range Min-Max Datastructures. See the project description in the PDF.

## Compile && Execution

Compile the programm using the Makefile, e.g., `make Release`. You can call the programm by the following way:

```
./ads_programm [pq|rmq] INPUT_FILE OUTPUT_FILE
```

For example for Predecessor:

```
>> ./ads_programm pd test_pd.txt output_test_pd.txt
********************
RESULT algo=pd namepatrick_steil time=0 space=1632
********************
```

or for RangeMinimumQueries:

```
>> ./ads_programm rmq test_rmq.txt output_test_rmq.txt
********************
RESULT algo=rmq namepatrick_steil time=0 space=256
********************
```