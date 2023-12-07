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

maxIter=3


# for instancia in "${instancias[@]}"; do
#     arquivo="./parte_2/resultados/guloso/$instancia.txt"

#     if [ -e "$arquivo" ]; then
#         rm "$arquivo"
#     fi

#     inicio=$(date '+%S.%N')
#     valores=0
#     total_resultado=0

#     for x in $(seq 1 1 $maxIter); do
#         resultado=$(./execGrupo19 "$instancia" teste.txt 1)
#         echo "$resultado" >> "$arquivo"

#         total=$(echo "$resultado" | awk '/total -/ {sum += $3} END {printf "%.3f", sum}')
#         valores=$(awk -v valores="$valores" -v total="$total" 'BEGIN {print valores + total}')

#         total_resultado=$(awk -v total_resultado="$total_resultado" -v total="$total" 'BEGIN {print total_resultado + total}')
#         echo >> "$arquivo"
#     done

#     fim=$(date '+%S.%N')
#     tempo_total=$(awk "BEGIN {print $fim - $inicio}")
#     echo "Tempo total: $tempo_total segundos" >> "$arquivo"
    
#     media=$(awk -v valores="$valores" 'BEGIN {print valores / '$maxIter'}')
#     echo "Média total para $instancia: $media" >> "$arquivo"
# done


# alphas=(0.05 0.10 0.15 0.30 0.50)

# for instancia in "${instancias[@]}"; 
# do
#     for alpha in "${alphas[@]}";
#     do

#         arquivo="./parte_2/resultados/guloso_randomizado/alpha_$alpha/$instancia.txt"
#         if [ -e "$arquivo" ]; then
#             rm "$arquivo"
#         fi
#         valores=0
#         total_resultado=0
        
#         inicio=$(date '+%S.%N')
        
#         for x in `seq 1 1 $maxIter`;
#         do  
#             resultado=$(./execGrupo19 $instancia teste.txt 2 $alpha 30)
#             echo "$resultado" >> "$arquivo"

#             total=$(echo "$resultado" | awk '/total -/ {sum += $3} END {printf "%.3f", sum}')
#             valores=$(awk -v valores="$valores" -v total="$total" 'BEGIN {print valores + total}')
#             total_resultado=$(awk -v total_resultado="$total_resultado" -v total="$total" 'BEGIN {print total_resultado + total}')
#             echo >> "$arquivo"
#         done
#         fim=$(date '+%S.%N')
#         tempo_total=$(awk "BEGIN {print $fim - $inicio}")
#         echo "Tempo total: $tempo_total segundos" >> "$arquivo"

#         media=$(awk -v valores="$valores" 'BEGIN {print valores / '$maxIter'}')
#         echo "Média total para $instancia: $media" >> "$arquivo"
#     done
# done

for instancia in "${instancias[@]}"; 
do
    arquivo="./parte_2/resultados/guloso_reativo/$instancia.txt"
    if [ -e "$arquivo" ]; then
        rm "$arquivo"
    fi
    echo >> "$arquivo"

    inicio=$(date '+%S.%N')
    resultado=$(./execGrupo19 $instancia teste.txt 3 250 2500 0.05 0.10 0.15 0.30 0.50)
    fim=$(date '+%S.%N')

    echo "$resultado" >> "$arquivo"
    echo >> "$arquivo"
    
    total=$(echo "$resultado" | awk '/total -/ {sum += $3} END {printf "%.3f", sum}')
    valores=$(awk -v valores="$valores" -v total="$total" 'BEGIN {print valores + total}')
    total_resultado=$(awk -v total_resultado="$total_resultado" -v total="$total" 'BEGIN {print total_resultado + total}')
        
    tempo_total=$(awk "BEGIN {print $fim - $inicio}")
    echo "Tempo total: $tempo_total segundos" >> "$arquivo"

    media=$(awk -v valores="$valores" 'BEGIN {print valores / 10}')
    echo "Média total para $instancia: $media" >> "$arquivo"
done    