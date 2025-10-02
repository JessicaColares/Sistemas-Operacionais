#ifndef V2_H
#define V2_H

// Escritores com prioridade sobre leitores. Não ocorre “leitura suja”.
void versao_escritores_prioridade(int num_leitores, int num_escritores);

// Variáveis de estatística para versão 2
extern int leituras_realizadas_v2;
extern int escritas_realizadas_v2;

#endif