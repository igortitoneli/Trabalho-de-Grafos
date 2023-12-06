instancias=(
    "A-n34-k5" 
    "A-n48-k7" 
    "B-n34-k5"
    "B-n50-k8"
    "Golden_1"
    "Golden_17"
    "M-n101-k10"
    "M-n151-k12"
    "X-n101-k25"
    "X-n280-k17"
    )

# for instancia in "${instancias[@]}"; 
# do
#     for x in `seq 1 1 10`;
#     do
#         ./execGrupo19 $instancia teste.txt 1 >> ./parte_2/resultados/guloso/$instancia.txt
#     done
# done


for instancia in "${instancias[@]}"; 
do
    for x in `seq 1 1 10`;
    do
        ./execGrupo19 $instancia teste.txt 2 0.3 1 >> ./parte_2/resultados/guloso_randomizado/$instancia.txt
    done
done

# for instancia in "${instancias[@]}"; 
# do
#     for x in `seq 1 1 10`;
#     do
#         ./execGrupo19 $instancia teste.txt 3 0.3 1 >> ./parte_2/resultados/guloso_reativo/$instancia.txt
#     done
# done