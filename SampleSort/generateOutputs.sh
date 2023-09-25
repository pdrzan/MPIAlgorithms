for i in 4 8 16; do
  mpiexec -np $i ./sampleQuick -n 10000000 >$i-quickNormal_10a7.txt
  mpiexec -np $i ./sampleQuick -n 100000000 > $i-quickNormal_10a8.txt
  mpiexec -np $i ./sampleQuick -n 1000000000 > $i-quickNormal_10a9.txt
  echo "done for $i processors"
done


