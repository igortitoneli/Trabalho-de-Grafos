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

for instancia in "${instancias[@]}"; 
do
    if [ -e "$arquivo" ]; then
        rm "$arquivo"
    fi
    for x in `seq 1 1 10`;
    do
        ./execGrupo19 $instancia teste.txt 1 >> ./parte_2/resultados/guloso/$instancia.txt
        date >> ./parte_2/resultados/guloso/$instancia.txt
    done
done


for instancia in "${instancias[@]}"; 
do
    for alpha in `seq 0.1 0.1 0.9`;
    do
        arquivo="./parte_2/resultados/guloso_randomizado/alpha_$alpha/$instancia.txt"
        if [ -e "$arquivo" ]; then
            rm "$arquivo"
        fi
        for x in `seq 1 1 10`;
        do  
            echo inicio
            date >> ./parte_2/resultados/guloso/$instancia.txt
            ./execGrupo19 $instancia teste.txt 2 0.3 30 >> "$arquivo"
            date >> ./parte_2/resultados/guloso/$instancia.txt
        done
    done
done

for instancia in "${instancias[@]}"; 
do
    if [ -e "$arquivo" ]; then
        rm "$arquivo"
    fi
    ./execGrupo19 $instancia teste.txt 3 10 100 0.05 0.1 0.15 0.2 0.3 0.4 >> ./parte_2/resultados/guloso_reativo/$instancia.txt
    date >> ./parte_2/resultados/guloso/$instancia.txt
done