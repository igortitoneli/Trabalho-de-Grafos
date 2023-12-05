echo "Resultados Instancia A-n34-k5 em: " > saidaA-n34-k5
date >> saidaA-n34-k5
echo "====================================================================" >> saidaA-n34-k5
echo "   RESULTADOS         Guloso   " >> saidaA-n34-k5
echo "====================================================================" >> saidaA-n34-k5
for x in `seq 1 1 10`
do
    ./execGrupo19 A-n34-k5 saida.txt 1  >> saidaA-n34-k5
    done;
date >> saidaA-n34-k5
