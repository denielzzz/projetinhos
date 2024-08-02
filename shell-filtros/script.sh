#!/bin/bash

# função que retira todas as linhas que possuem equivalência na coluna 11
tira_equivalencia()
{
    awk -F, '$11 != "EQUIVALENCIA" {print}' historico-alg1_SIGA_ANONIMIZADO.csv
}

# função para remover todas as linhas que façam parte do 2o semestre de 2022 
remove_2semestre_2022()
{
    sed '/2,2022/d' historico-alg1_SIGA_ANONIMIZADO.csv
}

# função para calcular a quantidade de individuos únicos por status
calcula_individuos_status()
{
    tira_equivalencia | awk -F, 'NR > 1 {print $1,$10}' | sort -u | cut -d' ' -f2 | sort | uniq -c
    echo
}


# função para calcular o máximo de vezes que um indivíduo cursou a disciplina antes de ser aprovado
# e quantos individuos possuem o mesmo máximo de vezes cursadas
calcula_max_cursadas_ate_aprovacao() 
{
  # vetor de matriculas
  matri=($(tira_equivalencia | awk -F, 'NR > 1 {print $1}'))
  # vetor de status
  status=($(tira_equivalencia | awk -F, 'NR > 1 {print $10}'))
  aux_max=1
  max=1
  vezes=(1 0 0 0 0 0)
  
  for((i=1; i<${#matri[@]}; i++))
  do 
    # se a matricula for igual a anterior e o status for diferente de aprovado, aumenta o aux_max
    if [ ${matri[i]} == ${matri[$((${i}-1))]} ] && [ ${status[i]} != "Aprovado" ]
    then
      aux_max=$((aux_max+1))
    # se a matricula for igual a anterior e o status for aprovado, aumenta o aux_max e o vetor de vezes
    elif [ ${matri[i]} == ${matri[$((${i}-1))]} ] && [ ${status[i]} == "Aprovado" ]
    then
      aux_max=$((aux_max+1))
      vezes[$((${aux_max}-1))]=$((${vezes[$((${aux_max}-1))]}+1))
      # se o aux_max for maior que o max, o max recebe o aux_max
      if [ ${aux_max} -gt ${max} ]
      then
        max=${aux_max}
      fi
      aux_max=1
    # se a matricula for diferente da anterior e o status for aprovado, aumenta o vetor de vezes
    elif [ ${matri[i]} != ${matri[$((${i}-1))]} ] && [ ${status[i]} == "Aprovado" ]
    then
      aux_max=1
      vezes[$((${aux_max}-1))]=$((${vezes[$((${aux_max}-1))]}+1))
    else
      aux_max=1
    fi
  done
  echo "Máximo de vezes cursadas até a aprovação: ${max}"
  for((i=0; i<${#vezes[@]}; i++))
  do
    echo "Número de indivíduos com o mesmo número máximo de vezes cursadas igual a $((i+1)): ${vezes[i]}"
  done
  echo
}

# Função para calcular a porcentagem de aprovação/reprovação por ano
calcula_porcentagem_aprovacao_reprovacao_por_ano() 
{
  # vetor da quantidade de inscrições anuais apartir de 2013
  inscr_anuais=($(tira_equivalencia | awk -F, ' NR > 1 && $5 != 2012 && $5 != 2011 {print $5}' | sort | uniq -c | awk '{print $1}'))
  # vetor da quantidade de aprovações anuais apartir de 2013
  aprov_anuais=($(tira_equivalencia | awk -F, ' $10 == "Aprovado" && $5 != 2012 && $5 != 2011 {print $5}' | sort | uniq -c | awk '{print $1}'))
  # vetor da quantidade de reprovações anuais apartir de 2013
  reprov_anuais=($(tira_equivalencia | awk -F, ' ($10 == "Reprovado" || $10 == "R-nota" || $10 == "R-freq") && $5 != 2012 && $5 != 2011 {print $5}' | sort | uniq -c | awk '{print $1}'))
  ano=2013
  # imprime as porcentagens de aprovações e reprovações dos anos de 2011 e 2012
  printf "Porcentagem de aprovações do ano 2011: 100%%\n"
  printf "Porcentagem de reprovações do ano 2011: 0%%\n\n"
  printf "Porcentagem de aprovações do ano 2012: 0%%\n"
  printf "Porcentagem de reprovações do ano 2012: 100%%\n\n"
  # imprime as porcentagens de aprovações e reprovações dos anos de 2013 a 2022
  for((i=0; i<${#inscr_anuais[@]}; i++))
  do
      printf "Porcentagem de aprovações do ano %d: %d%%\n" $ano $((${aprov_anuais[i]}*100/${inscr_anuais[i]}))
      printf "Porcentagem de reprovações do ano %d: %d%%\n\n" $ano $((${reprov_anuais[i]}*100/${inscr_anuais[i]}))
      ano=$((${ano}+1))
  done
}

calcula_media_dos_aprovados()
{
  # vetor de anos dos aprovados
  anos=($(tira_equivalencia | awk -F, '$10 == "Aprovado" {print $5,$8}' | sort | awk '{print $1}'))
  # vetor de notas dos aprovados
  notas=($(tira_equivalencia | awk -F, '$10 == "Aprovado" {print $5,$8}' | sort | awk '{print $2}'))
  total=0
  anual=0
  div_anual=0
  div_total=${#anos[@]}

  for((i=0; i<$((${#anos[@]}-1)); i++))
  do
    div_anual=$((${div_anual}+1))
    total=$((${total}+${notas[i]}))
    anual=$((${anual}+${notas[i]}))
    # se o ano for diferente do ano seguinte, calcula a média do ano e zera os contadores
    if [ "${anos[i]}" != "${anos[$((${i}+1))]}" ]
    then
      printf "Nota média dos aprovados do ano %d: %d\n" ${anos[${i}]} $((${anual}/${div_anual}))
      anual=0
      div_anual=0
    fi
  done
  # calcula a média do ano de 2022
  div_anual=$((${div_anual}+1))
  total=$((${total}+${notas[${#notas[@]}-1]}))
  anual=$((${anual}+${notas[${#notas[@]}-1]}))
  printf "Nota média dos aprovados do ano %d: %d\n" 2022 $((${anual}/${div_anual}))

  printf "Nota média total dos aprovados: %d\n\n" $((${total}/${div_total}))
}

calcula_media_dos_reprovados()
{
  # vetor de anos dos reprovados
  anos=($(awk -F, '$10 == "R-nota" {print $5,$8}' historico-alg1_SIGA_ANONIMIZADO.csv | sort | awk '{print $1}'))
  # vetor de notas dos reprovados
  notas=($(awk -F, '$10 == "R-nota" {print $5,$8}' historico-alg1_SIGA_ANONIMIZADO.csv | sort | awk '{print $2}'))
  total=0
  anual=0
  div_anual=0
  div_total=${#notas[@]}

  for((i=0; i<$((${#anos[@]}-1)); i++))
  do
    div_anual=$((${div_anual}+1))
    total=$((${total}+${notas[i]}))
    anual=$((${anual}+${notas[i]}))
    # se o ano for diferente do ano seguinte, calcula a média do ano e zera os contadores
    if [ "${anos[i]}" != "${anos[$((${i}+1))]}" ]
    then
      printf "Nota média dos reprovados do ano %d: %d\n" ${anos[${i}]} $((${anual}/${div_anual}))
      anual=0
      div_anual=0
    fi
  done
  # calcula a média do ano de 2022
  div_anual=$((${div_anual}+1))
  total=$((${total}+${notas[${#notas[@]}-1]}))
  anual=$((${anual}+${notas[${#notas[@]}-1]}))
  printf "Nota média dos reprovados do ano %d: %d\n" 2022 $((${anual}/${div_anual}))

  printf "Nota média total dos reprovados: %d\n\n" $((${total}/${div_total}))
}

calcula_media_da_frequencia_reprovados()
{
  # vetor de anos dos reprovados
  anos=($(awk -F, '$10 == "R-nota" {print $5,$9}' historico-alg1_SIGA_ANONIMIZADO.csv | sort | awk '{print $1}'))
  # vetor de frequencia dos reprovados
  freq=($(awk -F, '$10 == "R-nota" {print $5,$9}' historico-alg1_SIGA_ANONIMIZADO.csv | sort | awk '{print $2}'))
  total=0
  anual=0
  div_anual=0
  div_total=${#freq[@]}

  for((i=0; i<$((${#anos[@]}-1)); i++))
  do
    div_anual=$((${div_anual}+1))
    total=$((${total}+${freq[i]}))
    anual=$((${anual}+${freq[i]}))
    # se o ano for diferente do ano seguinte, calcula a média do ano e zera os contadores
    if [ "${anos[i]}" != "${anos[$((${i}+1))]}" ]
    then
      printf "Frequencia média dos reprovados do ano %d: %d\n" ${anos[${i}]} $((${anual}/${div_anual}))
      anual=0
      div_anual=0
    fi
  done
  # calcula a média do ano de 2022
  div_anual=$((${div_anual}+1))
  total=$((${total}+${freq[${#freq[@]}-1]}))
  anual=$((${anual}+${freq[${#freq[@]}-1]}))
  printf "Frequencia média dos reprovados do ano %d: %d\n" 2022 $((${anual}/${div_anual}))

  printf "Frequencia média total dos reprovados: %d\n\n" $((${total}/${div_total}))
}

calcula_porcentagem_de_evasoes()
{
  # vetor da quantidade de inscrições anuais
  inscr_anuais=($(tira_equivalencia | awk -F, ' NR > 1 {print $5}' | sort | uniq -c | awk '{print $1}'))
  # vetor da quantidade de inscrições totais
  inscr_totais=$(tira_equivalencia | awk 'NR > 1 { print }' | wc -l)
  # vetor da quantidade de evasões anuais
  evasoes_anuais=($(tira_equivalencia | awk -F, ' $14 == "Evasão" {print $5}' | sort | uniq -c | awk '{print $1}'))
  # vetor da quantidade de evasões totais
  evasoes_totais=$(grep -c "Evasão" historico-alg1_SIGA_ANONIMIZADO.csv)
  # vetor dos anos que contém evasões
  anos_evasoes=($(tira_equivalencia | awk -F, ' $14 == "Evasão" {print $5}' | sort | uniq -c | awk '{print $2}'))
  ano=2011
  cont=0

  for((i=0; i<${#inscr_anuais[@]}; i++))
  do
    # se o ano for igual ao ano que contém evasões, calcula a porcentagem de evasões
    if [ $ano == ${anos_evasoes[cont]} ]
    then
      printf "Porcentagem de evasões do ano %d: %d%%\n" $ano $((${evasoes_anuais[cont]}*100/${inscr_anuais[i]}))
      cont=$((${cont}+1))
    else
      printf "Porcentagem de evasões do ano %d: 0%%\n" $ano
    fi
    ano=$((${ano}+1))
  done
 
  printf "Porcentagem total de evasões: %d%%\n\n" $((${evasoes_totais}*100/${inscr_totais}))
}

# função para calcular a porcentagem de cancelamentos durante a pandemia e comparar com anos anteriores
calcula_porcentagem_cancelamentos_pandemia()
{
  cancel_pand=$(tira_equivalencia | awk -F, '$10 == "Cancelado" && ($5 == 2020 || $5 == 2021) {print $5}' | wc -l)
  cancel_totais=$(tira_equivalencia | awk -F, '$10 == "Cancelado" && $5 < 2020 {print $5}' | wc -l)
  printf "Número de cancelamentos antes da pandemia: %d\n" ${cancel_totais}
  printf "Número de cancelamentos durante a pandemia: %d\n" ${cancel_pand}
  printf "Aumento total da porcentagem de cancelamentos durante a pandemia: %d%%\n\n" $((${cancel_pand}*100/${cancel_totais}))
}

# função para calcular a porcentagem de notas durante a pandemia e comparar com anos anteriores
calcula_porcentagem_notas_pandemia()
{
  notas_pand=($(tira_equivalencia | awk -F, '$5 == 2020 || $5 == 2021 {print $8}'))
  notas_totais=($(tira_equivalencia | awk -F, '$5 != 2020 && $5 != 2021 {print $8}'))
  soma_pand=0
  soma_totais=0
  for nota in ${notas_pand[@]}
  do
    soma_pand=$((${soma_pand}+${nota}))
  done
  for nota2 in ${notas_totais[@]}
  do
    soma_totais=$((${soma_totais}+${nota2}))
  done
  media_pand=$((${soma_pand}/${#notas_pand[@]}))
  media_total=$((${soma_totais}/${#notas_totais[@]}))

  printf "Média das notas antes da pandemia: %d\n" ${media_total}
  printf "Média das notas durante a pandemia: %d\n" ${media_pand}
  printf "Descrescimo total da média das notas durante a pandemia: %d%%\n\n" $(((${media_total}-${media_pand})*100/${media_total}))
}

# função para calcular a porcentagem de frequencia durante a pandemia e comparar com anos anteriores
calcula_porcentagem_frequencia_pandemia()
{
  freq_pand=($(tira_equivalencia | awk -F, '($5 == 2020 || $5 == 2021) {print $9}'))
  freq_total=($(tira_equivalencia | awk -F, '($5 < 2020) {print $9}'))
  soma_pand=0
  soma_totais=0
  for freq in ${freq_pand[@]}
  do
    soma_pand=$((${soma_pand}+${freq}))
  done
  for freq2 in ${freq_total[@]}
  do
    soma_totais=$((${soma_totais}+${freq2}))
  done
  media_pand=$((${soma_pand}/${#freq_pand[@]}))
  media_total=$((${soma_totais}/${#freq_total[@]}))

  printf "Média das frequencias antes da pandemia: %d\n" ${media_total}
  printf "Média das frequencias durante a pandemia: %d\n" ${media_pand}
  printf "Descrescimo total da média das frequencias durante a pandemia: %d%%\n\n" $(((${media_total}-${media_pand})*100/${media_total}))  
}

# função para calcular a porcentagem de aprovações durante a pandemia e comparar com anos anteriores
calcula_porcentagem_aprovações_pandemia()
{
  aprov_pand=($(tira_equivalencia | awk -F, '($5 == 2020 || $5 == 2021) && $10 == "Aprovado" {print $10}' | wc -l))
  aprov_total=($(tira_equivalencia | awk -F, '$5 < 2020 && $10 == "Aprovado" {print $10}' | wc -l))
  inscr_pand=($(tira_equivalencia | awk -F, '($5 == 2020 || $5 == 2021) {print $10}' | wc -l))
  inscr_total=($(tira_equivalencia | awk -F, '($5 < 2020) {print $10}' | wc -l))
  media_pand=$((${aprov_pand}*100/${inscr_pand}))
  media_total=$((${aprov_total}*100/${inscr_total}))

  printf "Média das aprovações antes da pandemia: %d%%\n" ${media_total}
  printf "Média das aprovações durante a pandemia: %d%%\n" ${media_pand}
  printf "Descrescimo total da média das aprovações durante a pandemia: %d%%\n\n" $(((${media_total}-${media_pand})*100/${media_total}))  
}

# função para calcular a porcentagem de reprovações durante a pandemia e comparar com anos anteriores
calcula_porcentagem_reprovações_pandemia()
{
  reprov_pand=($(tira_equivalencia | awk -F, '($5 == 2020 || $5 == 2021) && ($10 == "R-nota" || $10 == "Reprovado" || $10 == "R-falta") {print $10}' | wc -l))
  reprov_total=($(tira_equivalencia | awk -F, '$5 < 2020 && ($10 == "R-nota" || $10 == "Reprovado" || $10 == "R-falta") {print $10}' | wc -l))
  inscr_pand=($(tira_equivalencia | awk -F, '($5 == 2020 || $5 == 2021) {print $10}' | wc -l))
  inscr_total=($(tira_equivalencia | awk -F, '($5 < 2020) {print $10}' | wc -l))
  media_pand=$((${reprov_pand}*100/${inscr_pand}))
  media_total=$((${reprov_total}*100/${inscr_total}))

  printf "Média das reprovações antes da pandemia: %d%%\n" ${media_total}
  printf "Média das reprovações durante a pandemia: %d%%\n" ${media_pand}
  printf "Decrescimo total da média das reprovações durante a pandemia: %d%%\n\n" $(((${media_total}-${media_pand})*100/${media_total}))  
}

# função para calcular o impacto da pandemia nas aulas
calcula_impacto_pandemia()
{
  calcula_porcentagem_notas_pandemia
  calcula_porcentagem_frequencia_pandemia
  calcula_porcentagem_aprovações_pandemia
  calcula_porcentagem_reprovações_pandemia
  calcula_porcentagem_cancelamentos_pandemia
}

# função para calcular a porcentagem de aprovações durante as aulas hibridas e comparar com anos anteriores
calcula_aprovações_hibridas()
{
  aprov_hib=($(tira_equivalencia | awk -F, '$5 == 2022 && $4 == 1 && $10 == "Aprovado" {print $10}' | wc -l))
  total_hib=($(tira_equivalencia | awk -F, '$5 == 2022 && $4 == 1 {print $10}' | wc -l))
  aprov_pand=($(tira_equivalencia | awk -F, '($5 == 2020 || $5 == 2021) && $10 == "Aprovado" {print $10}' | wc -l))
  total_pand=($(tira_equivalencia | awk -F, '($5 == 2020 || $5 == 2021) {print $10}' | wc -l))
  aprov_ant=($(tira_equivalencia | awk -F, '$5 < 2020 && $10 == "Aprovado" {print $10}' | wc -l))
  total_ant=($(tira_equivalencia | awk -F, '$5 < 2020 {print $10}' | wc -l))
  media_hib=$((${aprov_hib}*100/${total_hib}))
  media_pand=$((${aprov_pand}*100/${total_pand}))
  media_ant=$((${aprov_ant}*100/${total_ant}))

  printf "Média das aprovações antes da pandemia: %d%%\n" ${media_ant}
  printf "Média das aprovações durante a pandemia: %d%%\n" ${media_pand}
  printf "Média das aprovações no periodo híbrido: %d%%\n\n" ${media_hib}

}

# função para calcular a porcentagem de reprovações durante as aulas hibridas e comparar com anos anteriores
calcula_reprovações_hibridas()
{
  reprov_hib=($(tira_equivalencia | awk -F, '$5 == 2022 && $4 == 1 && ($10 == "R-nota" || $10 == "Reprovado" || $10 == "R-falta") {print $10}' | wc -l))
  total_hib=($(tira_equivalencia | awk -F, '$5 == 2022 && $4 == 1 {print $10}' | wc -l))
  reprov_pand=($(tira_equivalencia | awk -F, '($5 == 2020 || $5 == 2021) && ($10 == "R-nota" || $10 == "Reprovado" || $10 == "R-falta") {print $10}' | wc -l))
  total_pand=($(tira_equivalencia | awk -F, '($5 == 2020 || $5 == 2021) {print $10}' | wc -l))
  reprov_ant=($(tira_equivalencia | awk -F, '$5 < 2020 && ($10 == "R-nota" || $10 == "Reprovado" || $10 == "R-falta") {print $10}' | wc -l))
  total_ant=($(tira_equivalencia | awk -F, '$5 < 2020 {print $10}' | wc -l))
  media_hib=$((${reprov_hib}*100/${total_hib}))
  media_pand=$((${reprov_pand}*100/${total_pand}))
  media_ant=$((${reprov_ant}*100/${total_ant}))

  printf "Média das reprovações antes da pandemia: %d%%\n" ${media_ant}
  printf "Média das reprovações durante a pandemia: %d%%\n" ${media_pand}
  printf "Média das reprovações no periodo híbrido: %d%%\n\n" ${media_hib}
}

# função para calcular a mediana das notas durante as aulas hibridas e comparar com anos anteriores
calcula_mediana_nota_hibridas()
{
  notas_hib=($(tira_equivalencia | awk -F, '$5 == 2022 && $4 == 1 {print $8}' | sort -n))
  mediana_hib=$((${notas_hib[${#notas_hib[@]} / 2]}+${notas_hib[${#notas_hib[@]} / 2 - 1]}))
  mediana_hib=$((${mediana_hib}/2))
  notas_pand=($(tira_equivalencia | awk -F, '($5 == 2020 || $5 == 2021) {print $8}' | sort -n))
  mediana_pand=$((${notas_pand[${#notas_pand[@]} / 2]}+${notas_pand[${#notas_pand[@]} / 2 - 1]}))
  mediana_pand=$((${mediana_pand}/2))
  notas_ant=($(tira_equivalencia | awk -F, '$5 < 2020 {print $8}' | sort -n))
  mediana_ant=$((${notas_ant[${#notas_ant[@]} / 2]}+${notas_ant[${#notas_ant[@]} / 2 - 1]}))
  mediana_ant=$((${mediana_ant}/2))

  printf "Mediana das notas antes da pandemia: %d\n" ${mediana_ant}
  printf "Mediana das notas durante a pandemia: %d\n" ${mediana_pand}
  printf "Mediana das notas no periodo híbrido: %d\n\n" ${mediana_hib}
}

# função para calcular a porcentagem de cancelamentos durante as aulas hibirdas e comparar com anos anteriores
calcula_cancelamentos_hibridas()
{
  cancel_hib=($(tira_equivalencia | awk -F, '$5 == 2022 && $4 == 1 && $10 == "Cancelado" {print $10}' | wc -l))
  cancel_pand=($(tira_equivalencia | awk -F, '($5 == 2020 || $5 == 2021) && $10 == "Cancelado" {print $10}' | wc -l))
  cancel_ant=($(tira_equivalencia | awk -F, '$5 < 2020 && $10 == "Cancelado" {print $10}' | wc -l))

  printf "Quantidade de cancelamentos antes da pandemia: %d\n" ${cancel_ant}
  printf "Quantidade de cancelamentos durante a pandemia: %d\n" ${cancel_pand}
  printf "Quantidade de cancelamentos no periodo híbrido: %d\n\n" ${cancel_hib}
}

# função para calcular o impacto das aulas híbridas
calcula_aulas_hibridas()
{
  calcula_aprovações_hibridas
  calcula_reprovações_hibridas
  calcula_mediana_nota_hibridas
  calcula_cancelamentos_hibridas
}

remove_2semestre_2022
calcula_individuos_status
calcula_max_cursadas_ate_aprovacao
calcula_porcentagem_aprovacao_reprovacao_por_ano
calcula_media_dos_aprovados
calcula_media_dos_reprovados
calcula_media_da_frequencia_reprovados
calcula_porcentagem_de_evasoes
calcula_impacto_pandemia
calcula_aulas_hibridas