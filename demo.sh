user_program/master 10 input/target_file_1 input/target_file_2  input/target_file_3 input/target_file_4 input/target_file_5 input/target_file_6 input/target_file_7 input/target_file_8 input/target_file_9 input/target_file_10 fcntl
user_program/slave 10 output/target_file_1_fcntl output/target_file_2_fcntl output/target_file_3_fcntl output/target_file_4_fcntl output/target_file_5_fcntl output/target_file_6_fcntl output/target_file_7_fcntl output/target_file_8_fcntl output/target_file_9_fcntl output/target_file_10_fcntl fcntl 127.0.0.1
for ((i=1;i<=10;++i)); do diff input/target_file_$i output/target_file_$i"_fcntl"; done

user_program/master 10 input/target_file_1 input/target_file_2  input/target_file_3 input/target_file_4 input/target_file_5 input/target_file_6 input/target_file_7 input/target_file_8 input/target_file_9 input/target_file_10 mmap
user_program/slave 10 output/target_file_1_mmap output/target_file_2_mmap output/target_file_3_mmap output/target_file_4_mmap output/target_file_5_mmap output/target_file_6_mmap output/target_file_7_mmap output/target_file_8_mmap output/target_file_9_mmap output/target_file_10_mmap mmap 127.0.0.1
for ((i=1;i<=10;++i)); do diff input/target_file_$i output/target_file_$i"_mmap"; done

user_program/master 1 input/target_file fcntl
user_program/slave 1 output/tar_fcntl fcntl 127.0.0.1
diff input/target_file output/tar_fcntl

user_program/master 1 input/target_file mmap
user_program/slave 1 output/tar_mmap mmap 127.0.0.1
diff input/target_file output/tar_mmap

user_program/master 5 input/myfile1 input/myfile2 input/myfile3 input/myfile4 input/myfile5 fcntl
user_program/slave 5 output/myfile1_fcntl output/myfile2_fcntl output/myfile3_fcntl output/myfile4_fcntl output/myfile5_fcntl fcntl 127.0.0.1
for ((i=1;i<=5;++i)); do diff input/myfile$i output/myfile$i"_fcntl"; done

user_program/master 5 input/myfile1 input/myfile2 input/myfile3 input/myfile4 input/myfile5 mmap
user_program/slave 5 output/myfile1_mmap output/myfile2_mmap output/myfile3_mmap output/myfile4_mmap output/myfile5_mmap mmap 127.0.0.1
for ((i=1;i<=5;++i)); do diff input/myfile$i output/myfile$i"_mmap"; done

