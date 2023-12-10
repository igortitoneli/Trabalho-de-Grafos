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

maxIter=30


for instancia in "${instancias[@]}"; do
    arquivo="./parte_2/resultados/guloso/$instancia.txt"

    if [ -e "$arquivo" ]; then
        rm "$arquivo"
    fi
    echo >> "$arquivo"

    valores=0
    total_resultado=0

    for x in $(seq 1 1 $maxIter); do

        resultado=$(./execGrupo19 "$instancia" teste.txt 1)
        echo "$resultado" >> "$arquivo"
        
        total=$(echo "$resultado" | awk '/total -/ {sum += $3} END {printf "%.3f", sum}')
        valores=$(awk -v valores="$valores" -v total="$total" 'BEGIN {print valores + total}')
        total_resultado=$(awk -v total_resultado="$total_resultado" -v total="$total" 'BEGIN {print total_resultado + total}')
        
        tempo=$(echo "$resultado" | awk '/Tempo em ms:/ {print $4}')
        tempo_total=$(awk -v valores="$valores" -v total="$tempo" 'BEGIN {print tempo + total}')
        tempo_resultado=$(awk -v tempo_resultado="$tempo_resultado" -v tempo="$tempo" 'BEGIN {print tempo_resultado + tempo}')
        

        echo >> "$arquivo"      
    done

    media=$(awk -v valores="$valores" 'BEGIN {print valores / '$maxIter'}')
    echo "Média total para $instancia: $media" >> "$arquivo"


    tempo_medio=$(awk -v valores="$tempo_resultado" 'BEGIN {print valores / '$maxIter'}')
    echo "Tempo medio para $instancia: $tempo_medio" >> "$arquivo"
    
    tempo_medio=0
    tempo_total=0
    tempo_resultado=0
done


alphas=(0.05 0.10 0.15 0.30 0.50)

for instancia in "${instancias[@]}"; 
do
    for alpha in "${alphas[@]}";
    do

        arquivo="./parte_2/resultados/guloso_randomizado/alpha_$alpha/$instancia.txt"
        if [ -e "$arquivo" ]; then
            rm "$arquivo"
        fi
        echo >> "$arquivo"
        valores=0
        total_resultado=0
        
        for x in `seq 1 1 $maxIter`;
        do  
            resultado=$(./execGrupo19 $instancia teste.txt 2 $alpha 1000)
            echo "$resultado" >> "$arquivo"

            total=$(echo "$resultado" | awk '/total -/ {sum += $3} END {printf "%.3f", sum}')
            valores=$(awk -v valores="$valores" -v total="$total" 'BEGIN {print valores + total}')
            total_resultado=$(awk -v total_resultado="$total_resultado" -v total="$total" 'BEGIN {print total_resultado + total}')

            tempo=$(echo "$resultado" | awk '/Tempo em ms:/ {print $4}')
            tempo_total=$(awk -v valores="$valores" -v total="$tempo" 'BEGIN {print tempo + total}')
            tempo_resultado=$(awk -v tempo_resultado="$tempo_resultado" -v tempo="$tempo" 'BEGIN {print tempo_resultado + tempo}')
        done

        media=$(awk -v valores="$valores" 'BEGIN {print valores / '$maxIter'}')
        echo "Média total para $instancia: $media" >> "$arquivo"

        tempo_medio=$(awk -v valores="$tempo_resultado" 'BEGIN {print valores / '$maxIter'}')
        echo "Tempo medio para $instancia: $tempo_medio" >> "$arquivo"
    done
done

for instancia in "${instancias[@]}"; 
do
    arquivo="./parte_2/resultados/guloso_reativo/$instancia.txt"
    if [ -e "$arquivo" ]; then
        rm "$arquivo"
    fi
    echo >> "$arquivo"

    for x in `seq 1 1 $maxIter`;
    do  
        resultado=$(./execGrupo19 $instancia teste.txt 3 250 2500 0.05 0.10 0.15 0.30 0.50)
        echo "$resultado" >> "$arquivo"
        echo >> "$arquivo"
        
        total=$(echo "$resultado" | awk '/total -/ {sum += $3} END {printf "%.3f", sum}')
        valores=$(awk -v valores="$valores" -v total="$total" 'BEGIN {print valores + total}')
        total_resultado=$(awk -v total_resultado="$total_resultado" -v total="$total" 'BEGIN {print total_resultado + total}')


        tempo=$(echo "$resultado" | awk '/Tempo em ms:/ {print $4}')
        tempo_total=$(awk -v valores="$valores" -v total="$tempo" 'BEGIN {print tempo + total}')
        tempo_resultado=$(awk -v tempo_resultado="$tempo_resultado" -v tempo="$tempo" 'BEGIN {print tempo_resultado + tempo}')

    done

    tempo_medio=$(awk -v valores="$tempo_resultado" 'BEGIN {print valores / '$maxIter'}')
    echo "Tempo medio para $instancia: $tempo_medio" >> "$arquivo"
done    