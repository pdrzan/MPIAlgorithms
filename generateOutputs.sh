echo "removendo limite de tempo"

ulimit -t unlimited  

echo "começando a gerar resultados"
echo "para 16 processadores"

mpiexec -np 16 sample 100000000 > Results/'16 Processors'/sampleSort_binary_16Processors_10a7.txt
mpiexec -np 16 sample 1000000000 > Results/'16 Processors'/sampleSort_binary_16Processors_10a8.txt
mpiexec -np 16 sample 10000000000 > Results/'16 Processors'/sampleSort_binary_16Processors_10a9.txt

echo "terminou com sucesso"
echo ""
echo "para 8 processadores"

mpiexec -np 8 sample 100000000 > Results/'8 Processors'/sampleSort_binary_8Processors_10a7.txt
mpiexec -np 8 sample 1000000000 > Results/'8 Processors'/sampleSort_binary_8Processors_10a8.txt
mpiexec -np 8 sample 10000000000 > Results/'8 Processors'/sampleSort_binary_8Processors_10a9.txt

echo "terminou com sucesso"
echo ""
echo "para 4 processadores"

mpiexec -np 4 sample 100000000 > Results/'4 Processors'/sampleSort_binary_4Processors_10a7.txt
mpiexec -np 4 sample 1000000000 > Results/'4 Processors'/sampleSort_binary_4Processors_10a8.txt
mpiexec -np 4 sample 10000000000 > Results/'4 Processors'/sampleSort_binary_4Processors_10a9.txt

echo "terminou com sucesso"
echo "Tudo terminado com sucesso"