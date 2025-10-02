#ifndef V1_H
#define V1_H

// Leitores e escritores com sem “prioridade”. Pode ocorrer “leitura suja”. 
void versao_sem_prioridade(int num_leitores, int num_escritores);

#endif