count=0;while (($count < 5)); do  nohup mpirun -np 64 ./Small_5round $count; ((count = $count +1)); done;
