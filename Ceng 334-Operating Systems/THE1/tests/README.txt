Every folder contains an input, output and src folder. The input folder contains
the input that will be provided to your solution. The output is the resulting
outputs for Map model and MapReduce model. The file output_mr.txt is the output for
the MapReduce Model and the file output_m.txt is the output for the Map model.
The src folder contains the source codes for the mapper and reducer for the 
task. After compiling the mapper and reducer using make, you can give the path
of the mapper and reducer executables as arguments to your program.

In the word_count example provided, the tasks were run on 2 mappers and reducers.
In sort 2 mappers and reducers for the short input and 50 mappers and reducers
for the long input are used.

Mapper output a the long input may be very long and scrambled. In order to check the
correctnes of your output you can sort the output by character as following:

./mapreduce 50 Sort_Mapper < ../input/inp_long.txt | grep -o . | sort |tr -d " \n" > student_out.txt
cat ../output/out_long_m.txt | grep -o . | sort |tr -d " \n" > out_sorted.txt
diff ../output/out_sorted.txt student_out.txt


The last line should return empty.


